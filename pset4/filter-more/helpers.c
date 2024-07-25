#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            RGBTRIPLE current = image[h][w];
            int val = round((current.rgbtBlue + current.rgbtRed + current.rgbtGreen) / 3.0);
            image[h][w].rgbtBlue = val;
            image[h][w].rgbtRed = val;
            image[h][w].rgbtGreen = val;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Creates an empty "image"
    RGBTRIPLE i[height][width];

    // For every row in the image, the loop starts from the furthest right pixel
    // in the image and assigns it to the furthest left value in i, and keeps
    // doing that for the appropriate places. Second furthest on right would be
    // assigned to second pixel in row, etc.
    for (int h = 0; h < height; h++)
    {
        for (int w = width - 1; w >= 0; w--)
        {
            i[h][width - w - 1] = image[h][w];
        }
    }

    // Assigns every value of i to image
    for (int b = 0; b < height; b++)
    {

        for (int j = 0; j < width; j++)
        {

            image[b][j] = i[b][j];
            printf("[%i][%i]", b, j);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Creates a temporary "image" to store blurred values without changing original
    RGBTRIPLE temp[height][width];

    // uter 2 loops go through every pixel individually
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int red = 0;
            int blue = 0;
            int green = 0;
            double count = 0;

            // Inner 2 loops go through the 3x3 grid around each pixel and add up RGB values
            for (int i = h - 1; i <= h + 1; i++)
            {
                for (int j = w - 1; j <= w + 1; j++)
                {
                    if (i >= 0 && i < height && j >= 0 && j < width)
                    {
                        red += image[i][j].rgbtRed;
                        blue += image[i][j].rgbtBlue;
                        green += image[i][j].rgbtGreen;
                        count += 1;
                    }
                }
            }

            // RGB values are averaged and assigned to the corresponding pixel in the temp image
            temp[h][w].rgbtRed = round(red / count);
            temp[h][w].rgbtBlue = round(blue / count);
            temp[h][w].rgbtGreen = round(green / count);
        }
    }

    // Temp image is assigned to the actual image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int rgx = 0, bgx = 0, ggx = 0, rgy = 0, bgy = 0, ggy = 0;

            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {

                    if (k >= 0 && k < height && l >= 0 && l < width)
                    {
                        rgx += (image[k][l].rgbtRed * gx[k - i + 1][l - j + 1]);
                        bgx += (image[k][l].rgbtBlue * gx[k - i + 1][l - j + 1]);
                        ggx += (image[k][l].rgbtGreen * gx[k - i + 1][l - j + 1]);
                        rgy += (image[k][l].rgbtRed * gy[k - i + 1][l - j + 1]);
                        bgy += (image[k][l].rgbtBlue * gy[k - i + 1][l - j + 1]);
                        ggy += (image[k][l].rgbtGreen * gy[k - i + 1][l - j + 1]);
                    }
                }
            }

            int rval = round(sqrt(pow(rgx, 2) + pow(rgy, 2)));
            int bval = round(sqrt(pow(bgx, 2) + pow(bgy, 2)));
            int gval = round(sqrt(pow(ggx, 2) + pow(ggy, 2)));

            if (rval > 255)
            {
                rval = 255;
            }
            if (bval > 255)
            {
                bval = 255;
            }
            if (gval > 255)
            {
                gval = 255;
            }

            temp[i][j].rgbtRed = rval;
            temp[i][j].rgbtBlue = bval;
            temp[i][j].rgbtGreen = gval;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
        }
    }
    return;
}
