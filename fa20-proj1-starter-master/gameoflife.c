/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	Color *new_color = (Color *)malloc(sizeof(Color));
	new_color->R = 0;
	new_color->G = 0;
	new_color->B = 0;

	Color *current = &image->image[row][col];
	
	int left = (col - 1 + image->cols) % image->cols;
	int right = (col + 1) % image->cols;
	int up = (row - 1 + image->rows) % image->rows;
	int down = (row + 1) % image->rows;

	for (int i = 0; i < 8; i++)
	{
		int red = (int)((image->image[up][left].R >> i) & 1) +
				 	   ((image->image[up][col].R >> i) & 1) + 
					   ((image->image[up][right].R >> i) & 1) + 
					   ((image->image[row][left].R >> i) & 1) + 
					   ((image->image[row][right].R >> i) & 1) +
					   ((image->image[down][left].R >> i) & 1) + 
					   ((image->image[down][col].R >> i) & 1) + 
					   ((image->image[down][right].R >> i) & 1);

		int green = (int)((image->image[up][left].G >> i) & 1) + 
						 ((image->image[up][col].G >> i) & 1) + 
						 ((image->image[up][right].G >> i) & 1) + 
						 ((image->image[row][left].G >> i) & 1) + 
						 ((image->image[row][right].G >> i) & 1) + 
						 ((image->image[down][left].G >> i) & 1) + 
						 ((image->image[down][col].G >> i) & 1) + 
						 ((image->image[down][right].G >> i) & 1);

		int blue = (int)((image->image[up][left].B >> i) & 1) + 
						((image->image[up][col].B >> i) & 1) + 
						((image->image[up][right].B >> i) & 1) + 
						((image->image[row][left].B >> i) & 1) + 
						((image->image[row][right].B >> i) & 1) + 
						((image->image[down][left].B >> i) & 1) + 
						((image->image[down][col].B >> i) & 1) + 
						((image->image[down][right].B >> i) & 1);

		if (current->R >>i & 1)
		{
			if ((rule >> (red + 9)) & 1)
			{
				new_color->R |= (1 << i);
			}
		}
		else
		{
			if ((rule >> (red)) & 1)
			{
				new_color->R |= (1 << i);
			}
		}

		if (current->G >>i & 1)
		{
			if ((rule >> (green + 9)) & 1)
			{
				new_color->G |= (1 << i);
			}
		}
		else
		{
			if ((rule >> (green)) & 1)
			{
				new_color->G |= (1 << i);
			}
		}

		if (current->B >>i & 1)
		{
			if ((rule >> (blue + 9)) & 1)
			{
				new_color->B |= (1 << i);
			}
		}
		else
		{
			if ((rule >> (blue)) & 1)
			{
				new_color->B |= (1 << i);
			}
		}

	}
	
	return new_color;
	//YOUR CODE HERE
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	Image *newImage = malloc(sizeof(Image));
	newImage->image = (Color **)malloc(image->rows * sizeof(Color *));
	for (int i = 0; i < image->rows; i++)
	{
		newImage->image[i] = malloc(image->cols * sizeof(Color));
		for (int j = 0; j < image->cols; j++)
		{
			Color *newColor = evaluateOneCell(image, i, j, rule);
			newImage->image[i][j] = *newColor;
			free(newColor);
		}
	}
	newImage->rows = image->rows;
	newImage->cols = image->cols;
	return newImage;
	//YOUR CODE HERE
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3) {
        printf("usage: ./gameOfLife filename rule\n\
    			filename is an ASCII PPM file (type P3) with maximum value 255.\n\
    			rule is a hex number beginning with 0x; Life is 0x1808.");
        exit(-1);
    }

    // Read the original image data
    Image *original_image = readData(argv[1]);

	// turn string into int
	char *endptr; // for strtol
	uint32_t rule = (uint32_t) strtol(argv[2], &endptr, 0); // A base of 0 means that the base will be determined by the format of the string (e.g., "0x" for hex, "0" for octal).
    if (*endptr != '\0') {
        exit(-1); // the rule is illegal
    }
	
	if (original_image == NULL) {
        fprintf(stderr, "Failed to read data from %s\n", argv[1]);
        exit(-1);
    }

    // Perform steganography on the image
    Image *new_image = life(original_image, rule);
    if (new_image == NULL) {
        fprintf(stderr, "Steganography processing failed\n");
        freeImage(original_image); // Ensure the original image is freed
        exit(-1);
    }
	writeData(new_image);
    // Free the image memory
    freeImage(original_image);
    freeImage(new_image);
	return 0;
}
