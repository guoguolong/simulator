/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <csv.h>
#include "includes/product.h"

Product *product_link;
char *data_path;

void parse(FILE *fp) {
    Product *prev_product, *head_product;
    int max_line_size = 1024;
    int err, done = 0;
    int is_first_row = 1;
    while (!done) {
        char *line = fread_csv_line(fp, max_line_size, &done, &err);
        char **parsed = parse_csv(line);
        if (strcmp(*parsed, "编码") == 0 || strcmp(*parsed, "code") == 0) { // 标题行
            free_csv_line(parsed);
            continue;
        };
        
        char **ptr = parsed;
        if (strcmp(*ptr, "") != 0) { // 剔除非法空行.
            product_link = (Product*)malloc(sizeof(Product));
            if (*ptr) {
                product_link->code= *ptr[0];
            }

            ++ptr;
            if (*ptr) {
                product_link->name= (char *)malloc(strlen(*ptr));
                strcpy(product_link->name, *ptr);
            }
            
            ++ptr;
            if (*ptr) {        
                sscanf(*ptr, "%d", &product_link->price);
            }
            ++ptr;
            if (*ptr) {
                sscanf(*ptr, "%d", &product_link->stock);
            }

            product_link->next = NULL;
            if (is_first_row) {
                head_product = product_link;
            } else {
                prev_product->next = product_link;
            }
            product_link->head = head_product;
            is_first_row = 0;
            prev_product = product_link;

        }
        free_csv_line(parsed);
    }
    // 最后一个伪元素.便于使用next遍历.
    Product * last_p = (Product*)malloc(sizeof(Product));
    last_p->next = NULL;
    last_p->head = head_product;
    product_link->next = last_p;

    product_link = head_product; // reset 指针.
}

int load_data(char *bin_dir) {
    int errno = 0;
    data_path = (char *)malloc(1024);
    sprintf(data_path, "%sdata/product.csv", bin_dir);

    FILE *fp = fopen(data_path, "r");
    if (fp == NULL) {
        printf("%s not found.\n", data_path);
        errno = 1;
    }
    parse(fp);

    fclose(fp);
    return errno;
}

Product* product_get_first() {
    product_link = product_link->head;
    return product_link;
}

char** product_labels(int go_back) {
    char **labels = malloc(sizeof(char *) * 10);

    int pp = 0;
    product_link = product_link->head;
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

void list() {
    printf("(1) The displayed products are: \n");
    product_link = product_link->head;
    while(product_link->next) {
        printf("%c. %s ($%d)\n", product_link->code, product_link->name, product_link->price);
        product_link = product_link->next;
    }
}

Product* find_one(char code) {
    Product *product = NULL;
    if (code >= 'a' && code <= 'z') {
        code -= 32;
    }
    product_link = product_link->head;
    while(product_link->next) {
        if (product_link->code == code) {
            product = product_link;
            break;
        }
        product_link = product_link->next;
    }
    return product;
}

void flush() {
    Product *pl = product_link->head;
    FILE *fp = fopen(data_path, "w+");
    fprintf(fp, "%s\n", "编码,名字,价格,库存");
    while(pl->next) {
        fprintf(fp, "%c,%s,%d,%d\n", pl->code, pl->name, pl->price, pl->stock);
        pl = pl->next;
    }
    fclose(fp);
}

void reset() {
    Product *pl = product_link->head;
    while(pl->next) {
        pl->stock = 10;
        pl = pl->next;
    }
    flush();
}

ProductService product_factory_make() {
    ProductService srv = {find_one, list, flush, load_data, reset};
    return srv;
}