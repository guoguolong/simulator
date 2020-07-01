/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

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

typedef struct _ProductService {
    ProductFindOne find_one;
    ProductList list;
    ProductFlush flush;
    ProductLoad load;
    ProductReset reset;
} ProductService;

char** product_labels(int go_back);
Product* product_get_first(void);
ProductService product_factory_make(void);

#endif