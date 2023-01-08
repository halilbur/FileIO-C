/**
 * @file matrixLib.c
 * @description Includes the functions body of matrixLib.h
 * @assignment BP3 - PROJECT 2
 * @date 27/12/2022
 * @author HALIL BURAK OZTURK halilburak.ozturk@stu.fsm.edu.tr
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matrixLib.h"

typedef enum b
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

// Function for printing vector
void printVector(float *vec, int size)
{
    // Loop over the elements of the vector
    for (int i = 0; i < size; i++)
    {
        // Print the vector of each element
        printf("vec[%d] = %f \n", i, vec[i]);
    }
}
// Function for return specific column
float *returnSpecificColumn(float **mat, int row, int col, int specificCol)
{
    // Allocate memory for the vector
    float *specificColumn = returnVector(row);
    // Loop over the elements of the column
    for (int i = 0; i < row; i++)
    {
        // Store the elements of the column in the vector
        specificColumn[i] = mat[i][specificCol];
    }
    return specificColumn;
}

//@desc This function printing the matrix
void printMatrix(float **mat, int rows, int cols)
{
    // Loop over the elements of the matrix
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            // Print the matrix of each element
            printf("mat[%d][%d] = %f \t", i, j, mat[i][j]);
        }
        printf("\n");
    }
}
// Function for calculating the mean of the columns
float *columnMeans(float **mat, int row, int col)
{
    // Allocate memory for the vector
    float *colMeans = returnVector(col);

    // Loop over the columns of the matrix
    for (int i = 0; i < col; i++)
    {
        // Create a vector to store the elements of the column
        float *col = returnVector(row);
        // Loop over the elements of the column
        for (int j = 0; j < row; j++)
        {
            // Store the elements of the column in the vector
            col[j] = mat[j][i];
        }
        // Calculate the mean of the column
        colMeans[i] = mean(col, row);
        // Free the memory of the vector
        free(col);
    }

    // Return the vector
    return colMeans;
}
// Function for calculating the mean of the rows
float mean(float *vector, int size)
{
    // Create a variable to store the sum of the elements of the vector
    float sum = 0;
    // Loop over the elements of the vector and add them to the sum
    for (int i = 0; i < size; i++)
    {
        sum += vector[i];
    }

    // Return the mean
    return sum / size;
}
// Function for returning a vector
float *returnVector(int size)
{
    // Allocate memory for the vector
    float *vector = (float *)malloc(size * sizeof(float));
    // Return vector
    return vector;
}
// Function for calculating covariance
float covariance(float *vec1, float *vec2, int size1, int size2)
{
    // Check if the vectors are not empty and have the same size
    if (size1 == size2 && size1 != 0 && size2 != 0 && vec1 != NULL && vec2 != NULL)
    {
        // Calculate the mean of each vector
        float meanVec1 = mean(vec1, size1);
        float meanVec2 = mean(vec2, size1);

        // Define a variable of type float to store the sum
        float sum = 0;

        // Calculate the covariance
        for (int i = 0; i < size1; i++)
        {
            // Calculate the sum of the multiplication of the difference of each element of the vectors and the mean of each vector
            sum += ((vec1[i] - meanVec1) * (vec2[i] - meanVec2));
        }
        // Return the covariance
        return sum / (size1 - 1);
    }
    else
        // Print an error message if the vectors are empty or have different size
        printf("The vectors are empty or have different size");

    exit(1);
    return 1;
}
// Function for calulating the correlation
float correlation(float *vec, float *vec2, int size1, int size2)
{

    //  Check if the vectors are not empty and have the same size
    if (size1 != 0 && size2 != 0 && vec != NULL && vec2 != NULL && size1 == size2)
    {
        // Calculate the covariance
        float cov = covariance(vec, vec2, size1, size2);

        // Calculate the standard deviation of each vector
        float stdVec1 = sqrt(covariance(vec, vec, size1, size2));
        float stdVec2 = sqrt(covariance(vec2, vec2, size1, size2));

        // Return the correlation
        return cov / (stdVec1 * stdVec2);
    }
    else
        printf("The vectors are empty or have different size! \n");
    exit(1);
    return 1;
}
// Function for calulating the covariance matrix
float **covarianceMatrix(float **mat, int row, int col)
{
    // Check if the matrix is not empty and has the correct size
    if (mat != NULL && row != 0 && col != 0)
    {
        // Allocate memory for the covariance matrix
        float **covMat = returnMatrix(row, col);
        // Calculate and store the mean of each column of the given matrix
        float *colMeans = columnMeans(mat, row, col);
        // Allocate memory for the transpose matrix
        float **covMatT = returnMatrix(col, row);
        // Allocate memory for the Multiplication opeartion
        float **covMatMultiply = returnMatrix(row, row);
        // Loop over the elements of the matrix
        for (int i = 0; i < row; i++)
        {
            for (int j = 0; j < col; j++)
            {
                // Calculate the Xc matrix
                covMat[i][j] = mat[i][j] - colMeans[j];
            }
        }
        // Calculate the transpose of the Xc matrix
        covMatT = matrixTranspose(covMat, row, col);

        // Calculate the multiplication of the Xc matrix and the transpose of the Xc matrix
        covMatMultiply = (matrixMultiplication(covMatT, covMat, col, row, row, col));

        freeMatrix(covMat, row);
        freeMatrix(covMatT, col);
        free(colMeans);
        for (int i = 0; i < col; i++)
        {
            for (int j = 0; j < col; j++)
            {
                // Store the result in the matrix
                covMatMultiply[i][j] /= (float)row;
            }
        }

        return covMatMultiply;
    }
    else
        printf("The matrix is empty or row and col not equal!\n");
    exit(1);
    return NULL;
}
// Function for returning a matrix
float **returnMatrix(int rows, int cols)
{
    // Allacote memory for the matrix
    float **matrix = (float **)malloc(rows * sizeof(float *));
    for (int i = 0; i < rows; i++)
    {
        // Allocate memory for each row of the matrix
        matrix[i] = (float *)malloc(cols * sizeof(*(matrix[i])));
    }
    // Return matrix
    return matrix;
}
// Function for multiplying two matrices
float **matrixMultiplication(float **mat1, float **mat2, int row1, int col1, int row2, int col2)
{
    // Check if the number of columns of the first matrix is equal to the number of rows of the second matrix
    if (col1 == row2)
    {
        // Allocate memory for the result matrix
        float **matMulti = returnMatrix(row1, col2);
        // Loop over the rows of the first matrix
        for (int i = 0; i < row1; i++)
        {
            // Loop over the columns of the second matrix
            for (int j = 0; j < col2; j++)
            {
                // matMulti[i][j] = 0;
                for (int k = 0; k < col1; k++)
                {
                    // Calculate the multiplication of the two matrices
                    matMulti[i][j] += mat1[i][k] * mat2[k][j];
                }
            }
        }
        // Return the result matrix
        return matMulti;
    }
    else
        printf("The number of columns of the first matrix is not equal to the number of rows of the second matrix");
    exit(1);
    return NULL;
}
// Function for calculating the transpose of a matrix
float **matrixTranspose(float **mat, int row, int col)
{
    // Allocate memory for the transpose matrix
    float **matTranspose = returnMatrix(col, row);
    // Loop over the elements of the matrix
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            // Calculate the transpose of the matrix
            matTranspose[j][i] = mat[i][j];
        }
    }
    // Return the transpose matrix
    return matTranspose;
}
//@desc This function freeing the memory given to the matrix
void freeMatrix(float **matrix, int rows)
{
    // Free the memory given to each row of the matrix
    for (int i = rows - 1; i >= 0; i--)
    {
        free(matrix[i]);
    }
    // Free the memory given to the matrix itself
    free(matrix);
}