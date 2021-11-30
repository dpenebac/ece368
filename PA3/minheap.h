#ifndef minheap
#define minheap
#include <stdlib.h>

// Structure to represent a min heap node
struct MinHeapNode
{
	int v;
	int dist;
};

// Structure to represent a min heap
struct MinHeap
{
	// Number of heap nodes present currently
	int size;	

	// This is needed for decreaseKey()
	struct MinHeapNode **array; //array of (vertex, distance) where distance is the vertex's distance from the source
};

// A utility function to create a
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
	struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->size = 0;
	minHeap->array = (struct MinHeapNode**)malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode *t = *a;
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

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
/*
void minHeapify(struct MinHeap* minHeap, int idx) //i think this is downward heapify
{
	int smallest, left, right;
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

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		struct MinHeapNode *smallestNode;
		struct MinHeapNode *idxNode;

		smallestNode = minHeap->array[smallest];
		idxNode = minHeap->array[idx];

		// Swap positions
		
		//minHeap->pos[smallestNode->v] = idx;
		//minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
	return;
}
*/

void minHeapifyFluffy(struct MinHeap* minHeap, int idx, int **pos, int length) //i think this is downward heapify
{
	int smallest, left, right;
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

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		struct MinHeapNode *smallestNode;
		struct MinHeapNode *idxNode;

		smallestNode = minHeap->array[smallest];
		idxNode = minHeap->array[idx];

		// Swap positions
		(*pos)[smallestNode->v] = idx;
		(*pos)[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapifyFluffy(minHeap, smallest, pos, length);
	}
	return;
}

struct MinHeapNode* extractMinFluffy(struct MinHeap* minHeap, int **pos, int length)
{	
	if (minHeap->size == 0)
	{
		return NULL;
	}

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	(*pos)[root->v] = minHeap->size-1;
	(*pos)[lastNode->v] = 0;

	// Reduce heap size and heapify root
	minHeap->size -= 1;

	minHeapifyFluffy(minHeap, 0, pos, length); 

	return root;
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (minHeap->size == 0)
	{
		return NULL;
	}

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	//minHeap->pos[root->v] = minHeap->size-1;
	//minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;

	return root;
}

void update(struct MinHeap* m, int dest, int dist, int pos, int **posArr)
{
	int i = pos;

	m->array[i]->dist = dist;

	while (i > 0)
	{
		int Parent = (i - 1) / 2;
		int child = i;

		if (m->array[child]->dist < m->array[Parent]->dist) //heapify and update position array accordingly
		{
			//swap child vertex and parent vertex in position array
			//maybe make swap function for this as well
			int childIdx = m->array[child]->v;
			int parentIdx = m->array[Parent]->v;
			//swap(minHeap->pos, childIdx, parentIdx);
			(*posArr)[childIdx] = Parent;
			(*posArr)[parentIdx] = child;
			
			//minHeap->pos[childIdx] = Parent;
			//minHeap->pos[parentIdx] = child;
			

			//swap parent and child in heap
			swapMinHeapNode(&m->array[child], &m->array[Parent]);

			// move to parent index
		}
		i = (i - 1) / 2;
	}
	return;
}

bool inQueue(struct MinHeap *m, int idx)
{
	return(idx < m->size);
}

// Function to decrease dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
/*
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete
	// tree is not hepified.
	// This is a O(Logn) loop
	while (i > 0)
	{
		if (minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist) //heapify and update position array accordingly
		{
			// Swap this node with its parent
			int parent = (i - 1) / 2;
			int child = i;

			//swap child vertex and parent vertex in position array
			//minHeap->pos[minHeap->array[child]->v] = parent;
			//minHeap->pos[minHeap->array[parent]->v] = child;

			//swap parent and child in heap
			swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

			// move to parent index
		}
		i = (i - 1) / 2;
	}
	return;
}


// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
	{
		return true;
	}

	return false;
}
*/
#endif
