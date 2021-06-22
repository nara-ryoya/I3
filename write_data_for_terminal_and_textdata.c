#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>


int main(int argc, char **argv) {
    // ./serve_send2 50000
    int N = 1000;
    if (argc != 3) {
        perror("usage: text file name, sleep_time\n");
        return -1;
    }

    char *filename = argv[1];
    int t = atoi(argv[2]);

    char data[N];

    FILE *fp;

    if ((fp=fopen(filename, "w")) == NULL){
        perror("can't open file:\n");
        return -1;
    }
    while (1){
        int read_byte = fread(data, sizeof(char), N, stdin);
        if (read_byte == 0){
            printf("--------------------------------------------------------------\n");
            return 0;
        }
        fwrite(data, sizeof(char), read_byte, fp);
        fwrite(data, sizeof(char), read_byte, stdout);
        sleep(t);
    }
    return 0;
    
}