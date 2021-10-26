#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(Tnode*, int);
Tnode* buildBST(char*);

int height(Tnode*);
int max(int, int);
int getbalance(Tnode*);
void freeBST(Tnode*);

int height(Tnode* n)
{
    if (n == NULL)
    {
        return(0);
    }
    return(n->height);
}

int max(int a, int b)
{
    return((a > b) ? a : b);
}

int getbalance(Tnode* n)
{
    if (n == NULL)
    {
        return(0);
    }
    return(height(n->left) - height(n->right));
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
        inorder(node->left);
        inorder(node->right);     
        fprintf(stdout, "%d ", node->key);   
    }
}

Tnode* newNode(int val)
{
    Tnode* new = (struct _Tnode*)malloc(sizeof(struct _Tnode));
    new->key = val;
    new->left = NULL;
    new->right = NULL;
    new->height = 1;
    return(new);
}

Tnode* CR(Tnode* y)
{
    Tnode* x = y->left;
    Tnode* t = x->right;

    x->right = y;
    y->left = t;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return(x); 
}

Tnode* CCR(Tnode* x)
{
    Tnode* y = x->right;
    Tnode* t = y->left; //segfault

    y->left = x;
    x->right = t;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return(y);
}

Tnode* insert(Tnode* node, int val)
{
    if (node == NULL)
    {
        return(newNode(val));
    }

    if (val <= node->key) //duplicate keys go left
    {
        node->left = insert(node->left, val);
    }
    else if (val > node->key)//if val < node->key
    {
        node->right = insert(node->right, val);
    }

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getbalance(node);

    if (balance > 1 && val < node->left->key) //left left
    {
        return(CR(node));
    }

    if (balance < -1 && val > node->right->key) //right right
    {
        return(CCR(node));
    }

    if (balance > 1 && val > node->left->key) //left right
    {   
        node->left = CCR(node->left);
        return(CR(node));
    }

    if (balance < -1 && val < node->right->key) //right left
    {
        node->right = CR(node->right);
        return(CCR(node));
    }

    return(node);
}

Tnode* buildBST(char* filename)
{
    Tnode* bst = NULL;
    FILE* ops = fopen(filename, "rb");

    if (ops == NULL)
    {
        return(NULL);
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
            printf("intBuffer: %d\n", intBuffer);
            bst = insert(bst, intBuffer);
        }
        else //delete
        {
            printf("DELETE\n");
        }
    }

    preorder(bst);
    fprintf(stdout, "\n");
    inorder(bst);
    fprintf(stdout, "\n");
    postorder(bst);
    fprintf(stdout, "\n");
    fclose(ops);
    return(bst);
}

int main(int argc, char* argv[])
{
    if (argc > 3)
    {
        return EXIT_FAILURE;
    }

    Tnode* bst = NULL;

    //building height-balanced BST
    if (strcmp(argv[1], "-b") == 0) //if argv[1] == -b
    {
        Tnode* bst = buildBST(argv[2]);
        freeBST(bst);
    }
    
    //evaluating a height balanced BST
    else if (strcmp(argv[1], "-e") == 0) //if argv[1] == -e
    {
        printf("godb\n");
    }


    return EXIT_SUCCESS;
}