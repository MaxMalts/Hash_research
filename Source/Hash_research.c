#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Hash_Table.h"



unsigned int ConstHash(struct String value) {
	assert(value.string != NULL);
	
	return 1;
}


unsigned int WordLenHash(struct String value) {
	assert(value.string != NULL);

	return strlen(value.string) % (MAX_HASH + 1);
}


unsigned int WordSumHash(struct String value) {
	assert(value.string != NULL);

	unsigned int res = 0;
	for (int i = 0; i < strlen(value.string); ++i) {
		res += value.string[i];
	}

	return res % (MAX_HASH + 1);
}


unsigned int WordDivLenHash(struct String value) {
	assert(value.string != NULL);

	unsigned int len = strlen(value.string);
	unsigned int sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += value.string[i];
	}

	return sum / len % (MAX_HASH + 1);
}


unsigned int CycleShiftHash(struct String value) {
	assert(value.string != NULL);

	unsigned int len = strlen(value.string);
	unsigned int res = 0;
	for (int i = 0; i < len; ++i) {
		res ^= (unsigned int)value.string[i];

		unsigned int firstBit = res >> (sizeof(unsigned int) * 8 - 1);
		res = res << 1;
		res |= firstBit;
	}

	return res % (MAX_HASH + 1);
}


unsigned int Crc32Hash(struct String value) {
	unsigned int crc_table[256] = {0};
	unsigned int crc = 0;

	for (int i = 0; i < 256; i++) {
		crc = i;
		for (int j = 0; j < 8; j++)
			crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;

		crc_table[i] = crc;
	};

	crc = 0xFFFFFFFF;

	int len = strlen(value.string);
	unsigned char* curChar = &value.string[0];
	while (len--) {
		crc = crc_table[(crc ^ *curChar++) & 0xFF] ^ (crc >> 8);
	}

	return (crc ^ 0xFFFFFFFF) % (MAX_HASH + 1);
}



int FGetsNChars(FILE* fin, char buf[], int NChars, char spec[]) {
	assert(fin != NULL);
	assert(buf != NULL);
	assert(NChars >= 0);

	char formatStr[100] = "";
	sprintf(formatStr, "%%%d%ss", NChars, spec);

	return fscanf(fin, formatStr, buf);
}


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