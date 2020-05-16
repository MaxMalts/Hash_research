#pragma once

size_t strlen_fast(char* string) {
	assert(string != NULL);

	size_t res = 0;
	__asm__ volatile (".intel_syntax noprefix    \n"
					  "mov rax, %1               \n"
					  "dec rax                   \n"
					  ".strlen_loop:             \n"
					  "inc rax                   \n"
					  "cmp byte ptr [rax], 0     \n"
					  "jne .strlen_loop          \n"
					  "mov %0, rax               \n"
					  "sub %0, %1                \n"
					  ".att_syntax               \n"
					  : "=r" (res)
					  : "b" (string)
					  : "rax");

	return res;
}


int FGetsNChars(FILE* fin, char buf[], int NChars, char spec[]) {
	assert(fin != NULL);
	assert(buf != NULL);
	assert(NChars >= 0);

	char formatStr[100] = "";
	sprintf(formatStr, "%%%d%ss", NChars, spec);

	return fscanf(fin, formatStr, buf);
}