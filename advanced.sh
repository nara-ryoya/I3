#!/bin/bash

sox ./dq1.wav -t raw -c 1 - | ./advanced 1024 20 20000 | play -t raw -b 16 -c 1 -e s -r 44100 -