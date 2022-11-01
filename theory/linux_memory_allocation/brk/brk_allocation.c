#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
  void* current_program_break = sbrk(0); // we can access the current address of the program break by requesting the pointer from sbreak within allocating memory
  printf("The current program break sits at %p\n", current_program_break);

  void* new_program_break = sbrk(0x1000);
  printf("The new program break sits at %p\n", new_program_break);

  // use the newly allocated memory to store something in it
  int * int_arr = (int*)current_program_break; // we can use memory with the size of 0x1000 bytes (= 4096 bytes)

  int_arr[0] = 1;
  int_arr[1] = 2;
  int_arr[2] = 3;

  printf("%d, %d, %d\n", int_arr[0], int_arr[1], int_arr[2]);

}
