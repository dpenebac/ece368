// Number of vertices
// in the graph
// C / C++ program for Dijkstra's
// shortest path algorithm for adjacency
// list representation of graph
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

// A structure to represent a
// node in adjacency list
struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;
};

// A structure to represent
// an adjacency list
struct AdjList
{
	
// Pointer to head node of list
struct AdjListNode *head;
};

// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Graph
{
	int V;
	struct AdjList* array;
};

// A utility function to create
// a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
	struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates
// a graph of V vertices
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*)
			malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists.
	// Size of array will be V
	graph->array = (struct AdjList*)
	malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list
	// as empty by making head as NULL
    int i;
	for (i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
	// Add an edge from src to dest.
	// A new node is added to the adjacency
	// list of src. The node is
	// added at the beginning
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;
}

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

	// Capacity of min heap
	int capacity;

	// This is needed for decreaseKey()
	int *pos;
	struct MinHeapNode **array;
};

// A utility function to create a
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int dist)
{
	struct MinHeapNode* minHeapNode =
		(struct MinHeapNode*)
	malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =
		(struct MinHeap*)
	malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(
			capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**)
				malloc(capacity *
	sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->dist <
		minHeap->array[smallest]->dist )
	smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->dist <
		minHeap->array[smallest]->dist )
	smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		struct MinHeapNode *smallestNode;
		smallestNode = minHeap->array[smallest];
		struct MinHeapNode *idxNode;
		idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest],
						&minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if
// the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract
// minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root =
				minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode =
		minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete
	// tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->dist <
		minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] =
									(i-1)/2;
		minHeap->pos[minHeap->array[
							(i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i],
				&minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
if (minHeap->pos[v] < minHeap->size)
	return true;
return false;
}

// A utility function used to print the solution
void printArr(int dist[], int n)
{
	printf("Vertex Distance from Source\n");
    int i;
	for (i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);
}

// Function to print shortest
// path from source to j
// using parent array
void printPath(int parent[], int j)
{
       
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;
   
    printPath(parent, parent[j]);
   
    printf("%d ", j);
}

void printSolution(int dist[], int n, int parent[])
{
    int src = 13;
    printf("Vertex\t 		Distance\tPath");
    int i;
    for (i = 0; i < n; i++)
    {
        printf("\n%d -> %d \t\t %d\t\t%d ",
                      src, i, dist[i], src);
        printPath(parent, i);
    }
    return;
}

// The main function that calculates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src)
{
	
	// Get the number of vertices in graph
	int V = graph->V;

	// dist values used to pick
	// minimum weight edge in cut
	//int dist[V];	
    int *dist = (int*)malloc(V * sizeof(int));

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

	//array to hold path
	//int parent[V];
    int *parent = (int*)malloc(V * sizeof(int));

	// Initialize min heap with all
	// vertices. dist value of all vertices
    int v;
	for (v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

    for (v = 0; v < V; v++)
    {
        //parent
		parent[v] = -1;
    }

	// Make dist value of src vertex
	// as 0 so that it is extracted first
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the followin loop,
	// min heap contains all nodes
	// whose shortest distance
	// is not yet finalized.
	while (!isEmpty(minHeap))
	{
		// Extract the vertex with
		// minimum distance value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
	
		// Store the extracted vertex number
		int u = minHeapNode->v;

		// Traverse through all adjacent
		// vertices of u (the extracted
		// vertex) and update
		// their distance values
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			v = pCrawl->dest;

			// If shortest distance to v is
			// not finalized yet, and distance to v
			// through u is less than its
			// previously calculated distance
			if (isInMinHeap(minHeap, v) &&
					dist[u] != INT_MAX &&
			pCrawl->weight + dist[u] < dist[v])
			{
				//parent
				parent[v] = u;


				dist[v] = dist[u] + pCrawl->weight;

				// update distance
				// value in min heap also
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	// print the calculated shortest distances
	printArr(dist, V);
	printf("\n\n\n");
	printSolution(dist, V, parent);
	printf("\n\n\n");
}

// Driver program to test above functions
int test()
{
	// create the graph given in above fugure
	/*
	int V = 6; 
	struct Graph* graph = createGraph(V);

	addEdge(graph, 1, 0, 0); //1->0 weight 0
	addEdge(graph, 2, 0, 0); //2->0 weight 0

	addEdge(graph, 1, 2, 2); //1->2 weight 2
	addEdge(graph, 1, 3, 3); //1->3 weight 3

	addEdge(graph, 2, 1, 1); //2->1 weight 1
	addEdge(graph, 2, 4, 4); //2->4 weight 4

	addEdge(graph, 3, 1, 1); //3->1 weight 1
	addEdge(graph, 3, 4, 4); //3->4 weight 4

	addEdge(graph, 4, 2, 2); //4->2 weight 2
	addEdge(graph, 4, 3, 3); //4->3 weight 3

	//END POINTS COMBINED == 5
	addEdge(graph, 5, 3, 0); //5->3 weight 0
	addEdge(graph, 5, 4, 0); //5->4 weight 0
	*/

	int V = 14; 
	struct Graph* graph = createGraph(V);

	//addEdge(gra, S, E, W);

	//S
	addEdge(graph, 1, 0, 1);
	addEdge(graph, 2, 0, 1);
	addEdge(graph, 3, 0, 1);
	addEdge(graph, 4, 0, 1);

	//1
	addEdge(graph, 1, 2, 1);
	addEdge(graph, 1, 5, 1);
	/*							  S
	1	1	1	1			1	2	3	4
	1	1	10 	1	 ==		5	6	7	8
	10	10	10	1			9	10	11	12
								  E
	*/		

	//2
	addEdge(graph, 2, 1, 1);
	addEdge(graph, 2, 3, 1);
	addEdge(graph, 2, 6, 1);

	//3
	addEdge(graph, 3, 2, 1);
	addEdge(graph, 3, 4, 1);
	addEdge(graph, 3, 7, 10);

	//4
	addEdge(graph, 4, 3, 1);
	addEdge(graph, 4, 8, 1);

	//5
	addEdge(graph, 5, 1, 1);
	addEdge(graph, 5, 6, 1);
	addEdge(graph, 5, 9, 10);

	//6
	addEdge(graph, 6, 2, 1);
	addEdge(graph, 6, 5, 1);
	addEdge(graph, 6, 7, 10);
	addEdge(graph, 6, 10, 10);

	//7
	addEdge(graph, 7, 3, 1);
	addEdge(graph, 7, 6, 1);
	addEdge(graph, 7, 8, 1);
	addEdge(graph, 7, 11, 10);

	//8
	addEdge(graph, 8, 4, 1);
	addEdge(graph, 8, 7, 10);
	addEdge(graph, 8, 12, 1);

	//9
	addEdge(graph, 9, 5, 1);
	addEdge(graph, 9, 10, 10);
	
	//10
	addEdge(graph, 10, 6, 1);
	addEdge(graph, 10, 9, 10);
	addEdge(graph, 10, 11, 10);

	//11
	addEdge(graph, 11, 7, 10);
	addEdge(graph, 11, 10, 10);
	addEdge(graph, 11, 12, 1);

	//12
	addEdge(graph, 12, 8, 1);
	addEdge(graph, 12, 11, 10);

	//E
	addEdge(graph, 13, 9, 1);
	addEdge(graph, 13, 10, 1);
	addEdge(graph, 13, 11, 1);
	addEdge(graph, 13, 12, 1);

	//add all vertex and edges seperately

	dijkstra(graph, 13);

	return 0;
}

void printGrid(short r, short c, short *grid)
{
    int i;
    int counter = 0; //used to count when to change columns
    printf("%d %d\n", r, c);

    for (i = 0; i < r * c; i++)
    {
        if (counter == c)
        {
            printf("\n");
            counter = 0;
        }
        printf("%d ", grid[i]);
        counter += 1;
    }
    printf("\n");  
}

//write grid in text formatting to file
void writeGrid(short r, short c, short *grid, FILE* output)
{
    int i;
    int counter = 0;

    fprintf(output, "%d %d\n", r, c);

    for (i = 0; i < r * c; i++)
    {
        if (counter == c)
        {
            fprintf(output, "\n");
            counter = 0;
        }
        if (counter % c != 0)
        {
            fprintf(output, " ");
        }
        fprintf(output, "%d", grid[i]);
        counter += 1;
    }

    fprintf(output, "\n");
}

int main(int argc, char* argv[])
{
    FILE* inputGrid = fopen(argv[1], "rb");

    if (inputGrid == NULL)
    {
        return EXIT_FAILURE;
    }
    
    //find size
    short r,c; //rows, columns
    fread(&r, sizeof(r), 1, inputGrid);
    fread(&c, sizeof(c), 1, inputGrid);
    
    short *grid = (short *)malloc((r * c) * sizeof(short));

    int i;
    short temp;
    for (i = 0; i < r * c; i++)
    {
        fread(&temp, sizeof(temp), 1, inputGrid);
        grid[i] = temp;
    }

    //write grid in text formatting to file
    FILE* inputGridTxt = fopen(argv[2], "w");
    writeGrid(r, c, grid, inputGridTxt);
    printGrid(r, c, grid);


    //Finding all paths

	/*
	4 2 1 0 5 
	2 0 4 1 0 
	4 2 2 2 7 
	1 7 2 9 2 
	*/

	/*
			S
	1	2	3	4	5
	6	7	8	9	10
	11	12	13	14	15
	16	17	18	19	20
			E
	*/
	//make graph from matrix
	int vertices = r * c + 1; // +1 is going to go to the edge
	struct Graph* graph = createGraph(vertices);
	

	/*
    int V = 14; 
	struct Graph* graph = createGraph(V);
	dijkstra(graph, 13);
	*/



    //FILE* fastestTimes = fopen(argv[1], "wb");
    //FILE* fastestPath = fopen(argv[1], "wb");


    fclose(inputGrid);
    fclose(inputGridTxt);
    test();


    return EXIT_SUCCESS;
}

/*
    int graph[V][V] = { {0, 1, 1, 0, 0, 0},
                        {1, 0, 2, 3, 0, 0},
                        {1, 1, 0, 0, 4, 0},
                        {0, 1, 0, 0, 4, 1},
                        {0, 0, 2, 3, 0, 1},
                        {0, 0, 0, 1, 1, 0}};
*/