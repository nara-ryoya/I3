#!/bin/bash¥[]
./prepare_data.sh && ./serve.sh & ./client_recv 127.0.0.1 50000 |  play -t raw -b 16 -c 1 -e s -r 44100 - -q | python speech_text.py
