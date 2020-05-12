#pragma once
#include "List.h"

#define MAX_HASH 1500


typedef struct String HashTable_value_t;

struct HashTable {
	struct List* hashList[MAX_HASH + 1];
};


struct HashTable HashTable_Constructor();


void HashTable_AddElem(struct HashTable* hashTable, HashTable_value_t value, unsigned int (*HashFunc)(HashTable_value_t));


int HashTable_IthListSize(struct HashTable* hashTable, int i);