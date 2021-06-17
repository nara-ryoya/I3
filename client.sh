#!/bin/bash
./client_recv 127.0.0.1 50000 | ./advanced 1024 20 20000 | play -t raw -b 16 -c 1 -e s -r 44100 -
