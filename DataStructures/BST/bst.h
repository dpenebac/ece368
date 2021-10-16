#ifndef __bst__
#define __bst__

struct node
{
    int val;
    node *left;
    node *right;
};

node* newNode(int);
void inorder(node*); //print inorder
node* insert(node*, int); //insert node with int value
node* findMin(node*); //go left forever basically
node* delNode(node*, int); //delete node with int value

#endif