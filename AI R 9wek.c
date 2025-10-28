#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int product_id;
    char name[100];
    int price;
    int stock;
    int sales_quantity;
    long total_sales;
    int is_active;
} Product;

#define MAX_PRODUCTS 5
#define MAX_SAVE_SLOTS 3
#define FILENAME_FORMAT "products_%d.dat"

// 입력 버퍼를 비우는 매크로
#define FLUSH_INPUT_BUFFER() while (getchar() != '\n')

Product products[MAX_PRODUCTS];
int product_count = 0;

int find_product_index(int id) {
    for (int i = 0; i < MAX_PRODUCTS; i++) {
        if (products[i].is_active && products[i].product_id == id) {
            return i;
        }
    }
    return -1;
}

void get_filename_for_slot(int slot_number, char* filename_buffer) {
    sprintf(filename_buffer, FILENAME_FORMAT, slot_number);
}

int check_slot_status(int slot_number) {
    char filename[50];
    get_filename_for_slot(slot_number, filename);
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        return 0; // 비어있음
    } else {
        fclose(fp);
        return 1; // 저장됨
    }
}

void display_slot_status() {
    printf("--- [저장 슬롯 목록] ---\n");
    for (int i = 1; i <= MAX_SAVE_SLOTS; i++) {
        const char* status = check_slot_status(i) ? "저장됨" : "비어있음";
        printf("  %d. 슬롯 %d (%s)\n", i, i, status);
    }
    printf("-------------------------\n");
}

void save_products_to_file() {
    int slot_number = 0;
    char filename[50];

    printf("--- 5. 상품 정보 저장 ---\n");
    display_slot_status();
    
    while (1) {
        printf("저장할 슬롯 번호를 선택하세요 (1-%d): ", MAX_SAVE_SLOTS);
        if (scanf("%d", &slot_number) != 1 || slot_number < 1 || slot_number > MAX_SAVE_SLOTS) {
            printf(" 오류: 1에서 %d 사이의 숫자만 입력하세요.\n", MAX_SAVE_SLOTS);
            FLUSH_INPUT_BUFFER();
        } else {
            break;
        }
    }
    FLUSH_INPUT_BUFFER();

    get_filename_for_slot(slot_number, filename);
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf(" 오류: %d번 슬롯 파일(%s)을 열 수 없습니다.\n", slot_number, filename);
        return;
    }

    fwrite(&product_count, sizeof(int), 1, fp);
    fwrite(products, sizeof(Product), MAX_PRODUCTS, fp);
    fclose(fp);
    printf("\n--- %d번 슬롯에 상품 정보 저장 완료 ---\n", slot_number);
}

void load_products_from_file() {
    int slot_number = 0;
    char filename[50];

    printf("--- 6. 상품 정보 불러오기 ---\n");
    display_slot_status();

    while (1) {
        printf("불러올 슬롯 번호를 선택하세요 (1-%d): ", MAX_SAVE_SLOTS);
        if (scanf("%d", &slot_number) != 1 || slot_number < 1 || slot_number > MAX_SAVE_SLOTS) {
            printf(" 오류: 1에서 %d 사이의 숫자만 입력하세요.\n", MAX_SAVE_SLOTS);
            FLUSH_INPUT_BUFFER();
        } else if (check_slot_status(slot_number) == 0) {
             printf(" 오류: %d번 슬롯은 비어있습니다. 다른 슬롯을 선택하세요.\n", slot_number);
             FLUSH_INPUT_BUFFER();
        } else {
            break;
        }
    }
    FLUSH_INPUT_BUFFER();

    get_filename_for_slot(slot_number, filename);
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf(" 오류: %d번 슬롯 파일(%s)을 열 수 없습니다.\n", slot_number, filename);
        return;
    }

    // 불러오기 전 초기화
    product_count = 0;
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].is_active = 0;

    if (fread(&product_count, sizeof(int), 1, fp) != 1) {
        printf(" 오류: 파일 불러오기 실패 (count)\n");
        fclose(fp);
        return;
    }
    if (fread(products, sizeof(Product), MAX_PRODUCTS, fp) != MAX_PRODUCTS) {
        printf(" 오류: 파일 불러오기 실패 (data)\n");
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("\n--- %d번 슬롯에서 상품 정보 불러오기 완료 ---\n", slot_number);
}

void inventory_in() {
    int id, in_qty, price, index;
    char name_buffer[100];

    printf("상품ID: ");
    if (scanf("%d", &id) != 1) {
        printf("오류: 상품 ID 입력 오류입니다.\n");
        FLUSH_INPUT_BUFFER();
        return;
    }
    FLUSH_INPUT_BUFFER(); // ID 입력 후 버퍼 비우기 (상품명 fgets 때문)

    index = find_product_index(id);

    if (index != -1) {
        printf("기존 상품 (ID: %d, 상품명: %s)이 발견되었습니다. 입고량을 추가합니다.\n", id, products[index].name);
        printf("  - 입고량: ");
        if (scanf("%d", &in_qty) != 1 || in_qty < 0) {
            printf("오류: 입고량 입력 오류입니다.\n");
            FLUSH_INPUT_BUFFER();
            return;
        }
        products[index].stock += in_qty;
        printf("입고 완료. %s의 현재 재고는 %d개입니다.\n", products[index].name, products[index].stock);
    } else {
        if (product_count >= MAX_PRODUCTS) {
            printf("오류: 상품 갯수 제한 (%d개)을 초과하여 새로운 상품을 등록할 수 없습니다.\n", MAX_PRODUCTS);
            return;
        }

        int new_index = -1;
        for (int i = 0; i < MAX_PRODUCTS; i++) {
            if (!products[i].is_active) {
                new_index = i;
                break;
            }
        }

        if (new_index == -1) {
            printf("오류: 상품 슬롯을 찾을 수 없습니다.\n"); 
            return;
        }

        printf("새로운 상품을 등록합니다.\n");
        printf("상품명: ");
        fgets(name_buffer, sizeof(name_buffer), stdin); // ID 입력 후 버퍼를 비웠으므로 안전
        name_buffer[strcspn(name_buffer, "\n")] = 0;

        printf("입고량: ");
        if (scanf("%d", &in_qty) != 1 || in_qty < 0) {
            printf("오류: 입고량 입력 오류입니다.\n");
            FLUSH_INPUT_BUFFER();
            return;
        }

        printf("판매가격: ");
        if (scanf("%d", &price) != 1 || price <= 0) {
            printf("오류: 판매가격 입력 오류입니다.\n");
            FLUSH_INPUT_BUFFER();
            return;
        }

        products[new_index].product_id = id;
        strncpy(products[new_index].name, name_buffer, sizeof(products[new_index].name) - 1);
        products[new_index].name[sizeof(products[new_index].name) - 1] = '\0';
        products[new_index].price = price;
        products[new_index].stock = in_qty;
        products[new_index].sales_quantity = 0;
        products[new_index].total_sales = 0;
        products[new_index].is_active = 1;
        product_count++;
        printf(" 새 상품 '%s' (ID: %d) 등록 및 입고 완료. 현재 재고: %d개.\n", products[new_index].name, id, in_qty);
    }
    FLUSH_INPUT_BUFFER(); // 함수 종료 전 버퍼 비우기
}

void inventory_out() {
    int id, out_qty, index;

    printf("상품ID: ");
    if (scanf("%d", &id) != 1) {
        printf(" 오류: 상품 ID 입력 오류입니다.\n");
        FLUSH_INPUT_BUFFER();
        return;
    }

    index = find_product_index(id);
    if (index == -1) {
        printf(" 오류: 상품 ID %d에 해당하는 상품을 찾을 수 없습니다.\n", id);
        FLUSH_INPUT_BUFFER();
        return;
    }

    printf("판매량: ");
    if (scanf("%d", &out_qty) != 1 || out_qty <= 0) {
        printf(" 오류: 판매량은 1 이상이어야 합니다.\n");
        FLUSH_INPUT_BUFFER();
        return;
    }

    if (out_qty > products[index].stock) {
        printf(" 오류: 재고 부족. 현재 재고는 %d개입니다.\n", products[index].stock);
        FLUSH_INPUT_BUFFER();
        return;
    }

    products[index].stock -= out_qty;
    products[index].sales_quantity += out_qty;
    products[index].total_sales += (long)out_qty * products[index].price;

    printf(" 판매 완료. 상품명: %s, 판매량: %d개.\n", products[index].name, out_qty);
    printf("     현재 재고: %d개, 누적 판매금액: %ld원\n", products[index].stock, products[index].total_sales);
    FLUSH_INPUT_BUFFER();
}

void view_individual_status() {
    int id, index;
    printf("\n--- 3. 개별 상품 정보 실행 ---\n");
    printf("상품ID: ");
    if (scanf("%d", &id) != 1) {
        printf(" 오류: 상품 ID 입력 오류입니다.\n");
        FLUSH_INPUT_BUFFER();
        return;
    }

    index = find_product_index(id);
    if (index == -1) {
        printf(" 오류: 상품 ID %d에 해당하는 상품을 찾을 수 없습니다.\n", id);
        FLUSH_INPUT_BUFFER();
        return;
    }

    Product p = products[index];
    double sales_rate = 0.0;
    int total_quantity = p.stock + p.sales_quantity;
    if (total_quantity > 0) {
        sales_rate = ((double)p.sales_quantity / total_quantity) * 100;
    }

    printf("\n--- [ %s (%d) 상품 정보 ] ---\n", p.name, p.product_id);
    printf("  - ID: %d, 상품명: %s\n", p.product_id, p.name);
    printf("  - 가격: %d원, 현재 재고: %d\n", p.price, p.stock);
    printf("  - 총 입고량(재고+판매): %d, 총 판매량: %d\n", total_quantity, p.sales_quantity);
    printf("  - 총 판매금액: %ld원, 판매율: %.2f%%\n", p.total_sales, sales_rate);
    printf("--------------------------------------------------\n");
    FLUSH_INPUT_BUFFER();
}

void view_all_status() {
    if (product_count == 0) {
        printf("현재 등록된 상품이 없습니다.\n");
        return;
    }
    long total_stock = 0, total_sales_quantity = 0, total_sales_amount = 0;
    int active_count = 0;

    for (int i = 0; i < MAX_PRODUCTS; i++) {
        if (products[i].is_active) {
            Product p = products[i];
            active_count++;

            double sales_rate = 0.0;
            int total_quantity = p.stock + p.sales_quantity;
            if (total_quantity > 0) {
                sales_rate = ((double)p.sales_quantity / total_quantity) * 100;
            }

            printf("\n  [%d/%d] ID: %d, 상품명: %s\n", active_count, product_count, p.product_id, p.name);
            printf("  - 가격: %d원, 현재 재고: %d\n", p.price, p.stock);
            printf("  - 총 입고량: %d, 총 판매량: %d\n", total_quantity, p.sales_quantity);
            printf("  - 총 판매금액: %ld원, 판매율: %.2f%%\n", p.total_sales, sales_rate);
            printf("--------------------------------------------------");
            
            total_stock += p.stock;
            total_sales_quantity += p.sales_quantity;
            total_sales_amount += p.total_sales;
        }
    }
    printf("\n\n [총계]: 등록 상품 %d개, 총 현재 재고 %ld개, 누적 총 판매량 %ld개, 누적 총 판매금액 %ld원\n",
        product_count, total_stock, total_sales_quantity, total_sales_amount);
}

int main() {
    for (int i = 0; i < MAX_PRODUCTS; i++) products[i].is_active = 0;
    product_count = 0;
    
    printf("재고 관리 프로그램이 시작되었습니다.\n");
    printf("(저장된 데이터를 사용하려면 6번 메뉴 [불러오기]를 이용하세요.)\n");

    int menu = 0;
    while (1) {
        printf("\n==================================================");
        printf("\n원하는 메뉴를 선택하세요. (1. 입고, 2. 판매, 3. 개별현황, 4. 전체현황, 5. 저장, 6. 불러오기, 7. 종료)\n");
        printf(">> ");

        if (scanf("%d", &menu) != 1) {
            printf("\n 잘못된 입력입니다. 숫자를 입력해주세요.\n");
            FLUSH_INPUT_BUFFER();
            continue;
        }
        FLUSH_INPUT_BUFFER(); // 메뉴 선택 후 버퍼 비우기

        printf("==================================================\n");

        switch (menu) {
            case 1: inventory_in(); break;
            case 2: inventory_out(); break;
            case 3: view_individual_status(); break;
            case 4: view_all_status(); break;
            case 5: save_products_to_file(); break;
            case 6: load_products_from_file(); break;
            case 7: 
                printf(" 프로그램을 종료합니다.\n");
                return 0;
            default:
                printf(" 잘못된 메뉴 번호입니다. (1~7 사이의 숫자 입력)\n");
                break;
        }
    }
    return 0;
}
