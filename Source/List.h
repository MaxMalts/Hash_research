#pragma once


#define MAX_WORD_LEN 100

struct String {
	char string[MAX_WORD_LEN + 1];
};

void StringCpy(struct String dest, struct String src);


typedef struct String List_value_t;

struct List_node {
	List_value_t value;

	struct List_node* nextNode;
};

struct List {
	struct List_node* head;
	struct List_node* tail;
	int size;
};


struct List List_Contructor();


void List_PushBack(struct List* list, List_value_t val);