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
int getBalance(Tnode*);
Tnode* inorderPredessesor(Tnode*);
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

int getBalance(Tnode* n)
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

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    if (balance > 1 && val <= node->left->key) //left left
    {
        return(CR(node));
    }

    if (balance < -1 && val >= node->right->key) //right right
    {
        return(CCR(node));
    }

    if (balance > 1 && val >= node->left->key) //left right
    {
        node->left = CCR(node->left);
        return(CR(node));
    }

    if (balance < -1 && val <= node->right->key) //right left
    {
        node->right = CR(node->right);
        return(CCR(node));
    }

    return(node);
}

Tnode * minValueNode(Tnode* node)
{
    Tnode* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}
 
// Recursive function to delete a node with given key
// from subtree with given root. It returns root of
// the modified subtree.
Tnode* delete(Tnode* root, int key)
{
    // STEP 1: PERFORM STANDARD BST DELETE
 
    if (root == NULL)
        return root;
 
    // If the key to be deleted is smaller than the
    // root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = delete(root->left, key);
 
    // If the key to be deleted is greater than the
    // root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = delete(root->right, key);
 
    // if key is same as root's key, then This is
    // the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            Tnode *temp = root->left ? root->left :
                                             root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of
                            // the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            Tnode* temp = minValueNode(root->right);
 
            // Copy the inorder successor's data to this node
            root->key = temp->key;
 
            // Delete the inorder successor
            root->right = delete(root->right, temp->key);
        }
    }
 
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to
    // check whether this node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return CR(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left =  CCR(root->left);
        return CR(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return CCR(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = CR(root->right);
        return CCR(root);
    }
 
    return root;
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
            printf("Insert: %d\n", intBuffer);
            bst = insert(bst, intBuffer);
        }
        else //delete
        {
            printf("Delete: %d\n", intBuffer);
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

    else if (strcmp(argv[1], "-t") == 0)
    {
        int i;
        for (i = 1; i < 1000000; i++)
        {
            bst = insert(bst, i);
            
        }
        for (i = 1; i < 1000000; i++)
        {
            bst = delete(bst, i);
            
        }
        inorder(bst);
        fprintf(stdout, "\n");
    }

    return EXIT_SUCCESS;
}