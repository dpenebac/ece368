// Number of vertices
// in the graph
// C / C++ program for Dijkstra's
// shortest path algorithm for adjacency
// list representation of graph
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "matrix.h"
#include "adjList.h"
#include "minheap.h"

// A utility function used to print the solution
void printArr(int dist[], int n)
{
	printf("Vertex Distance from Source\n");
    int i;
	for (i = 0; i < n; ++i)
		printf("%d \t\t %d\n", i, dist[i]);
}

void writePath(int parent[], int j, int *length, int r, int c, FILE* output)
{
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

	short row = j / c;

	//NEED TO CHANGE THIS TO SOMETHING ELSE
	//j is the (x,y) position, need to translate to r = x, c = y
		short col = j;
		while (col > c)
		{
			col -= c;
		}
		if (col == c)
		{
			col = 0;
		}

	//printf("%d %d\n", row, col);
	fwrite(&row, sizeof(short), 1, output);
	fwrite(&col, sizeof(short), 1, output);

    writePath(parent, parent[j], length, r, c, output);
	*length += 1;
}

// The main function that calculates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
/*
void dijkstra(struct Matrix* m , int src, int parent[], int dist[])
{
	int V = m->V;

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

    int v;
	for (v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, INT_MAX);
		minHeap->pos[v] = v;
		parent[v] = -1;
	}

	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	minHeap->size = V;
	
	struct AdjListNode* edge = NULL;

	struct MinHeapNode* min = NULL;
	while (minHeap->size != 0)
	{
		min = extractMin(minHeap); //extract root which should be min distance from srcs
								   //extract min is based on distance
		minHeapify(minHeap, 0); //heapify
	
		int vertex = min->v; //corresponding vertex based on min position

		edge = m->list[vertex].head; //edge is used to travel across adjlist to determine new min distances
		while (edge != NULL) //crawl until end of adjlist
		{
			int dest = edge->dest; //destination of edge
			int newWeight = edge->weight + dist[vertex];

			if (newWeight < dist[dest]) //if the new calculated distance is less than the current distance
			{
				parent[dest] = vertex; //updating path for shortest parent

				dist[dest] = newWeight; //update new shortest path

				decreaseKey(minHeap, dest, dist[dest]); //update distance in minheap and heapify
			}
			edge = edge->next; //travel to next edge
		}
		free(min); //extracted root is now free
	}

    free(minHeap->pos);
    free(minHeap->array);
    free(minHeap);	

	return;
}

void dijkstraOriginal(struct Matrix* m , int src, int parent[], int dist[])
{
	int V = m->V;

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

    int v;
	for (v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, INT_MAX);
		minHeap->pos[v] = v;
		parent[v] = -1;
	}

	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	minHeap->size = V;
	
	struct AdjListNode* edge = NULL;

	struct MinHeapNode* min = NULL;
	while (minHeap->size != 0)
	{
		min = extractMin(minHeap); //extract root which should be min distance from srcs
		//extract min is based on distance
		minHeapify(minHeap,0);
	
		int vertex = min->v; //corresponding vertex based on min position

		edge = m->list[vertex].head; //pointer crawl is used to travel across adjlist to determine new min distances
		while (edge != NULL) //crawl until end of adjlist
		{
			int dest = edge->dest; //destination of edge
			int weight = edge->weight; //weight of edge

			if (  (minHeap->pos[dest] < minHeap->size) //not sure why this is necessary
			   && (dist[vertex] != INT_MAX) //distance of source -> minVertex is not inf
			   && (weight + dist[vertex] < dist[dest]) //if the new calculated weight is less than the current weight
			   )
			{
				parent[dest] = vertex;

				dist[dest] = dist[vertex] + weight;

				decreaseKey(minHeap, dest, dist[dest]);
			}
			edge = edge->next; //travel to next edge
		}
		free(min);
	}

    free(minHeap->pos);
    free(minHeap->array);
    free(minHeap);	

	return;
}
*/

void dijkstraFluff(struct Matrix* m , int src, int parent[], int dist[])
{
	int V = m->V;

	int *pos = (int *)malloc(V * sizeof(int));

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

    int v;
	for (v = 0; v < V; ++v)
	{
		if (v != src)
		{
			dist[v] = INT_MAX;
		}
		minHeap->array[v] = newMinHeapNode(v, INT_MAX);
		pos[v] = v;
		minHeap->size += 1;
		parent[v] = -1;
	}

	dist[src] = 0;

/*
Instead of filling the priority queue with all nodes 
in the initialization phase, it is also possible to 
initialize it to contain only source; then, inside the 
if alt < dist[v] block, the decrease_priority() 
becomes an add_with_priority() operation if the node is 
not already in the queue. wikipedia
*/

int i;
//     decreaseKey(minHeap, src, dist[src]);
//void decreaseKey(struct MinHeap* minHeap, int v, int dist)
// Get the index of v in heap array

i = pos[src];

// Get the node and update its dist value
minHeap->array[i]->dist = dist[src];

//CHANGE THIS ONLY NEEDS TO SWAP SOURCE FROM BOTTOM TO TOP
//OF MIN HEAP

pos[minHeap->array[i]->v] = 0;
pos[minHeap->array[0]->v] = i;

swapMinHeapNode(&minHeap->array[i], &minHeap->array[0]);

	
	struct AdjListNode* edge = NULL;
	struct MinHeapNode* min = NULL;
	
	while (!(isEmpty(minHeap)))
	{	
		min = extractMinFluffy(minHeap, &pos, V); //extract min and heapify
	
		int vertexIdx = min->v; //corresponding vertex index based on min position

		edge = m->list[vertexIdx].head; //edge is used to travel across adjlist to determine new min distances
								  //vertex in adjList 
								  /*example
									vertex = 5
									m->list[5].head
									5->3->2->1
									means 5 is connected to 3,2,1
								  */
		while (edge != NULL) //crawl until end of adjlist
		{
			int dest = edge->dest; //destination of edge
			int newWeight = edge->weight + dist[vertexIdx];
			bool inQueue = pos[dest] < minHeap->size;
			if (inQueue && newWeight < dist[dest]) //if the new calculated distance is less than the current distance
			{
				parent[dest] = vertexIdx; //updating path for shortest parent

				dist[dest] = newWeight; //update new shortest path

				//decreaseKey(minHeap, dest, dist[dest]); //update distance in minheap and heapify
				//void decreaseKey(struct MinHeap* minHeap, int v, int dist)

				//UPDATE (PQ)
				//update(minHeap, dest, dist[dest], pos[dest], &pos);
				// Get the index of v in heap array
				//i = pos[dest];
				//i = minHeap->pos[dest];
				// Get the node and update its dist value
				//minHeap->array[i]->dist = dist[dest];
				// Travel up while the complete
				// tree is not hepified.
				// This is a O(Logn) loop
				//keep min heap property/

				update(minHeap, dest, dist[dest], pos[dest], &pos);
				/*i = pos[dest];
				minHeap->array[i]->dist = dist[dest];
				while (i > 0) //change to for loop lmao
				{
					Parent = (i - 1) / 2;
					child = i; //separate this into two ints this looks dumb
					if (minHeap->array[child]->dist < minHeap->array[Parent]->dist) //heapify and update position array accordingly
					{
						//swap child vertex and parent vertex in position array
						//maybe make swap function for this as well
						childIdx = minHeap->array[child]->v;
						parentIdx = minHeap->array[Parent]->v;
						//swap(minHeap->pos, childIdx, parentIdx);
						pos[childIdx] = Parent;
						pos[parentIdx] = child;
						
						//minHeap->pos[childIdx] = Parent;
						//minHeap->pos[parentIdx] = child;
						

						//swap parent and child in heap
						swapMinHeapNode(&minHeap->array[child], &minHeap->array[Parent]);

						// move to parent index
					}
					i = (i - 1) / 2;
				}*/


			}
			edge = edge->next; //travel to next edge
		}
		free(min); //extracted root is now free
	}

    //free(minHeap->pos);
    free(minHeap->array);
    free(minHeap);	
	free(pos);

	return;
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
    short r = 0, c = 0; //rows, columns
    fread(&r, sizeof(r), 1, inputGrid);
    fread(&c, sizeof(c), 1, inputGrid);
	
    short *grid = (short *)malloc((r * c) * sizeof(short *)); //r * c * sizeof(short) = 40

    int i = 0;
    short temp = 0;
    for (i = 0; i < r * c; i++) 
    {
        fread(&temp, sizeof(short), 1, inputGrid);
        grid[i] = temp;
    }

    //write grid in text formatting to file
    FILE* inputGridTxt = fopen(argv[2], "w");
    writeGrid(r, c, grid, inputGridTxt);
    //printGrid(r, c, grid);

    //Finding all paths
	//make graph from matrix
	int v;
	v = r * c; //vertices, +2 is for Start and End
	struct Matrix* m = createGraph(v + 1);

	for (i = 0; i < v; i++)
	{
		if (i % c != 0) //left
		{
			addEdge(m, i, i - 1, grid[i - 1]);
			//printf("Left of  %d: Adding: %d to %d with weight of %d\n", grid[i], i, i - 1, grid[i - 1]);
		}

		//so not 4, 9, 14, 19
		//0,1,2,3
		//5,6,7,8
		//10,11,12,13
		if ((i + 1) % c != 0) //right
		{
			addEdge(m, i, i + 1, grid[i + 1]);
			//printf("Right of %d: Adding: %d to %d with weight of %d\n", grid[i], i, i + 1, grid[i + 1]);
		}

		if (i - c >= 0) //up
		{
			addEdge(m, i, i - c, grid[i - c]);
			//printf("Up of    %d: Adding: %d to %d with weight of %d\n", grid[i], i, i - c, grid[i - c]);
		}

		if (i + r < r * c) //down
		{
			addEdge(m, i, i + c, grid[i + c]);
			//printf("Down of  %d: Adding: %d to %d with weight of %d\n", grid[i], i, i + c, grid[i + c]);
		}

		//printf("\n");
	}

	//E edges
	//printf("End Edge: ");
	for (i = v - c; i < v; i++)
	{
		addEdge(m, v, i, grid[i]);
		//printf("%d ", i);
	}
	//printf("\n\n");

	int *dist = (int*)malloc(v * sizeof(int*));
	int *parent = (int*)malloc(v * sizeof(int*));
	
	dijkstraFluff(m, v, parent, dist); //v is the last node which combines all the destinations

	//fastest path
	int minidx = 0;
	for (i = 1; i < c; i++) //finding min path index
	{
		if (dist[i] < dist[minidx])
		{
			minidx = i;
		}
	}

	int length = 0;
	int j = minidx;
	//printPath(parent, minidx, &length, r, c); //NEED TO COUNT LENGTH BEFORE WRITING TO BINARY FILE DONT DELETE THIS

//NOT ME BUT STILL FLUFFY I THINK
			while (parent[j] != -1)
			{
				length += 1;
				j = parent[j];
				//printf("%d\n", parent[j]);
			}
//

	//writing fastest times
    FILE* fastestTimes = fopen(argv[3], "wb");
	fwrite(&c, sizeof(c), 1, fastestTimes);
	for (i = 0; i < c; i++)
	{
		fwrite(&dist[i], sizeof(dist[i]), 1, fastestTimes);
	}
	
	//writing fastest path
    FILE* fastestPath = fopen(argv[4], "wb");
	fwrite(&dist[minidx], sizeof(dist[minidx]), 1, fastestPath);
	fwrite(&length, sizeof(length), 1, fastestPath);
	writePath(parent, minidx, &length, r, c, fastestPath);

	//free matrix
	int d;
	for(d=0; d<m->V; d++)
    {
        struct AdjListNode *p1 = m->list[d].head, *p2;
        while(p1)
        {
              p2=p1;
              p1=p1->next;
              free(p2);
        }
    }

	free(m->list);
	free(m);

	free(grid);
	free(dist);
	free(parent);

    fclose(inputGrid);
    fclose(inputGridTxt);
	fclose(fastestTimes);
	fclose(fastestPath);

    return EXIT_SUCCESS;
}