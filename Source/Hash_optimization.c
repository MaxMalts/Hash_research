#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "Hash_Table.h"


#pragma GCC optimize("O3")


int constClocks = 0, wordLenClocks = 0, wordSumClocks = 0, wordDivLenClocks = 0, cycleShiftClocks = 0, crc32Clocks = 0;

unsigned int ConstHash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
	constClocks += clock() - tempClock;
	__asm__(".intel_syntax noprefix\n\t"
			"mov rax, 0\n\t"
			".att_syntax\n\t"
			:
			:
			: "rax");

	return 1;
}


unsigned int WordLenHash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
	unsigned int res = strlen(value.string);
	wordLenClocks += clock() - tempClock;

	return res % (MAX_HASH + 1);
}


unsigned int WordSumHash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
	unsigned int res = 0;
	for (int i = 0; i < strlen(value.string); ++i) {
		res += value.string[i];
	}
	wordSumClocks += clock() - tempClock;

	return res % (MAX_HASH + 1);
}


unsigned int WordDivLenHash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
	unsigned int len = strlen(value.string);
	unsigned int sum = 0;
	for (int i = 0; i < len; ++i) {
		sum += value.string[i];
	}

	unsigned int res = sum / len;

	wordDivLenClocks += clock() - tempClock;
	return res % (MAX_HASH + 1);
}


unsigned int CycleShiftHash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
	unsigned int len = strlen(value.string);
	unsigned int res = 0;
	for (int i = 0; i < len; ++i) {
		res ^= (unsigned int)value.string[i];

		unsigned int firstBit = res >> (sizeof(unsigned int) * 8 - 1);
		res = res << 1;
		res |= firstBit;
	}

	cycleShiftClocks += clock() - tempClock;
	return res % (MAX_HASH + 1);
}


unsigned int Crc32Hash(struct String value) {
	assert(value.string != NULL);

	int tempClock = clock();
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
	unsigned int res = (crc ^ 0xFFFFFFFF);

	crc32Clocks += clock() - tempClock;
	return res % (MAX_HASH + 1);
}



int FGetsNChars(FILE* fin, char buf[], int NChars, char spec[]) {
	assert(fin != NULL);
	assert(buf != NULL);
	assert(NChars >= 0);

	char formatStr[100] = "";
	sprintf(formatStr, "%%%d%ss", NChars, spec);

	return fscanf(fin, formatStr, buf);
}


void CalcHashs(FILE* fin) {
	assert(fin != NULL);

	const char nonWordChars[] = ".,-:;!?()\"' \r\n";

	struct String curStr = {""};
	char specStr[sizeof(nonWordChars) + 3] = "";
	sprintf(specStr, "[^%s]", nonWordChars);

	int charsRead = FGetsNChars(fin, curStr.string, MAX_WORD_LEN, specStr);
	int i = 0;
	while (charsRead != EOF) {

		ConstHash(curStr);
		ConstHash(curStr);
		WordLenHash(curStr);
		WordSumHash(curStr);
		WordDivLenHash(curStr);
		CycleShiftHash(curStr);
		Crc32Hash(curStr);

		while (strchr(nonWordChars, fgetc(fin))) {
			continue;
		}
		if (!feof(fin)) {
			fseek(fin, -1, SEEK_CUR);
		}
		charsRead = FGetsNChars(fin, curStr.string, MAX_WORD_LEN, specStr);
		++i;
	}
}


int main() {
	const char finName[] = "Data/text.txt";

	FILE* fin = fopen(finName, "r");
	assert(fin != NULL);

	for (int i = 0; i < 300; ++i) {
		CalcHashs(fin);
		fseek(fin, 0, SEEK_SET);
	}
	fclose(fin);

	printf("const: %fs,\n"
		"wordLen: %fs,\n"
		"wordSum: %fs,\n"
		"wordDivLen: %fs,\n"
		"cycleShift: %fs,\n"
		"crc32: %fs\n", (float)constClocks / CLOCKS_PER_SEC, (float)wordLenClocks / CLOCKS_PER_SEC, (float)wordSumClocks / CLOCKS_PER_SEC,
		(float)wordDivLenClocks / CLOCKS_PER_SEC, (float)cycleShiftClocks / CLOCKS_PER_SEC, (float)crc32Clocks / CLOCKS_PER_SEC);

	printf("total: %fs\n", (float)clock() / CLOCKS_PER_SEC);
	return 0;
}