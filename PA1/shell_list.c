#include <stdio.h>
#include <stdlib.h>
#include "shell_list.h"
#include "list_of_list.h"
static Node* newNode(long); //create new node with long
static Node* insertNext(Node*, long); //insert long while keeping track of tail
static long findSize(Node*); //find size of link list
static Node* ksort(Node* list, long k, long** n_comp, int size); //popopoppop pushpushpushpush
static Node* pop(Node** head); //pop first value from linked list
static void push(Node** head, Node* n); //push node into front of linked list
static void insertInorder(Node** head, Node* node, long**** n_comp); //insert Node into ordered list in order
static void insertInorder2(Node** head, Node* node, long*** n_comp); //copy
static Node* insertionSort(Node* head, long*** n_comp); //perform insertion sort on linked list
static Node* insertionSort2(Node* head, long** n_comp); //copy
static Node* append(Node*, Node* ); //insert node while keeping track of tail 
static void freeList(List* l); //free list of nodes

static void freeList(List* l)
{
    List* temp = NULL;
    while(l)
    {
        temp = l;
        l = l->next;
        free(temp);
    }
}

static void insertInorder(Node** head, Node* node, long**** n_comp) //from lecture notes, takes node and inserts it into a sorted array
{
    Node* dummy = newNode(-1); //need to initialize and free cause if not causes error
    dummy->next = *head;
    Node* curr = dummy->next;
    Node* prev = dummy;
    while (curr && curr->value < node->value)
    {
        prev = curr;
        curr = curr->next;
        ****n_comp += 1;
    }
    prev->next = node;
    node->next = curr;
    *head = dummy->next;
    free(dummy);
}

static Node* insertionSort(Node* head, long*** n_comp) //insertion sort of a linked list 
{
    Node* sorted = NULL; //empty sorted linked list
    Node* sort = head; //node to insert in sorted array

    while (head != NULL)
    {
        head = head->next;
        insertInorder(&sorted, sort, &n_comp);
        sort = head;
    }

    return(sorted);
}

static void insertInorder2(Node** head, Node* node, long*** n_comp) //from lecture notes, takes node and inserts it into a sorted array
{
    Node* dummy = newNode(-1); //need to initialize and free cause if not causes error
    dummy->next = *head;
    Node* curr = dummy->next;
    Node* prev = dummy;
    while (curr && curr->value < node->value)
    {
        prev = curr;
        curr = curr->next;
        ***n_comp += 1; //need to fix this
    }
    prev->next = node;
    node->next = curr;
    *head = dummy->next;
    free(dummy);
}

static Node* insertionSort2(Node* head, long** n_comp) //insertion sort of a linked list 
{
    //used for k = 1
    //reverses list so since it should be close to sorted, 1 2 4 3 7 5
    //if reverse, inserting in order on new list should be faster
    //5 7 3 4 2 1 inserted in order without traversing entire array

    Node* sorted = NULL; //empty sorted linked list
    Node* reverse = NULL; //reverse linked list
    Node* temp = NULL; //temp node

    while(head) //reverse list
    {
        temp = pop(&head);
        push(&(reverse), temp);
    }

    while (reverse != NULL)
    {
        temp = reverse->next;
        insertInorder2(&sorted, reverse, &n_comp);
        reverse = temp;
    }

    return(sorted);
}

static long findSize(Node* n) //find length/size of linked list
{
    long c = 0;
    while(n->next != NULL)
    {
        c++;
        n = n->next;
    }
    return(c + 1);
}

static Node* newNode(long val) //create new node
{
    Node* n = (struct _Node*)malloc(sizeof(struct _Node));
    n->value = val;
    n->next = NULL;
    return(n);
}

static Node* insertNext(Node* tail, long val) //create new node and insert it at the end of the array, used for loading file
{
    Node* n = newNode(val);
    
    tail->next = n;
    n->next = NULL;

    return(n);
}

static List* newList(Node* n) //create new list of list
{
    List* l = (struct _List*)malloc(sizeof(struct _List));
    l->node = n;
    l->next = NULL;
    return(l);
}

static void push(Node** head, Node* n) //push node into linked list
{
    n->next = (*head);
    (*head) = n;
}

static Node* pop(Node** head) //pop node from list and return the popped node
{
    Node* popped = *head;
    (*head) = (*head)->next;
    popped->next = NULL;
    return(popped);
}

static Node* append(Node* last, Node* n) //same as insertNext but applies a node rather than creating the node inside
{
    if(last != NULL)
    {
        last->next = n;
        n->next = NULL;
        return(n);
    }
    else
    {
        return(n);
    }
}

Node *List_Load_From_File(char *filename) //should be g
{
    FILE* input = fopen(filename, "rb");

    if (input == NULL)
    {
        return(NULL);
    }

    fseek(input, 0L, SEEK_END);
    int size = ftell(input) / 8; //num of long integers
    rewind(input);

    if (size == 0)
    {
        return(NULL);
    }

    int c;
    long buffer;

    //Assign "head" to first value taken from list
    fread(&buffer, sizeof(buffer), 1, input);
    Node *head = newNode(buffer);
    Node *tail = head;

    for (c = 0; c < size - 1; c++) //size - 1 since the first value is already taken out
    {
        fread(&buffer, sizeof(buffer), 1, input);
        tail = insertNext(tail, buffer); //insert to next node over and over//
    }

    fclose(input);
    return(head);
}

Node *List_Shellsort(Node *list, long *n_comp) //THIS IS MINE
{
    if (list == NULL)
    {
        return(NULL);
    }

    long k = 1; //ksort
    long size;

    size = findSize(list);

    while (k < size) //finding largest k
    {
        k = k * 3 + 1;
    }
    k = (k - 1) / 3;

    Node* head = list;

    while (k > 1) //last sort is just a plain insertion sort
    {
        head = ksort(head, k, &n_comp, size);
        k = (k - 1) / 3; //decreasing k by same formula
    }
    head = insertionSort2(head, &n_comp); //insertion sort over entire array for k = 1

    return(head);
}

Node* ksort(Node* list, long k, long** n_comp, int size)
{
    List* l = newList(NULL);
    List* head = l;

    Node* temp = NULL;
    Node* sorted = NULL;

    int i = 0;
    int pass;

    for(i = 0; i < k - 1; i++) //initialize empty sublists
    {
        l->next = newList(NULL);
        l = l->next;
    }

    l = head;
    while (list) //fill sublists until nothing left in list
    {
        temp = pop(&list);
        push(&(l->node), temp);
        if (l->next != NULL)
        {
            l = l->next;
        }
        else
        {
            l = head;
        }
    }

    l = head;
    for(i = 0; i < k; i++) //sort sublists
    {
        l->node = insertionSort(l->node, &n_comp);
        l = l->next;
    }
    
    l = head;
    pass = 0;
    Node* sortedHead = NULL;
    for(i = 0; i < size; i++) //put sub lists back together
    {
        temp = pop(&(l->node));
        sorted = append(sorted, temp);
        if (pass == 0)
        {
            sortedHead = sorted;
            pass = 1;
        }
        if (l->next != NULL)
        {
            l = l->next;
        }
        else
        {
            l = head;
        }
    }

    //need to free all sublists
    freeList(head);

    return(sortedHead);
}

int List_Save_To_File(char *filename, Node *list)
{
    FILE* output = fopen(filename, "wb");
    int count = 0;

    if (output == NULL)
    {
        return(NULL);
    }

    while(list != NULL)
    {
        fwrite(list, sizeof(list), 1, output);
        list = list->next;
        count++;
    }

    fclose(output);

    return(count);
}

