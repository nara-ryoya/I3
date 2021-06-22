import sys, sqlite3
from collections import namedtuple
from pprint import pprint
from time import sleep

conn = sqlite3.connect("./wnjpn.db")
cur = conn.execute("select * from word where lang = 'jpn' ORDER BY RANDOM() LIMIT 100;")
wordlist = [ record[2] for record in cur.fetchall()]

for i in range(20):
    print(wordlist[i])
    # sleep(5)