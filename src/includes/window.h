/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */
#ifndef __WINDOW_H__
#define __WINDOW_H__

struct MenuItem {
    char code; // 操作键字符
    char *label; // 菜单名
    struct MenuItem *children[10]; // 子菜单
};

#define MENU_TOP_ITEMS_NUM 6
#define MENU_ITEMS_NUM 10

void window_show_home_panel();
char window_show_top_menu();
void window_show_menu(int level, char code);
void window_screen_clear();

extern struct MenuItem menus[]; 
#endif