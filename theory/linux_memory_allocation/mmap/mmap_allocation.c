#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
  void * new_memory = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0); // see https://man7.org/linux/man-pages/man2/mmap.2.html for details

  if (new_memory == NULL) {
    perror("mmap: ");
    return 1;
  }

  printf("Newly allocated memory starting at adress %p\n", new_memory);

  int * int_arr = (int*) new_memory;

  int_arr[0] = 1;
  int_arr[1] = 2;
  int_arr[2] = 3;

  printf("%d, %d, %d\n", int_arr[0], int_arr[1], int_arr[2]);

  munmap(new_memory, 0x1000);

  return 0;
}
