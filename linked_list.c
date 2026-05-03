#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TList {
  char name[50];
  char definition[300];
  char DoB[10];
  char DoD[10];
  struct TList *next;
  struct TList *prev;
};

typedef struct TList TList;

struct TQueue {
  TList *head, *tail;
  int size;
};

typedef struct TQueue TQueue;

TList *getPersonality(FILE *f) {
  // reading the file
  if (!f) {
    printf("File is NULL\n");
    return NULL;
  }
  rewind(f);
  TList *head = malloc(sizeof(TList));
  head->prev = NULL;
  TList *node = head;
  char info[500];

  while (fgets(info, 500, f)) {
    // checking if event
    int i = 0;
    int flag = 0;
    while (info[i] != '=') {
      if (info[i] == ':') {
        flag = 1;
        break;
      }
      i++;
    }
    if (flag == 1) {
      continue;
    }

    // getting name and definition
    char name[150];
    char def[600];
    flag = 0; // if flag == 0 readign name | if flag == 1 reading def
    i = 0;
    int temp;
    while (info[i] != '\n') {

      // config something
      if (info[i] == '{') {
        if (name[i - 1] == ' ')
          name[i - 1] = '\0';
        else
          name[i] = '\0';

        while (info[i] != '=')
          i++;
        i++;
        temp = i;
        flag = 1;
      }

      if (flag == 0) { // reading name
        name[i] = info[i];
      } else if (flag == 1) { // reading def
        def[i - temp] = info[i];
      }
      i++;
    }
    if (def[i - temp] == '\n')
      def[i - temp] = '\0';
    else
      def[i - temp + 1] = '\0';

    // craeting a node
    TList *new_node = malloc(sizeof(TList));
    strcpy(new_node->name, name);
    strcpy(new_node->definition, def);
    new_node->prev = node;
    node->next = new_node;
    node = new_node;
    for (int k = 0; k < 150; k++)
      name[k] = '\0';
    for (int k = 0; k < 600; k++)
      def[k] = '\0';
  }
  TList *new_head = head->next;
  free(head);
  return new_head;
}

TList *getDatePersonality(FILE *f) {
  if (f == NULL) {
    printf("Could Not Read File\n");
    return NULL;
  }
  rewind(f);
  char info[500];
  TList *head = (TList *)malloc(sizeof(TList));
  TList *node = head;

  while (fgets(info, 500, f)) {
    int i = 0;
    int keep = 0;
    while (info[i] != '\0') {
      if (info[i] == '=') {
        break;
      } else if (info[i] == ':') {
        keep = 1;
        break;
      }
      i++;
    }
    if (keep == 1) {
      continue;
    }
    i = 0;
    int flag = 0; // flag == 0 --> reading name | flag == 1 --> reading date of
                  // birth | flag == 2 --> reading date of death
    int temp;
    char name[100];
    char date_birth[20] = {0};
    char date_death[20] = {0};
    while (info[i] != '\0') {
      if (info[i] == '{') {
        flag = 1;
        if (info[i - 1] == ' ') {
          name[i - 1] = '\0';
        } else {
          name[i] = '\0';
        }
        i++;
        temp = i;
        continue;
      } else if (info[i] == '-') {
        flag = 2;
        date_birth[i - temp] = '\0';
        i++;
        temp = i;
        continue;
      } else if (info[i] == '}') {
        date_death[i - temp] = '\0';
        break;
      }
      if (flag == 0) {
        name[i] = info[i];
      } else if (flag == 1) {
        date_birth[i - temp] = info[i];
      } else if (flag == 2) {
        date_death[i - temp] = info[i];
      }
      i++;
    }

    TList *something = malloc(sizeof(TList));
    strcpy(something->name, name);
    strcpy(something->DoB, date_birth);
    strcpy(something->DoD, date_death);
    something->next = NULL;
    node->next = something;
    node = node->next;
  }
  TList *new_head = head->next;
  free(head);
  return new_head;
}

// Takes as an input a Linked list and a node with the date_of_birth in it. We
// have to fill in the rest of the gaps

void getInfoByDates(TList *s, TList *DoB) {
  for (TList *node = s; node; node = node->next) {
    if (strcmp(node->DoB, DoB->DoB) == 0) {
      strcpy(DoB->name, node->name);
      strcpy(DoB->definition, node->definition);
      strcpy(DoB->DoD, node->DoD);
      break;
    }
  }
  printf("Name: %s\n Definition: %s\n Date of birth: %s\n Date of death: %s\n",
         DoB->name, DoB->definition, DoB->DoB, DoB->DoD);
}

void getInfoByDates2(TList *s, TList *DoD) {
  int flag = 0;
  for (TList *node = s; node; node = node->next) {
    if (strcmp(node->DoD, DoD->DoD) == 0) {
      strcpy(DoD->name, node->name);
      strcpy(DoD->definition, node->definition);
      strcpy(DoD->DoB, node->DoB);
      flag = 1;
      break;
    }
  }
  if (flag == 0) {
    printf("No Such Personality. \n");
    return;
  }
  printf("Name: %s\n Definition: %s\n Date of birth: %s\n Date of death: %s\n",
         DoD->name, DoD->definition, DoD->DoB, DoD->DoD);
}

int compareAlpha(TList *n1, TList *n2) {
  return strcmp(n1->name, n2->name) > 0;
}

int compareLength(TList *n1, TList *n2) {
  return strlen(n1->name) > strlen(n2->name);
}
int compareAge(TList *n1, TList *n2) {
  int age1 = (n1->DoD[6] - '0') * 1000 + (n1->DoD[7] - '0') * 100 +
             (n1->DoD[8] - '0') * 10 + (n1->DoD[9] - '0') -
             (n1->DoB[0] - '0') * 1000 - (n1->DoB[1] - '0') * 100 -
             (n1->DoB[2] - '0') * 10 - (n1->DoB[3] - '0');
  int age2 = (n2->DoD[6] - '0') * 1000 + (n2->DoD[7] - '0') * 100 +
             (n2->DoD[8] - '0') * 10 + (n2->DoD[9] - '0') -
             (n2->DoB[0] - '0') * 1000 - (n2->DoB[1] - '0') * 100 -
             (n2->DoB[2] - '0') * 10 - (n2->DoB[3] - '0');
  return age1 > age2;
}
TList *
split(TList *head) { // returns a pointer to the head of the second linked list
  // getting the length of the LLL
  int length = 0;
  TList *node = head;
  while (node != NULL) {
    length++;
    node = node->next;
  }
  node = head;
  for (int i = 0; i < length / 2 - 1; i++) {
    node = node->next;
  }
  TList *temp = node->next;
  node->next = NULL;
  return temp;
}
TList *merge(TList *head1, TList *head2, int (*compare)(TList *, TList *)) {
  TList *node1 = head1;
  TList *node2 = head2;
  TList *head;
  if (compare(node1, node2)) {
    head = node1;
    node1 = node1->next;
  } else {
    head = node2;
    node2 = node2->next;
  }
  TList *new_head = head;
  while (node1 != NULL && node2 != NULL) {
    if (compare(node1, node2)) {
      new_head->next = node1;
      node1 = node1->next;
    } else {
      new_head->next = node2;
      node2 = node2->next;
    }
    new_head = new_head->next;
  }
  while (node1 != NULL) {
    new_head->next = node1;
    node1 = node1->next;
    new_head = new_head->next;
  }
  while (node2 != NULL) {
    new_head->next = node2;
    node2 = node2->next;
    new_head = new_head->next;
  }
  return head;
}
TList *sortWords(TList *syn) {
  if (syn->next == NULL) {
    return syn;
  }
  TList *head2 = split(syn);
  syn = sortWords(syn);
  head2 = sortWords(head2);
  TList *amazing_head = merge(syn, head2, compareAlpha);
  return amazing_head;
}

TList *sortWords2(TList *syn) {
  if (syn->next == NULL) {
    return syn;
  }
  TList *head2 = split(syn);
  syn = sortWords2(syn);
  head2 = sortWords2(head2);
  TList *amazing_head = merge(syn, head2, compareLength);
  return amazing_head;
}

TList *sortPersonality(TList *syn) {
  if (syn->next == NULL) {
    return syn;
  }
  TList *head2 = split(syn);
  syn = sortPersonality(syn);
  head2 = sortPersonality(head2);
  TList *amazing_head = merge(syn, head2, compareAge);
  return amazing_head;
}

TList *deletePersonality(FILE *f, TList *s, TList *a, char *name) {
  // looking for personality
  if (strcmp(s->name, name) == 0) {
    TList *temp = s;
    s = s->next;
    free(temp);
  } else {
    TList *node_s = s;
    while (node_s->next != NULL && strcmp(node_s->next->name, name) != 0) {
      node_s = node_s->next;
    }
    if (node_s->next == NULL) {
      printf("No such personality in the list\n");
    } else {
      printf("%s\n", node_s->next->name);
      TList *temp = node_s->next;
      node_s->next = temp->next;
      free(temp);
    }
  }
  if (strcmp(a->name, name) == 0) {
    TList *temp = a;
    a = a->next;
    free(temp);
  } else {
    TList *node_a = a;
    while (node_a->next != NULL && strcmp(node_a->next->name, name) != 0) {
      node_a = node_a->next;
    }
    printf("%s\n", node_a->name);
    if (node_a->next == NULL) {
      printf("No such personality in the list\n");
    } else {
      TList *temp = node_a->next;
      node_a->next = temp->next;
      free(temp);
    }
  }
  rewind(f);
  FILE *temp = fopen("temp.txt", "w");
  char *line = malloc(sizeof(char) * 300);
  while (fgets(line, 300, f)) {
    // getting name from file
    char *line_name = malloc(sizeof(char) * 100);
    int i = 0;
    while (line[i] != '=' && line[i] != '\0' && line[i] != '\n' &&
           line[i] != '{') {
      line_name[i] = line[i];
      i++;
    }
    if (line_name[i - 1] == ' ') {
      line_name[i - 1] = '\0';
    } else {
      line[i] = '\0';
    }
    if (strcmp(line_name, name) != 0) {
      fputs(line, temp);
    }
  }
  free(line);
  // copying everything in temp in f
  rewind(temp); // back to start of temp
  rewind(f);    // back to start of f
  // ftruncate(fileno(f), 0); // clear f's old content
  char buff[4096];
  size_t n;
  while ((n = fread(buff, 1, sizeof(buff), temp)) > 0) {
    fwrite(buff, 1, n, f);
  }
  fclose(f);
  fclose(temp);

  remove("sample.txt");
  rename("temp.txt", "sample.txt");
  return s;
}

TList *findPersonality(TList *head, char *name) {
  TList *node = head;
  for (; node; node = node->next) {
    if (strcmp(node->name, name) == 0) {
      return node;
    }
  }
  printf("There is no such personality!!\n");
  return NULL;
}

TList *updatePersonality(FILE *f, TList *s, TList *a, char *name,
                         char *definition, char *DoB, char *DoD) {
  TList *node_s = findPersonality(s, name);
  TList *node_a = findPersonality(a, name);

  strcpy(node_s->definition, definition);
  strcpy(node_s->DoB, DoB);
  strcpy(node_s->DoD, DoD);

  rewind(f);
  FILE *temp = fopen("temp.txt", "w");
  char *line = malloc(sizeof(char) * 300);
  while (fgets(line, 300, f)) {
    // getting name from file
    char *line_name = malloc(sizeof(char) * 100);
    int i = 0;
    while (line[i] != '=' && line[i] != '\0' && line[i] != '\n' &&
           line[i] != '{') {
      line_name[i] = line[i];
      i++;
    }
    if (line_name[i - 1] == ' ') {
      line_name[i - 1] = '\0';
    } else {
      line[i] = '\0';
    }
    if (strcmp(line_name, name) == 0) {
      fprintf(temp, "%s {%s-%s}= %s\n", name, DoB, DoD, definition);
    } else {
      fputs(line, temp);
    }
  }
  free(line);
  // copying everything in temp in f
  rewind(temp); // back to start of temp
  rewind(f);    // back to start of f
  // ftruncate(fileno(f), 0); // clear f's old content
  char buff[4096];
  size_t n;
  while ((n = fread(buff, 1, sizeof(buff), temp)) > 0) {
    fwrite(buff, 1, n, f);
  }
  fclose(f);
  fclose(temp);

  remove("sample.txt");
  rename("temp.txt", "sample.txt");
  return s;
}

TList *findPersonality_withDate(TList *head, char *DoB, char *DoD) {
  TList *node = head;
  for (; node; node = node->next) {
    if (strcmp(node->DoB, DoB) == 0 || strcmp(node->DoD, DoD) == 0) {
      return node;
    }
  }
  return NULL;
}

TList *copyNode(TList *node) {
  if (node == NULL) {
    return NULL;
  }
  TList *new_node = malloc(sizeof(TList));

  strcpy(new_node->name, node->name);
  strcpy(new_node->definition, node->definition);
  strcpy(new_node->DoB, node->DoB);
  strcpy(new_node->DoD, node->DoD);
  return new_node;
}
// assuming that the word is going to be the year of death +

TList *similarPersonality(TList *s, char *DoB, char *DoD) {
  TList *target = findPersonality_withDate(s, DoB, DoD);
  TList *node = copyNode(target);
  TList *head = node;
  while (node != NULL) {
    target = findPersonality_withDate(target->next, DoB, DoD);
    node->next = copyNode(target);
    node = node->next;
  }
  return head;
}

TList *countPersonality(TList *s, char *ptr) {
  TList *similar = similarPersonality(s, ptr, ptr);
  // Searching in definition
  TList *node = s;
  int length = strlen(ptr);
  for (; node; node = node->next) {
    for (int i = 0; node->definition[i] != '\0'; i++) {
      int flag = 1;
      for (int j = 0; j < length; j++) {
        if (node->definition[i + j] != ptr[j]) {
          flag = 0;
          break;
        }
      }
      if (flag == 1) {
        TList *new_node = copyNode(node);
        new_node->next = similar;
        similar = new_node;
        break;
      }
    }
  }

  return sortWords(similar);
}

void find_palindrome(char *text) {

  for (int i = 0; i < strlen(text); i++) {
    if (text[i] == text[i - 1]) { // if the palindrome is of even size
      int j = 1;
      while (text[i - j] == text[i + j - 1]) {
        j++;
      }
      j--;
      if (j < 3) {
        continue;
      }
      char result[2 * j];
      for (int k = j; k >= 1; k--) {
        result[j - k] = text[i - k];
      }
      for (int k = 0; k < j; k++) {
        result[j + k] = text[i + k];
      }
      i += j;
    } else if (text[i - 1] == text[i + 1]) { // if the palindrome is of odd size
      int j = 1;
      while (text[i - j] == text[i + j]) {
        j++;
      }
      j--;
      if (j < 10) {
        continue;
      }
      int size = 2 * j + 1;
      char result[size];
      for (int k = j; k >= 1; k--) {
        result[j - k] = text[i - k];
      }
      for (int k = 1; k <= j; k++) {
        result[j + k] = text[i + k];
      }
      result[j] = text[i];
      printf("Result: %s\n", result);
      i += j;
    }
  }
}

TList *palindromeName(TList *s) {
  // initializing the list

  TList *head = malloc(sizeof(TList));
  head->next = NULL;
  TList *node = s;
  TList *node_head = head;
  // traversing the list s
  while (node != NULL) {
    char *text = malloc(sizeof(char) * 500);
    strcpy(text, node->definition);
    for (int i = 0; i < strlen(text); i++) {
      if (text[i] == text[i - 1] &&
          text[i] != ' ') { // if the palindrome is of even size
        int j = 1;
        while (text[i - j] == text[i + j - 1] && text[i - j] != ' ') {
          j++;
        }
        if (text[i - j] != ' ' || text[i + j - 1] != ' ')
          continue;
        j--;
        if (j < 2)
          continue;
        int size = 2 * j;
        char *result = malloc(sizeof(char) * size);
        for (int k = j; k >= 1; k--) {
          result[j - k] = text[i - k];
        }
        for (int k = 0; k < j; k++) {
          result[j + k] = text[i + k];
        }
        TList *new_node = malloc(sizeof(TList));
        strcpy(new_node->name, result);
        node_head->next = new_node;
        node_head = node_head->next;
        node_head->next = NULL;
        free(result);
        i += j;
      } else if (text[i - 1] == text[i + 1] &&
                 text[i] != ' ') { // if the palindrome is of odd size

        int j = 1;
        while (text[i - j] == text[i + j] && text[i - j] != ' ') {
          j++;
        }
        if (text[i + j] != ' ' || text[i - j] != ' ')
          continue;
        j--;
        if (j < 2)
          continue;
        int size = 2 * j + 1;
        char *result = malloc(size * sizeof(char));
        for (int k = j; k >= 1; k--) {
          result[j - k] = text[i - k];
        }
        for (int k = 1; k <= j; k++) {
          result[j + k] = text[i + k];
        }
        result[j] = text[i];
        TList *new_node = malloc(sizeof(TList));
        strcpy(new_node->name, result);
        node_head->next = new_node;
        node_head = node_head->next;
        node_head->next = NULL;
        free(result);

        i += j;
      }
    }
    node = node->next;
  }
  TList *temp = head->next;
  free(head);
  TList *new_temp = sortWords2(temp);
  return new_temp;
}

TList *mergeNodes(TList *s, TList *a) {
  // sort both so that we find the same thing
  TList *new_s = sortWords(s);
  TList *new_a = sortWords(a);

  TList *head = malloc(sizeof(TList));
  TList *node = head;
  head->next = NULL;
  head->prev = NULL;

  while ((new_a != NULL) && (new_s != NULL)) {
    if (strcmp(new_s->name, new_a->name) > 0) {
      new_s = new_s->next;
    } else if (strcmp(new_s->name, new_a->name) < 0) {
      new_a = new_a->next;
    }

    TList *new_node = malloc(sizeof(TList));
    strcpy(new_node->name, new_s->name);
    strcpy(new_node->definition, new_s->definition);
    strcpy(new_node->DoB, new_a->DoB);
    strcpy(new_node->DoD, new_a->DoD);
    new_node->next = NULL;
    new_node->prev = node;
    node->next = new_node;
    node = new_node;

    new_s = new_s->next;
    new_a = new_a->next;
  }
  TList *temp = head->next;
  free(head);
  temp->prev = NULL;
  return temp;
}

TList *merge2Nodes(TList *s, TList *a) {
  // sort both so that we find the same thing
  TList *new_s = sortWords(s);
  TList *new_a = sortWords(a);

  TList *head = malloc(sizeof(TList));
  TList *node = head;
  head->next = NULL;
  head->prev = NULL;

  while ((new_a != NULL) && (new_s != NULL)) {
    if (strcmp(new_s->name, new_a->name) > 0) {
      new_s = new_s->next;
    } else if (strcmp(new_s->name, new_a->name) < 0) {
      new_a = new_a->next;
    }

    TList *new_node = malloc(sizeof(TList));
    strcpy(new_node->name, new_s->name);
    strcpy(new_node->definition, new_s->definition);
    strcpy(new_node->DoB, new_a->DoB);
    strcpy(new_node->DoD, new_a->DoD);
    new_node->next = NULL;
    new_node->prev = node;
    node->next = new_node;
    node = new_node;

    new_s = new_s->next;
    new_a = new_a->next;
  }
  TList *temp = head->next;
  free(head);
  temp->prev = node;
  node->next = temp;
  return temp;
}

TList **addPersonality(FILE *f, TList **s, TList **a, char *name, char *DoB,
                       char *DoD, char *definition) {
  // Creating the new two nodes

  TList *new_s = malloc(sizeof(TList));
  TList *new_a = malloc(sizeof(TList));

  // filling in the information

  strcpy(new_s->name, name);
  strcpy(new_s->definition, definition);
  strcpy(new_a->DoB, DoB);
  strcpy(new_a->DoD, DoD);

  new_s->next = *s;
  new_a->next = *a;

  *s = sortWords(new_s);
  *a = sortWords(new_a);

  rewind(f);
  FILE *temp = fopen("temp.txt", "w");
  char *line = malloc(sizeof(char) * 300);
  fprintf(temp, "%s {%s-%s}= %s\n", name, DoB, DoD, definition);
  while (fgets(line, 300, f)) {
    fputs(line, temp);
  }
  free(line);
  // copying everything in temp in f
  rewind(temp); // back to start of temp
  rewind(f);    // back to start of f
  // ftruncate(fileno(f), 0); // clear f's old content
  char buff[4096];
  size_t n;
  while ((n = fread(buff, 1, sizeof(buff), temp)) > 0) {
    fwrite(buff, 1, n, f);
  }
  fclose(f);
  fclose(temp);

  remove("sample.txt");
  rename("temp.txt", "sample.txt");
  return s;
}

TList *addEvent(FILE *f, TList *b, char *event, char *date, char *definition) {

  TList *new_b = malloc(sizeof(TList));
  strcpy(new_b->name, event);
  strcpy(new_b->definition, definition);
  strcpy(new_b->DoB, date);
  new_b->next = b;

  rewind(f);
  FILE *temp = fopen("temp.txt", "w");
  char *line = malloc(sizeof(char) * 300);
  fprintf(temp, "%s {%s}: %s\n", event, date, definition);
  while (fgets(line, 300, f)) {
    fputs(line, temp);
  }
  free(line);
  // copying everything in temp in f
  rewind(temp); // back to start of temp
  rewind(f);    // back to start of f
  // ftruncate(fileno(f), 0); // clear f's old content
  char buff[4096];
  size_t n;
  while ((n = fread(buff, 1, sizeof(buff), temp)) > 0) {
    fwrite(buff, 1, n, f);
  }
  fclose(f);
  fclose(temp);

  remove("sample.txt");
  rename("temp.txt", "sample.txt");
  return new_b;
}

TQueue *createQueue() {
  TQueue *q = malloc(sizeof(TList));
  q->head = NULL;
  q->tail = NULL;
  q->size = 0;
  return q;
}

void dequeue(TQueue *q) {
  TList *p = q->head;
  q->head = q->head->next;
  q->size--;
  free(p);
}

void enqueue(TQueue *q, TList *node) {
  if (q->head == NULL) {
    q->head = node;
    q->tail = node;
    q->size++;
    return;
  }
  q->tail->next = node;
  q->tail = node;
  q->size++;
}

int countWords(char *name) {
  int count = 0;

  for (int i = 0; name[i] != '\0'; i++) {
    if (name[i] == ' ' && name[i + 1] != '\0') {
      count++;
    }
  }
  return count + 1;
}

TQueue *sName(TList *s) {
  TQueue *q = createQueue();
  printf("test\n");
  TList *node = s;
  while (node != NULL) {
    // inserting node in the queue
    TList *qNode = copyNode(node);

    if (q->size == 0) {
      q->head = qNode;
      q->tail = q->head;
      node = node->next;
      q->size++;
    } else {
      int priority = countWords(node->name);
      TList *curr = q->head;
      TList *prev = NULL;
      while (curr != NULL && countWords(curr->name) >= priority) {
        prev = curr;
        curr = curr->next;
      }
      if (prev == NULL) {
        qNode->next = q->head;
        q->head = qNode;
      } else {
        prev->next = qNode;
        qNode->next = curr;
        if (curr == NULL) {
          q->tail = qNode;
        }
      }
      q->size++;
    }
    node = node->next;
  }
  return q;
}

TQueue *Page(TList *s) {
  TQueue *q = createQueue();
  TList *node = s;
  while (node != NULL) {
    // inserting node in the queue
    TList *qNode = copyNode(node);

    if (q->size == 0) {
      q->head = qNode;
      q->tail = q->head;
      node = node->next;
      q->size++;
    } else {
      TList *curr = q->head;
      TList *prev = NULL;
      while (curr != NULL && !compareAge(qNode, curr)) {
        prev = curr;
        curr = curr->next;
      }
      if (prev == NULL) {
        qNode->next = q->head;
        q->head = qNode;
      } else {
        prev->next = qNode;
        qNode->next = curr;
        if (curr == NULL) {
          q->tail = qNode;
        }
      }
      q->size++;
    }
    node = node->next;
  }
  return q;
}

TQueue *toQueue(TList *merged) {
  TQueue *q = createQueue();
  TList *node = merged;
  while (node != NULL) {
    // inserting node in the queue
    TList *qNode = copyNode(node);

    if (q->size == 0) {
      q->head = qNode;
      q->tail = q->head;
      node = node->next;
      q->size++;
    } else {
      int priority = countWords(node->name);
      TList *curr = q->head;
      TList *prev = NULL;
      while (curr != NULL && countWords(curr->name) >= priority) {
        prev = curr;
        curr = curr->next;
      }
      if (prev == NULL) {
        qNode->next = q->head;
        q->head = qNode;
      } else {
        prev->next = qNode;
        qNode->next = curr;
        if (curr == NULL) {
          q->tail = qNode;
        }
      }
      q->size++;
    }
    node = node->next;
  }
  return q;
}

/*
-----------------------display functions--------------------------
*/

void displayLLL(TList *header) {
  TList *node = header;

  while (node != NULL) {
    printf("(%s, %s) --> \n", node->name, node->definition);
    node = node->next;
  }
  printf("NULL\n");
}

void displayDateLLL(TList *header) {
  TList *node = header;

  while (node != NULL) {
    printf("(%s, Birth: %s, Death: %s) --> \n", node->name, node->DoB,
           node->DoD);
    node = node->next;
  }
  printf("NULL\n");
}

void FullDisplay(TList *header) {
  TList *node = header;
  printf("NULL\n");
  while (node != NULL) {
    printf("(%s, Birth: %s, Death: %s, Definition:%s) <--> \n", node->name,
           node->DoB, node->DoD, node->definition);
    node = node->next;
  }
  printf("NULL\n");
}
void displayCircular(TList *header) {
  TList *node = header->next;
  printf("(%s, Birth: %s, Death: %s, Definition:%s) <--> \n", header->name,
         header->DoB, header->DoD, header->definition);
  while (node != NULL && strcmp(header->name, node->name) != 0) {
    printf("(%s, Birth: %s, Death: %s, Definition:%s) <--> \n", node->name,
           node->DoB, node->DoD, node->definition);
    node = node->next;
  }
  printf("(%s, Birth: %s, Death: %s, Definition:%s) <--> \n", header->name,
         header->DoB, header->DoD, header->definition);
}
void displayQueue(TQueue *h) {
  TList *node = h->head;
  while (node->next != NULL) {
    printf("(%s, Birth: %s, Death: %s, Definition:%s) --> \n", node->name,
           node->DoB, node->DoD, node->definition);
    node = node->next;
  }
  printf("(%s, Birth: %s, Death: %s, Definition:%s) --> \n", node->name,
         node->DoB, node->DoD, node->definition);
  printf("NULL\n");
}
