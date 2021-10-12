#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_array.h"
#include "shell_list.h"
#include "list_of_list.h"

int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        return EXIT_FAILURE;
    }

    //Shell Array stuff
    if (strcmp(argv[1], "-a") == 0)
    {
        int size = 0; //value to store size of binary file
        long *arr; //pointer to array created in Array_Load_from_File
        long n_comp = 0; //amount of comparisons for the array swap stuff
        int count;

        arr = Array_Load_From_File(argv[2], &size);

        if (arr == NULL)
        {
            return(EXIT_FAILURE);
        }
        
        Array_Shellsort(arr, size, &n_comp);
        count = Array_Save_To_File(argv[3], arr, size);
        printf("%ld\n", n_comp);

        if (count != size)
        {
            fprintf(stderr, "Found %d nodes but expected %d nodes\n", size, count);
        }
        free(arr);

        return EXIT_SUCCESS;
    }

    //Shell List stuff
    else if (strcmp(argv[1], "-l") == 0)
    {
        Node *head;
        Node* temp;
        long n_comp = 0;
        int count;
        int ncount = 0;
        head = List_Load_From_File(argv[2]);
        head = List_Shellsort(head, &n_comp);
        count = List_Save_To_File(argv[3], head);

        while(head != NULL)
        {
            temp = head->next;
            free(head);
            head = temp;
            ncount++;
        }

        if (ncount != count)
        {
            fprintf(stderr, "Found %d nodes but expected %d nodes\n", ncount, count);
        }

        printf("%ld\n", n_comp);

        return EXIT_SUCCESS;
    }

    return(EXIT_FAILURE);
}