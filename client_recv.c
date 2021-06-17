#include "stdio.h"
#include "stdlib.h"
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

// ./client_recv 127.0.0.1 50000 | play -t raw -b 16 -c 1 -e s -r 44100 -

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("usage: filename port_number\n");
        return -1;
    }


    char *ip_address = argv[1];
    int port_number = atoi(argv[2]);

    printf("ip_address: %s, port number: %d\n", ip_address, port_number);
    
    int s = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_address);
    addr.sin_port = htons(50000);
    int ret = connect(s, (struct sockaddr * )&addr, sizeof(addr));

    if (ret == -1) {
        perror("failed to connect to designated IP address\n");
        return -1;
    }

    int N = 100000;
    unsigned char data[N];

    while(1) {

    int read_byte = read(s, data, N);

    int write_byte = write(1, data, read_byte);

    if (read_byte != write_byte) {
        perror("failed to read data\n");
        return 1;
    }


    }
    close(s);

    return 0;

}



