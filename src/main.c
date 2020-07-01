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

#ifdef _WIN32
    #define SEP '\\'
#else
    #define SEP '/'
#endif

char * get_current_dir(char *filepath) {
    char *path = (char *)malloc(strlen(filepath) + 2);
    char *head = path;
    strcpy(path, filepath);
    char *pos = strrchr(path, SEP);
    if (pos == NULL) {
        sprintf(path, ".%c", SEP);
    } else {
        pos++;
        *pos = '\0';
    }
    return head;
}

void main_loop() {
    ProductService prd_srv = product_factory_make();
    for(;;) {
        window_show_home_panel();
        char c = window_show_top_menu();
        switch (c) {
            case OP_CODE_PRD_VIEW: {
                prd_srv.list();
                puts("Press any key to continue");
                fflush(stdout);

                getchar(); // 暂停.
                window_screen_clear();
                break;
            }
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
            case OP_CODE_RESET:
                prd_srv.reset();
                machine_reset();
                puts("Reset done.");
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

int main(int argc, char **argv) {
    ProductService prd_srv = product_factory_make();
    char *curr_dir = get_current_dir(*argv);
    
    prd_srv.load(curr_dir);
    machine_init(curr_dir);

    main_loop();
    return 0;
}