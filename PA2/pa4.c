#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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
            bit = 3;//11
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
int getHeight(Tnode* n) //tired of rewriting this shit
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
int bigger(int a, int b) //tired of rewriting this shit
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

//CounterClockwise rotation
Tnode* CCR(Tnode* old)
{
    Tnode* new;

    new = old->right;
    old->right = new->left;
    new->left = old;
    //printf("CCR on %d Height: %d, new = %d Height: %d\n", old->key, old->height, new->key, new->height);

    return(new);
}

//Function to balance nodes and perform rotations if necessary
Tnode* balance(Tnode* node, int key)
{
    Tnode* temp = NULL;
    int nodeBalance = 0;
    int rightBalance = 0;
    int leftBalance = 0;

    nodeBalance = getBalance(node);
    rightBalance = getBalance(node->right);
    leftBalance = getBalance(node->left);

    //logic from slides
    if ((nodeBalance < 2) && (nodeBalance > -2))
    {
        return(node);
    }

    if (nodeBalance == 2) //left side is heavier
    {
        if (leftBalance == 1) //same direction
        {
            temp = CR(node);
        }
        else if (leftBalance == -1) //oppossite direction
        {
            node->left = CCR(node->left);
            temp = CR(node);
        }
    }
    else if (nodeBalance == -2) //right side is heavier
    {
        if (rightBalance == -1) //same direction
        {
            temp = CCR(node);
        }
        else if (rightBalance == 1) //opposite direction
        {
            node->right = CR(node->right);
            temp = CCR(node);
        }
    }

    if (temp != NULL) //if rotated need to update heights
    {
        temp->height = calcNewHeight(temp);
        temp->left->height = calcNewHeight(temp->left);
        temp->right->height = calcNewHeight(temp->right);
        return(temp);
    }
    else
    {
        return(node);
    }
}

//Insert node and balance
Tnode* insertAVL(int key, Tnode* node)
{
    //insertion like normal
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

        if (temp) //if only either left or right child return temp
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
            node->left = deleteAVL(predecessor->key, node->left);
        }
        node->height = calcNewHeight(node);
    }
 
    //balance
    node = balance(node, key);
    return(node);
}

int BST_check(Tnode* root) {
  if (root == NULL) {
    return 1;
  }
  Tnode* lc = root -> left;
  Tnode* rc = root -> right;
  // check if this node satisfy the BST.
  if ((lc != NULL) && (lc -> key > root -> key)) {
    return 0;
  }
  if ((rc != NULL) && (rc -> key < root -> key)) {
    return 0;
  }
  int lrtv = BST_check(root -> left);
  int rrtv = BST_check(root -> right);
  return (lrtv && rrtv);
}

int balance_check(Tnode* root) {
  if (root == NULL) {
    return 1;
  }
  int bal = getBalance(root);
  if ((bal < -1) || (bal > 1)) {
    return 0;
  }
  char lb = balance_check(root -> left);
  char rb = balance_check(root -> right);
  return (lb && rb);
}

Tnode* insertBST(int* keys, char* branches, int* index, int size) {
  if (*index > size) {
    // reach the end of the array.
    return NULL;
  }
  Tnode* p = newNode(keys[*index]);
  char child = branches[*index];
  (*index) ++;
  if (child == 3) {
    // this node has both children.
    p -> left = insertBST(keys, branches, index, size);
    p->height = calcNewHeight(p);
    p -> right = insertBST(keys, branches, index, size);
    p->height = calcNewHeight(p);
    return p;
  }
  if (child == 2) {
    // this node has only left child.
    p -> left = insertBST(keys, branches, index, size);
    p->height = calcNewHeight(p);
    return p;
  }
  if (child == 1) {
    // this node only has left child.
    p -> right = insertBST(keys, branches, index, size);
    p->height = calcNewHeight(p);
    return p;
  }
  // this node does not have any child.

  p->height = calcNewHeight(p);
  return p;
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
        //Load File
        FILE* ops = fopen(argv[2], "rb");

        if (ops == NULL)
        {
            fprintf(stdout, "%d\n", 0);
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
        }

        //Load Output File
        FILE* opsOut = fopen(argv[3], "wb");

        if (opsOut == NULL)
        {
            fprintf(stdout, "%d\n", 0);
            return EXIT_FAILURE;
        }

        //Write to file
        writePreorder(bst, opsOut);

        /*
        preorder(bst);
        fprintf(stdout, "\n");
        inorder(bst);
        fprintf(stdout, "\n");
        postorder(bst);
        fprintf(stdout, "\n");
        */

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
            fprintf(stdout, "%d\n", 0);
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
        }

        int index = 0;
        bst = insertBST(keyArr, branchArr, &index, size);
        int isGood = BST_check(bst);
        int isGood2 =  balance_check(bst);

        free(keyArr);
        free(branchArr);
        freeBST(bst);
        fclose(tree);
        printf("BST????: %d %d\n", isGood, isGood2); //change
        return EXIT_SUCCESS;
    }

    else 
    {
        fprintf(stdout, "%d\n", 0);
        return EXIT_FAILURE;
    }
}