#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "Hash_Table.h"
#include "Hash_Funcs.h"
#include "Addit_Funcs.h"



void CalcHashs(FILE* fin) {
	assert(fin != NULL);

	const char letters[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

	unsigned int (*HashFuncs[])(struct String) = {CycleShiftHash, Crc32Hash};

	int NFuncs = sizeof(HashFuncs) / sizeof(HashFuncs[0]);

	struct String curStr = {""};
	char specStr[sizeof(letters) + 3] = "";
	sprintf(specStr, "[%s]", letters);

	float times[NFuncs];

	for (int i = 0; i < NFuncs; ++i) {

		clock_t tempClock = clock();
		for (int j = 0; j < 20; ++j) {
			int charsRead = FGetsNChars(fin, curStr.string, MAX_WORD_LEN, specStr);
			while (charsRead != EOF) {

				HashFuncs[i](curStr);

				char lastCh = fgetc(fin);
				while (!strchr(letters, lastCh) && !(lastCh == EOF)) {
					lastCh = fgetc(fin);
					continue;
				}
				curStr.string[0] = lastCh;
				charsRead = FGetsNChars(fin, curStr.string + 1, MAX_WORD_LEN - 1, specStr);
			}
			fseek(fin, 0, SEEK_SET);
		}
		times[i] = (float)(clock() - tempClock) / CLOCKS_PER_SEC;
	}

	printf("cycleShift: %fs,\n"
		   "crc32: %fs\n", times[0], times[1]);
}


void FindWords(FILE* textF, FILE* wordsF) {
	assert(textF != NULL);

	const char letters[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";

	struct HashTable constHash = HashTable_Constructor();

	struct String curStr = {""};
	char specStr[sizeof(letters) + 3] = "";
	sprintf(specStr, "[%s]", letters);

	int charsRead = FGetsNChars(textF, curStr.string, MAX_WORD_LEN, specStr);
	while (charsRead != EOF) {

		if (!HashTable_ElemExists(&constHash, curStr, ConstHash)) {
			HashTable_AddElem(&constHash, curStr, ConstHash);
		}

		memset(curStr.string, 0, MAX_WORD_LEN + 1);
		char lastCh = fgetc(textF);
		while (!strchr(letters, lastCh) && !(lastCh == EOF)) {
			lastCh = fgetc(textF);
			continue;
		}
		curStr.string[0] = lastCh;
		charsRead = FGetsNChars(textF, curStr.string + 1, MAX_WORD_LEN - 1, specStr);
	}

	clock_t tempClock = clock();
	memset(curStr.string, 0, MAX_WORD_LEN + 1);
	charsRead = FGetsNChars(wordsF, curStr.string, MAX_WORD_LEN, specStr);
	while (charsRead != EOF) {

		HashTable_ElemExists(&constHash, curStr, ConstHash);

		memset(curStr.string, 0, MAX_WORD_LEN + 1);
		char lastCh = fgetc(wordsF);
		while (!strchr(letters, lastCh) && !(lastCh == EOF)) {
			lastCh = fgetc(wordsF);
			continue;
		}
		curStr.string[0] = lastCh;
		charsRead = FGetsNChars(wordsF, curStr.string + 1, MAX_WORD_LEN - 1, specStr);
	}
	float searchTime = (float)(clock() - tempClock) / CLOCKS_PER_SEC;
	printf("search time: %f\n", searchTime);
}


int main() {
	const char finName[] = "Data/text.txt";
	const char wordsFName[] = "Data/words.txt";

	FILE* fin = fopen(finName, "r");
	assert(fin != NULL);
	FILE* wordsF = fopen(wordsFName, "r");
	assert(wordsF != NULL);

	clock_t tempClock = clock();
	CalcHashs(fin);
	FindWords(fin, wordsF);
	fclose(fin);

	float totalTime = (float)(clock() - tempClock) / CLOCKS_PER_SEC;
	printf("total time: %f\n", totalTime);

	return 0;
}