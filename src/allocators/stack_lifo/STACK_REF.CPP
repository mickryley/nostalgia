#include <cstddef>   // For size_t
#include <cstdint>   // For uint32_t, uint8_t
#include <atomic>    // For std::atomic
#include <new>       // For std::bad_alloc
#include <stdexcept> // For std::runtime_error (for specific error messages)
#include <utility>   // For std::move

// Assuming PtrSize, U32, U8, Bool are typedefs for size_t, uint32_t, uint8_t, bool respectively
// And NonCopyable is a base class or macro preventing copy construction/assignment

struct MemoryBlockHeader {
    uint32_t size; // Size of the allocated block, including this header [10]
};

// Simplified NonCopyable for this example
class NonCopyable {
protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

/// Thread-safe memory pool class [9]
class StackMemoryPool : public NonCopyable {
public:
    /// Default constructor
    StackMemoryPool(size_t size, uint32_t alignmentBits = 16)
        : memsize(size), alignmentBits(alignmentBits) {
        if (memsize == 0) {
            throw std::bad_alloc(); // Ensure non-zero size
        }
        memory = static_cast<uint8_t*>(::malloc(memsize)); // Allocate underlying memory [11]
        if (memory != nullptr) {
            top = memory;
        } else {
            throw std::bad_alloc(); // Indicate malloc failure [11]
        }
    }

    /// Move constructor [9]
    StackMemoryPool(StackMemoryPool&& other) noexcept { *this = std::move(other); }

    /// Destructor [9]
    ~StackMemoryPool() {
        if (memory != nullptr) {
            ::free(memory); // Free the allocated memory chunk [11]
        }
    }

    /// Move assignment operator [9]
    StackMemoryPool& operator=(StackMemoryPool&& other) noexcept {
        if (this != &other) { // Prevent self-assignment
            if (memory != nullptr) {
                ::free(memory);
            }
            memory = other.memory;
            memsize = other.memsize;
            top.store(other.top.load());
            alignmentBits = other.alignmentBits;

            other.memory = nullptr;
            other.memsize = 0;
            other.top = nullptr;
        }
        return *this;
    }

    /// Access the total size [9]
    size_t getSize() const { return memsize; }

    /// Get the currently allocated size [9]
    size_t getAllocatedSize() const { return top.load() - memory; }

    /// Allocate memory [9]
    /// @return The allocated memory or nullptr on failure
    void* allocate(size_t size_) noexcept {
        // Calculate the aligned size, including space for the header [10, 11]
        size_t size = calcAlignSize(size_ + sizeof(MemoryBlockHeader));
        if (size == 0 || size > (std::numeric_limits<uint32_t>::max() - sizeof(MemoryBlockHeader))) {
            return nullptr; // Prevent too big allocations or overflow
        }

        // Atomically increment the 'top' pointer to reserve space [11]
        uint8_t* out = top.fetch_add(size);

        // Check if the allocation fits within the total memory [11]
        if (out + size <= memory + memsize) {
            // Write the block header with the total allocated size (including header) [10, 11]
            (reinterpret_cast<MemoryBlockHeader*>(out))->size = static_cast<uint32_t>(size);
            // Return pointer to the user's usable memory (after the header) [10, 11]
            out += sizeof(MemoryBlockHeader);
        } else {
            // Allocation failed, roll back the 'top' if it was advanced beyond capacity
            // Note: A more robust approach might involve a compare_exchange_strong loop here
            // to handle contention if fetch_add overshoots the buffer boundary.
            // For simplicity, this example just sets out to nullptr if it goes out of bounds.
            out = nullptr;
            top.fetch_sub(size); // Attempt to revert the top if allocation failed
        }
        return out;
    }

    /// Free memory in StackMemoryPool. Only works if ptr is the last allocation (LIFO) [9, 12]
    /// @param[in] ptr Memory block to deallocate
    /// @return True if the deallocation actually happened and false otherwise
    bool free(void* ptr) noexcept {
        if (ptr == nullptr) return false;

        // Correct the pointer to point to the block header [10, 11]
        uint8_t* realptr = static_cast<uint8_t*>(ptr) - sizeof(MemoryBlockHeader);

        // Ensure the pointer is within the pool's bounds (assertion removed for general C++)
        // ANKI_ASSERT(realptr >= memory && realptr < memory + memsize);

        // Get the total size of the block from its header [10, 11]
        uint32_t size = (reinterpret_cast<MemoryBlockHeader*>(realptr))->size;

        // Attempt to atomically move the 'top' pointer backward if 'ptr' is indeed the last allocation [11, 12]
        uint8_t* expected = realptr + size; // 'top' should be at the end of this block
        uint8_t* desired = realptr;         // 'top' will be moved to the beginning of this block

        // compare_exchange_strong is essential for thread-safety [12]
        return top.compare_exchange_strong(expected, desired);
    }

    /// Reinitializes the pool. All existing allocated memory is lost [9]
    void reset() noexcept {
        top = memory; // Simply reset the 'top' pointer to the beginning [11]
    }

private:
    uint8_t* memory = nullptr; // Pre-allocated memory chunk [9]
    size_t memsize = 0;         // Size of the pre-allocated memory chunk [9]
    std::atomic<uint8_t*> top = {nullptr}; // Pointer to the top of the stack (current allocation position) [9]
    uint32_t alignmentBits;      // Alignment requirement in bits [9]

    // Calculates the aligned size for an allocation [9, 11]
    size_t calcAlignSize(size_t size) const {
        // Align up to the nearest multiple of (alignmentBits / 8)
        size_t alignmentBytes = alignmentBits / 8;
        return (size + alignmentBytes - 1) / alignmentBytes * alignmentBytes;
    }
};