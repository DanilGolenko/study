#include <locale.h>
#include <stdio.h>

void main() {
	setlocale(LC_CTYPE, "RUS");
	int A;
	int B;
	puts("������� ������ �������:");
	scanf("%d", &A);
	puts("������� ������ �������:");
	scanf("%d", &B);
	printf("������������� �� �������� %d � %d\n�������� �������������: %d\n������� �������������: %d", A, B, A * 2 + B * 2, A * B);
	getchar();
}