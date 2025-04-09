#include "helpers.h"
#include <math.h>

typedef struct
{
    int rgbtRed;
    int rgbtGreen;
    int rgbtBlue;
} temptrip;

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int grayval = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = grayval;
            image[i][j].rgbtGreen = grayval;
            image[i][j].rgbtBlue = grayval;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //place in temporary values
            int tempr = image[i][j].rgbtRed;
            int tempg = image[i][j].rgbtGreen;
            int tempb = image[i][j].rgbtBlue;

            //swap values
            image[i][j].rgbtRed = image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][width - 1 - j].rgbtBlue;

            //swap values
            image[i][width - 1 - j].rgbtRed = tempr;
            image[i][width - 1 - j].rgbtGreen = tempg;
            image[i][width - 1 - j].rgbtBlue = tempb;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //copy image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    //blur

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            float count = 0.0;
            float redtot = 0.0;
            float greentot = 0.0;
            float bluetot = 0.0;
            for (int x = -1; x < 2; x++)
            {
                //checking height level
                if (((i + x) >= 0) && ((i + x) <= (height - 1)))
                {
                    for (int z = -1; z < 2; z++)
                    {
                        //checking width level
                        if (((j + z) >= 0) && ((j + z) <= (width - 1)))
                        {
                            //get totals
                            redtot += copy[i + x][j + z].rgbtRed;
                            greentot += copy[i + x][j + z].rgbtGreen;
                            bluetot += copy[i + x][j + z].rgbtBlue;
                            count++;
                        }
                    }
                }
            }
            image[i][j].rgbtRed = round(redtot / count);
            image[i][j].rgbtGreen = round(greentot / count);
            image[i][j].rgbtBlue = round(bluetot / count);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //tables for gx and gy
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //images
    temptrip gximage[height][width];
    temptrip gyimage[height][width];

    //getting gx values

    //current row
    for (int i = 0; i < height; i++)
    {
        //current column
        for (int j = 0; j < width; j++)
        {
            //reset values
            double redtot = 0;
            double greentot = 0;
            double bluetot = 0;
            //row adjustor
            for (int x = -1; x < 2; x++)
            {
                if (((i + x) >= 0) && ((i + x) < height))
                {
                    //column adjustor
                    for (int z = -1; z < 2; z++)
                    {
                        if ((j + z) >= 0 && (j + z) < width)
                        {
                            redtot += (image[i + x][j + z].rgbtRed) * (gx[x + 1][z + 1]);
                            greentot += (image[i + x][j + z].rgbtGreen) * (gx[x + 1][z + 1]);
                            bluetot += (image[i + x][j + z].rgbtBlue) * (gx[x + 1][z + 1]);
                        }
                    }
                }
            }
            gximage[i][j].rgbtRed = redtot;
            gximage[i][j].rgbtGreen = greentot;
            gximage[i][j].rgbtBlue = bluetot;
        }
    }

    //getting gy values

    //current row
    for (int i = 0; i < height; i++)
    {
        //current column
        for (int j = 0; j < width; j++)
        {
            //reset values
            double redtot = 0;
            double greentot = 0;
            double bluetot = 0;

            //adjustor for rows
            for (int x = -1; x < 2; x++)
            {
                if (((i + x) >= 0) && ((i + x) < height))
                    //adjustor for columns
                    for (int z = -1; z < 2; z ++)
                    {
                        if (((j + z) >= 0) && ((j + z) < width))
                        {
                            redtot += (image[i + x][j + z].rgbtRed) * (gy[x + 1][z + 1]);
                            greentot += (image[i + x][j + z].rgbtGreen) * (gy[x + 1][z + 1]);
                            bluetot += (image[i + x][j + z].rgbtBlue) * (gy[x + 1][z + 1]);
                        }
                    }
            }
            gyimage[i][j].rgbtRed = redtot;
            gyimage[i][j].rgbtGreen = greentot;
            gyimage[i][j].rgbtBlue = bluetot;
        }
    }

    //combining values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int combinedred;
            int combinedgreen;
            int combinedblue;

            combinedred = round(sqrt(pow(gximage[i][j].rgbtRed, 2) + pow(gyimage[i][j].rgbtRed, 2)));
            combinedgreen = round(sqrt(pow(gximage[i][j].rgbtGreen, 2) + pow(gyimage[i][j].rgbtGreen, 2)));
            combinedblue = round(sqrt(pow(gximage[i][j].rgbtBlue, 2) + pow(gyimage[i][j].rgbtBlue, 2)));

            if (combinedred > 255)
            {
                combinedred = 255;
            }

            if (combinedgreen > 255)
            {
                combinedgreen = 255;
            }

            if (combinedblue > 255)
            {
                combinedblue = 255;
            }


            image[i][j].rgbtRed = combinedred;
            image[i][j].rgbtGreen = combinedgreen;
            image[i][j].rgbtBlue = combinedblue;
        }
    }
}
