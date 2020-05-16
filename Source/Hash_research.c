#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Hash_Table.h"
#include "Hash_Funcs.h"
#include "Addit_Funcs.h"



struct HashTables {
	struct HashTable constHash;
	struct HashTable wordLenHash;
	struct HashTable wordSumHash;
	struct HashTable wordDivLenHash;
	struct HashTable cycleShiftHash;
	struct HashTable crc32Hash;
};

struct HashTables ReadWords(FILE* fin) {
	assert(fin != NULL);

	const char letters[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

	struct HashTable constHash = HashTable_Constructor();
	struct HashTable wordLenHash = HashTable_Constructor();
	struct HashTable wordSumHash = HashTable_Constructor();
	struct HashTable wordDivLenHash = HashTable_Constructor();
	struct HashTable cycleShiftHash = HashTable_Constructor();
	struct HashTable crc32Hash = HashTable_Constructor();


	struct String curStr = {""};
	char specStr[sizeof(letters) + 3] = "";
	sprintf(specStr, "[%s]", letters);

	int charsRead = FGetsNChars(fin, curStr.string, MAX_WORD_LEN, specStr);
	int i = 0;
	while (charsRead != EOF) {

		if (!HashTable_ElemExists(&constHash, curStr, ConstHash)) {
			HashTable_AddElem(&constHash, curStr, ConstHash);
			HashTable_AddElem(&wordLenHash, curStr, WordLenHash);
			HashTable_AddElem(&wordSumHash, curStr, WordSumHash);
			HashTable_AddElem(&wordDivLenHash, curStr, WordDivLenHash);
			HashTable_AddElem(&cycleShiftHash, curStr, CycleShiftHash);
			HashTable_AddElem(&crc32Hash, curStr, Crc32Hash);
		}

		memset(curStr.string, 0, MAX_WORD_LEN + 1);
		char lastCh = fgetc(fin);
		while (!strchr(letters, lastCh) && !(lastCh == EOF)) {
			lastCh = fgetc(fin);
			continue;
		}
		curStr.string[0] = lastCh;
		charsRead = FGetsNChars(fin, curStr.string + 1, MAX_WORD_LEN - 1, specStr);
	}

	struct HashTables res = {constHash, wordLenHash, wordSumHash, wordDivLenHash, cycleShiftHash, crc32Hash};
	return res;
}


void WriteLengths(FILE* fout, struct HashTables hashTables) {
	assert(fout != NULL);

	int NTables = sizeof(hashTables) / sizeof(struct HashTable);

	struct HashTable* curTable = &hashTables.constHash;
	for (int i = 0; i < NTables; ++i) {
		for (int j = 0; j <= MAX_HASH; ++j) {
			int curLen = HashTable_IthListSize(curTable, j);
			fprintf(fout, "%d, ", curLen);
		}

		fprintf(fout, "\n");
		++curTable;
	}
}


int main() {
	const char finName[] = "Data/text.txt";
	const char foutName[] = "Data/lengths.csv";


	FILE* fin = fopen(finName, "r");
	assert(fin != NULL);

	struct HashTables hashTables = ReadWords(fin);
	fclose(fin);
	

	FILE* fout = fopen(foutName, "w");
	assert(fout != NULL);

	WriteLengths(fout, hashTables);
	fclose(fout);


	return 0;
}