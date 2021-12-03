#include <stdlib.h>
#include <limits.h>

struct Path
{
	int v;
	int dist;
};

struct MinHeap
{
	int size;
	struct Path **array; //array of path(vertex, distance) where distance is the vertex's distance from the source
};

struct Path* newPath(int v, int dist)
{
	struct Path* minHeapNode = (struct Path*)malloc(sizeof(struct Path));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return(minHeapNode);
}

struct MinHeap* createHeap(int size)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->array = (struct Path**)malloc(size * sizeof(struct Path*));
	minHeap->size = 0;

	int i;
	for (i = 0; i < size; i++)
	{
		minHeap->array[i] = newPath(i, INT_MAX);
		minHeap->size += 1;
	}

	return(minHeap);
}

void swapPath(struct Path** a, struct Path** b)
{
	struct Path *t = *a;
	*a = *b;
	*b = t;
	return;
}

bool isEmpty(struct MinHeap* m)
{
	if(m->size != 0)
	{
		return(false);
	}
	else
	{
		return(true);
	}
}

void heapifyDown(struct MinHeap* minHeap, int idx, int **pos, int length) //downward heapify
{
	int smallest, left, right;
	struct Path *smallestNode, *idxNode;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
	{
		smallest = left;
	}

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
	{
		smallest = right;
	}

	smallestNode = minHeap->array[smallest];
	idxNode = minHeap->array[idx];

	if (smallest != idx)
	{
		//Swap nodes in positions
		(*pos)[smallestNode->v] = idx;
		(*pos)[idxNode->v] = smallest;

		//Swap
		swapPath(&minHeap->array[smallest], &minHeap->array[idx]);

		//maintain heap
		heapifyDown(minHeap, smallest, pos, length);
	}
	return;
}

struct Path* extractMin(struct MinHeap* minHeap, int **pos, int length)
{	
	if (minHeap->size == 0)
	{
		return NULL;
	}

	// Store the root node
	struct Path* root = minHeap->array[0];

	// Replace root node with last node
	struct Path* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	(*pos)[root->v] = minHeap->size-1;
	(*pos)[lastNode->v] = 0;

	// Reduce heap size and heapify root
	minHeap->size -= 1;

	heapifyDown(minHeap, 0, pos, length); 

	return root;
}

void update(struct MinHeap* m, int dest, int dist, int pos, int **posArr)
{
	int Parent, child, i, childIdx, parentIdx;
	i = pos;

	m->array[i]->dist = dist;

	while (i > 0)
	{
		Parent = (i - 1) / 2;
		child = i;

		if (m->array[child]->dist < m->array[Parent]->dist) //heapify and update position array accordingly
		{
			//swap child vertex and parent vertex in position array
			//maybe make swap function for this as well
			childIdx = m->array[child]->v;
			parentIdx = m->array[Parent]->v;
			
			(*posArr)[childIdx] = Parent;
			(*posArr)[parentIdx] = child;

			//swap parent and child in heap
			swapPath(&m->array[child], &m->array[Parent]);

		}
		//move to parent index
		i = (i - 1) / 2;
	}
	return;
}

bool inQueue(struct MinHeap *m, int idx)
{
	return(idx < m->size);
}
