#include<stdio.h>
#include<stdlib.h>

struct TList_person {
    char name[50];
    char def[300];
    char DoB[10];
    char DoD[10];
    struct TList_person* next;
};

typedef struct TList_person TList_person;


struct TList_event {
    char name[50];
    char def[300];
    char date[10];
    struct TList_event* next;
};

typedef struct TList_event TList_event;




