#include <string.h>
#include <malloc.h>
#include <stdio.h>

int main(){
  char * heap_mem = malloc(1024);
  if (heap_mem != NULL) {
    strcpy(heap_mem, "Hello Heap Memory!\n");
  }
  printf("%s", heap_mem);
  free(heap_mem);
}
