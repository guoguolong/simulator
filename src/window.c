/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/os.h"
#include "includes/window.h"
#include "includes/product.h"
#include "includes/machine.h"

struct MenuItem menus[] = {
    {'1', "1. Read product information", {
        &(struct MenuItem){'a', "A. Juice ($10)"},
        &(struct MenuItem){'b', "B. Cola ($6)"},
        &(struct MenuItem){'c', "C. Tea ($5)"},
        &(struct MenuItem){'d', "D. Water ($8)"},
        &(struct MenuItem){'e', "E. Coffee ($7)"},
        NULL
    }},
    {'2', "2. Insert coin", {
        &(struct MenuItem){'1', "1. Insert $1"},
        &(struct MenuItem){'2', "2. Insert $2"},
        &(struct MenuItem){'3', "3. Insert $5"},
        &(struct MenuItem){'4', "4. Insert $10"},
        &(struct MenuItem){'0', "0. Go back"},
        NULL
    }},
    {'3', "3. Press product button"},
    {'4', "4. Press return button"},
    {'9', "9. Open service menu(code required)", {
        &(struct MenuItem){'1', "1. Inspect machine status"},
        &(struct MenuItem){'2', "2. Withdraw all money"},
        &(struct MenuItem){'3', "3. Refill product"},
        &(struct MenuItem){'4', "4. Change product"},
        &(struct MenuItem){'0', "0. Go back"},
        NULL
    }},
    {'0', "0. Quit"}
};

char** _menu_labels(struct MenuItem *node) {
    char **labels = (char **)malloc(sizeof(char *) * (MENU_ITEMS_NUM + 1));
    char **ptr = labels;
    int k = 0;
    while(node->children[k]) {
        *ptr = node->children[k]->label;
        ptr++;
        k++;
    }
    *ptr = NULL;
    return labels;
}

char** _menu_toplevel_labels() {
    char **labels = (char **)malloc(sizeof(char *) * (MENU_TOP_ITEMS_NUM + 1));
    char **ptr = labels;

    short length = sizeof(menus)/sizeof(struct MenuItem);
    for (int i = 0; i < length; ++i) {
        *ptr = menus[i].label;
        ptr++;
    }    
    *ptr = NULL;
    return labels;
}

char _prompt(char *title, char **items, int is_pointer) {
    puts("\n****************************************");
    puts(title);
    char **iter = items;
    while(*iter) {
        printf("%s\n", *iter);
        if (is_pointer) {
            free(*iter);
        }
        iter++;
    }
    // 释放指针
    free(items);
    items = NULL;

    printf("Your choice: ");
    fflush(stdout);

    char c;
    for(;;) {
        c = getchar();
        if (c == '\n') {
            printf("\nValid choice, try again pls: ");
            fflush(stdout);
        } else {
            break; 
        }
    }
    getchar(); // 读取回车符，并忽略之.
    puts("");
    return c;
}

void window_show_home_panel() {
    struct PanelData {
        char coins[3];
        char *product_names;
        char *product_prices;
        char *status_flags;
        char product_code;
    } panel_data = {
        "   ",
        (char *)malloc(100),
        (char *)malloc(100),
        (char *)malloc(100),
        '='
    };
    char *prompt = (char *)malloc(1000);
    
    // 初始值
    sprintf(panel_data.product_names, "%s", "");
    sprintf(panel_data.product_prices, "%s", "");
    sprintf(panel_data.status_flags, "%s", "");

    int i = 0;
    struct Product *product_link = product_get_first();
    while(product_link->next) {
        char status_flag = ' ';
        struct Product *prd = product_link;
        char *sep = "";

        if (i == 0) sep = ""; else sep = "    ";
        sprintf(panel_data.product_names, "%s%s%c", panel_data.product_names, sep, prd->code);

        if (i == 0) sep = ""; else sep = "  ";
        sprintf(panel_data.product_prices, "%s%s$%2d", panel_data.product_prices, sep, prd->price);
        if (machine.coins > 0 && prd->price <= machine.coins) {
            if (prd->stock > 0) {
                status_flag = 'O';
            } else {
                status_flag = 'X';
            }
        }
        if (i == 0) sep = ""; else sep = "  ";
        sprintf(panel_data.status_flags, "%s%s[%c]", panel_data.status_flags, sep, status_flag);
        i++;
        product_link = product_link->next;
    }

    sprintf(panel_data.coins, "%2d", machine.coins);

    sprintf(prompt, "%s\n", "*---------------------------*");
    sprintf(prompt, "%s%s\n", prompt, "|      Vending Machine      |");
    sprintf(prompt, "%s%s\n", prompt, "*---------------------------*");
    sprintf(prompt, "%s|   %s   |\n", prompt, panel_data.product_names);
    sprintf(prompt, "%s|  %s  |\n", prompt, panel_data.product_prices);
    sprintf(prompt, "%s|  %s  |\n", prompt, panel_data.status_flags);
    sprintf(prompt, "%s%s\n", prompt, "*---------------------------*");
    sprintf(prompt, "%s|                    [$%s]  |\n", prompt, panel_data.coins);
    sprintf(prompt, "%s|                           |\n", prompt);

    if (machine.chose_product_code != '*') panel_data.product_code = machine.chose_product_code;
    sprintf(prompt, "%s|           [=%c=]           |\n", prompt, panel_data.product_code);
    sprintf(prompt, "%s%s\n", prompt, "*---------------------------*");
    printf("%s\n", prompt);

    machine.chose_product_code =  '*'; // 恢复默认值

    free(panel_data.product_names);
    free(panel_data.product_prices);
    free(panel_data.status_flags);

    free(prompt);
    prompt = NULL; // 没有后续引用，其实是不必要的.
}

char window_show_top_menu() {
    return _prompt("What would you like to do?", _menu_toplevel_labels(), 0);
}

void window_show_menu(int level, char code) {
    if (level < 1) return;
    struct MenuItem *item;
    if (level == 1) { // 二级菜单.
        for (int i = 0; i < MENU_TOP_ITEMS_NUM; ++i) {
            if (menus[i].code == code) {
                item = &menus[i];
                break;
            }
        }
        if (code == '3') { //  取出产品.
            char c = _prompt("(3) Which product button would you like to press?", product_labels(1), 1);
            struct Product *prd = product_choose_one(c);
            if (prd) {
                machine_puchase_product(prd);
            }
        } else if (code == '4') { // 找零.
            puts("(4) Return button is pressed.");
            printf("$%d has been returned.\n", machine.coins);
            machine_clear_coins();
        } else if (code == '2') { // 投硬币
            for(;;) {
                char c = _prompt("(2) Which coin would you like to insert?", _menu_labels(item), 0);
                if (c == '0' || c == 'q') break;
                window_screen_clear();
                short coin = 0;
                switch (c) {
                    case '1':
                        coin = 1;
                        break;
                    case '2':
                        coin = 2;
                        break;
                    case '3':
                        coin = 5;
                        break;
                    case '4':
                        coin = 10;
                        break;
                    default:
                        coin = 0;
                        break;
                }
                int is_ok = machine_insert_coins(coin);
                if (is_ok) {
                    printf("You have inserted $%d.\n\n", machine.coins);
                } else {
                    puts("Inserted coins can't exceed 100.");
                }
                window_show_home_panel();
            }
        } else if (code == '9') { // Inspect machine status
            for(;;) {
                char c = _prompt("(9) What would you like to do?", _menu_labels(item), 0);
                if (c == '0' || c == 'q') break;
                switch (c) {
                    case '1':
                        machine_status_show();
                        puts("");
                        break;
                    case '2':
                        puts("(9-2) All money is being withdrawn.");
                        int all_money = machine_withdraw_all_money();
                        printf("$%d is withdrawn.\n", all_money);
                        break;
                    case '3': { // Refill product.
                        char c = _prompt("(9-3) Which product would you like to refill?", product_labels(1), 1);

                        struct Product* prd = product_choose_one(c);
                        if (prd) {
                            int is_full = machine_refill_product(prd);
                            if (is_full) {
                                printf("You have refilled product %c to full.\n", prd->code);
                            } else {
                                printf("You failed to refill product\n");
                            }
                        }
                        break;
                    }
                    case '4': { // Change product.
                        char c = _prompt("(9-4) Which product would you like to change?", product_labels(1), 1);
                        struct Product *product_p = product_choose_one(c);
                        if (product_p) {
                            printf("You are changing product %c.\n", product_p->code);

                            printf("Enter new product name: ");
                            fflush(stdout);

                            char *name = malloc(20); // 可能内存泄漏(如果输入字符超过20个).
                            scanf("%s", name);
                            getchar();
                            
                            printf("Enter new product price: ");
                            fflush(stdout);

                            int price;
                            scanf("%d", &price);
                            printf("price:::::: %d\n", price);
                            getchar();

                            machine_change_product(product_p, name, price, MACHINE_MAX_STOCK);

                            printf("The new product %c has been filled to full.\n", product_p->code);
                        } else {
                            puts("No matched product to change.");
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
}

void window_screen_clear() {
    if (OS == "_WIN32" || OS == "_WIN64") {
        system("cls");
    } else {
        system("clear");
    }
}
