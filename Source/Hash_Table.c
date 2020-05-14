#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Hash_Table.h"



struct HashTable HashTable_Constructor() {
	struct HashTable res = {{NULL}};

	for (int i = 0; i <= MAX_HASH; ++i) {
		struct List* curList = (struct List*)calloc(1, sizeof(struct List));
		res.hashList[i] = curList;
	}

	return res;
}


void HashTable_AddElem(struct HashTable* hashTable, HashTable_value_t value, unsigned int (*HashFunc)(HashTable_value_t)) {
	assert(hashTable != NULL);

	unsigned int hash = HashFunc(value);

	List_PushBack(hashTable->hashList[hash], value);
}


int HashTable_IthListSize(struct HashTable* hashTable, int i) {
	assert(hashTable != NULL);

	return hashTable->hashList[i]->size;
}


int HashTable_ElemExists(struct HashTable* hashTable, HashTable_value_t value, unsigned int (*HashFunc)(HashTable_value_t)) {
	assert(hashTable != NULL);

	unsigned int hash = HashFunc(value);

	return List_ElemExists(hashTable->hashList[hash], value);
}