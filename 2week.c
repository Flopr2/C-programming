#include <stdio.h>

int main()
{
	int arr; //��ǰ��
	int stock[100]; //�԰����
	int sell[100]; //�Ǹż���
	int id; //��ǰ���̵�
	int in[100];

	printf("��ǰ ���� : ");
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