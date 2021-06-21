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


int main(int argc, char **argv) {
    // ./serve_send2 50000
    int N = 1000;
    if (argc != 2) {
        perror("usage: port_number\n");
        return -1;
    }
    int port_num = atoi(argv[1]);

    unsigned char data[N];
    int ss = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_num);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(ss, (struct sockaddr *)&addr, sizeof(addr));


    listen(ss, 10);

    struct sockaddr_in client_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int s = accept(ss, (struct sockaddr *)&client_addr, &len);

    char* cmd = "rec -t raw -b 16 -c 1 -e s -r 44100 - -q";
    FILE *fp;

    if ((fp = popen(cmd, "r")) != NULL) {
        while(1)
        {
            int read_byte = fread(data, N , sizeof(char), fp);
            if (read_byte == 0)
            {
                break;
            }

            int write_byte = write(s, data, N);
        }
    }
    return -1;
    close(ss);

}