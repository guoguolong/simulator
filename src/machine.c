/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include "includes/machine.h"
#include "includes/product.h"

struct Machine machine = {0, 0, '*'};

void machine_status_show() {
    printf("Amount of revenue: $%d\n", machine.revenue);
    printf("Amount of inserted coins: $%d\n", machine.coins);
    printf("Product information:\n");

    char *stock_info = (char *)malloc(100);
    struct Product *prod_link = product_get_first();
    while(prod_link->next) {
        sprintf(stock_info, "%s", "sold out");

        if (prod_link->stock > 0) {
            sprintf(stock_info, "%d left", prod_link->stock);
        }
        printf("%c. %s ($%d) (%s)\n", prod_link->code, prod_link->name, prod_link->price, stock_info);
        prod_link = prod_link->next;        
    }
    free(stock_info);
}

int machine_withdraw_all_money() {
    int all_money =  machine.revenue + machine.coins;
    machine.revenue = 0;
    machine.coins = 0;
    return all_money;
}

void machine_clear_coins() {
    machine.coins = 0;
}

int machine_insert_coins(int coin) {
    int ret = 0;
    if ((machine.coins + coin) < 100) {
        machine.coins += coin;
        ret = 1;
    }
    return ret;
}

int machine_puchase_product(struct Product *product) {
    int ret = 0;

    if (product->stock >=1 && machine.coins >= product->price) {
        machine.coins -= product->price;
        machine.revenue += product->price;
        product->stock--;
        machine.chose_product_code = product->code;
        ret = 1;
    }

    return ret;
}

int machine_refill_product(struct Product *product) {
    product->stock = MACHINE_MAX_STOCK;
    if (product->stock == MACHINE_MAX_STOCK) {
        return 1;
    }
    return 0;
}

void machine_change_product(struct Product *product_p, char *name, int price, int qty) {
    if (price > 100) price = 99;
    if (price < 0) price = 1;
    product_p->name = name;
    product_p->price = price;
    product_p->stock = 10;
}