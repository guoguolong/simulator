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

void machine_init(char *bin_dir);
void machine_reset(void);
void machine_status_show(void);
int machine_withdraw_all_money(void);

int machine_insert_coins(int);
void machine_clear_coins(void);

int machine_puchase_product(Product *);
int machine_refill_product(Product*);
void machine_change_product(Product *product_p, char *name, int price, int qty);

extern struct Machine machine;
#endif