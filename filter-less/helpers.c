#include "helpers.h"
#include <math.h>

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int serpiaRed = round((.393 * image[i][j].rgbtRed) + (.769 * image[i][j].rgbtGreen) +
                                  (.189 * image[i][j].rgbtBlue));
            int serpiaGreen = round((.349 * image[i][j].rgbtRed) + (.686 * image[i][j].rgbtGreen) +
                                    (.168 * image[i][j].rgbtBlue));
            int serpiaBlue = round((.272 * image[i][j].rgbtRed) + (.534 * image[i][j].rgbtGreen) +
                                   (.131 * image[i][j].rgbtBlue));
            //for red
            if (serpiaRed > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else
            {
                image[i][j].rgbtRed = serpiaRed;
            }
            //for green
            if (serpiaGreen > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = serpiaGreen;
            }
            //for blue
            if (serpiaBlue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else
            {
                image[i][j].rgbtBlue = serpiaBlue;
            }
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