#include <stdio.h>
#include <stdlib.h>
#include "shell_array.h"
#include "list_of_list.h"
/*
long *Array_Load_From_File(char *filename, int *size);

int Array_Save_To_File(char *filename, long *array, int size);

void Array_Shellsort(long *array, int size, long *n_comp);
*/

long *Array_Load_From_File(char *filename, int *size)
{

    FILE* input = fopen(filename, "rb");

    //Determine size of binary file
    if (input != NULL)
    {
        fseek(input, 0L, SEEK_END);
        *size = ftell(input) / 8; //for long int nums = 15 size = 15 (int) (divided by 8 because long is 8 (120 / 8))
        rewind(input);
    }
    else
    {
        return(NULL);
    }

    //Fread binary file into array
    int c;
    long buffer;
    long *arr = malloc(sizeof(long) * *size);
    if (arr == NULL)
    {
        return(arr);
    }

    //Read each long value 1 by 1 from binary and put them into the array
    for(c = 0; c < *size; c++)
    {
        fread(&buffer, sizeof(buffer), 1, input);
        arr[c] = buffer;
    }

    fclose(input);
    
    return(arr);
}

void Array_Shellsort(long *array, int size, long *n_comp)
{
    long k = 1; //ksort
    while (k < size) //finding largest k
    {
        k = k * 3 + 1;
    }
    k = (k - 1) / 3;

    long i, j, temp;
    while (k >= 1) //last k value should be 1
    {
        for (j = k; j <= size - 1; j++)
        {
            temp = array[j];
            i = j;
            while((i >= k) && (array[i - k] > temp))
            {
                array[i] = array[i - k];
                i -= k;
                *n_comp += 1;
            }
            array[i] = temp;
        }
        k = (k - 1) / 3; //decreasing k by same formula
    }
    return;
}

int Array_Save_To_File(char *filename, long *array, int size) //Write array to file
{
    FILE* output = fopen(filename, "wb");

    if(output == NULL)
    {
        return(-1);
    }

    fwrite(array, sizeof(array), size, output);

    fclose(output);

    return(size);
}