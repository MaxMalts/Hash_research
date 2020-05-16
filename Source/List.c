#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "List.h"



int Strcmp(char* str1, char* str2) {
	assert(str1 != NULL);
	assert(str2 != NULL);

#ifndef ACCELERATE


	int res = 0;
	//__asm__ volatile (".intel_syntax noprefix \n"
	//		
	//		"movbe rcx, [%1]          \n"
	//		"movbe rdx, [%2]          \n"

	//		"cmp rcx, rdx             \n"
	//		"ja .above                \n"
	//		"jb .below                \n"

	//		"movbe rcx, [%1 + 8]      \n"
	//		"movbe rdx, [%2 + 8]      \n"

	//		"cmp rcx, rdx             \n"
	//		"ja .above                \n"
	//		"jb .below                \n"
	//		"xor %0, %0               \n"
	//		"jmp .end                 \n"

	//		".above:                  \n"
	//		"mov %0, 1                \n"
	//		"jmp .end                 \n"

	//		".below:                  \n"
	//		"mov %0, -1               \n"
	//		".end:                    \n"

	//		".att_syntax              \n"
	//		: "=a" (res)
	//		: "b" (str1), "r" (str2)
	//		: "rcx", "rdx");

	__asm__ volatile (".intel_syntax noprefix       \n"
					  "xor rax, rax                 \n"
					  ".cmp_loop:                   \n"
					  "cmpsb                        \n"
					  "jne .end                     \n"
					  "dec rsi                      \n"
					  "lodsb                        \n"
					  "cmp al, 0                    \n"
					  "jne .cmp_loop                \n"

					  ".end:                        \n"
					  "xor ebx, ebx                 \n"
					  "mov bl, [rsi - 1]            \n"
					  "sub bl, [rdi - 1]            \n"

					  ".att_syntax                  \n"
					  : "=b"(res)
					  : "S"(str1), "D" (str2)
					  : "rax");
	//#undef strcmp
	//	if (res == 0 && strcmp(bstr1, bstr2) != 0 || res != 0 && strcmp(bstr1, bstr2) == 0) {
	//		printf("there %d", res);
	//	}
	//#define strcmp strcmp_fast
	return res;
#else
	return strcmp(str1, str2);
#endif
}


void StringCpy(struct String* dest, struct String* src) {
	strcpy(dest->string, src->string);
}


struct List List_Contructor() {
	struct List res = {NULL, 0};

	return res;
}


void List_PushBack(struct List* list, List_value_t val) {
	assert(list->size >= 0);

	struct List_node* newNode = (struct List_node*)calloc(1, sizeof(struct List_node));
	StringCpy(&newNode->value, &val);

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


int List_ElemExists(struct List* list, List_value_t val) {
	assert(list != NULL);

	struct List_node* curNode = list->head;
	while (curNode != NULL) {
		if (Strcmp(curNode->value.string, val.string) == 0) {
			return 1;
		}

		curNode = curNode->nextNode;
	}

	return 0;
}