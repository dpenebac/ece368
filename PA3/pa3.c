#include <stdio.h>
#include <stdlib.h>

void printGrid(short, short, short*);
void writeGrid(short, short, short*, FILE*);

void printGrid(short r, short c, short *grid)
{
    int i;
    for (i = 0; i < r * c; i++)
    {
        if ((i + i) % c == 0)
        {
            printf("\n");
        }
        printf("%d ", grid[i]);
    }
    printf("\n");  
}

//write grid in text formatting to file
void writeGrid(short r, short c, short *grid, FILE* output)
{
    int i;
    for (i = 0; i < r * c; i++)
    {
        if ((i + i) % c == 0 && i != 0)
        {
            fprintf(output, "\n");
        }
        fprintf(output, "%d ", grid[i]);
    }
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
    printf("%d %d", r, c);
    
    short *grid = (short *)malloc((r * c) * sizeof(short));

    int i;
    short temp;
    for (i = 0; i < r * c; i++)
    {
        fread(&temp, sizeof(temp), 1, inputGrid);
        grid[i] = temp;
    }

    FILE* inputGridTxt = fopen(argv[2], "w");
    //need to split up into multiple arrays to write line by line???
    printGrid(r, c, grid);

    //write grid in text formatting to file
    writeGrid(r, c, grid, inputGridTxt);





    //FILE* fastestTimes = fopen(argv[1], "wb");
    //FILE* fastestPath = fopen(argv[1], "wb");


    fclose(inputGrid);
    fclose(inputGridTxt);


    return EXIT_SUCCESS;
}