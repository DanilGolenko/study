#include <stdio.h>
#include <locale.h>

int calculations() {
	setlocale(LC_CTYPE, "RUS");
	int n = 3;
	int L = 335;
	printf("Дано:\n%9d\n%9d\n%9s\n", n, L, "----");
	printf("Ответ:\n%-+4.2f", 1.*n/L);
	getchar();
}