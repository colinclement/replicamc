#ifndef LL_H
#define LL_H

typedef struct ll ll;

struct ll {
    int i;
    struct ll *next;
};

typedef struct lol lol;

struct lol {
    ll *list;
    struct lol *next;
};

ll *initList(int i);

void destroyList(ll *list);

void push(int i, ll **list);

void printll(ll *list);

int search(int i, ll *list);

#endif
