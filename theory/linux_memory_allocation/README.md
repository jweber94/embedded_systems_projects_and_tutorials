# Memory allocation within Linux User Space

## Memory allocation with userspace libraries
+ Allocating memory with a userspace library like glibc/stdlib creates a heap datastructure on top of the allocated memory by the ELF-executable
+ Advantages:
  - Easy to understand syntax, less code
+ Disadvantages:
  - Creates a complex datastructure based on a linked list in the background. This might be not as efficient as if you use a syscall

## Memoryallocation with systemcall `brk` or `sbrk`
+ Definition: Program Break
  - Is the last memory address, that is allocated by the .data/.bss section of the process (which was created by its corresponding executable ELF-file)
  - It defines the last address, before the uninitialized memory starts
+ With `brk` and `sbrk`, we can enhance the .data/.bss section of the program by incrementing (`sbrk`) or setting a fixed address (`brk`) for the program break pointer
  - This is basically allocating more memory to the process, since it enhances the data section of the process

## `mmap` systemcall memory allocation
+ We request a specific amount of memory by the kernel and it will give us some back, but we will induce a high granularity by the `mmap` call, since the kernel selects an arbitrary part of the currently uninitialized memory
+ It is *much* more efficient then using the glibc implementation
+ We have full control over the kind of memory (with regard permission rights) that we are allocating
+ Drawback: Relatively complicated syntax

## Reference
+ https://www.youtube.com/watch?v=c7xf5dvUb_Q
+ https://man7.org/linux/man-pages/man2/sbrk.2.html
+ https://man7.org/linux/man-pages/man2/mmap.2.html
