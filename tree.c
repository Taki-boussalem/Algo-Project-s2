#include <stdio.h>
// #include <stdlib.h>
// // #include"linked_list.h"
#include "stack.c"

struct TTree {
  struct TTree *right_child;
  char name[50];
  char definition[300];
  char DoB[10];
  char DoD[10];
  struct TTree *left_child;
};
typedef struct TTree TTree;

TTree *insertTree(TTree *tr, TList *node) {
  if (tr == NULL) {
    TTree *new_node = malloc(sizeof(TTree));
    strcpy(new_node->name, node->name);
    strcpy(new_node->definition, node->definition);
    strcpy(new_node->DoB, node->DoB);
    strcpy(new_node->DoD, node->DoD);

    new_node->right_child = new_node->left_child = NULL;
    return new_node;
  }

  if (strcmp(tr->name, node->name) < 0) {
    tr->right_child = insertTree(tr->right_child, node);
  } else {
    tr->left_child = insertTree(tr->left_child, node);
  }

  return tr;
}

TTree *toTree(TStack *stack) {
  TTree *tr = NULL;

  tr = insertTree(tr, (stack)->top);
  pop(stack);

  while (!isEmpty(*stack)) {
    tr = insertTree(tr, (stack)->top);
    pop(stack);
  }

  return tr;
}

TTree *fillTree(FILE *f) {
  if (!f) {
    printf("Could not read file\n");
    return NULL;
  }
  // getting name from a file

  char info[500];
  TTree *tr = malloc(sizeof(TTree));
  fgets(info, 500, f);
  char name[100];
  int i;
  for (i = 0; info[i] != '{'; i++) {
    name[i] = info[i];
  }
  name[i - 1] = '\0';
  strcpy(tr->name, name);

  while (fgets(info, 500, f)) {
    char name[100];
    for (i = 0; info[i] != '{'; i++) {
      name[i] = info[i];
    }
    name[i - 1] = '\0';
    TList *node = malloc(sizeof(TList));
    strcpy(node->name, name);
    insertTree(tr, node);
    free(node);
  }
  return tr;
}

TTree *getInfoNameTree(TTree *tr, char *name) {
  if (tr == NULL) {
    return NULL;
  } else if (strcmp(tr->name, name) == 0) {
    return tr;
  }

  if (strcmp(tr->name, name) > 0) {
    return getInfoNameTree(tr->left_child, name);
  } else {
    return getInfoNameTree(tr->right_child, name);
  }
}

TTree *addNameBST(TTree *tr, char *name, char *DoB, char *DoD) {
  TList *node = malloc(sizeof(TList));
  strcpy(node->name, name);
  strcpy(node->DoB, DoB);
  strcpy(node->DoD, DoD);

  tr = insertTree(tr, node);
  return tr;
}

TTree *getMinTree(TTree *tr) {
  if (tr == NULL)
    return NULL;
  TTree *node = tr->right_child;

  while (node->left_child->left_child != NULL)
    node = node->left_child;

  return node;
}

TTree *deleteNameBST(TTree *tr, char *name) {
  if (tr == NULL) {
    return NULL;
  } else if (strcmp(tr->name, name) == 0) {
    // getting minimum
    if (tr->left_child == NULL) {
      TTree *temp = tr->right_child;
      free(tr);
      return temp;
    } else if (tr->right_child == NULL) {
      TTree *temp = tr->right_child;
      free(tr);
      return temp;
    }
    TTree *pre_min = getMinTree(tr);
    // copy all the information here
    TTree *min = pre_min->left_child;
    pre_min->left_child = NULL;
    min->right_child = tr->right_child;
    min->left_child = tr->left_child;
    free(tr);
    return min;
  }

  if (strcmp(tr->name, name) > 0) {
    tr->left_child = deleteNameBST(tr->left_child, name);
  } else {
    tr->right_child = deleteNameBST(tr->right_child, name);
  }

  return tr;
}

/* -------------------- displayTree ----------------------------*/
void displayTree(TTree *R) {
  if (R != NULL) {
    printf("%s, ", R->name);
    displayTree(R->right_child);
    displayTree(R->left_child);
  };
}

/* ------------------- main ----------------------------- */
int main() {
  FILE *f = fopen("sample.txt", "r");
  if (f == NULL) {
    printf("File have not been read\n");
    return 1;
  }
  TList *head = getPersonality(f);
  TList *header = getDatePersonality(f);
  TList *merged = mergeNodes(head, header);
  TStack *stk = toStack(merged);
  TTree *tr = toTree(stk);
  TTree *info = getInfoNameTree(tr, "Emir Abdelkader");
  printf("(%s, Birth: %s, Death: %s, Definition:%s, size: %d) --> \n",
         info->name, info->DoB, info->DoD, info->definition,
         (int)(strlen(info->definition)));
  return 0;
}
