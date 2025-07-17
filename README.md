# Nostalgia (Memory Allocation Toolset)

Welcome to the modular implementation and profiling tool that will make you long for the days before you mastered memory allocation.

## What we do here…

> Memory allocation is a powerful tool when managed with even a glimmer of understanding.
> 

However it is also an area of systems programming prone to misunderstandings, assumptions, and micro-benchmarks of heavily disputed relevance, leaving the field feeling intimidating to some and not worth the headache to others. 

I assure you the reality is a lot easier to wrangle, and a spoonful of Nostalgia will help the memory faults go down.

---

## Quick Start & Roadmap

- Precompiled Windows (MSVC) and MacOS (Clang) Releases are available.
  >
[![Windows](https://img.shields.io/badge/Windows-v0.0.1-0078D6?logo=windows&logoColor=white&style=for-the-badge)](https://github.com/mickryley/nostalgia/releases/download/v0.0.1/nostalgia-windows-v0.0.1.zip)
[![macOS](https://img.shields.io/badge/macOS-v0.0.1-000000?logo=apple&logoColor=white&style=for-the-badge)](https://github.com/mickryley/nostalgia/releases/download/v0.0.1/nostalgia-mac-v0.0.1.zip)


<details><summary>Compiling Instructions</summary>

- Requirements: Vulkan SDK
- CMake will fetch Dear ImGui, ImPlot, and json (nlohman) - while SDL3 comes included with nostalgia.
- Debug presets are included but benchmarking should always be run in release mode.
- Windows CMake Preset:
    
    ```cpp
    cmake --preset windows-msvc
    cmake --build --preset build-release-msvc
    ```
    
- MacOs CMake Preset:
    
    ```cpp
    cmake --preset macos-clang
    cmake --build --preset build-release-macos
    ```
    

</details>

nostalgia is an ongoing side project I’m constantly expanding, for the full scope of what is included and what’s coming next you can check the Notion roadmap.

[![View Roadmap](https://img.shields.io/badge/Roadmap-View-yellow?style=for-the-badge&logo=notion)](https://www.mickryley.com/nostalgia)


What started as a library of custom allocator methods, I started rewriting into a benchmark tool for more thorough comparison and communication of the subtleties between implementations. 
I intend to eventually bundle nostalgia with realtime visualisation tools, as per the roadmap.

---

## Implementation VS Allocation…

Memory Allocation is often focused on in isolation as a discussion of allocator types and their use-cases, which is vitally important, but the allocation / allocator types themselves are only one of three pillars we need to become fluent in to make the right decisions for memory allocation in any individual system.

### Memory Allocation 201

To provide some common ground understanding, here is a simplified visualisation of how memory allocation works from the hardware up.

</details>
<details><summary>The Hardware Level</summary>

<br>

- The CPU is surrounded by a tiny pool of our fastest memory - SRAM, the L1, L2, L3 caches.
- One step away we have the (~10x slower) DRAM.
- One more step away we have the disk storage (~100’000x slower than DRAM).

This is the physical hardware memory as it exists on our systems, but its not how we address it programmatically or think about it in software terms.
This is because, unavoidably, our programs are going to use more resources than can fit entirely in the SRAM, or entirely in the DRAM, so we try to move those resources around to ensure we are able to access them as quickly as possible when we need to.

We therefore need to understand the “Virtual Memory” that our code *actually* interacts with.

- While the physical memory caches data in Physical Pages, in software terms we’re actually referencing an abstracted Page (Look Up) Table.
- The Page Table keeps track of whether the data it represents is currently cached in a Physical Page in RAM or whether it needs to be loaded from a Virtual Page that’s on the disk → into the RAM first (aka a page fault).
- You may be asking “Why isn’t all the Virtual Memory just in the RAM already if we’re referencing it?” and good question - that’s because Virtual Memory can be a lot larger than our Physical Memory, and this is the feature that enables us to seamlessly* shuffle the new data we need IN and the old data we aren’t using back OUT.

</details>
<details><summary>The Problem With Page Faults</summary>

<br>

So now you can probably deduce what a page fault is, it’s not an error, its a necessary step in memory management that occurs when the data we wish to access isn’t already cached in physical memory. 
When we try to access that data through the Virtual Table, it returns a cache miss (a page fault exception), and we must then take the time to load it into the RAM.

Why this is a very slow problem is that these operations are actually handled by the **kernel**, which necessitates switching the CPU from user mode into kernel mode **every tim**e we need to swap pages. The details *why* aren’t particularly worth elaborating on here - suffice to say the Virtual Memory system is actually translated by a physical MMU the CPU communicates through, and there’s no getting away from that on a hardware level. 

</details>
<details><summary>The Custom Solution!</summary>

<br>

The underlying memory management handled by the CPU and Page Tables are designed to keep the entire system running safely, with many programs able to operate on the OS all happily asking for and receiving memory without ever interfering with each other (that would be a segmentation fault).

It will not prioritise ***your*** program, or the performance of any one program over another - that is simply not its purpose - so if you simply want more performance, more consistency, or more information on what’s going on internally - you can introduce custom memory allocation.

The most basic feature of Custom Memory Allocators is the reduction of context switching as much as possible by effectively creating our own page table layer, meaning:

1. Rather than relying on the Virtual Table to juggle memory pages for us and keep our resources in memory whenever they’re created or deleted…
2. We can (rather greedily) request a single large amount of memory and then use our own code to allocate within that space, and just like that…
3. We’ve ensured our resources stay in (at least) DRAM, and we’re avoiding all context switching interruptions!

</details>

### Implementation

While understanding the underlying mechanics of memory management is important for optimising allocation, so too is the much higher level abstraction of how we implement our allocators in code.

It is very easy to focus on creating a custom allocator that optimises away all kernel operations, and unnecessary checks at the lower level, but then implement it in a way that loses much of the performance it has gained due to excessive indirection, polymorphism, or neglecting a compiler optimisation you weren’t aware of.

Allocator implementations typically fall into one of the following categories.

- Object-Specific new/delete overrides.
- Global new/delete override.
- Templated Allocators.

However there is significant performance variance across implementations even **within** those categories that shatter any simple ranking of one category over another.

Furthermore the realities of container allocation must also be considered, a custom allocator for objects isn’t relevant if you’re using std::containers, you’ll need to alternatively (or additionally) create your own containers, or allocators for the pre-existing containers your team will be using.

> Nostalgia prioritises the following implementations…
> 

| **Allocator Implementation** | **Allocator Accessibility** | **STL Container Compatibility** |
| --- | --- | --- |
| Global Override with Static Allocator | Static | 🛠️ Affects STL internals |
| Global Override with Singleton Allocator | Singleton | 🛠️ Affects STL internals |
| Object Override with Static Allocator (with pointer container) | Static | ❌ Not STL compatible |
| Object Override with Static Allocator (w/ std container) | Static | ❌ Not STL compatible |
| Object Override with Singleton Allocator (with pointer container) | Singleton | ❌ Not STL compatible |
| Object Override with Singleton Allocator (w/ std container) | Singleton | ❌ Not STL compatible |
| Template Allocator (with pointer container) | Template-based | ❌ Not STL compatible |
| Template Allocator (w/ std container) | Template-based | ❌ Not STL compatible |
| STL-conformed Template Allocator (w/ std ptr cont.) | Template-based | ✅ Drop-in for any STL container |
| STL-conformed Template Allocator (w/ std container) | Template-based | ✅ Drop-in for any STL container |
| PMR-conformed Static Allocator (w/ pmr container) | Static | 🛠️ Requires std::pmr::* containers |
| PMR-conformed Singleton Allocator (w/ pmr container) | Singleton | 🛠️ Requires std::pmr::* containers |
| No Implementation (System) | N/A | ✅ Drop-in for any STL container |

### Allocator Types

Comparing allocator types isn’t purely a battle of performance, its a question of feature set. 
For example an allocator that allocates in a stack, and only ever deallocates the entire stack in one go will intuitively perform much better than a mixed-size allocator for objects of varying lifetimes that require constant coalescing. That classic semi-trailer vs F1 car problem.

> Nostalgia prioritises the following allocator types…
> 

| **Allocator Type** | **Expandable** | **Deallocates** | **Coalesces** | **Notes** |
| --- | --- | --- | --- | --- |
| **No Allocator (System)** | ✅ | ✅ | ✅ | malloc/free, new/delete |
| **Linear (Bump)** | ❌ | ❌ (all at once) | ❌ | Fastest O(1) allocation, deallocates all together. |
| **Stack (LIFO)** | ❌ | 🟡 (LIFO only) | ❌ | O(1) alloc & dealloc, deallocation in reverse order only. |
| **Pool / Slab / Free List (Fixed)** | ✅ | ✅ | ❌ | O(1) alloc & dealloc, fixed-size blocks, avoids need for coalescing (avoids external frag). |
| **Free List (Variable, First Fit, RedBlack)** | ✅ | ✅ | ✅ | O(log N) alloc, any size, alloc and dealloc in any order. |
| **Free List (Variable, Best Fit, RedBlack)** | ✅ | ✅ | ✅ | O(log N) alloc, any size, alloc and dealloc in any order. |
| **Free List (Variable, First Fit)** | ✅ | ✅ | ✅ | O(N) alloc, any size, alloc and dealloc in any order. |
| **Free List (Variable, Best Fit)** | ✅ | ✅ | ✅ | O(N) alloc, any size, alloc and dealloc in any order. |
| **Segregated Fit (Seg Fit)** | ✅ | ✅ | 🟡 (within size) | Multiple fixed free lists of (arbitrarily defined) fixed sizes. |
| **Buddy (2ⁿ)** | ✅ | ✅ | ✅ | SegFit with pools sizes defined by powers of 2. Larger blocks are split recursively as required. (Risks internal frag) |
| **🚩 Thread-Cached** | ✅ | ✅ | 🟡 | Not an allocator, a common strategy of distinct allocators per thread to increase multi-threaded performance. |
| **🚩 Profiled** | ✅ | ✅ | ✅ | Not an allocator, the inclusion of internal debugging / profiling / visualising tools to make better system and design decisions.  |

It is, of course, worth mentioning that there are always subtleties within the design of these allocator types that will change their performance, and their feature-set even more finely. But this list should serve as a good set of starting points for building an allocator that works perfectly for you.

## … VS Usability!

The third pillar of Custom Memory Allocator design is considering how your team is realistically going to use the thing!

If you’re working on a solo project or in a small team from day 1 this mightn’t be an immediate concern, but with pre-existing codebases or larger / siloed departments the fact is that your choice of implementation will severely impact what is required of everyone else working on your codebase.

### Realistically Required Rewriting (Preexisting Code)

Consider a simple long forgotten line using a standard container like this in your code:

<details><summary>[Click to Show] Example std::vector issues</summary>

<br>

```cpp
// Default STL Vector and custom Foo Object
std::vector<Foo> bar;
bar.emplace_back(...);
```

Depending on your implementation type, to actually make that old code use your allocator, you may need to edit the old code in-place.

```cpp
// STL Conformed Custom Allocator
std::vector<Foo, STLConformedAllocator<Foo>> bar;       // 🟡 Template reference required.
bar.emplace_back(...);                                  // ✅ No change

// Non-STL Conformed Object Override Custom Allocator, pointer reference collection
std::vector<Foo*> barPointers;                          // 🟡 Collection type change to pointer
bar.push_back(new Foo(...));                            // ❌ Explicit object construction rewrite required

// PMR Template Custom Allocator
std::pmr::vector<Foo> bar(&PMRAllocator);               // ❌ Collection Change, Allocator reference required
bar.emplace_back(...);                                  // 🟡 No obvious change but using a different library.
```

</details>

<details><summary>[Click to Show] Example std::vector solutions</summary>

<br>

While this is technically necessary, it’s not ideal, it’s not quick and it’s definitely not going to be an error-free migration - despite your neovim macro’s best efforts. So what are the workarounds?

- The most obvious solution is to use your own custom collections, even if they start purely as thin wrappers around STL collections, once written they can easily be changed in one place - however this method will induce its own indirection overhead as a tradeoff.
    
    ```cpp
    // Custom Collection Swap-out
    bazVector<Foo> bar;                                  // 🟡 Collection type swap required
    bar.emplace_back(...);                               // ✅ No change
    ```
    
- A template alias wrapper is another reflexive fix to future proof against changes, but it will still require in-place edits to old code to initially set up.
    
    ```cpp
    template<typename T>                                             // ❌
    using vectorAlias = std::vector<T, STLConformedAllocator<T>>;    // ❌ Alias Template Declaration Required
    
    // Custom Collection Swap-out
    vectorAlias<Foo> bar;                                // 🟡 Collection alias swap required
    bar.emplace_back(...);                               // ✅ No change
    ```
    
- The most obvious exception is the global override - which does impact std containers - it’s an impractical fix but worth mentioning.
    
    ```cpp
    // Global Operate New / Free Overrides
    void* operator new(std::size_t size) {
      // ...
    }
    ```
    

</details>

However these compromises won’t solve the problem for all possible implementations, and the more effort you spend trying to force / redirect something to work invisibly for every use-case - the more likely you are to lower performance and increase the potential for issues.

Obviously the best solution would be to have it done correctly from the beginning and have everyone on the team understanding the nuances of how to write performance custom-allocator-aware code, congratulations if that is your reality.

The real solution is to know your scope, know your system, and only build to the implementation and type you have decided upon, it will undoubtedly be unique to you and require some training for other users.

> Nostalgia’s profiled allocators are packaged with custom collections for ease of use, while Nostalgia’s benchmarking includes a range of user-unfriendly implementations for completeness.
> 

### Feedback First

When choosing a custom allocator, a key advantage is the additional insights you will be able to gain into how your memory is performing. 
This isn’t merely for showing off what a good decision it was, but to diagnose and understand how changing certain configurations of your allocator’s behaviour will impact performance.

This is where the type of polymorphism used by your implementation becomes more important, perhaps you use a dynamically polymorphic implementation during development for rapid prototyping, but plan to pivot to static polymorphism for the hottest paths come release time. 
In other situations you may want objects to remain dynamically polymorphic during runtime to make use of a smart switching mechanism in your allocator manager that reroutes them to different allocators depending upon certain runtime conditions.

All of these decisions can be made most effectively with profiling, which is an area in which custom allocators shine despite requiring a bit of work to get started. 
Generally external memory profiling tools will be obscured from seeing what’s actually going on with custom allocators as we’re minimising the kernel memory assignment operations that are typically logged. 

So we will need to build our own memory profiling tools, the benefits here are not only the quality of information, but how we can feed that information it back to our system/product (this is most obviously demonstrated through in-editor feedback in video games).

> Nostalgia profiling tools serve as a minimalistic demonstration of the feedback and granularity available when using a custom allocator.
> 

### Fail Gracefully

Importantly, when messing with memory, hope for the best and expect the worse.

The default allocators will always be slower because they can handle anything that is thrown at them.
Therefore your custom allocator should always be checking (preferably asserting) that it is receiving exactly what it expects, and if it doesn’t - then it will always be safe to fail-forward to the defaults. This is actually a key built-in feature of the std::pmr allocators that help make them such a great starting point for anyone looking to start mastering memory.

---

## Considerations…

<details><summary>Of the many common thoughts and concerns you may encounter when advocating for custom Memory Allocation, Nostalgia addresses the following:</summary>

<br>

---

<pre><em>“C++17 added pmr templates, which they’ve continued to expand, so custom allocators are no longer as relevant.”</em></pre>

- std::pmr primarily serves to provide an alternative for **implementation** of allocators into your codebase, this does not sidestep the problems addressed by custom allocators, it provides another tool with which to become more familiar with them.
- std::pmr also provides several default **allocators types** that can be used with minimal setup through the std::pmr implementation.
- This is an excellent starting point for pivoting a system to be more allocation aware.

> Nostalgia includes both std::pmr implementations and std::pmr default allocator types in its comparison suite, they are competitive, but not without peer.
> 

---

<pre><em>“Amdahl’s Law means memory allocation isn’t that important.”</pre></em>

- Amdahl’s Law for the unfamiliar is simply that you can’t optimise a task beyond the total time it already takes, so if memory allocation takes 3μs and you optimise it to perform instantly you’ve still only saved 3μs.
- The implication then is that memory allocation is likely not going to be your bottleneck. If you’re building a VR game maintaining 8ms (8,000μs) frame times, and memory allocation is 3μs per frame then optimising it to be 30% faster will save less than 1μs of your 8,000μs budget.
- This is true, but slightly misleading for three reasons:
    1. These numbers are completely made up, neither I nor anyone else knows the reality of your system, maybe it’s a game, maybe it’s crypto, and maybe memory allocation is taking up a sizeable portion of your processing budget.
    2. This assumes an optimised processing time as the sole reason to use a custom memory allocator, this is simply untrue. Improved performance is one benefit, but custom allocation allows you to store memory where you want, when you want, and clear it - preserve it - transform it - observe it - debug it - however you want. 
    3. Even in a system with tight budgets of which memory allocation is only a small part, optimising it by 30% still has the benefit of increasing the available throughput of the allocator by 30% without needing to reshuffle or optimise any other part of the system - potentially raising ceilings on what is reasonable or even plausible for high-throughput operations such as simulations.

> Nostalgia aims to provide realistic use-cases for benchmarks and to ensure the results are presented in a way that both accurately communicates the limits as defined by Amdahl’s Law, but also the benefits that remain in spite of it.
> 

---

<pre><em>“Micro-benchmarking is unrepresentative of the reality of software.”</pre></em>

- Micro-benchmarking isn’t inherently bad, but memory allocation benchmarking can often be so isolated from its surroundings as to not be taken seriously.
- One problem is that micro-benchmarking allocators typically only focuses on the speed of allocation and deallocation (and coalescing etc if applicable), but not on actually writing data into that allocated space and then passing around pointers and manipulating it. Which makes sense, as the purpose of an allocator micro-benchmark is typically to illustrate the benefits of one **allocator type** compared to another (Such as pool vs linear). However much like showing an empty plane can fly and an empty ship can float, it doesn’t do much to demonstrate the realities of travelling as a passenger on either.
- Questionable metaphors aside, micro-benchmarks are useful, and very illustrate for educational purposes, but beyond that they’re not seen as particularly meaningful to some.

> Nostalgia confronts this problem head-on by providing not only various **allocator types** but various **implementation methods** that can be paired with them, and then a veritable menagerie of tests for benchmarking in an effort to be more honestly representative of what custom memory allocation could do for your system.
> 

---

<pre><em>“Unless you have evidence a custom allocator can improve performance in your use case, you shouldn’t implement one.”</pre></em>

- This is a reasonable statement for someone unfamiliar with allocation, especially if they’ve heard horror stories of custom allocators gone wrong.
- But the evidence is already in the question. A **custom** allocator will always improve performance, as is best explained in the next dropdown: “*90% of the time…*”

> Nostalgia provides the evidence that not only the most highly customised allocators are beneficial, but that even very minor adjustments to your code <em>will</em> improve the performance of your system.
> 

---

<pre><em>“90% of time the built-in allocator is good enough.”</pre></em>

- This statement either comes from a looming project deadline, or a fear of memory allocation gone wrong, and in both those cases - it is correct.
- But as a statement about relative performance, 0% of the time of the time is the built-in allocator comparable to anything custom.
- And it’s easy to understand why, the “built-in” allocator needs to be smart, it needs to handle anything that is thrown at it - be that an integer, or a 1gb 3D sculpt. Where-as when we design custom allocators we know what they will be allocating, so we don’t need to provide branching possibilities for every size and object lifetime. We can still make them fail gracefully or forward objects that have called them mistakenly, but by designing an allocator that perfectly manages 10’000’s of similarly-sized objects - we are avoiding 10’000’s of checks <em>every frame</em> because we already know the answer to them.

> Nostalgia exists to be honest and approachable, and to that end there are examples of simple and nonintrusive allocation workflows that can help you become more comfortable with managing allocation without making <em>everything</em> custom.
> 

---

<pre><em>“Compilers these days basically optimise it all away to the same thing anyway.”</em></pre>

- This I believe comes from a place of confusion when the real-world performance testing of an allocator doesn’t behave as expected and is hand-waved away as “the compiler did it.”
- Compilers don’t really impact memory allocators themselves, but they will have significant impacts on our **implementations**, and implementations are truly half the battle when it comes to optimising allocation - especially in micro-benchmarking.
- However a compiler is incapable of optimising away the differences between **allocator types,** and therefore selecting a more fitting **allocator type** for your system will be beneficial regardless of any compiler improvement in the foreseeable future.
- Keep in mind that the compiler optimisations made in a release mode build will not necessarily logically match what was observed in a debug build, an optimisation that’s actually the fastest in release may run more slowly than its competitors when compiled in debug mode.

> Nostalgia provides all benchmarks with the compile mode and compiler annotated, your own compiler may indeed shift the relative performance of **implementations**.
> 

</details>

---
