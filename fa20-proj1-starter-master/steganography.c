/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	Color *this_color = (Color *)malloc(sizeof(Color));
	uint8_t blue_value = image->image[row][col].B;
	if (blue_value & 1) {
        this_color->R = 255, this_color->G = 255, this_color->B = 255;
    } else {
        this_color->R = 0, this_color->G = 0, this_color->B = 0;
    }
	return this_color;
	//YOUR CODE HERE
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
	Image *newImage = malloc(sizeof(Image));
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	newImage->image = malloc(image->rows * sizeof(Color *));
	for (int i = 0; i < image->rows; i++)
	{
		newImage->image[i] = malloc(image->cols * sizeof(Color));
		for (int j = 0; j < image->cols; j++)
		{
			Color *newColor = evaluateOnePixel(image, i, j);
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	return newImage;
	//YOUR CODE HERE
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	for (int i = 0; i < argc; i++)
	{
		Image *image = readData(argv[i]);
		Image *newImage = steganography(image);
		writeData(newImage);
		freeImage(image);
	}
	//YOUR CODE HERE
}
