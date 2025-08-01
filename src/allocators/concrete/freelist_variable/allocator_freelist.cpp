#include "allocator_freelist.h"

#include <cstddef> // For size_t, std::max_align_t, std::byte

#include "utils/log.h"

namespace nostalgia::freelist {
		
	namespace {											// Launch Buffer
		constexpr size_t buffer_size = 1024 * 1024;     // 1 MB
		std::byte* buffer = new std::byte[buffer_size]; // Mallocate a buffer of 1 MB
	}

	// === Global Static Allocator ===
	FreeAllocator g_free_allocator(buffer, buffer_size, "Global");

	// === Singleton Static Allocator ===
	FreeAllocator& SingletonFreeAllocator::get_instance() {
		static FreeAllocator s_free_allocator(buffer, buffer_size, "Singleton Static");
		return s_free_allocator;
	}

	// === Utilities ===
#pragma region Utils
	constexpr size_t	WSIZE									= 8;
	constexpr size_t	DSIZE									= 16;

	constexpr size_t	pack(size_t size, bool alloc) {			return size | static_cast<size_t>(alloc);			}

	inline size_t		get(const std::byte* ptr) {				return *reinterpret_cast<const std::size_t*>(ptr);	}
	inline void			put(std::byte* ptr, size_t value) {		*reinterpret_cast<std::size_t*>(ptr) = value;		}

	inline size_t		get_size(const std::byte* ptr) {		return get(ptr) & ~static_cast<size_t>(0x7);		}
	inline bool			get_alloc(const std::byte* ptr) {		return get(ptr) & 0x1;								}

	inline std::byte*	header(std::byte* block) {				return block - WSIZE;								}
	inline std::byte*	footer(std::byte* block) {				return block + get_size((header(block))) - DSIZE;	}

	inline std::byte*	next_block(std::byte* block) {			return block + get_size((header(block)));			}
	inline std::byte*	prev_block(std::byte* block) {			return block - get_size(block - DSIZE);				}
#pragma endregion

	// === Base Class Constructors ===
	FreeAllocator::FreeAllocator(std::byte* buf, size_t cap, const char* caller) {
		
		// Align
		std::byte* aligned_buffer = reinterpret_cast<std::byte*>(
			(reinterpret_cast<std::uintptr_t>(buf) + (DSIZE - 1)) & ~(DSIZE - 1));
		
		size_t aligned_capacity = cap - static_cast<size_t>(aligned_buffer - buf);

		// Sentinel
		put(aligned_buffer, pack(DSIZE, true));
		put(aligned_buffer + WSIZE, pack(DSIZE, true));

		m_buffer = aligned_buffer + DSIZE + WSIZE;
		m_capacity = aligned_capacity > DSIZE + WSIZE ? aligned_capacity - DSIZE - WSIZE : 0;

		put(header(m_buffer), pack(m_capacity, false));
		put(footer(m_buffer), pack(m_capacity, false));

		put(m_buffer + m_capacity, pack(0, true));
		m_head = m_buffer;

		log::print("[{}] Free List Allocator: Initialised from location [{}] with an aligned capacity of [{}] bytes", caller, (void*)m_buffer, m_capacity);
	};

	std::byte* FreeAllocator::findFirstFit(size_t size) {

		for (std::byte* ptr = m_buffer; get_size(header(ptr)) > 0; ptr = next_block(ptr)) {
			if (!get_alloc(header(ptr)) && (size <= get_size(header(ptr)))) {
				return ptr;
			}
		}

		return nullptr;
	}

	std::byte* FreeAllocator::findNextFit(size_t size) {
		if (m_head == nullptr || m_head >= (m_buffer + m_capacity)) m_head = m_buffer;

		std::byte* search = m_head;
		std::byte* found = nullptr;

		// First pass
		for (; get_size(header(search)) > 0; search = next_block(search)) {
			if (!get_alloc(header(search)) && (size <= get_size(header(search)))) {
				found = search;
				break;
			}
		}

		// Second pass
		if (found == nullptr && m_head != m_buffer) {
			search = m_buffer;

			for (; search < m_head && get_size(header(search)) > 0; search = next_block(search)) {
				if (!get_alloc(header(search)) && (size <= get_size(header(search)))) {
					found = search;
					break;
				}
			}
		}

		if (found != nullptr) {
			m_head = found;
		}

		return found;
	}

	void* FreeAllocator::allocate(size_t size) {
		//if (!m_head || size == 0) return nullptr;

		// Align size
		size_t wrappedSize = (size <= DSIZE)
			? DSIZE * 2
			: DSIZE * ((size + (DSIZE)+(DSIZE - 1)) / DSIZE);

		// First Fit Implementation
		std::byte* ptr = findNextFit(wrappedSize);
		if (ptr != nullptr) {

			size_t availableSize = get_size(header(ptr));

			if ((availableSize - wrappedSize) >= (2 * DSIZE)) {
				//log::print("Large Size Alloction");
				put(header(ptr), pack(wrappedSize, 1));
				put(footer(ptr), pack(wrappedSize, 1));
				std::byte* next_ptr = next_block(ptr);
				put(header(next_ptr), pack(availableSize - wrappedSize, 0));
				put(footer(next_ptr), pack(availableSize - wrappedSize, 0));
			}
			else {
				//log::print("Small Size Alloction");
				put(header(ptr), pack(availableSize, 1));
				put(footer(ptr), pack(availableSize, 1));
			}
			//log::print("Logging Alloc success [{}] at [{}]", m_allocCount++, (void*)ptr);
			return ptr;
		}
		log::print("Logging Bad Alloc no fit");
		throw std::bad_alloc();
		//return nullptr;
	}

	void* FreeAllocator::coalesce(std::byte* ptr) {

		//log::print("Attempting to coalesce");
		//log::print("Current pointer is [{}]", (void*)ptr);

		size_t prev_alloc = get_alloc(footer(prev_block(ptr)));
		size_t next_alloc = get_alloc(header(next_block(ptr)));
		size_t size = get_size(header(ptr));

		// No Space
		if (prev_alloc && next_alloc) {
			return ptr;
		}
		else if (prev_alloc && !next_alloc) {
			size += get_size(header(next_block(ptr)));
			put(header(ptr), pack(size, 0));
			put(footer(ptr), pack(size, 0));
		}
		else if (!prev_alloc && next_alloc) {
			size += get_size(footer(prev_block(ptr)));
			put(footer(ptr), pack(size, 0));
			put(header(prev_block(ptr)), pack(size, 0));
			ptr = prev_block(ptr);
		}
		else {
			size += get_size(footer(prev_block(ptr))) +
				get_size(header(next_block(ptr)));
			put(footer(next_block(ptr)), pack(size, 0));
			put(header(prev_block(ptr)), pack(size, 0));
			ptr = prev_block(ptr);
		}

		return ptr;
	}

	void FreeAllocator::deallocate(std::byte* ptr) {
		
		size_t size = get_size(header(ptr));

		if (ptr < m_head) m_head = ptr;

		put(header(ptr),pack(size, 0));
		put(footer(ptr), pack(size, 0));
		
		//Coalescing every deallocation for now
		coalesce(ptr);
	}

}