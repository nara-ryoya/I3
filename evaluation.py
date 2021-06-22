import collections
import sys
from sys import stdin

def I():
    return stdin.readline().rstrip()
def MI():
    return map(int,stdin.readline().rstrip().split())
def LI():
    return list(map(int,stdin.readline().rstrip().split()))
inf = 10**9 + 7

def main():
    with open('output.txt', 'r') as f1:
        s1 = f1.read().split('\n')
    with open('ans.txt', 'r') as f2:
        s2 = f2.read().split('\n')
    
    if (len(s1) < len(s2)-1):
        print("最後まで受験してください")
        return
    else:
        score = 0
        max_score = len(s2)-1
        for i in range(len(s2)-1):
            if s1[i] == s2[i]:
                score += 1
        print(f'your score is {score} / {max_score}')
        return




if __name__ == '__main__':
    main()








