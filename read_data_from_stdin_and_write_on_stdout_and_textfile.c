#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    // ./serve_send2 50000
    int N = 1;
    if (argc != 2) {
        perror("usage: text file name\n");
        return -1;
    }

    char *filename = argv[1];

    char data[N];

    FILE *fp;

    if ((fp=fopen(filename, "w")) == NULL){
        perror("can't open file:\n");
        return -1;
    }
    // fwrite("why\n", 4, 4, stdout);
    int read_byte;
    while (1){
        read_byte = fread(data, sizeof(char), N, stdin);
        printf("read byte is %d\n", read_byte);
        if (data[0] == 'c'){
            printf("end of file\n");
            break;
        }
        if (read_byte != 0){
        int wd1 = fwrite(data, sizeof(char), read_byte, fp);
        int wd2 = fwrite(data, sizeof(char), read_byte, stdout);
        // printf("wd1: %d, wd2: %d\n", wd1, wd2);
        }
    }
    fclose(fp);
    
    return 0;
    
}