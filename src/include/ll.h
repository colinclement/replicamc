#ifndef LL_H
#define LL_H

// Macro for checking memory allocation
#define checkptr(ptr) if (ptr == NULL)\
    {\
        fprintf(stderr, "Failed to allocated memory\n");\
        free(ptr);\
        exit(0);\
    }

typedef struct ll ll;

// Standard linked list which contains integers
struct ll {
    int i;
    struct ll *next;
};

typedef struct lol lol;

// Linked list of linked lists
struct lol {
    ll *list;
    struct lol *next;
};

// initialize a linked list with element i
ll *initList(int i);

// Initialize a list of lists with element list
lol* initListofLists(ll *list);

// Destroy linked list
void destroyList(ll *list);

// add element i to list
void pushint(int i, ll **list);

// remove and return the first element (returns -1 if empty)
int popint(ll **head);

// find the first instance of element i and remove it
// or return -1 if i is not in head
int removeval(int i, ll **head) ;

// add list to list of lists lolists
void pushList(ll* list, lol **lolists);

// destroy a list of lists (also destroys lists!)
void destroyListofLists(lol *lolists);

// print the elements of list
void printll(ll *list);

// print all the lists
void printlol(lol *lolists);

// return 1 or 0 if i is in list or not
int search(int i, ll *list);

#endif
