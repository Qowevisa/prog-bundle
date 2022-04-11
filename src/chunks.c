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
            , name);
}

static const char hex_str[] = "0123456789ABCDEF";

#define print_char(c) \
    switch (def_style) { \
        case bin: \
            printf("%u%u%u%u%u%u%u%u:%u\n", \
                    (past&128)>0, (past&64)>0, (past&32)>0, (past&16)>0, \
                    (past&8)>0, (past&4)>0, (past&2)>0, (past&1)>0, count); \
            break; \
        case sdec: \
            printf("%+*d:%u\n", 4, past, count); \
            break; \
        case udec: \
            printf("%*u:%u\n", 3, past, count); \
            break; \
        case hex: \
            printf("%c%c:%u\n", hex_str[(past&0xF0) >> 4], hex_str[past&0xF], count); \
            break; \
    }

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
    uint8_t def_style = hex;
    int32_t num = 0;
    uint8_t num_i = 0;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] != '-') {
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
    uint32_t count;
    for (int i = 1; i < argc; i++) {
        if (*argv[i] == '-' || i == num_i) {
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
        uint8_t curr, past;
        past = fgetc(file);
        count = 0;
        for (__off_t i = 0; i < buffer.st_size; i++) {
            if (num > 0 && i == num) {
                if (count != 0) {
                    print_char(curr);
                }
                break;
            }
            curr = fgetc(file);
            count++;
            if (curr != past) {
                print_char(past);
                count = 0;
            }
            past = curr;
        }
    }
    return 0;
}
