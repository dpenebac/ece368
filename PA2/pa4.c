#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(Tnode*, int);
Tnode* delete(Tnode*, int);
Tnode* buildBST(char*);

int calcHeight(Tnode*);
int height(Tnode*);
int max(int, int);
void freeBST(Tnode*);

int calcHeight(Tnode* node)
{
    int left;
    int right;
    int height;

    if (node == NULL)
    {
        return(-1);
    }

    left = calcHeight(node->left);
    right = calcHeight(node->right);
    node->height = left - right;
    height = max(left, right) + 1;
    return(height);
}

int max(int a, int b)
{
    return((a > b) ? a : b);
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

Tnode* CR(Tnode* old)
{
    Tnode* new = old->left;
    old->left = new->right;
    new->right = old;

    return(new); 
}

Tnode* CCR(Tnode* old)
{
    Tnode* new = old->right;
    old->right = new->left;
    new->left = old;
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
    else if (val > node->key)
    {
        node->right = insert(node->right, val);
    }

    //balance
    Tnode* temp = NULL;
    calcHeight(node); //calc "balance"

    int leftHeight = 0;
    int rightHeight = 0;
    if (node->left != NULL)
    {
        leftHeight = node->left->height;
    }

    if (node->right != NULL)
    {
        rightHeight = node->right->height;
    }
    
    if ((node->height > 1) && (leftHeight > 0)) //left left
    {
        temp = CR(node);
        return(temp);
    }
    else if ((node->height < -1) && (rightHeight < 0)) //right right
    {
        temp = CCR(node);
        return(temp);
    }
    else if ((node->height > 1) && (leftHeight < 0)) //left right
    {
        node->left = CCR(node -> left);
        temp = CR(node);
        return(temp);
    }
    else if ((node->height < -1) && (rightHeight > 0)) //right left
    {
        node->right = CR(node -> right);
        temp = CCR(node);
        return(temp);
    }
    else //balanced
    {
        return node;
    }
}

Tnode* delete(Tnode* root, int key)
{
    Tnode* temp;

    if (root == NULL)
    {
        return(NULL);
    }
    
    if (key < root->key)
    {
        root->left = delete(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = delete(root->right, key);
    }
    else //val == root->key
    {
        if ((root->left == NULL) && (root->right == NULL)) //root has no child
        {
            free(root);
            return(NULL);
        }
        else if (root->right != NULL && root->left == NULL) //if only right child
        {
            temp = root->right;
            free(root);
            return(temp);
        }
        else if (root->left != NULL && root->right == NULL) //if only left child
        {
            temp = root->left;
            free(root);
            return(temp);
        }
        else if (root->left && root->right) //both left and right child
        {   
            temp = root->left; //finding inorder successor
            while (temp->right != NULL)
            {
                temp = temp->right;
            }

            //swap inorder successor with target node
            root->key = temp->key;
            temp->key = key;

            root->left = delete(root->left, key);
        }
    }
    
    //balance
    calcHeight(root); //calc "balance"

    int leftHeight = 0;
    int rightHeight = 0;
    if (root->left != NULL)
    {
        leftHeight = root->left->height;
    }

    if (root->right != NULL)
    {
        rightHeight = root->right->height;
    }
    
    if ((root->height > 1) && (leftHeight > 0)) //left left
    {
        temp = CR(root);
        return(temp);
    }
    else if ((root->height < -1) && (rightHeight < 0)) //right right
    {
        temp = CCR(root);
        return(temp);
    }
    else if ((root->height > 1) && (leftHeight < 0)) //left right
    {
        root->left = CCR(root -> left);
        temp = CR(root);
        return(temp);
    }
    else if ((root->height < -1) && (rightHeight > 0)) //right left
    {
        root->right = CR(root -> right);
        temp = CCR(root);
        return(temp);
    }
    else //balanced
    {
        return root;
    }
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
            //printf("Insert: %d\n", intBuffer);
            bst = insert(bst, intBuffer);
        }
        else //delete
        {
            //printf("Delete: %d\n", intBuffer);
            bst = delete(bst, intBuffer);
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
    if (argc > 3)
    {
        return EXIT_FAILURE;
    }

    Tnode* bst = NULL;

    //building height-balanced BST
    if (strcmp(argv[1], "-b") == 0) //if argv[1] == -b
    {
        Tnode* bst = buildBST(argv[2]);
    }
    
    //evaluating a height balanced BST
    else if (strcmp(argv[1], "-e") == 0) //if argv[1] == -e
    {
        printf("godb\n");
    }

    return EXIT_SUCCESS;
}