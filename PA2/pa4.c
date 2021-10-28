#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hbt.h"
Tnode* newNode(int);
Tnode* CR(Tnode*);
Tnode* CCR(Tnode*);
Tnode* insert(int, Tnode*);
Tnode* delete(int, Tnode*);
Tnode* buildBST(char*);

int height(Tnode*);
int max(int, int);
int getBalance(Tnode*);
Tnode* inorderPredessesor(Tnode*);
void freeBST(Tnode*);

int calcNewHeight(Tnode* node)
{
    int leftHeight = getHeight(node->left) + 1;
    int rightHeight = getHeight(node->right) + 1;
    int newHeight = max(leftHeight, rightHeight);

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

int max(int a, int b)
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
    new->height = 0; //1
    return(new);
}

Tnode* CR(Tnode* y) //change in terms of old and new
{
    Tnode* x = y->left;
    Tnode* t = x->right;

    x->right = y;
    y->left = t;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1; //+1
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1; //+1

    //printf("CR on %d, new = %d\n", y->key, x->key);

    return(x); 
}

Tnode* CCR(Tnode* old) //change in terms of old and new
{
    Tnode* new;

    new = old->right;
    old->right = new->left;
    new->left = old;

    old->height = calcNewHeight(old);
    new->height = calcNewHeight(new);

    //printf("CCR on %d Height: %d, new = %d Height: %d\n", old->key, old->height, new->key, new->height);

    return(new);
}

Tnode* insert(int val, Tnode* node)
{

    //insertion
    if (node == NULL)
    {
        return(newNode(val));
    }

    if (val > node->key)
    {
        node->right = insert(val, node->right);
    }
    else //duplicate or less than
    {
        node->left = insert(val, node->left);
    }

    //balance
    Tnode* temp = NULL;
    int nodeBalance;
    int rightBalance;
    int leftBalance;

    node->height = calcNewHeight(node);
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
        if (rightBalance < 1) //right right
        {
            temp = CCR(node);
        }
        else if (rightBalance > 0) //right left
        {
            node->right = CR(node->right);
            temp = CCR(node);
        }
    }

    return (temp != NULL) ? temp : node; //if temp return temp if not temp return node
}

Tnode* delete(int key, Tnode* root)
{
  if (root == NULL) {
    return NULL;
  }
  if (key < root -> key) {
    root -> left = delete(key, root -> left);
  }
  else if (key > root -> key) {
    root -> right = delete(key, root -> right);
  }
  // now val == root -> key.
  else {
    // root has no child.
    if ((root -> left == NULL) && (root -> right == NULL)) {
      free(root);
      return NULL;
    }
    if (root -> left == NULL) {
      // there must be right child.
      Tnode* rc = root -> right;
      free(root);
      return rc;
    }
    if (root -> right == NULL) {
      // must have left child.
      Tnode* lc = root -> left;
      free(root);
      return lc;
    }
    // root has both left and right child.
    // find the largest one in left sub-tree.
    Tnode* p = root -> left;
    while (p -> right != NULL) {
      p = p -> right;
    }
    // pass its value to root and delete it.
    root -> key = p -> key;
    p -> key = key;
    root -> left = delete(key, root -> left);
  }
 
    // If the tree had only one node then return
    if (root == NULL)
      return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(getHeight(root->left),
                           getHeight(root->right));
 
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
    Tnode* temp = NULL;
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
        for (i = 1; i < 20; i++)
        {
            bst = insert(i, bst);
        }
        /*
        for (i = 1; i < 1000000; i++)
        {
            bst = delete(bst, i);
            
        }
        */
        preorder(bst);
        fprintf(stdout, "\n");
        inorder(bst);
        fprintf(stdout, "\n");
        postorder(bst);
        fprintf(stdout, "\n");
    }

    return EXIT_SUCCESS;
}