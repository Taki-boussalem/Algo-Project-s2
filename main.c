#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"linked_list.h"
#include"stack.c"
#include"recursion.c"
#include"tree.c"

int main () {
  FILE* f = fopen("sample.txt", "r");
  if(f == NULL) {
    printf("File have not been read\n");
    return 1;
  }
  TList* head = getPersonality(f);
  TList* header = getDatePersonality(f);
  TList* merged = mergeNodes(head, header);

  TStack* stk = toStack(merged);
  displayStack(*stk);
  return 0;
}