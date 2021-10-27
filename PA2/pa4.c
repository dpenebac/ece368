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

int height(Tnode*);
int max(int, int);
int getbalance(Tnode*);
Tnode* inorderPredessesor(Tnode*);
void freeBST(Tnode*);

int calcHeight(Tnode* root) //"balance"
{
    if (root == NULL)
    {
        return(-1);
    }

    int left = calcHeight(root->left);
    int right = calcHeight(root->right);
    root->height = left - right;
    return(max(left, right) + 1);
}

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

Tnode* inorderPredessesor(Tnode* n)
{
    Tnode* current = n;

    while (current->left != NULL)
    {
        current = current->left;
    }

    return(current);
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
    Tnode* t = y->left;

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

    //balance
    Tnode* temp = NULL;
    Tnode* root = node;
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