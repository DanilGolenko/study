#include <locale.h>
#include <stdio.h>

void main() {
	setlocale(LC_CTYPE, "RUS");
	int A;
	int B;
	puts("Введите первую сторону:");
	scanf("%d", &A);
	puts("Введите вторую сторону:");
	scanf("%d", &B);
	printf("Прямоугольник со сторнами %d и %d\nПериметр прямоугольник: %d\nПлощадь прямоугольник: %d", A, B, A * 2 + B * 2, A * B);
	getchar();
}