#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "symnmf.h"

/*prototypes*/
double *stringToDouble(char *str, double *array, int dim);
void matrix_printer(double *mat, int rows, int columns);

/*this function receives a string with a vector's coordinates, and the number of coordinates
and turns the string into an array*/
double *stringToDouble(char *str, double *array, int dim)
{
    char *ptr;
    double num;
    int i;

    for (i = 0; i < dim; i++)
    {
        num = strtod(str, &ptr);
        array[i] = num;
        str = ptr + 1;
    }
    return (array);
}

/*this function prints our matrices in requested format*/
void matrix_printer(double *mat, int rows, int columns)
{
    int i, j;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < columns - 1; j++)
        {
            printf("%.4f,", mat[i * columns + j]);
        }
        printf("%.4f", mat[i * columns + columns - 1]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{

    /*variables declerations*/
    int initialCapacity = 100; /*num of vectors i can get*/
    double *vectors = NULL;    /*Pointer to the allocated memory block*/
    double *tmp = NULL;
    int N = 0;                      /*Number of vectors entered by the user*/
    int capacity = initialCapacity; /*Capacity of the allocated memory block*/
    int dim = 1;
    size_t size = 150;
    int i, j;
    char *string;
    int bytes_read;
    double *tmp_cordi;
    char *goal;
    char *file_path;
    FILE *file;
    double *result = NULL;

    if (argc == 3)
    {
        goal = argv[1];
        file_path = argv[2];
    }
    else
    {
        printf("An Error Has Occurred\n");
        return 1;
    }

    file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("An Error Has Occurred\n");
        return 1;
    }

    string = (char *)malloc(size); /*Allocate memory for character array*/
    if (string == NULL)
    {
        printf("An Error Has Occurred\n");
        fclose(file);
        return 1;
    }

    bytes_read = getline(&string, &size, file); /*Read line from user input*/

    if (bytes_read == -1)
    {
        free(string);
        printf("An Error Has Occurred\n");
        fclose(file);
        return 1;
    }

    else
    {
        /*finds how many coordinates in each vector - it knows because of commas*/
        for (i = 0; i < bytes_read; i++)
        {
            if (string[i] == ',')
            {
                dim++;
            }
        }

        vectors = (double *)malloc(capacity * sizeof(double) * dim);
        if (vectors == NULL)
        {
            free(string);
            printf("An Error Has Occurred\n");
            fclose(file);
            return 1;
        }

        tmp_cordi = (double *)malloc(dim * sizeof(double));
        if (tmp_cordi == NULL)
        {
            free(string);
            free(vectors);
            printf("An Error Has Occurred\n");
            fclose(file);
            return 1;
        }

        /*creating our vectors array*/
        while (bytes_read != -1)
        {
            N++; /*counting our vectors*/

            tmp_cordi = stringToDouble(string, tmp_cordi, dim); /*we recieve here an array of double*/

            /*resize the memory alloc*/
            if (N >= capacity - 2)
            {
                capacity *= 2;

                tmp = (double *)realloc(vectors, capacity * sizeof(double) * dim);
                if (tmp == NULL)
                {
                    free(string);
                    free(vectors);
                    free(tmp_cordi);
                    printf("An Error Has Occurred\n");
                    fclose(file);
                    return 1;
                }
                vectors = tmp;
                tmp = NULL;
            }

            /*put values into vector*/
            for (j = 0; j < dim; j++)
            {
                vectors[(N - 1) * dim + j] = tmp_cordi[j];
            }

            bytes_read = getline(&string, &size, file); /*each vector as a string*/
        }

        free(tmp_cordi);
        /*here we are done and have a list of vectors 2D matrix actually*/
    }
    free(string);

    if (strcmp(goal, "sym") == 0)
    {
        result = calc_sym(vectors, N, dim);
    }
    else if (strcmp(goal, "ddg") == 0)
    {
        result = calc_ddg(vectors, N, dim);
    }
    else /*assuming that the input is valid - the goal that left is norm*/
    {
        result = calc_norm(vectors, N, dim);
    }

    free(vectors);

    if (result != NULL)/*for succesfull running*/
    {
        matrix_printer(result, N, N);
        free(result);
    }
    else
    {
        printf("An Error Has Occured\n");
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}
