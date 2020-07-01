/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include "./product.h"

#ifndef __MACHINE_H__
#define __MACHINE_H__

struct Machine {
    int revenue;
    int coins;
    char chose_product_code;
};

#define MACHINE_MAX_STOCK 10

void machine_status_show();
int machine_withdraw_all_money();

int machine_insert_coins(int);
void machine_clear_coins();

int machine_puchase_product(struct Product *);
int machine_refill_product(struct Product*);
void machine_change_product();

extern struct Machine machine;
#endif