#include <stdio.h>

int main()
{
	int arr; //상품수
	int stock[100]; //입고수량
	int sell[100]; //판매수량
	int id; //상품아이디
	int in[100];

	printf("상품 개수 : ");
	scanf_s("%d",&arr);

	for (int i = 0;i < arr; i++) {
		scanf_s("%d", &stock[i]);
	}
	for (int i = 0; i < arr; i++) {
		scanf_s("%d", &sell[i]);
	}
	for (int i = 0; i < arr; i++) {
		in[i] = stock[i] - sell[i];
	}
	scanf_s("%d", &id);

	printf("%d\n", in[id - 1]);

	for (int i = 0; i < arr; i++) {
		printf("%d", in[i]);
	}
}
