#include <stdlib.h>
#include <limits.h>

struct Path
{
	int vertex; //target vertex
	int dist; //distance
};

struct MinHeap
{
	int size;
	struct Path **array; //array of path(vertex, distance) where distance is the vertex's distance from the source
};

struct Path* newPath(int vertex, int dist)
{
	struct Path* minHeapNode = (struct Path*)malloc(sizeof(struct Path));
	minHeapNode->vertex = vertex;
	minHeapNode->dist = dist;
	return(minHeapNode);
}

struct MinHeap* createHeap(int size) //create heap with all paths as INTMAX
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
	int smallest, leftChild, rightChild;
	struct Path *smallestNode, *idxNode;
	smallest = idx;
	leftChild = 2 * idx + 1;
	rightChild = 2 * idx + 2;

	int size = minHeap->size;
	int smallestDist = minHeap->array[smallest]->dist;

	if (leftChild < size && minHeap->array[leftChild]->dist < smallestDist)
	{
		if (minHeap->array[leftChild]->dist < smallestDist)
		{
			smallest = leftChild;
		}
		else
		{
			smallest = idx;
		}
	}

	int newIdx = smallest;
	smallestDist = minHeap->array[smallest]->dist;
	if (rightChild < minHeap->size)
	{
		if (minHeap->array[rightChild]->dist < smallestDist)
		{
			smallest = rightChild;
		}
		else
		{
			smallest = newIdx;
		}
	}

	//nodes to swap
	smallestNode = minHeap->array[smallest];
	idxNode = minHeap->array[idx];

	if (smallest != idx)
	{
		//Swap nodes in positions array
		(*pos)[smallestNode->vertex] = idx;
		(*pos)[idxNode->vertex] = smallest;

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
	int lastPos = minHeap->size - 1;
	struct Path* lastNode = minHeap->array[lastPos];
	minHeap->array[0] = lastNode;

	// Update position of last node
	(*pos)[root->vertex] = lastPos;
	(*pos)[lastNode->vertex] = 0;

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
			childIdx = m->array[child]->vertex;
			parentIdx = m->array[Parent]->vertex;
			
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

bool inQueue(struct MinHeap *m, int i)
{
	if (i < m->size)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}
