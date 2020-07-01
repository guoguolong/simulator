/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#ifndef __PRODUCT_H__
#define __PRODUCT_H__

struct Product {
    char code;
    char *name;
    int price;
    int stock;
    struct Product *next;
};

void product_list();
char** product_labels(int go_back);
struct Product* product_get_first();
struct Product* product_choose_one(char code);
void product_init(int stock);

extern struct Product *product_link;
#endif