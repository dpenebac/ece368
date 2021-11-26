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

// Function to print shortest
// path from source to j
// using parent array
void printPath(int parent[], int j, int *length, int r, int c)
{
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

	//int row = j / c;

	//NEED TO CHANGE THIS TO SOMETHING ELSE
		int col = j;
		while (col > c)
		{
			col -= c;
		}
		if (col == c)
		{
			col = 0;
		}

	//printf("%d %d\n",row, col);

    printPath(parent, parent[j], length, r, c);
	*length += 1;
}

void writePath(int parent[], int j, int *length, int r, int c, FILE* output)
{
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;

	short row = j / c;

	//NEED TO CHANGE THIS TO SOMETHING ELSE
		short col = j;
		while (col > c)
		{
			col -= c;
		}
		if (col == c)
		{
			col = 0;
		}

	fwrite(&row, sizeof(row), 1, output);
	fwrite(&col, sizeof(row), 1, output);

    writePath(parent, parent[j], length, r, c, output);
	*length += 1;
}

void printSolution(int dist[], int n, int parent[])
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("\n%d\t", dist[i]);
        printPath(parent, i, 0, 0, 0);
    }
    return;
}

// The main function that calculates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Matrix* g, int src, int parent[], int dist[])
{
	
	// Get the number of vertices in graph
	int V = g->V;

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

	// Initialize min heap with all
	// vertices. dist value of all vertices
    int v;
	for (v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
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
	while (minHeap->size != 0)
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
		struct AdjListNode* pCrawl = g->array[u].head;
		while (pCrawl != NULL)
		{
			v = pCrawl->dest;

			// If shortest distance to v is
			// not finalized yet, and distance to v
			// through u is less than its
			// previously calculated distance
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
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
    //printGrid(r, c, grid);

    //Finding all paths
	//make graph from matrix
	int v = r * c; //vertices, +2 is for Start and End
	struct Matrix* g = createGraph(v + 1);

	for (i = 0; i < v; i++)
	{
		if (i % c != 0) //left
		{
			addEdge(g, i, i - 1, grid[i - 1]);
			//printf("Left of  %d: Adding: %d to %d with weight of %d\n", grid[i], i, i - 1, grid[i - 1]);
		}

		//so not 4, 9, 14, 19
		//0,1,2,3
		//5,6,7,8
		//10,11,12,13
		if ((i + 1) % c != 0) //right
		{
			addEdge(g, i, i + 1, grid[i + 1]);
			//printf("Right of %d: Adding: %d to %d with weight of %d\n", grid[i], i, i + 1, grid[i + 1]);
		}

		if (i - c >= 0) //up
		{
			addEdge(g, i, i - c, grid[i - c]);
			//printf("Up of    %d: Adding: %d to %d with weight of %d\n", grid[i], i, i - c, grid[i - c]);
		}

		if (i + r < r * c) //down
		{
			addEdge(g, i, i + c, grid[i + c]);
			//printf("Down of  %d: Adding: %d to %d with weight of %d\n", grid[i], i, i + c, grid[i + c]);
		}

		//printf("\n");
	}

	//E edges
	//printf("End Edge: ");
	for (i = v - c; i < v; i++)
	{
		addEdge(g, v, i, grid[i]);
		//printf("%d ", i);
	}
	//printf("\n\n");

	int *dist = (int*)malloc(v * sizeof(int));
	int *parent = (int*)malloc(v * sizeof(int));
	dijkstra(g, v, parent, dist); //v is E
	//printSolution(dist, c, parent); //can only enter from top
	//printf("\n");


	//fastest times from each entrance
	/*
	printf("%d\n", c);
	for (i = 0; i < c; i++)
    {
        printf("%d ", dist[i]);
    }
	printf("\n");
	*/

	//fastest path
	int minidx = 0;
	for (i = 1; i < c; i++) //finding min path index
	{
		if (dist[i] < dist[minidx])
		{
			minidx = i;
		}
	}

	//printf("\n%d\n%d\n", dist[minidx], 1); //replace 1 with len(path)
	int length;
	printPath(parent, minidx, &length, r, c); //NEED TO COUNT LENGTH BEFORE WRITING TO BINARY FILE DONT DELETE THIS
	length -= 1;
	//printf("REPLACE WITH 1 ABOVE: %d\n\n", length);

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


    fclose(inputGrid);
    fclose(inputGridTxt);


    return EXIT_SUCCESS;
}