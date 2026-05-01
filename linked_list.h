#ifndef LINKED_LISTS_H
#define LINKED_LISTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ─────────────────────────────────────────────
   Structures
───────────────────────────────────────────── */

struct TList {
    char name[50];
    char definition[300];
    char DoB[10];
    char DoD[10];
    struct TList* next;
    struct TList* prev;
};
typedef struct TList TList;

struct TQueue {
    TList* head;
    TList* tail;
    int size;
};
typedef struct TQueue TQueue;

/* ─────────────────────────────────────────────
   File Parsing
───────────────────────────────────────────── */

/* Reads personalities (name + definition) from file f.
   Returns a linked list of TList nodes. */
TList* getPersonality(FILE* f);

/* Reads personalities (name + DoB + DoD) from file f.
   Returns a linked list of TList nodes. */
TList* getDatePersonality(FILE* f);

/* ─────────────────────────────────────────────
   Search
───────────────────────────────────────────── */

/* Fills in DoB->name, definition, DoD by matching DoB->DoB in list s.
   Prints the result. */
void getInfoByDates(TList* s, TList* DoB);

/* Fills in DoD->name, definition, DoB by matching DoD->DoD in list s.
   Prints the result. */
void getInfoByDates2(TList* s, TList* DoD);

/* Returns the node in head whose name matches name, or NULL. */
TList* findPersonality(TList* head, char* name);

/* Returns the first node matching DoB or DoD, or NULL. */
TList* findPersonality_withDate(TList* head, char* DoB, char* DoD);

/* Returns a list of all nodes sharing DoB or DoD with the given strings. */
TList* similarPersonality(TList* s, char* DoB, char* DoD);

/* Returns a sorted list of personalities whose DoB/DoD or definition
   contains ptr. */
TList* countPersonality(TList* s, char* ptr);

/* ─────────────────────────────────────────────
   Sorting Comparators
───────────────────────────────────────────── */

/* Returns 1 if n1->name comes before n2->name alphabetically. */
int compareAlpha(TList* n1, TList* n2);

/* Returns 1 if n1->name is longer than n2->name. */
int compareLength(TList* n1, TList* n2);

/* Returns 1 if n1 is older than n2 (larger age = DoD year - DoB year). */
int compareAge(TList* n1, TList* n2);

/* ─────────────────────────────────────────────
   Sorting (Merge Sort)
───────────────────────────────────────────── */

/* Splits the list at the midpoint; returns the head of the second half. */
TList* split(TList* head);

/* Merges two sorted lists using the given comparator. */
TList* merge(TList* head1, TList* head2, int (*compare)(TList*, TList*));

/* Sorts by name alphabetically. */
TList* sortWords(TList* syn);

/* Sorts by name length (longest first). */
TList* sortWords2(TList* syn);

/* Sorts by age (oldest first). */
TList* sortPersonality(TList* syn);

/* ─────────────────────────────────────────────
   CRUD Operations
───────────────────────────────────────────── */

/* Adds a new personality to lists s and a, and writes it to file f.
   Returns updated pointer to s. */
TList** addPersonality(FILE* f, TList** s, TList** a, char* name, char* DoB, char* DoD, char* definition);

/* Adds a new event to list b and writes it to file f.
   Returns the updated head of b. */
TList* addEvent(FILE* f, TList* b, char* event, char* date, char* definition);

/* Deletes a personality by name from lists s and a, and from file f.
   Returns the updated head of s. */
TList* deletePersonality(FILE* f, TList* s, TList* a, char* name);

/* Updates a personality's definition, DoB, DoD in lists s and a,
   and in file f. Returns the updated head of s. */
TList* updatePersonality(FILE* f, TList* s, TList* a, char* name, char* definition, char* DoB, char* DoD);

/* ─────────────────────────────────────────────
   List Utilities
───────────────────────────────────────────── */

/* Returns a deep copy of a single node (next and prev are not copied). */
TList* copyNode(TList* node);

/* Merges list s (name + definition) with list a (DoB + DoD) by matching
   names. Returns a new doubly linked list. */
TList* mergeNodes(TList* s, TList* a);

/* Same as mergeNodes but makes the result circular. */
TList* merge2Nodes(TList* s, TList* a);

/* ─────────────────────────────────────────────
   Palindrome
───────────────────────────────────────────── */

/* Scans text for palindromes and prints any found (used internally). */
void find_palindrome(char* text);

/* Returns a sorted list (by name length) of palindromes found in the
   definition fields of list s. */
TList* palindromeName(TList* s);

/* ─────────────────────────────────────────────
   Queue
───────────────────────────────────────────── */

/* Allocates and returns an empty queue. */
TQueue* createQueue();

/* Returns the number of space-separated words in name. */
int countWords(char* name);

/* Returns a priority queue of s sorted by word count in the name
   (fewest words = highest priority / front). */
TQueue* sName(TList* s);

/* Returns a priority queue of s sorted by age (oldest first). */
TQueue* Page(TList* s);

/* Converts a merged list into a queue sorted by word count. */
TQueue* toQueue(TList* merged);

/* ─────────────────────────────────────────────
   Display
───────────────────────────────────────────── */

/* Prints name and definition for each node. */
void displayLLL(TList* header);

/* Prints name, DoB, DoD for each node. */
void displayDateLLL(TList* header);

/* Prints all fields for each node (singly linked). */
void FullDisplay(TList* header);

/* Prints all fields for each node (doubly linked / circular). */
void displayCircular(TList* header);

/* Prints all fields for each node in the queue. */
void displayQueue(TQueue* h);

#endif /* LINKED_LISTS_H */