#include <stdio.h>

#define NUM_PRODUCTS 5

int main()
{
	int inventory[NUM_PRODUCTS] = { 0 };
	int sales[NUM_PRODUCTS] = { 0 };
	int choice;

	while (1)
	{
		printf("\n>> 원하는 메뉴를 선택하세요 (1.입고, 2.판매, 3.상품현황, 4.종료)\n");
		printf("> ");
		scanf_s("%d", &choice);

		switch (choice)
		{
		case 1:
		{
			int type;
			printf("입고 방식 선택 : 1. 전체 상품, 2. 개별 상품\n> ");
			scanf_s("%d", &type);

			if (type == 1)
			{
				printf(">>> 5개 상품의 입고 수량을 순서대로 입력하세요 (예: 10 20 30 40 50)\n> ");
				for (int i = 0; i < NUM_PRODUCTS; i++)
				{
					int quantity;
					scanf_s("%d", &quantity);
					inventory[i] += quantity;
				}
				printf(">>> 전체 상품 입고가 완료되었습니다.\n");
			}
			else if (type == 2)
			{
				int id, quantity;
				printf(">>> 상품 ID (1-%d)를 입력하세요: ", NUM_PRODUCTS);
				scanf_s("%d", &id);

				if (id >= 1 && id <= NUM_PRODUCTS)
				{
					printf(">>> 해당 상품의 입고 수량을 입력하세요: ");
					scanf_s("%d", &quantity);
					inventory[id - 1] += quantity;
					printf(">>> 상품 %d의 입고가 완료되었습니다.\n", id);
				}
				else
				{
					printf(">>> 잘못된 상품 ID입니다.\n");
				}
			}
			break;
		}
		case 2:
		{
			int id, quantity;
			printf(">>> 판매할 상품의 ID (1-%d)를 입력하세요: ", NUM_PRODUCTS);
			scanf_s("%d", &id);

			if (id >= 1 && id <= NUM_PRODUCTS)
			{
				printf(">>> 판매할 수량을 입력하세요: ");
				scanf_s("%d", &quantity);

				if (inventory[id - 1] >= quantity)
				{
					inventory[id - 1] -= quantity;
					sales[id - 1] += quantity;
					printf(">>> 상품 %d가 %d개 판매되었습니다. (남은 재고: %d)\n", id, quantity, inventory[id - 1]);
				}
				else
				{
					printf(">>> [오류] 재고가 부족합니다! (상품 %d 현재 재고: %d)\n", id, inventory[id - 1]);
				}
			}
			else
			{
				printf(">>> 잘못된 상품 ID입니다.\n");
			}
			break;
		}
		case 3:
		{
			printf("\n");
			printf("재고수량 : ");
			for (int i = 0; i < NUM_PRODUCTS; i++)
			{
				printf("%d ", inventory[i]);
			}
			printf("\n");

			int tsales = 0;
			int tinven = 0;
			for (int i = 0; i < NUM_PRODUCTS; i++)
			{
				tsales += sales[i];
				tinven += inventory[i];
			}

			double sales_rate = 0.0;
			int total_stock_ever = tsales + tinven;
			if (total_stock_ever > 0)
			{
				sales_rate = (double)tsales / total_stock_ever * 100.0;
			}
			printf("총 판매량 : %d (판매율 %.2f%%)\n", tsales, sales_rate);

			if (tsales > 0)
			{
				int msales = sales[0];
				int misales = sales[0];
				int mid = 1;
				int miid = 1;

				for (int i = 1; i < NUM_PRODUCTS; i++)
				{
					if (sales[i] > msales)
					{
						msales = sales[i];
						mid = i + 1;
					}
					if (sales[i] < misales)
					{
						misales = sales[i];
						miid = i + 1;
					}
				}
				printf("가장 많이 판매된 상품 : ID %d, 판매량 %d\n", mid, msales);
				printf("가장 적게 판매된 상품 : ID %d, 판매량 %d\n", miid, misales);
			}
		

			const int LOW_STOCK_THRESHOLD = 3;
			for (int i = 0; i < NUM_PRODUCTS; i++)
			{
				if (inventory[i] < LOW_STOCK_THRESHOLD)
				{
					printf("상품 ID %d : 재고부족(%d)\n", i + 1, inventory[i]);
				}
			}
			break;
		}
		case 4:
		{
			printf("프로그램을 종료합니다.\n");
			return 0;
		}
		}
	}
}
