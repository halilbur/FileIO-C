/**
 * @file main.c
 * @description Includes the functions body of matrixLib.h
 * @assignment BP3 - PROJECT 2
 * @date 27/12/2022
 * @author HALIL BURAK OZTURK halilburak.ozturk@stu.fsm.edu.tr
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrixLib.h"

typedef enum
{
    Iris_setosa = 1,
    Iris_versicolor,
    Iris_virginica
} Species;

typedef struct
{
    float sepal_length;
    float sepal_width;
    float petal_length;
    float petal_width;
    Species species;
} IrisDataType;

void dataToMatrix(IrisDataType *iris, float **matrix);

int main()
{

    // Open the file for reading
    FILE *fp = fopen("Iris.csv", "r");

    // Check if file exists
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }

    // Read the file one row at a time
    char row[256];

    // Getting the number of rows
    int rowCount = atoi(fgets(row, sizeof(row), fp));

    // Getting the column names
    fgets(row, sizeof(row), fp);

    // Creating the array of IrisDataType struct to store the data
    IrisDataType *iris = (IrisDataType *)malloc(rowCount * sizeof(IrisDataType));

    // Reading the data from the file and storing it in the array
    for (int i = 0; i < rowCount; i++)
    {
        // Reading the row
        char row[256];
        fgets(row, sizeof(row), fp);

        // Getting the first signal
        strtok(row, ",");

        // Getting the rest of the tokens
        char *signal;
        // Loop over the columns of the file
        for (int j = 1; j <= 5; j++)
        {
            // Getting the signal
            signal = strtok(NULL, ",");

            // If the signal is the first column, store it in the sepal_length field
            if (j == 1)
                iris[i].sepal_length = atof(signal);
            // If the signal is the second column, store it in the sepal_width field
            else if (j == 2)
                iris[i].sepal_width = atof(signal);
            // If the signal is the third column, store it in the petal_length field
            else if (j == 3)
                iris[i].petal_length = atof(signal);
            // If the signal is the fourth column, store it in the petal_width field
            else if (j == 4)
                iris[i].petal_width = atof(signal);
            // If the signal is the fifth column, store it in the species field
            else if (j == 5)
            {
                // If the signal is Iris-setosa, store 1 in the array
                if (strcmp(signal, "Iris-setosa") == 0)
                {
                    // store 1 in the array
                    iris[i].species = Iris_setosa;
                    continue;
                }
                // If the signal is Iris-versicolor, store 2 in the array
                else if (strcmp(signal, "Iris-versicolor") == 0)
                {
                    // store 2 in the array
                    iris[i].species = Iris_versicolor;
                    continue;
                }
                // If the signal is Iris-virginica, store 3 in the array
                else if (strcmp(signal, "Iris-virginica") == 0)
                {
                    // store 3 in the array
                    iris[i].species = Iris_virginica;
                    continue;
                }
            }
        }
    }
    
    // Create the matrix dynamically
    float **matrix = (float **)malloc(rowCount * sizeof(float *));
    
    // Sending file data to matrix
    dataToMatrix(iris, matrix);

    // Creating vectors for each column of the matrix
    float *sepalLength = returnSpecificColumn(matrix, rowCount, 4, 0);
    float *sepalWidht = returnSpecificColumn(matrix, rowCount, 4, 1);
    float *petalLength = returnSpecificColumn(matrix, rowCount, 4, 2);
    float *petalWidht = returnSpecificColumn(matrix, rowCount, 4, 3);

    // Calculating the covariance of each column
    float varianceSL = covariance(sepalLength, sepalLength, rowCount, rowCount);
    float varianceSW = covariance(sepalWidht, sepalWidht, rowCount, rowCount);
    float variancePL = covariance(petalLength, petalLength, rowCount, rowCount);
    float variancePW = covariance(petalWidht, petalWidht, rowCount, rowCount);

    // Creating the file for writing
    FILE *fp2 = fopen("IrisStatistic.txt", "w");
    
    // Writing the Variance data to the file
    fprintf(fp2, "***************Variance***************\n");
    fprintf(fp2, "SepalLength Variance: %f \nSepalWidht Variance: %f \nPetalLength Variance: %f \nPetalWidht Variance: %f \n", varianceSL, varianceSW, variancePL, variancePW);

    // Writing the Column means data to the file
    fprintf(fp2, "***************ColumnMeans***********************\n");
    fprintf(fp2, "Column Means: ");
    float *colMeans = columnMeans(matrix, rowCount, 4);
    for (int i = 0; i < 4; i++)
    {
        fprintf(fp2, "%f ", colMeans[i]);
    }
    fprintf(fp2, "\n");
    
    // Writing the correlation data to the file
    float correlationSL_SW = correlation(sepalLength, sepalWidht, rowCount, rowCount);
    float correlationSL_PL = correlation(sepalLength, petalLength, rowCount, rowCount);
    float correlationSL_PW = correlation(sepalLength, petalWidht, rowCount, rowCount);
    float correlationSW_PL = correlation(sepalWidht, petalLength, rowCount, rowCount);
    float correlationSW_PW = correlation(sepalWidht, petalWidht, rowCount, rowCount);
    float correlationPL_PW = correlation(petalLength, petalWidht, rowCount, rowCount);
    fprintf(fp2, "***************Correlation************\n");
    fprintf(fp2, "SepalLength-SepalWidht: %f \nSepalLength-PetalLength: %f \nSepalLength-PetalWidht: %f \nSepalWidht-PetalLength: %f \nSepalWidht-PetalWidht: %f \nPetalLength-PetalWidht: %f \n", correlationSL_SW, correlationSL_PL, correlationSL_PW, correlationSW_PL, correlationSW_PW, correlationPL_PW);

    // Writing the covariance matrix data to the file
    fprintf(fp2, "***************CovarianceMatrix************\n");
    float **covMat = covarianceMatrix(matrix, rowCount, 4);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            // Print the matrix of each element
            fprintf(fp2, "%f ", covMat[i][j]);
        }
        fprintf(fp2, "\n");
    }

    // Freeing the memory
    freeMatrix(covMat, 4);
    free(colMeans);
    freeMatrix(matrix, rowCount);
    free(iris);

    // Closing the files
    fclose(fp2);
    fclose(fp);

    return 0;
}
// Function to creating matrix with file data
void dataToMatrix(IrisDataType *iris, float **matrix)
{
    // Loop each row
    for (int i = 0; i < 150; i++)
    {
        // Allocate memory for each column
        matrix[i] = (float *)malloc(5 * sizeof(float));
        // Assign the data to the matrix
        matrix[i][0] = iris[i].sepal_length;
        matrix[i][1] = iris[i].sepal_width;
        matrix[i][2] = iris[i].petal_length;
        matrix[i][3] = iris[i].petal_width;
        matrix[i][4] = iris[i].species;
    }
}