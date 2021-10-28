#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(int, Tnode*);
Tnode* delete(int, Tnode*);
Tnode* buildBST(char*, char*);

int height(Tnode*);
int bigger(int, int);
int getBalance(Tnode*);
void freeBST(Tnode*);
void writePreorder(Tnode*, FILE*);

int calcNewHeight(Tnode* node) //compares left and right
{
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    int newHeight = bigger(leftHeight, rightHeight) + 1;

    return(newHeight);
}

int getHeight(Tnode* n)
{
    int height;

    if (n == NULL)
    {
        height = -1;
    }
    else
    {
        height = n->height;
    }
    return(height);
}

int bigger(int a, int b)
{
    return((a > b) ? a : b);
}

int getBalance(Tnode* n)
{
    int left;
    int right;
    if (n == NULL)
    {
        return(0);
    }
    left = getHeight(n->left);
    right = getHeight(n->right);
    return(left - right);
}

void freeBST(Tnode* node)
{
    if (node == NULL)
    {
        return;
    }
    freeBST(node->left);
    freeBST(node->right);
    free(node);
}

void preorder(Tnode *node)
{
    if (node != NULL)
    {
        fprintf(stdout, "%d ", node->key);
        preorder(node->left);
        preorder(node->right);
    }
}

void inorder(Tnode *node)
{
    if (node != NULL)
    {
        inorder(node->left);
        fprintf(stdout, "%d ", node->key);
        inorder(node->right);
    }
}

void postorder(Tnode *node)
{
    if (node != NULL)
    {
        postorder(node->left);
        postorder(node->right);     
        fprintf(stdout, "%d ", node->key);
    }
}

Tnode* newNode(int val)
{
    Tnode* new = (struct _Tnode*)malloc(sizeof(struct _Tnode));
    new->key = val;
    new->left = NULL;
    new->right = NULL;
    new->height = 0; //1
    return(new);
}

Tnode* CR(Tnode* y) //change in terms of old and new
{
    Tnode* x = y->left;
    Tnode* t = x->right;

    x->right = y;
    y->left = t;

    y->height = bigger(getHeight(y->left), getHeight(y->right)) + 1; //+1
    x->height = bigger(getHeight(x->left), getHeight(x->right)) + 1; //+1

    //printf("CR on %d, new = %d\n", y->key, x->key);

    return(x); 
}

Tnode* CCR(Tnode* old)
{
    Tnode* new;

    new = old->right;

    old->right = new->left;
    old->height = calcNewHeight(old);

    new->left = old;
    new->height = calcNewHeight(new);

    //printf("CCR on %d Height: %d, new = %d Height: %d\n", old->key, old->height, new->key, new->height);

    return(new);
}

Tnode* balance(Tnode* node)
{
    Tnode* temp = NULL;
    int nodeBalance;
    int rightBalance;
    int leftBalance;

    nodeBalance = getBalance(node);
    rightBalance = getBalance(node->right);
    leftBalance = getBalance(node->left);

    if (nodeBalance > 1) //left side is heavier
    {
        if (leftBalance > -1) //left left
        {
            temp = CR(node);
        }
        else if (leftBalance < 0) //left right
        {
            node->left = CCR(node->left);
            temp = CR(node);
        }
    }
    else if (nodeBalance < -1) //right side is heavier
    {
        if (rightBalance < 1) //right left
        {
            temp = CCR(node);
        }
        else if (rightBalance > 0) //right right
        {
            node->right = CR(node->right);
            temp = CCR(node);
        }
    }

    return (temp != NULL) ? temp : node; //if temp return temp if not temp return node
}

Tnode* insert(int key, Tnode* node)
{
    //insertion
    if (node == NULL)
    {
        return(newNode(key));
    }
    if (key > node->key)
    {
        node->right = insert(key, node->right);
    }
    else //duplicate or less than
    {
        node->left = insert(key, node->left);
    }

    //balance
    node->height = calcNewHeight(node);
    node = balance(node);

    return(node);
}

Tnode* delete(int key, Tnode* node)
{
    Tnode* temp = NULL;
    Tnode* predecessor = NULL;

    if (node == NULL)
    {
        return(node);
    }
    if (key > node->key)
    {
        node->right = delete(key, node->right);
    }
    else if (key < node->key)
    {
        node->left = delete(key, node->left);
    }
    else if (key == node->key)
    {
        if (node->left == NULL && node->right == NULL) //no children
        {
            free(node);
            return(NULL);
        }
        else if (node->left == NULL && node->right != NULL) //only right child
        {
            temp = node->right;
            free(node);
            return(temp);
        }
        else if (node->left != NULL && node->right == NULL) //only left child
        {
            temp = node->left;
            free(node);
            return(temp);
        }
        else if (node->left != NULL && node->right != NULL) //both children
        {
            predecessor = node->left; //find inorder predecessor
            while(predecessor->right != NULL)
            {
                predecessor = predecessor->right;
            }

            node->key = predecessor->key; //swap values
            predecessor->key = key;
            node->left = delete(predecessor->key, node->left);
        }
    }

    if (node == NULL) //edge case for when bst is empty
    {
        return(NULL);
    }
 
    //balance
    node->height = calcNewHeight(node);
    node = balance(node);
    return(node);
}

Tnode* buildBST(char* input, char* output)
{
    //CHANGE TO FILE AND FILE FOR INPUT AND OUTPUT OR DO IT SIMILAR TO PA1 WHERE THE FILE LOAD AND SAVE FUNCTIONS ARE SEPARATE SO YOU 
    //CAN RETURN EXIT FAILURE
    Tnode* bst = NULL;
    Tnode* temp = NULL;

    FILE* ops = fopen(input, "rb");

    if (ops == NULL)
    {
        return (NULL);
    }

    fseek(ops, 0L, SEEK_END);
    int size = ftell(ops) / 5; //amount of (int char) in file
    rewind(ops);

    int intBuffer;
    char charBuffer;
    int i;

    for (i = 0; i < size; i++)
    {
        fread(&intBuffer, sizeof(intBuffer), 1, ops);
        fread(&charBuffer, sizeof(charBuffer), 1, ops);
        if (charBuffer == 'i') //insert
        {
            //printf("\nInsert: %d\n", intBuffer);
            temp = insert(intBuffer, bst);
            if (temp)
            {
                bst = temp;
            }
        }
        else //delete
        {
            //printf("Delete: %d\n", intBuffer);
            bst = delete(intBuffer, bst);
        }
    }
    
    
    preorder(bst);
    fprintf(stdout, "\n");
    inorder(bst);
    fprintf(stdout, "\n");
    postorder(bst);
    fprintf(stdout, "\n");
    

    fclose(ops);
    freeBST(bst);
    return(bst);
}

int main(int argc, char* argv[])
{
    if (argc > 4)
    {
        return EXIT_FAILURE;
    }

    Tnode* bst = NULL;

    //building height-balanced BST
    if (strcmp(argv[1], "-b") == 0) //if argv[1] == -b
    {
        Tnode* bst = buildBST(argv[2], argv[3]);
    }
    
    //evaluating a height balanced BST
    else if (strcmp(argv[1], "-e") == 0) //if argv[1] == -e
    {
        printf("godb\n");
    }

    else if (strcmp(argv[1], "-t") == 0)
    {
        int i;
        for (i = 1; i < 1000; i++)
        {
            bst = insert(i, bst);
        }
        for (i = 1; i < 950; i++)
        {
            bst = delete(i, bst);
        }
        preorder(bst);
        fprintf(stdout, "\n");
        inorder(bst);
        fprintf(stdout, "\n");
        postorder(bst);
        fprintf(stdout, "\n");
    }

    return EXIT_SUCCESS;
}