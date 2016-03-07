#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

#define MAXN 1024
#define MAXHEIGHT 11

struct Node {
    int size;
    int startPos;
    bool allocated;
};

int first[MAXHEIGHT] = {-1}; // 用两个链表维护不同大小空块位置
int nextFree[MAXN] = {-1};
Node nodes[MAXN]; // 用数组来表示完全二叉树

void init(int id, int size, int startPos) {
    nodes[id].size = size;
    nodes[id].startPos = startPos;
    if (size == 1) return;
    init(id * 2 + 1, size / 2, startPos);
    init(id * 2 + 2, size / 2, startPos + size / 2);
}

int my_allocate(int height) {
    int curHeight = height;
    while (first[curHeight] == -1) {
        curHeight ++;
        if (curHeight == MAXHEIGHT) return -1;
    }
    int curID = first[curHeight];
    for (int tempHeight = curHeight; tempHeight != height; tempHeight --) {
        curID = curID * 2 + 1;
        nextFree[curID + 1] = first[tempHeight - 1];
        first[tempHeight - 1] = curID + 1;
    }
    nodes[curID].allocated = true;
    first[curHeight] = nextFree[first[curHeight]];
    return nodes[curID].startPos;
}

void my_free(int startPos) {
    int curID = 0;
    while (!nodes[curID].allocated) {
        if (nodes[curID*2].startPos > startPos) {
            curID = curID * 2 + 1;
        } else {
            curID = curID * 2 + 2;
        }
    }
    nodes[curID].allocated == false;
    while (curID != 0) {
        int pairID;
        if (curID % 2 == 0) pairID = curID - 1;
        else pairID = curID + 1;
        if (nodes[pairID].allocated == false) {
            curID = (curID-1) / 2;
            int height = 1, tempID = curID;
            while (tempID < MAXN) {
                height ++;
                tempID *= 2;
            }
            tempID = first[height];
            while (tempID != -1 || nextFree[tempID] == -1) {
                if (nextFree[tempID] == curID) {
                    nextFree[tempID] = nextFree[curID];
                } else if (nextFree[tempID] == pairID) {
                    nextFree[tempID] = nextFree[pairID];
                }
            }
            nextFree[curID] = first[height-1];
            first[height-1] = curID;
        }
    }
}

int main() {
    // init
    init(0, MAXN, 0);
    first[10] = 0;
    
    // opreate
    int v1, v2;
    while (scanf("%d %d", &v1, &v2)) {
        if (v1 == 0) { // allocate
            int size = 1, height = 0;
            while (size < v2) {
                size *= 2;
                height ++;
            }
            int result = my_allocate(height);
        }
        if (v1 == 1) { // free
            my_free(v2);
        }
    }
}