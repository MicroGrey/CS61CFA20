/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) // read the data from the ppm file
{
	FILE *file = fopen(filename, "r");
	if (file == NULL)
	{
		fprintf(stderr, "Error: File not found\n");
		exit(-1);
	}
	Image *image;
	int rows, cols;
	Color **imageData;
	fscanf(file, "P3\n%d %d\n255\n", &cols, &rows);
	image = malloc(sizeof(Image));
	imageData = malloc(rows * sizeof(Color *));
	for (int i = 0; i < rows; i++)
	{
		imageData[i] = malloc(cols * sizeof(Color));
		for (int j = 0; j < cols; j++)
		{
			fscanf(file, "%hhu %hhu %hhu", &imageData[i][j].R, &imageData[i][j].G, &imageData[i][j].B);
		}
	}
	image->image = imageData;
	image->rows = rows;
	image->cols = cols;
	fclose(file);
	return image;
	//YOUR CODE HERE
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	printf("P3\n%d %d\n255\n", image->cols, image->rows);
	for (int i = 0; i<image->rows; i++)
	{
		for (int j = 0; j<image->cols; j++)
		{
			printf("%d %d %d ", image->image[i][j].R, image->image[i][j].G, image->image[i][j].B);
		}
		printf("\n");
	}
	//YOUR CODE HERE
}

//Frees an image
void freeImage(Image *image)
{
	for (int i = 0; i < image->rows; i++)
	{
		free(image->image[i]);
	}
	free(image->image);
	free(image);
	//YOUR CODE HERE
}