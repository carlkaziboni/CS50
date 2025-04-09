#include <cs50.h>
#include <stdio.h>

int main(void)
{
//declare height variable
    int height;
//get value for height between 1-8
    do
    {
        height = get_int("Height: ");
    }
    while (height <= 0 || height > 8);
//print # in shape of pyramid
    for (int i = 0; i < height; i++)
    {
//printing spaces
        for (int k = height - 1 - i; k > 0; k = k - 1)
        {
            printf(" ");
        }
//printing #
        for (int j = 0; j <= i; j++)
        {
            printf("#");
        }
        printf("  ");
        for (int z = 0; z <= i; z++)
        {
            printf("#");
        }
        printf("\n");
    }
}