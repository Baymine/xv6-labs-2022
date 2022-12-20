# include "kernel/types.h"
#include "user/user.h"

int main(int argc, char const * argv[]) {
    if (argc!= 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}