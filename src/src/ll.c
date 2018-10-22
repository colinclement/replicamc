#include <stdlib.h>
#include <stdio.h>
#include <ll.h>

ll* initList(int i)
{
    ll *newList = (ll *)malloc(sizeof(ll));
    if (newList == NULL)
    {
        fprintf(stderr, "Failed to allocated new list\n");
        free(newList);
        exit(0);
    }
    newList->i = i;
    newList->next = NULL;
    return newList;
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
    if (new == NULL)
    {
        fprintf(stderr, "Failed to allocated new list\n");
        free(new);
        exit(0);
    }
    new->i = i;
    new->next = *list;
    *list = new;
}

void pushList(ll* list, lol *lists)
{
    lists->N ++;
    lol->lists    
}

void printll(ll *list)
{
    ll *head = list;
    int i = 0;
    while (head != NULL)
    {
        printf("%i\n", head->i);
        head = head->next;
        i += 1;
        if (i > 10)
            break;
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
