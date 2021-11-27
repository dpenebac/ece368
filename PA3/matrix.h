#ifndef matrix
#define matrix
#include "adjList.h"

// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Matrix
{
	int V;
	struct AdjList* list;
};

// A utility function that creates
// a graph of V vertices
struct Matrix* createGraph(int V)
{
	struct Matrix* m = malloc(sizeof(struct Matrix));
	m->V = V;

	// Create an array of adjacency lists.
	// Size of array will be V
	m->list = malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list
	// as empty by making head as NULL
    int i;
	for (i = 0; i < V; ++i)
	{
		m->list[i].head = NULL;
	}

	return m;
}

// Adds an edge to an undirected graph
void addEdge(struct Matrix* m, int src, int dest, int weight)
{
	// Add an edge from src to dest.
	// A new node is added to the adjacency
	// list of src. The node is
	// added at the beginning
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = m->list[src].head;
	m->list[src].head = newNode;
}

#endif