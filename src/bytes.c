#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


enum style {
   hex  = 1,
   bin  = 2,
   sdec = 3,
   udec = 4
};

enum options {
   space_indent = 1 
};

static inline void print_help_menu(char *name) {
    printf( "Usage: %s [OPTIONS] <FILE> \n"
            "Options:\n"
            "  --block N : prints bytes in block of N\n"
            "  --num N   : reads only first N bytes if possible\n"
            , name);
}

static const char hex_str[] = "0123456789ABCDEF";

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_help_menu(argv[0]);
        return 1;
    }
    if (strcmp(argv[1], "--help") == 0) {
        print_help_menu(argv[0]);
        return 1;
    }
    // check style and opts
    uint8_t def_style = udec;
    int8_t block_size = 0;
    uint8_t block_i = 0;
    int32_t num = 0;
    uint8_t num_i = 0;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] != '-') {
            continue;
        }
        if (strcmp(argv[i], "--block") == 0) {
            if (i == argc) {
                fprintf(stderr, "No usefull args for option --block\n");
                return 2;
            }
            block_size = atoi(argv[i + 1]);
            if (block_size <= 0) {
                fprintf(stderr, "Incorrect value for --block option\n");
                return 3;
            }
            block_i = i + 1;
            continue;
        }
        if (strcmp(argv[i], "--num") == 0) {
            if (i == argc) {
                fprintf(stderr, "No usefull args for option --num\n");
                return 2;
            }
            num = atoi(argv[i + 1]);
            if (num <= 0) {
                fprintf(stderr, "Incorrect value for --num option\n");
                return 3;
            }
            num_i = i + 1;
            continue;
        }
        for (size_t j = 1; j < strlen(argv[i]); j++) {
            switch (argv[i][j]) {
                // styles
                case 'h':
                    def_style = hex;
                    break;
                case 's':
                    def_style = sdec;
                    break;
                case 'u':
                    def_style = udec;
                    break;
                case 'b':
                    def_style = bin;
                    break;
            }
        }
    }
    // default setting for block_size
    if (block_size == 0) {
        block_size = 8;
    }
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-' || i == block_i || i == num_i) {
            continue;
        }
        struct stat buffer;
        if (stat(argv[i], &buffer) != 0) {
            fprintf(stderr, "%s: %s: %s\n", argv[0], argv[i], strerror(errno));
            return 4;
        }
        if (!S_ISREG(buffer.st_mode)) {
            fprintf(stderr, "%s: %s: Not a regular file\n", argv[0], argv[i]);
            return 5;
        }
        FILE *file = fopen(argv[i], "r");
        if (!file) {
            fprintf(stderr, "%s: fopen: %s\n", argv[0], strerror(errno));
            return 6;
        }
        printf("%s\n", argv[i]);
        uint8_t c;
        uint8_t block_counter = 1;
        for (__off_t i = 0; i < buffer.st_size; i++) {
            if (num > 0 && i == num) {
                break;
            }
            c = fgetc(file);
            switch (def_style) {
                case bin:
                    printf("%u%u%u%u%u%u%u%u ", (c&128)>0, (c&64)>0, (c&32)>0, (c&16)>0, (c&8)>0, (c&4)>0, (c&2)>0, (c&1)>0);
                    break;
                case sdec:
                    printf("%+*d ", 4, c);
                    break;
                case udec:
                    printf("%*u ", 3, c);
                    break;
                case hex:
                    printf("%c%c ", hex_str[(c&0xF0) >> 4], hex_str[c&0xF]);
                    break;
                default:
                    printf("%+*d ", 4, c);
                    break;
            }
            if (block_counter++ == block_size) {
                block_counter -= block_size;
                fputc('\n', stdout);
            }
        }
        fputc('\n', stdout);
    }
    return 0;
}
