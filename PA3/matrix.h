#ifndef matrix
#define matrix
#include "adjList.h"
#include <stdlib.h>

struct Matrix
{
	int numVertexes;
	struct AdjList* list; //matrix contains lists of adjacency lists which contain adjacency listnodes
};

struct Matrix* createGraph(int numVertexes)
{
	struct Matrix* m = malloc(sizeof(struct Matrix));
	m->numVertexes = numVertexes;
	m->list = malloc(numVertexes * sizeof(struct AdjList));

    int i;
	for (i = 0; i < numVertexes; ++i) //initializing adjlist nodes
	{
		m->list[i].head = NULL;
	}

	return m;
}

void addEdge(struct Matrix* m, int src, int dest, int weight)
{
	struct AdjListNode* newNode = newListNode(dest, weight);
	newNode->next = m->list[src].head;
	m->list[src].head = newNode;
}

#endif