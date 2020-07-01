/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include "includes/product.h"

struct Product _products[] = {
    {'A', "Juice", 10, 10},
    {'B', "Cola", 6, 10},
    {'C', "Tea", 5, 10},
    {'D', "Water", 8, 10},
    {'E', "Coffee", 7, 10},
};
struct Product *product_link, *head_product_link;

void product_init(int stock) {
    struct Product *prev_product_link;
    short length = sizeof(_products)/sizeof(struct Product);
    for (int i = 0; i <= length; ++i) {
        product_link = (struct Product*)malloc(sizeof(struct Product));
        if (i != length) { // 不是最后一个元素，从数组获得产品信息.
            product_link = &_products[i];
            product_link->stock = stock;
        }
        product_link->next = NULL;
        if (i > 0) {
            prev_product_link->next = product_link;
        } else {
            head_product_link = product_link;
        }
        prev_product_link = product_link;
    }
    product_link = head_product_link;
}

struct Product* product_get_first() {
    product_link = head_product_link;
    return product_link;
}

void product_list() {
    printf("(1) The displayed products are: \n");
    product_link = product_get_first();
    while(product_link->next) {
        printf("%c. %s ($%d)\n", product_link->code, product_link->name, product_link->price);
        product_link = product_link->next;
    }
}

char** product_labels(int go_back) {
    char **labels = malloc(sizeof(char *) * 10);

    int pp = 0;
    product_link = product_get_first();
    while(product_link->next) {
        labels[pp] = (char *)malloc(100);
        sprintf(labels[pp], "%c. %s", product_link->code, product_link->name);
        product_link = product_link->next;
        pp++;
    }
    if (go_back == 1) {
        labels[pp] = (char *)malloc(100);
        sprintf(labels[pp], "%s", "0. Go back");
        pp++;
    }
    labels[pp] = NULL;
    return labels;
}

struct Product* product_choose_one(char code) {
    struct Product *product = NULL;
    if (code >= 'a' && code <= 'z') {
        code -= 32;
    }
    product_link = product_get_first();
    while(product_link->next) {
        if (product_link->code == code) {
            product = product_link;
            break;
        }
        product_link = product_link->next;
    }
    return product;
}