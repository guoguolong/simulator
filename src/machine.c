/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/machine.h"
#include <hashmap.h>

struct Machine machine;
char *datafile_path;

void _flush_product() {
    ProductService prd_srv = product_factory_make();
    prd_srv.flush();
}

void _flush_machine() {
    FILE *fp = fopen(datafile_path, "w+");
    fprintf(fp, "%d,%d,%c\n", machine.revenue, machine.coins, '*');
    fclose(fp);
}

void machine_init(char *bin_dir) {
    int errno = 0;
    datafile_path = (char *)malloc(1024);
    sprintf(datafile_path, "%sdata/machine.dat", bin_dir);
    int is_null = 1;
    FILE *fp = fopen(datafile_path, "r");
    if (fp) {
        is_null = 0;
        fscanf(fp, "%d,%d,%c", &machine.revenue, &machine.coins, &machine.chose_product_code);
        fclose(fp);
    }
    if (is_null) {
        machine_reset();
    }
}

void machine_reset() {
    machine.revenue = 0;
    machine.coins = 0;
    machine.chose_product_code = '*';

    _flush_machine();
}

int _each_product_status(any_t handler, any_t item) {
    Product* prd_link = (Product*)item;

    char *stock_info = (char *)malloc(100);
    sprintf(stock_info, "%s", "sold out");

    if (prd_link->stock > 0) {
        sprintf(stock_info, "%d left", prd_link->stock);
    }
    printf("%c. %s ($%d) (%s)\n", prd_link->code, prd_link->name, prd_link->price, stock_info);
    free(stock_info);
    return MAP_OK;
}
void machine_status_show() {
    printf("Amount of revenue: $%d\n", machine.revenue);
    printf("Amount of inserted coins: $%d\n", machine.coins);
    printf("Product information:\n");

    
    ProductService prd_srv = product_factory_make();
    map_t prod_map =  prd_srv.get_map();

    hashmap_iterate(prod_map, _each_product_status, NULL);
}

int machine_withdraw_all_money() {
    int all_money =  machine.revenue + machine.coins;
    machine.revenue = 0;
    machine.coins = 0;

    _flush_machine();
    return all_money;
}

void machine_clear_coins() {
    machine.coins = 0;
    _flush_machine();
}

int machine_insert_coins(int coin) {
    int ret = 0;
    if ((machine.coins + coin) < 100) {
        machine.coins += coin;
        ret = 1;
    }
    _flush_machine();
    return ret;
}

int machine_puchase_product(Product *product) {
    int ret = 0;

    if (product->stock >=1 && machine.coins >= product->price) {
        machine.coins -= product->price;
        machine.revenue += product->price;
        product->stock--;
        machine.chose_product_code = product->code;
        ret = 1;
    }

    _flush_machine();
    _flush_product();
    return ret;
}

int machine_refill_product(Product *product) {
    int errno = 0;;
    product->stock = MACHINE_MAX_STOCK;
    printf("product->stock:: %d\n", product->stock);
    _flush_product();
    if (product->stock != MACHINE_MAX_STOCK) {
        errno = 1;
    }
    return errno;
}

void machine_change_product(Product *product_p, char *name, int price, int qty) {
    if (price > 100) price = 99;
    if (price < 0) price = 1;
    product_p->name = name;
    product_p->price = price;
    product_p->stock = 10;

    _flush_product();
}