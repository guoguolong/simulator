/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */
#include <hashmap.h>

#ifndef __PRODUCT_H__
#define __PRODUCT_H__

typedef struct _Product {
    char code;
    char *name;
    int price;
    int stock;
    struct _Product *next;
    struct _Product *head;
} Product;

typedef Product* (*ProductFindOne)(char);
typedef void (*ProductList)(void);
typedef void (*ProductReset)(void);
typedef void (*ProductFlush)(void);
typedef int (*ProductLoad)(char *bin_dir);
typedef map_t (*ProductGetMap)(void);
typedef char** (*ProductLabels)(int go_back);

typedef struct _ProductService {
    ProductFindOne find_one;
    ProductList list;
    ProductFlush flush;
    ProductLoad load;
    ProductReset reset;
    ProductGetMap get_map;
    ProductLabels labels;
} ProductService;

ProductService product_factory_make(void);

#endif