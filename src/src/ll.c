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

//NOTE: if you pop the of a list without moving the pointer of 
int popint(ll **head)  //remove and return first element
{
    ll *next=NULL;
    if (*head == NULL)
        return -1;
    next = (*head)->next;
    int toreturn = (*head)->i;
    free(*head);
    *head = next;
    return toreturn;
}

int removeval(int i, ll **head)
{
    ll *current = *head;
    int toreturn = -1;
    if (current == NULL) // empty 
        return toreturn;
    if (current->i == i)
        return popint(head);

    while (current != NULL)
    {  // look ahead by one
        if (current->next == NULL)
            return -1;
        if (current->next->i == i)
            break; 
        current = current->next;
    }
    ll *tmp = current->next;
    toreturn = tmp->i;
    current->next = tmp->next;
    free(tmp);
    return toreturn;  // i is not in head
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
