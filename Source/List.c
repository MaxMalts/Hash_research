#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.h"


void StringCpy(struct String dest, struct String src) {
	strcpy(dest.string, src.string);
}


struct List List_Contructor() {
	struct List res = {NULL, 0};

	return res;
}


void List_PushBack(struct List* list, List_value_t val) {
	assert(list->size >= 0);

	struct List_node* newNode = (struct List_node*)calloc(1, sizeof(struct List_node));
	StringCpy(newNode->value, val);

	if (list->size == 0) {
		list->head = newNode;
		list->tail = newNode;
		++list->size;
		return;
	}

	list->tail->nextNode = newNode;
	list->tail = newNode;
	++list->size;
}