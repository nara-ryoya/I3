#!/bin/bash
./client_recv 127.0.0.1 50000 | ./advanced 8192 20 18000 $1 | play -t raw -b 16 -c 1 -e s -r 48000 - -q