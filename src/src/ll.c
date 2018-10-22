#include <stdlib.h>
#include <stdio.h>
#include <ll.h>

// Macro for checking memory allocation
#define checkptr(ptr) if (ptr == NULL)\
    {\
        fprintf(stderr, "Failed to allocated memory\n");\
        free(ptr);\
        exit(0);\
    }

ll* initList(int i)
{
    ll *new = (ll *)malloc(sizeof(ll));

    checkptr(new)

    new->i = i;
    new->next = NULL;
    return new;
}

lol* initListofLists(ll *list)
{
    lol *new = (lol *)malloc(sizeof(lol));
    
    checkptr(new);

    new->list = list;
    new->next = NULL;
    return new;
}

void destroyList(ll *list)
{
    ll *head = list, *tmp;
    while (head != NULL)
    {
        tmp = head->next;
        free(head);
        head = tmp;
    }
}

void pushint(int i, ll **list)
{
    ll *new = (ll *)malloc(sizeof(ll));

    checkptr(new)

    new->i = i;
    new->next = *list;
    *list = new;
}

void pushList(ll* list, lol **lolists)
{
    lol *new = (lol *)malloc(sizeof(lol));
    
    checkptr(new)

    new->list = list;
    new->next = *lolists;
    *lolists = new;
}

void destroyListofLists(lol *lolists)
{
    lol *head = lolists, *tmp;
    while (head != NULL)
    {
        tmp = head->next;
        destroyList(head->list);
        free(head);
        head = tmp;
    }
}

void printll(ll *list)
{
    ll *head = list;
    while (head != NULL)
    {
        printf("%i ", head->i);
        head = head->next;
    }
    printf("\n");
}

void printlol(lol *lolists)
{
    lol *head = lolists;
    while (head != NULL)
    {
        printll(head->list);
        head = head->next;
    }
}

int search(int i, ll *list)
{
    ll *head = list;
    while (head != NULL)
    {
        if (head->i == i)
            return 1;
        head = head->next;
    }
    return 0;
}
