#include <stdio.h>
#include <stdlib.h>

//Problem 1
typedef struct _tnode
{
    int info;
    struct _tnode *left;
    struct _tnode *right;
} Tnode;

Tnode* Tnode_construct(int info)
{
    Tnode *node = malloc(sizeof(*node));
    if (node == NULL)
    {
        fprintf(stderr, "can't get memory\n");
        return(NULL);
    }

    node->info = info;
    node->left =  node->right = NULL;
    return(node);
}

Tnode* BST_build(int *array, int lb, int ub)
{
    if (lb > ub)
    {
        return NULL;
    }

    int mid = (lb + ub) / 2;
    Tnode *node = Tnode_construct(array[mid]);
    node->left = BST_build(array, lb, mid - 1);
    node->right = BST_build(array, mid + 1, ub);
    return(node);
}

void preorder(Tnode *node)
{
    if (node != NULL)
    {
        fprintf(stdout, "%d ", node->info);
        preorder(node->left);
        preorder(node->right);
    }
}

void inorder(Tnode *node)
{
    if (node != NULL)
    {
        inorder(node->left);
        fprintf(stdout, "%d ", node->info);
        inorder(node->right);
    }
}

void postorder(Tnode *node)
{
    if (node != NULL)
    {
        inorder(node->left);
        inorder(node->right);     
        fprintf(stdout, "%d ", node->info);   
    }
}

//Problem 2
typedef struct _stack
{
    Tnode* node;
    struct _stack* next;
}Lnode;

Lnode* newNode(Tnode* tnode)
{
    Lnode *node = malloc(sizeof(*node));
    node->node = tnode;
    node->next = NULL;
    return(node);
}

int isEmpty(Lnode *node)
{
    return(!node);
}

void push(Lnode** root, Tnode* node)
{
    Lnode *lnode = newNode(node);
    lnode->next = *root;
    *root = lnode;
}

Tnode* pop(Lnode** head)
{
    Lnode* temp = *head;
    *head = (*head)->next;
    return(temp->node);
}

Tnode* top(Lnode* root)
{
    return(root->node);
}

void traversal(Tnode* node)
{
    Lnode *stack = NULL;
    //Tnode *head = node; //original head of node used for postorder

    do 
    {
        while(node != NULL)
        {
            push(&stack, node);
            //printf("%d ", node->info); //preorder
            //if (node->left == NULL) {printf("%d ", node->info);} //postorder
            node = node->left;
        }

        //printf("%d ", stack->node->info); //inorder
        while (stack && node == (top(stack))->right)
        {
            node = pop(&stack);
        }
        
        //if (stack && node) {printf("%d ", stack->node->info);} //inorder
        
        //if (!stack) {printf("%d ", node->info);} //last popped value (7) postorder

        if (stack)
        {
            node = (top(stack))->right;
            //if (stack->node != head && stack->node->left && stack->node->right) {printf("%d ", stack->node->info);} //postorder
        }

    } while(!isEmpty(stack));

    printf("\n");
}

int main()
{
    //Problem 1
    printf("Problem 1:\n\n");
    int array[] = {1,3,5,7,9,11,13,15};
    int array_size = sizeof(array) / sizeof(array[0]);
    Tnode *bst = BST_build(array, 0, array_size - 1);
    printf("preorder: ");
    preorder(bst);
    printf("\n");
    printf("inorder: ");
    inorder(bst);
    printf("\n");
    printf("postorder: ");
    postorder(bst);
    printf("\n\n");

    //Problem 2
    printf("Problem 2:\n\n");
    traversal(bst);
    return EXIT_SUCCESS;
}
