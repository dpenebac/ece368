#ifndef adjList
#define adjList
#include <stdlib.h>

struct AdjList
{
	struct AdjListNode *head;
};

struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;
};

struct AdjListNode* newListNode(int dest, int weight)
{
	struct AdjListNode* newNode = malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return(newNode);
}

#endif