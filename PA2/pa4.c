#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "hbt.h"

Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(int, Tnode*);
Tnode* delete(int, Tnode*);
Tnode* buildBST(char*, char*);

int calcNewHeight(Tnode*);
int getHeight(Tnode*);
int height(Tnode*);
int bigger(int, int);
int getBalance(Tnode*);
void freeBST(Tnode*);
void writePreorder(Tnode*, FILE*);

//write bst to file using preorder traversal
void writePreorder(Tnode* node, FILE* file)
{
    char bit; 

    if (node != NULL)
    {
        //solving bitwise
        if (node->right != NULL && node->left != NULL) //if left and right child
        {
            bit = 3; //11
        }
        else if (node->right == NULL && node->left != NULL) //if only left child
        {
            bit = 2; //10
        }
        else if (node->right != NULL && node->left == NULL) //if only right child
        {
            bit = 1; //01
        }
        else if (node->right == NULL && node->left == NULL) //if no child
        {
            bit = 0; //00
        }

        char direction = bit | 0;

        //printf("Node: %d Child: %d\n",node->key, direction);
        fwrite(&(node->key), sizeof(int), 1, file);
        fwrite(&direction, sizeof(char), 1, file);
        writePreorder(node->left, file);
        writePreorder(node->right, file);
    }

    return;
}

//Calculate height of entire tree
int calcNewHeight(Tnode* node)
{
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    int newHeight = bigger(leftHeight, rightHeight) + 1; //height is higher of the left and right child + 1

    return(newHeight);
}

//Get height cause you can't get height from null so this makes it way easier
int getHeight(Tnode* n) //tired of rewriting this
{
    int height;

    if (n == NULL)
    {
        height = -1; //height of most bottom node is 0
    }
    else
    {
        height = n->height;
    }
    
    return(height);
}

//Get bigger of two values
int bigger(int a, int b) //tired of rewriting this
{
    if (a > b)
    {
        return(a);
    }
    else
    {
        return(b);
    }
}

//Calculate the balance of a node using the heights of the left and right child
int getBalance(Tnode* n)
{
    int left;
    int right;
    int balance;

    if (n == NULL) //if no node balance is 0 so that the above can be + or - 1
    {
        return(0);
    }

    left = getHeight(n->left);
    right = getHeight(n->right);
    balance = left - right;

    return(balance);
}

//free for memory
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

//Printing functions for testing
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

//Create new node
Tnode* newNode(int val)
{
    Tnode* new = (struct _Tnode*)malloc(sizeof(struct _Tnode));
    new->key = val;
    new->left = NULL;
    new->right = NULL;
    new->height = 0;
    return(new);
}

//Clockwise rotation
Tnode* CR(Tnode* old)
{
    Tnode* new;

    new = old->left;
    old->left = new->right;
    new->right = old;
    
    //printf("CR on %d Height: %d, new = %d Height: %d\n", old->key, old->height, new->key, new->height);

    return(new);
}

Tnode* CCR(Tnode* old)
{
    Tnode* new;

    new = old->right;
    old->right = new->left;
    new->left = old;

    //printf("CCR on %d Height: %d, new = %d Height: %d\n", old->key, old->height, new->key, new->height);

    return(new);
}

Tnode* balance(Tnode* node, int key)
{
    Tnode* temp = NULL;
    Tnode* old = NULL;
    Tnode* old2 = NULL;
    int nodeBalance = 0;
    int rightBalance = 0;
    int leftBalance = 0;

    nodeBalance = getBalance(node);
    rightBalance = getBalance(node->right);
    leftBalance = getBalance(node->left);

    if (nodeBalance > 1) //left side is heavier
    {
        if (leftBalance > -1) //left left
        {
            old = node;
            temp = CR(node);
        }
        else if (leftBalance < 0) //left right
        {
            old = node->left;
            node->left = CCR(node->left);
            old2 = node;
            temp = CR(node);
        }
    }
    else if (nodeBalance < -1) //right side is heavier
    {
        if (rightBalance < 1) //right left
        {
            old = node;
            temp = CCR(node);
        }
        else if (rightBalance > 0) //right right
        {
            old = node->right;
            node->right = CR(node->right);
            old2 = node;
            temp = CCR(node);
        }
    }

    if (old != NULL)
    {
        old->height = calcNewHeight(old);

        if (old->left != NULL)
        {
            old->left->height = calcNewHeight(old->left);
        }

        if (old->right != NULL)
        {
            old->right->height = calcNewHeight(old->right);
        }
    }

    if (old2 != NULL)
    {
        old2->height = calcNewHeight(old2);

        if (old2->left != NULL)
        {
            old2->left->height = calcNewHeight(old2->left);
        }

        if (old2->right != NULL)
        {
            old2->right->height = calcNewHeight(old2->right);
        }
    }

    if (temp)
    {
        temp->height = calcNewHeight(temp);
        temp->right->height = calcNewHeight(temp->right);
        temp->left->height = calcNewHeight(temp->left);
    }

    return (temp != NULL) ? temp : node; //if temp return temp if not temp return node
}

//Insert node and balance
Tnode* insertAVL(int key, Tnode* node)
{
    //insertion
    if (node == NULL)
    {
        return(newNode(key));
    }
    if (key > node->key)
    {
        node->right = insertAVL(key, node->right);
        node->height = calcNewHeight(node);
    }
    else //duplicate or less than
    {
        node->left = insertAVL(key, node->left);
        node->height = calcNewHeight(node);
    }

    //balance
    node = balance(node, key);

    return(node);
}

//Delete node and balance
Tnode* deleteAVL(int key, Tnode* node)
{
    Tnode* temp = NULL;
    Tnode* predecessor = NULL;

    //normal deletion
    if (node == NULL)
    {
        return(node);
    }
    if (key > node->key)
    {
        node->right = deleteAVL(key, node->right);
        node->height = calcNewHeight(node);
    }
    else if (key < node->key)
    {
        node->left = deleteAVL(key, node->left);
        node->height = calcNewHeight(node);
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
        }
        else if (node->left != NULL && node->right == NULL) //only left child
        {
            temp = node->left;
        }

        if (temp) //if only either left or right child replace with temp
        {
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
            node->left = deleteAVL(key, node->left); //delete the same key again
        }
        node->height = calcNewHeight(node);
    }
 
    //balance
    node = balance(node, key);
    return(node);
}

//check to make sure if tree is a binary search tree
int isBST(Tnode* node)
{
    if (node == NULL) //is balanced
    {
        return(1); //true
    }

    if (((node->left != NULL) && (node->left->key > node->key)) || ((node->right != NULL) && (node->right->key < node->key)))
    {
        return(0); //false
    }

    int leftBst = isBST(node->left);
    int rightBst = isBST(node->right);

    if ((leftBst == 0) || (rightBst == 0))
    {
        return(0); //false
    }
    else
    {
        return(1); //true
    }
}

//check if tree is balanced
int isBalanced(Tnode* node)
{
    if (node == NULL)
    {
        return(1);
    }

    int balance = getBalance(node);
    bool balanced = !((balance < -1) || (balance > 1));

    if (balanced)
    {
        int leftBalance = isBalanced(node->left);
        int rightBalance = isBalanced(node->right);
        int isBal = leftBalance && rightBalance;

        return(isBal);
    }
    else if (!balanced)
    {
        return(0);
    }
    else
    {
        return(0);
    }
}

//make bst using characters and keys arrays
Tnode* insertBST(int* keys, char* branches, int* index, int size) 
{
    if (*index > size)
    {
        return(NULL);
    }
    else if (*index < size)
    {
        int i = *index;
        *index += 1;

        Tnode* temp = newNode(keys[i]);
        char branch = branches[i];

        if (branch == 1) //01
        {
            temp->right = insertBST(keys, branches, index, size);
            temp->height = calcNewHeight(temp);
        }
        else if (branch == 2) //10
        {
            temp->left = insertBST(keys, branches, index, size);
            temp->height = calcNewHeight(temp);
        }
        else if (branch == 3) //11
        {
            temp->left = insertBST(keys, branches, index, size);
            temp->height = calcNewHeight(temp);
            temp->right = insertBST(keys, branches, index, size);
            temp->height = calcNewHeight(temp);
        }

        temp->height = calcNewHeight(temp);
        return(temp);
    }
    else
    {
        return(NULL);
    }
}

int main(int argc, char* argv[])
{
    if (argc > 4)
    {
        fprintf(stdout, "%d\n", 0);
        return EXIT_FAILURE;
    }

    Tnode* bst = NULL;
    Tnode* temp = NULL;

    //building height-balanced BST (AVL)
    if (strcmp(argv[1], "-b") == 0) //if argv[1] == -b
    {
        //Load Input File
        FILE* ops = fopen(argv[2], "rb");

        if (ops == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }

        //Load Output File
        FILE* opsOut = fopen(argv[3], "wb");

        if (opsOut == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;
        }

        fseek(ops, 0L, SEEK_END); //check if fseek fails
        int size = ftell(ops) / 5; //amount of (int char) in file
        rewind(ops);

        //Create and fill AVL using file
        int intBuffer;
        char charBuffer;
        int i;

        for (i = 0; i < size; i++)
        {
            fread(&intBuffer, sizeof(intBuffer), 1, ops);
            fread(&charBuffer, sizeof(charBuffer), 1, ops);
            //maybe check if intbuffer and charbuffer are holding values?
            if (charBuffer == 'i') //insert
            {
                //printf("\nInsert: %d\n", intBuffer);
                temp = insertAVL(intBuffer, bst);
                if (temp)
                {
                    bst = temp;
                }
            }
            else if (charBuffer == 'd')//delete
            {
                //printf("Delete: %d\n", intBuffer);
                bst = deleteAVL(intBuffer, bst);
            }
            else //wrong format
            {
                writePreorder(bst, opsOut);
                freeBST(bst);
                fclose(ops);
                fclose(opsOut);
                fprintf(stdout, "%d\n", 0);
                return EXIT_FAILURE;
            }
        }

        //Write to file
        writePreorder(bst, opsOut);

        fclose(ops);
        fclose(opsOut);
        freeBST(bst);
        fprintf(stdout, "%d\n", 1);
        return EXIT_SUCCESS;
    }
    
    //evaluating a BST
    else if (strcmp(argv[1], "-e") == 0) //if argv[1] == -e
    {
        FILE* tree = fopen(argv[2], "rb");

        if (tree == NULL)
        {
            fprintf(stdout, "%d %d %d\n", -1, 0, 0);
            return EXIT_FAILURE;
        }

        fseek(tree, 0L, SEEK_END);
        int size = ftell(tree) / 5; //amount of (int char) in file
        rewind(tree);

        int intBuffer;
        char charBuffer;
        //check if malloc fails
        int *keyArr = (int*)malloc(size * sizeof(int)); //array containing keys
        char *branchArr = (char*)malloc(size * sizeof(char)); //array containing # of children for each key
        int i;

        for (i = 0; i < size; i++)
        {
            fread(&intBuffer, sizeof(intBuffer), 1, tree);
            fread(&charBuffer, sizeof(charBuffer), 1, tree);
            keyArr[i] = intBuffer;
            branchArr[i] = charBuffer;

            if (charBuffer != 0 && charBuffer != 1 && charBuffer != 2 && charBuffer != 3)
            {
                fprintf(stdout, "%d %d %d\n", 0, 0, 0);
                free(keyArr);
                free(branchArr);
                fclose(tree);
                return EXIT_FAILURE;
            }
        }

        int index = 0;
        bst = insertBST(keyArr, branchArr, &index, size);
        int isBst = isBST(bst);
        int isBal =  isBalanced(bst);

        free(keyArr);
        free(branchArr);
        freeBST(bst);
        fclose(tree);
        fprintf(stdout, "%d %d %d\n", 1, isBst, isBal);
        return EXIT_SUCCESS;
    }

    else if (strcmp(argv[1], "-test") == 0)
    {
        int num, i;
        srand(time(NULL));

        for(i=30; i > 0; --i)
        {
            num = rand();
            num = num % 59;

            //printf("\nInsert: %d\n", num);
            bst = insertAVL(num, bst);
        }

        for(i=0; i < 40; ++i)
        {
            num = rand();
            num = num % 59;

            //printf("\nDelete: %d\n", num);
            bst = deleteAVL(num, bst);
        }

        FILE* opsOut = fopen(argv[3], "wb");
        writePreorder(bst, opsOut);

        freeBST(bst);
        fclose(opsOut);
        return EXIT_SUCCESS;
    }

    else 
    {
        fprintf(stdout, "%d\n", 0);
        return EXIT_FAILURE;
    }
}