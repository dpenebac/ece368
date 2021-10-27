#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"

Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(Tnode*, int, int*);
Tnode* delete(Tnode*, int);
Tnode* buildBST(char*);

int calcHeight(Tnode*);
int height(Tnode*);
int max(int, int);
void freeBST(Tnode*);

int calcHeight(Tnode* node) //calc balance of each node
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
    new->height = 0;
    return(new);
}

Tnode* CR(Tnode* old)
{
    Tnode* new = old->left;
    old->left = new->right;
    new->right = old;

    printf("CR on %d\n", old->key);
    return(new); 
}

Tnode* CCR(Tnode* old)
{
    Tnode* new = old->right;
    old->right = new->left;
    new->left = old;

    //printf("CRR on %d new Val: %d\n", old->key, new->key);
    return(new);
}

Tnode* insert(Tnode* node, int val, int *rotate) //rotate is 0 or 1
{
    if (node == NULL)
    {
        return(newNode(val));
    }
    if (val <= node->key) //duplicate keys go left
    {
        node->left = insert(node->left, val, rotate);
        node->height += 1;
    }
    else if (val > node->key)
    {
        node->right = insert(node->right, val, rotate);
        node->height -= 1;
    }

    //balance
    Tnode* temp = NULL;
    //calcHeight(node);
    if (*rotate != 0) 
    {
        //printf("\nold: node: %d height: %d", node->key, node->height);
        //calcHeight(node); //this is still checking root every single time for all of them, maybe just update + or - 1 based on left or right
        
        //probably realllllly buggy
        if (node->height <= -1)
        {
            node->height += 1;
        }
        else if (node->height >= 1)
        {
            node->height -= 1;
        }
        
        //printf("\nnew: node: %d height: %d\n\n", node->key, node->height);
    }

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
    }
    else if ((node->height < -1) && (rightHeight < 0)) //right right
    {
        temp = CCR(node);
    }
    else if ((node->height > 1) && (leftHeight < 0)) //left right
    {
        node->left = CCR(node->left);
        temp = CR(node);
    }
    else if ((node->height < -1) && (rightHeight > 0)) //right left
    {
        node->right = CR(node->right);
        temp = CCR(node);
    }

    if (temp != NULL) {*rotate = 1;}
    calcHeight(temp); //calculate balance after rotations
    return (temp != NULL) ? temp : node; //return temp if temp if not node
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
        root->left = CCR(root->left);
        temp = CR(root);
        return(temp);
    }
    else if ((root->height < -1) && (rightHeight > 0)) //right left
    {
        root->right = CR(root->right);
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
    int rotate = 0; //bool if rotate

    for (i = 0; i < size; i++)
    {
        fread(&intBuffer, sizeof(intBuffer), 1, ops);
        fread(&charBuffer, sizeof(charBuffer), 1, ops);
        if (charBuffer == 'i') //insert
        {
            //printf("\nInsert: %d\n", intBuffer);
            bst = insert(bst, intBuffer, &rotate);
            rotate = 0;
        }
        else //delete
        {
            //printf("Delete: %d\n\n", intBuffer);
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
    if (argc > 3) //CHANGE THIS
    {
        return EXIT_FAILURE;
    }

    Tnode* bst = NULL;

    //building height-balanced BST
    if (strcmp(argv[1], "-b") == 0) //if argv[1] == -b
    {
        bst = buildBST(argv[2]);
    }
    
    //evaluating a height balanced BST
    else if (strcmp(argv[1], "-e") == 0) //if argv[1] == -e
    {
        printf("godb\n");
    }

    else if (strcmp(argv[1], "-t") == 0)
    {
        int i;
        int rotate = 0;
        for (i = 1; i < 1000000; i++)
        {
            bst = insert(bst, i, &rotate);
            rotate = 0;
            
        }

        /*
        preorder(bst);
        fprintf(stdout, "\n");
        inorder(bst);
        fprintf(stdout, "\n");
        postorder(bst);
        fprintf(stdout, "\n");
        */
    }

    return EXIT_SUCCESS;
}