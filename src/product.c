/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <csv.h>
#include <hashmap.h>
#include "includes/product.h"

char *data_path;
map_t product_map;
int labels_count = 0;

void parse(FILE *fp) {
    Product *prev_product, *head_product;
    int max_line_size = 1024;
    int err, done = 0;
    int is_first_row = 1;
    product_map = hashmap_new();    
    Product* product_link;
    while (!done) {
        char *line = fread_csv_line(fp, max_line_size, &done, &err);
        char **parsed = parse_csv(line);
        if (strcmp(*parsed, "编码") == 0 || strcmp(*parsed, "code") == 0) { // 标题行
            free_csv_line(parsed);
            continue;
        };
        
        char **ptr = parsed;
        if (strcmp(*ptr, "") != 0) { // 剔除非法空行.
            Product *real_prod = (Product*)malloc(sizeof(Product));

            // product code
            real_prod->code = *ptr[0];
    
            // product name
            ++ptr;
            if (*ptr) {
                real_prod->name= (char *)malloc(strlen(*ptr));
                strcpy(real_prod->name, *ptr);
            }
            
            // product price
            ++ptr;
            if (*ptr) {
                sscanf(*ptr, "%d", &real_prod->price);
            }

            // product stock
            ++ptr;
            if (*ptr) {
                sscanf(*ptr, "%d", &real_prod->stock);
            }


            char *key_code = (char *)malloc(sizeof(char));
            sprintf(key_code, "%c", real_prod->code);
            hashmap_put(product_map, key_code, real_prod);
        }
        free_csv_line(parsed);
    }
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

int _labels_each_one(any_t handler, any_t data) {
    Product* prod_link = (Product*) data;
    char **ptr = (char**) handler;
    ptr += labels_count;

    *ptr = (char *)malloc(100);
    sprintf(*ptr, "%c. %s", prod_link->code, prod_link->name);

    labels_count++;
    return MAP_OK;
}

char** product_labels(int go_back) {
    char **labels = (char**)malloc(sizeof(char *) * 10);
    char **ptr = labels;
    hashmap_iterate(product_map, _labels_each_one, ptr);

    ptr = ptr + labels_count;
    if (go_back == 1) {
        *ptr = (char *)malloc(100);
        sprintf(*ptr, "%s", "0. Go back");
    }
    ptr++;
    *ptr = NULL;
    labels_count = 0;
    return labels;
}

int _list_each_one(any_t handler, any_t data) {
    Product* product_link = (Product*)data;
    printf("%c. %s ($%d)\n", product_link->code, product_link->name, product_link->price);
    return MAP_OK;
}

void list() {
    printf("(1) The displayed products are: \n");
    hashmap_iterate(product_map, _list_each_one, NULL);
}

Product* find_one(char code) {
    Product *product = (Product *)malloc(sizeof(Product));
    if (code >= 'a' && code <= 'z') {
        code -= 32;
    }
    char *key_code = (char *)malloc(sizeof(char));
    sprintf(key_code, "%c", code);
    int error = hashmap_get(product_map, key_code, (void**)(&product));
    if (error != MAP_OK) {
        product = NULL;
    }
    free(key_code);
    return product;
}

int _flush_each_one(any_t fp, any_t data) {
    Product* pl = (Product*)data;
    fprintf(fp, "%c,%s,%d,%d\n", pl->code, pl->name, pl->price, pl->stock);    
    return MAP_OK;
}

void flush() {
    FILE *fp = fopen(data_path, "w+");
    fprintf(fp, "%s\n", "编码,名字,价格,库存");

    hashmap_iterate(product_map, _flush_each_one, fp);
    fclose(fp);
}

int _reset_each_one(any_t handler, any_t data) {
    Product* prod_link = (Product*) data;
    prod_link->stock = 10;
    return MAP_OK;
}
void reset() {
    hashmap_iterate(product_map, _reset_each_one, NULL);    
    flush();
}

map_t get_map() {
    return product_map;
}

ProductService product_factory_make() {
    ProductService srv = {find_one, list, flush, load_data, reset, get_map, product_labels};
    return srv;
}