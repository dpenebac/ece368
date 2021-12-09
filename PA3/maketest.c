#include <stdio.h>
#include <stdlib.h>

int main()
{

    short row, col;
    scanf("%hd %hd", &row, &col);

    printf("\nRow: %d Col: %d\n", row, col);

    FILE* f = fopen("examples/test.b", "wb");
    fwrite(&row, sizeof(row), 1, f);
    fwrite(&col, sizeof(col), 1, f);

    int i;
    for (i = 0; i < row * col; i++)
    {
        short l = i;
        fwrite(&l, sizeof(l), 1, f);
    }

    

    return EXIT_SUCCESS;
}