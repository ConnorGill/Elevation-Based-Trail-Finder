/*
 * Connor Gill - CS100 - Project 6
 * This program will read a set
 * of elevation data and identify / create 
 * an image of a path across the grid
 * of data. This path can go any cardinal
 * direction, depedning on user input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct pgm {
	int rows;
	int cols;
	int **pixels;
} pgmPic;


int main(int argc, char *argv[])
{
	//~~~~~~~~~~~~~~~~~~~~Error Messages~~~~~~~~~~~~~~~~~~~~~~//
	if ( argc != 3 )			//If wrong number of arguments
        { 
		printf("ERROR: Incorrect input\n"); 
		return 0;
	}
	
	FILE *fp1 = fopen(argv[1], "r");
	if ( ! fp1 )				//If file does not exist
        { 
		printf("ERROR: File %s does not exist\n", argv[1]);
		return 0;
	}
	
	if (strcmp(argv[2], "N-S") != 0 && strcmp(argv[2], "S-N") != 0 
	&& strcmp(argv[2], "W-E") != 0 && strcmp(argv[2], "E-W") != 0)
	{
		printf("ERROR: Directions are not correct.\n");
		return 0;
	}
	//~~~~~~~~~~~~~~~~~~~~Main Program Begins~~~~~~~~~~~~~~~~~~~~~~// 
	int k = 0; 
	int j = 0;

	pgmPic *myPic = (pgmPic*)malloc(sizeof(pgmPic));

	fscanf(fp1, "%d", &myPic->rows);		//Assigning rows/cols values
	fscanf(fp1, "%d", &myPic->cols);
	
	myPic->pixels = (int **) malloc ( sizeof(int *) * myPic->rows );

	for (j=0; j < myPic->rows; j++)		//Allocating space for 2d Arr
	{
		myPic->pixels[j] = (int *) malloc ( sizeof(int) * myPic->cols );
	}

	//~~~~~~~~~~~~~~~~~~~~Array Asignments~~~~~~~~~~~~~~~~~~~~~~//
	for (j=0; j < myPic->rows; j++)	//Moves data into 2d array
	{
		for (k=0; k < myPic->cols; k++)		
		{
			fscanf(fp1, "%d", &myPic->pixels[j][k]);
		}
	}
	fclose(fp1);

	//~~~~~~~~~~~~~~~~~~~~Value Conversions~~~~~~~~~~~~~~~~~~~~~~//
	int max = myPic->pixels[0][0]; 
	int min = myPic->pixels[0][0];
	int range = 0;
	double mult = 200.0;

	for (j=0; j < myPic->rows; j++)	//Finds Max / Min
	{
		for (k=0; k < myPic->cols; k++)		
		{
			if (myPic->pixels[j][k] > max)
			{
				max = myPic->pixels[j][k];
			}
			else if (myPic->pixels[j][k] < min)
			{
				min = myPic->pixels[j][k];
			}	
		}
	}

	range = max - min;
	mult = mult / range;

	for (j=0; j < myPic->rows; j++)	//Converts Values
	{
		for (k=0; k < myPic->cols; k++)		
		{
			myPic->pixels[j][k] = round((myPic->pixels[j][k] - min) *  mult);
			myPic->pixels[j][k] = 255 - myPic->pixels[j][k];
		}
	}
	//~~~~~~~~~~~~~~~~~~~~PGM File Created~~~~~~~~~~~~~~~~~~~~~~//
	
	char file[] = " ";
	strcpy(file, argv[1]);		//Adds pgm extension for new file
	strcat(file, ".pgm");

	FILE *fp2 = fopen(file, "w");
	fprintf(fp2, "P2\n");
	fprintf(fp2, "%d %d\n", myPic->cols, myPic->rows);
	fprintf(fp2, "%d\n", 255);
	

	for (j=0; j < myPic->rows; j++)	//TESTING - DELETE
	{
		for (k=0; k < myPic->cols; k++)		
		{
			fprintf(fp2, "%d ", myPic->pixels[j][k]);
		}
		fprintf(fp2, "\n");
	}
	fclose(fp2);
	//~~~~~~~~~~~~~~~~~~~~Trail PGM File Created~~~~~~~~~~~~~~~~~~~~~~//
	char start = argv[2][0];
	char fileTrail[] = " ";
	strcpy(fileTrail, argv[1]);		//Adds pgm extension for new file
	strcat(fileTrail, "-trail.pgm");

	FILE *fp3 = fopen(fileTrail, "w");
	fprintf(fp3, "P2\n");
	fprintf(fp3, "%d %d\n", myPic->cols, myPic->rows);
	fprintf(fp3, "%d\n", 255);

	//~~~~~~~~~~~~~~~~~~~~Trail Created~~~~~~~~~~~~~~~~~~~~~~//
	
	int curRow = 0;
	int curCol = 0;
	
	if (start == 'N')	//IF NORTH TO SOUTH
	{
		curCol = myPic->cols / 2;		//North Starting Point
		myPic->pixels[curRow][curCol] = 0;

		while (curRow < myPic->rows - 1)	//While not end of grid
		{
			if (curCol == 0)		//If on far left side
			{
				if (myPic->pixels[curRow + 1][curCol + 1] > myPic->pixels[curRow + 1][curCol] )
				{
					curCol++;
				}
			}
			if (curCol == myPic->cols - 1)		//If on far right side
			{
				if (myPic->pixels[curRow + 1][curCol - 1] > myPic->pixels[curRow + 1][curCol] )
				{
					curCol--;
				}		
			}
			else					//If not touching edge
			{
				if (myPic->pixels[curRow + 1][curCol - 1] > myPic->pixels[curRow + 1][curCol] && myPic->pixels[curRow + 1][curCol - 1] >= myPic->pixels[curRow + 1][curCol + 1])	//If left is greatest
				{
					curCol--;
				}

				else if (myPic->pixels[curRow + 1][curCol + 1] > myPic->pixels[curRow + 1][curCol] && myPic->pixels[curRow + 1][curCol + 1] > myPic->pixels[curRow + 1][curCol - 1])	//If right is greatest
				{
					curCol++;
				}
				
			}

			curRow++;			//ensure it always moves forward
			myPic->pixels[curRow][curCol] = 0;
		}
	}

	else if (start == 'S')		//IF SOUTH TO NORTH
	{
		curCol = myPic->cols / 2;		//South Starting Point
		curRow = myPic->rows - 1;
		myPic->pixels[myPic->rows - 1][curCol] = 0;

		while (curRow > 0)	//While not end of grid
		{
			if (curCol == 0)		//If on far left side
			{
				if (myPic->pixels[curRow - 1][curCol + 1] > myPic->pixels[curRow - 1][curCol] )
				{
					curCol++;
				}
			}
			if (curCol == myPic->cols - 1)		//If on far right side
			{
				if (myPic->pixels[curRow - 1][curCol - 1] > myPic->pixels[curRow - 1][curCol] )
				{
					curCol--;
				}		
			}
			else					//If not touching edge
			{
				if (myPic->pixels[curRow - 1][curCol + 1] > myPic->pixels[curRow - 1][curCol] && myPic->pixels[curRow - 1][curCol + 1] >= myPic->pixels[curRow - 1][curCol - 1])	//If right is greatest
				{
					curCol++;
				}

				else if (myPic->pixels[curRow - 1][curCol - 1] > myPic->pixels[curRow - 1][curCol] && myPic->pixels[curRow - 1][curCol - 1] > myPic->pixels[curRow - 1][curCol + 1])	//If left is greatest
				{
					curCol--;
				}
				
			}

			curRow--;			//ensure it always moves forward
			myPic->pixels[curRow][curCol] = 0;
		}
	}

	else if (start == 'W')		//IF WEST TO EAST
	{
		curRow = myPic->rows / 2;		//West Starting Point
		myPic->pixels[curRow][curCol] = 0;

		while (curCol < myPic->cols - 1)	//While not end of grid
		{
			if (curRow == 0)		//If on top side
			{
				if (myPic->pixels[curRow + 1][curCol + 1] > myPic->pixels[curRow][curCol + 1] )
				{
					curRow++;
				}
			}
			if (curRow == myPic->rows - 1)		//If on bottom side
			{
				if (myPic->pixels[curRow - 1][curCol + 1] > myPic->pixels[curRow][curCol + 1] )
				{
					curRow--;
				}		
			}
			else					//If not touching edge
			{
				if (myPic->pixels[curRow + 1][curCol + 1] > myPic->pixels[curRow][curCol + 1] && myPic->pixels[curRow + 1][curCol + 1] >= myPic->pixels[curRow - 1][curCol + 1])	//If bottom is greatest
				{
					curRow++;
				}

				else if (myPic->pixels[curRow - 1][curCol + 1] > myPic->pixels[curRow][curCol + 1] && myPic->pixels[curRow - 1][curCol + 1] > myPic->pixels[curRow + 1][curCol + 1])	//If top is greatest
				{
					
					curRow--;
				}
				
			}

			curCol++;			//ensure it always moves forward
			myPic->pixels[curRow][curCol] = 0;
		}
	}

	else if (start == 'E')		//IF EAST TO WEST
	{
		curRow = myPic->rows / 2;		//East Starting Point
		curCol = myPic->cols - 1;
		myPic->pixels[curRow][myPic->cols - 1] = 0;

		while (curCol > 0)	//While not end of grid
		{
			if (curRow == 0)		//If on top side
			{
				if (myPic->pixels[curRow + 1][curCol - 1] > myPic->pixels[curRow][curCol - 1] )
				{
					curRow++;
				}
			}
			if (curRow == myPic->rows - 1)		//If on bottom side
			{
				if (myPic->pixels[curRow - 1][curCol - 1] > myPic->pixels[curRow][curCol - 1] )
				{
					curRow--;
				}		
			}
			else					//If not touching edge
			{
				if (myPic->pixels[curRow - 1][curCol - 1] > myPic->pixels[curRow][curCol - 1] && myPic->pixels[curRow - 1][curCol - 1] >= myPic->pixels[curRow + 1][curCol - 1])	//If top is greatest
				{
					curRow--;
				}

				else if (myPic->pixels[curRow + 1][curCol - 1] > myPic->pixels[curRow][curCol - 1] && myPic->pixels[curRow + 1][curCol - 1] > myPic->pixels[curRow - 1][curCol - 1])	//If bottom is greatest
				{
					curRow++;
				}
				
			}

			curCol--;			//ensure it always moves forward
			myPic->pixels[curRow][curCol] = 0;
		}
	}

	//~~~~~~~~~~~~~~~~~~~~Trail Printed~~~~~~~~~~~~~~~~~~~~~~//
	for (j=0; j < myPic->rows; j++)	//TESTING - DELETE
	{
		for (k=0; k < myPic->cols; k++)		
		{
			fprintf(fp3, "%d ", myPic->pixels[j][k]);
		}
		fprintf(fp3, "\n");
	}
	
	fclose(fp3);


	return 0;
}

