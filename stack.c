#include <stdio.h>
#include <stdlib.h>
// #include"linked_list.h"
#include "linked_list.c"

struct TStack {
  TList *top;
  int size;
};

typedef struct TStack TStack;

void push(TStack *stack, char *name, char *DoB, char *DoD, char *definition) {
  TList *node = malloc(sizeof(TList));
  node->next = stack->top;
  strcpy(node->name, name);
  strcpy(node->definition, definition);
  strcpy(node->DoB, DoB);
  strcpy(node->DoD, DoD);
  stack->top = node;
  stack->size += 1;
}
void pushNode(TStack *stack, TList *node) {
  node->next = stack->top;
  stack->top = node;
  stack->size += 1;
}

int isEmpty(TStack stack) { return stack.top == NULL; }

void pop(TStack *stack) {
  if (isEmpty(*stack)) {
    return;
  }
  TList *curr = stack->top;
  stack->top = curr->next;
  stack->size -= 1;
  free(curr);
}

void recursiveDisplay(TStack stack) {
  if (isEmpty(stack))
    return;
  printf("(%s, Birth: %s, Death: %s, Definition:%s, size: %d) --> \n",
         stack.top->name, stack.top->DoB, stack.top->DoD, stack.top->definition,
         (int)(strlen(stack.top->definition)));
  TList *node = copyNode(stack.top);
  pop(&stack);
  recursiveDisplay(stack);
  push(&stack, node->name, node->DoB, node->DoD, node->definition);
}

void displayStack(TStack stack) {
  printf("Beginning of Stack \n");
  recursiveDisplay(stack);
  printf("end of stack\n");
}

TStack *toStack(TList *merged) {
  TStack *stk = malloc(sizeof(TStack));
  stk->size = 0;
  stk->top = NULL;
  TList *node = merged;

  for (TList *node = merged; node; node = node->next) {
    push(stk, node->name, node->DoB, node->DoD, node->definition);
  }

  return stk;
}

TList *getInfoPersonality(TStack *stk, char *name) {
  if (isEmpty(*stk))
    return NULL;
  else if (strcmp(stk->top->name, name) == 0) {
    return stk->top;
  }

  char *n_name = malloc(100 * sizeof(char)), *DoB = malloc(20 * sizeof(char)),
       *DoD = malloc(20 * sizeof(char)),
       *definition = malloc(500 * sizeof(char));
  strcpy(n_name, stk->top->name);
  strcpy(definition, stk->top->definition);
  strcpy(DoB, stk->top->DoB);
  strcpy(DoD, stk->top->DoD);
  pop(stk);
  TList *wanted_thing = getInfoPersonality(stk, name);
  push(stk, n_name, DoB, DoD, definition);
  free(n_name);
  free(definition);
  free(DoB);
  free(DoD);
  return wanted_thing;
}

TStack *pushSorted(TStack *stk, TList *node,
                   int (*compare)(TList *n1, TList *n2)) {
  if (stk->top == NULL || compare(stk->top, node)) {
    pushNode(stk, node);
    stk->top = node;
    return stk;
  }

  TList *poped_node = copyNode(stk->top);
  pop(stk);
  pushSorted(stk, node, compare);
  pushNode(stk, poped_node);
  return stk;
}

TStack *sortNameStack(TStack *s) {
  if (s->top == NULL) {
    return s;
  }
  TList *node = copyNode(s->top);
  pop(s);
  sortNameStack(s);
  pushSorted(s, node, compareAlpha);
  return s;
}

TStack *deleteName(TStack *s, char *name) {
  if (s == NULL) {
    TStack *q = malloc(sizeof(TStack));
    q->top = NULL;
    q->size = 0;
    return q;
  } else if (strcmp(s->top->name, name) == 0) {
    pop(s);
    return s;
  }

  TList *node = copyNode(s->top);
  pop(s);
  deleteName(s, name);
  pushNode(s, node);
  return s;
}

TStack *updateStack(TStack *s, char *name, char *DoB, char *DoD,
                    char *definition) {
  if (s == NULL) {
    TStack *q = malloc(sizeof(TStack));
    q->top = NULL;
    q->size = 0;
    return q;
  } else if (strcmp(s->top->name, name) == 0) {
    strcpy(s->top->name, name);
    strcpy(s->top->definition, definition);
    strcpy(s->top->DoB, DoB);
    strcpy(s->top->DoD, DoD);
    return s;
  }

  TList *node = copyNode(s->top);
  pop(s);
  updateStack(s, name, DoB, DoD, definition);
  pushNode(s, node);
  return s;
}

TQueue *stackToQueue(TStack *stk) {
  if (isEmpty(*stk)) {
    TQueue *q = createQueue();
    return q;
  }

  TList *node = copyNode(stk->top);
  TList *another_node = copyNode(stk->top);
  pop(stk);
  TQueue *q = stackToQueue(stk);
  pushNode(stk, node);
  enqueue(q, another_node);
  return q;
}

TList *s2l(TStack *stk) {
  if (isEmpty(*stk)) {
    return NULL;
  }

  TList *node = copyNode(stk->top);
  TList *list_node = copyNode(stk->top);
  pop(stk);
  TList *list = s2l(stk);
  pushNode(stk, node);
  list_node->next = list;
  return list_node;
}

TList *stackToList(TStack *stk) { return sortWords(s2l(stk)); }

TStack *addNameStack(TStack *s, char *name, char *DoB, char *DoD,
                     char *definition) {
  TList *node = malloc(sizeof(TList));
  strcpy(node->name, name);
  strcpy(node->definition, definition);
  strcpy(node->DoB, DoB);
  strcpy(node->DoD, DoD);
  return pushSorted(s, node, compareAlpha);
}

int compareDef(TList *n1, TList *n2) {
  int count1 = 0;
  for (int i = 0; n1->definition[i] != '\0'; i++) {
    if ((n1->definition[i] == ' ' || n1->definition[i] == ',' ||
         n1->definition[i] == '.' || n1->definition[i] == '?' ||
         n1->definition[i] == '!') &&
        n1->definition[i - 1] != ' ') {
      count1++;
    }
  }
  int count2 = 0;
  for (int i = 0; n2->definition[i] != '\0'; i++) {
    if ((n2->definition[i] == ' ' || n2->definition[i] == ',' ||
         n2->definition[i] == '.' || n2->definition[i] == '?' ||
         n2->definition[i] == '!') &&
        n2->definition[i - 1] != ' ') {
      count2++;
    }
  }
  return count1 > count2;
}

TStack *sortDefinition(TStack *stk) {
  if (isEmpty(*stk)) {
    return stk;
  }

  TList *node = copyNode(stk->top);
  pop(stk);
  stk = sortDefinition(stk);
  pushNode(stk, node);
  return stk;
}

int isShort(char *statement) { return strlen(statement) < 200; }

TStack *pronounciationStack(TStack *s) {
  TStack *stack = malloc(sizeof(TStack));
  stack->top = NULL;
  stack->size = 0;
  TList *node = s->top;
  TList *prev = NULL;
  while (node != NULL) {
    if (isShort(node->definition)) {
      TList *temp = node->next;
      prev->next = temp;
      pushNode(stack, node);
      node = temp;
    }
    prev = node;
    node = node->next;
  }
  return stack;
}

char *getSmallest(TStack *stack) {
  if (isEmpty(*stack)) {
    return "";
  }

  char *def = malloc(sizeof(char) * 500);
  strcpy(def, stack->top->definition);
  pop(stack);
  char *result = malloc(500 * sizeof(char));
  strcpy(result, getSmallest(stack));
  if (strlen(result) > 10 && strlen(result) < strlen(def)) {
    return result;
  } else {
    return def;
  }
}

int searchWord(char *text, char *word) {
  int text_len = strlen(text);
  int word_len = strlen(word);

  for (int i = 0; i <= text_len - word_len; i++) {
    int j = 0;
    while (j < word_len && text[i + j] == word[j]) {
      j++;
    }
    if (j == word_len) {
      int left = (i == 0 || text[i - 1] == ' ');
      int right = (i + word_len == text_len || text[i + word_len] == ' ');
      if (left && right)
        return i;
    }
  }
  return 0;
}

int isPersonalityKilled(TStack *stk, char *word) {
  if (isEmpty(*stk)) {
    return 0;
  } else if (strcmp(stk->top->name, word) == 0) {
    if (searchWord(stk->top->definition, "killed")) {
      return 1;
    } else {
      return 0;
    }
  }
  TList *node = copyNode(stk->top);
  pop(stk);
  int result = isPersonalityKilled(stk, word);
  pushNode(stk, node);
  return result;
}

TStack *pushBottom(TStack *stk, TList *node) {
  if (stk->top == NULL) {
    node->next = stk->top;
    stk->top = node;
    return stk;
  }

  TList *poped_node = copyNode(stk->top);
  pop(stk);
  pushBottom(stk, node);
  pushNode(stk, poped_node);
  return stk;
}

TStack *recRevStack(TStack *stk) {
  if (isEmpty(*stk)) {
    return stk;
  }
  TList *node = copyNode(stk->top);
  pop(stk);
  recRevStack(stk);
  pushBottom(stk, node);
  return stk;
}

// int main() {
//   FILE *f = fopen("sample.txt", "r");
//   if (f == NULL) {
//     printf("File have not been read\n");
//     return 1;
//   }
//   TList *head = getPersonality(f);
//   TList *header = getDatePersonality(f);
//   TList *merged = mergeNodes(head, header);
//   TStack *stk = toStack(merged);
//   recRevStack(stk);
//   displayStack(*stk);
//   return 0;
// }
