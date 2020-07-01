/**
 * Copyright (C) 2020 Banyuan All rights reserved
 * @author Allen elon.guo@gmail.com
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "includes/window.h"
#include "includes/machine.h"
#include "includes/product.h"
#include "includes/main.h"

void main_loop() {
    for(;;) {
        window_show_home_panel();
        char c = window_show_top_menu();
        switch (c) {
            case OP_CODE_PRD_VIEW:
                product_list();
                puts("Press any key to continue");
                fflush(stdout);

                getchar(); // 暂停.
                window_screen_clear();
                break;
            case OP_CODE_COIN_IN:
            case OP_CODE_PRD_PUCHASE:
            case OP_CODE_COIN_OUT:
                window_show_menu(1, c);
                window_screen_clear();
                break;
            case OP_CODE_SERVICE:
                window_screen_clear();
                puts("(9) Opening service menu. Access code is required.");
                printf("Enter access code: ");
                fflush(stdout);
                
                char auth_code[5];
                scanf("%s", auth_code);
                getchar();

                if (strcmp(auth_code, "1101") == 0) {
                    puts("Correct code!");
                    window_show_menu(1, '9');
                    window_screen_clear();
                } else {
                    puts("Invalid code, try again please!");
                }
                break;
            case OP_CODE_QUIT:
            case OP_CODE_EXIT:
                break;
            default:
                break;
        }
        if (c == OP_CODE_EXIT || c == OP_CODE_QUIT) {
            puts("Bye!\n");
            break;
        }
    }
 }

int main() {
    product_init(MACHINE_MAX_STOCK);
    main_loop();
    return 0;
}