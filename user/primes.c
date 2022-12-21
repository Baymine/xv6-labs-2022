#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const uint INT_LEN = sizeof(int);

/*
* Intuition: Passing the left neighbor's data to the right except
* the data which can be exact divided by the first number of the 
* left neighbor. In this case, the first number of the left neighbor 
* is primer number;
*/

/// @brief Read the first data from left pip
/// @param lpipe left pip
/// @param dst the prime data
/// @return exit code
int lpipe_first_data(int lpipe[2], int* dst){
    if(read(lpipe[RD], dst, INT_LEN) == INT_LEN){
        printf("prime %d\n", *dst);
        return 0;
    }
    return -1;
}

/// @brief passing the data from left pipe to the right pipe except the data can be exact divided by first
/// @param lpipe 
/// @param rpip 
/// @param first : first data of lpipe
void transmit_data(int lpipe[2], int rpip[2], int first){
    int data;

    while(read(lpipe[RD], &data, INT_LEN) == INT_LEN){
        if(data % first != 0){
            write(rpip[WR], &data, INT_LEN);
        }
    }
    close(lpipe[RD]);
    close(rpip[WR]);
}

void primes(int lpipe[2]){
    close(lpipe[WR]);
    int first;

    if(lpipe_first_data(lpipe, &first) == 0){
        int p[2];
        pipe(p);
        transmit_data(lpipe, p, first);

        if(fork() == 0){
            primes(p);  // Recurse: the rpipe p is now treated as lpipe
        } else{
            close(p[RD]);
            wait(0);
        }
    }
    exit(0);
}

int main(int argc, char const *argv[]){
    int p[2];
    pipe(p);

    for(int i = 2; i <= 35; ++i){
        write(p[WR], &i, INT_LEN);
    }
    if(fork() == 0) {
        primes(p);
    } else {
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    exit(0);
}