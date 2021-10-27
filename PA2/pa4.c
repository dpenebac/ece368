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

    int balance = getbalance(node);

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

int balance_cal(Tnode* root) {
  // use post-order to calculate the height.
  if (root == NULL) {
    return -1;
  }
  int lh = balance_cal(root -> left);
  int rh = balance_cal(root -> right);
  int bal = lh - rh;
  root -> height = bal;
  // the height is the maximum value among lh and rh
  return(lh >= rh) ? (lh + 1) : (rh + 1);
}

Tnode* delete(Tnode* root, int val)
{
  if (root == NULL) {
    return NULL;
  }
  if (val < root -> key) {
    root -> left = delete(root->left, val);
  }
  else if (val > root -> key) {
    root -> right = delete(root->right, val);
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
    p -> key = val;
    root -> left = delete(root->left, val);
  }
  // update height and balance.
  // may need balance at each level.
  balance_cal(root);
  int bal = root -> height;
  int lb = (root -> left == NULL) ? 0: root -> left -> height;
  int rb = (root -> right == NULL) ? 0: root -> right -> height;
  if ((bal > 1) && (lb >= 0)) {
      printf("hi\n");
    return CR(root);
  }
  if ((bal < -1) && (rb <= 0)) {
    return CCR(root);
  }
  if ((bal > 1) && (lb < 0)) {
    root -> left = CCR(root -> left);
    return CR(root);
  }
  if ((bal < -1) && (rb > 0)) {
    root -> right = CR(root -> right);
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
        for (i = 1; i < 11; i++)
        {
            bst = insert(bst, i);
            
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