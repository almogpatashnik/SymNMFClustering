#include "symnmf.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*PROTOTYPES*/
double **calc_a_d(double *vectors, int N, int dim, int both);
double *calc_sym(double *vectors, int N, int dim);
double *calc_ddg(double *vectors, int N, int dim);
double *calc_norm(double *vectors, int N, int dim);
double *calc_symnmf(double *h, double *w, int k, int N);

/*calculate the metrices a and d*/
double **calc_a_d(double *vectors, int N, int dim, int both)
{
    /*first we calc a*/
    double **ret;/*will contain pointers to a and d*/
    double *a, *d;
    int i, j, m;
    double sum, val;

    ret = (double **)malloc(2 * sizeof(double *));
    if (ret == NULL)
    {
        return NULL;
    }

    a = (double *)calloc(N * N, sizeof(double));
    if (a == NULL)
    {
        free(ret);
        return NULL;
    }

    /*calculate squared euclidean distance*/
    for (i = 0; i < N; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            sum = 0;
            for (m = 0; m < dim; m++)
            {
                sum += pow(vectors[i * dim + m] - vectors[j * dim + m], 2);
            }
            val = exp(-sum / 2);
            a[i * N + j] = val;
            a[j * N + i] = val;
        }
    }

    if (both == 0){  /*we only wish to calculate a*/
        ret[0] = a;
        ret[1] = NULL;
        return ret;
    }

    /*now we calc d*/
    d = (double *)calloc(N * N, sizeof(double));
    if (d == NULL)
    {
        free(ret);
        free(a);
        return NULL;
    }

    /*filling the d matrix*/
    for (i = 0; i < N; i++)
    {
        sum = 0;
        for (j = 0; j < N; j++)
        {
            sum += a[i * N + j];
        }
        d[i * N + i] = sum;
    }

    ret[0] = a;
    ret[1] = d;
    return ret;
}

/*calculate the similarity matrix*/
double *calc_sym(double *vectors, int N, int dim)
{
    double **metrices_array;
    double *a;
    metrices_array = calc_a_d(vectors, N, dim, 0);
    if (metrices_array == NULL){
        return NULL;
    }
    a = metrices_array[0];
    free(metrices_array);
    return a;
}



/*calculate the diagnal degree matrix */
double *calc_ddg(double *vectors, int N, int dim)
{
    double **metrices_array;
    double *a, *d;
    metrices_array = calc_a_d(vectors, N, dim, 1);
    if (metrices_array == NULL){
        return NULL;
    }
    a = metrices_array[0];
    free(a);
    d = metrices_array[1];
    free(metrices_array);
    return d;
}

/*calculate the normalized similarity matrix*/
double *calc_norm(double *vectors, int N, int dim)
{   
    double **metrices_array;
    double *d, *a, *w, *minus_sqrt_d, *da;
    int i, j;

    /*calculate the A and D matrices*/
    metrices_array = calc_a_d(vectors, N, dim, 1);
    if (metrices_array == NULL){
        return NULL;
    }
    a = metrices_array[0];
    d = metrices_array[1];
    free(metrices_array);
    
    /*alocate space for w*/
    w = (double *)calloc(N * N, sizeof(double));
    if (w == NULL)
    {
        free(a);
        free(d);
        return NULL;
    }

    minus_sqrt_d = d;

    /*calculate D^-0.5*/
    for (i = 0; i < N; i++)
    {
        minus_sqrt_d[i * N + i] = pow(d[i * N + i], -0.5);
    }

    da = (double *)calloc(N * N, sizeof(double));
    if (da == NULL)
    {
        free(a);
        free(d);
        free(w);
        return NULL;
    }

    /* calc (D**-0.5)*A  */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            da[i * N + j] = minus_sqrt_d[i * N + i] * a[i * N + j];
        }
    }

    /*now we multiply (D**-0.5)*A by (D**-0.5) to get :   (D**-0.5)*A*(D**-0.5)    */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            w[i * N + j] = da[i * N + j] * minus_sqrt_d[j * N + j];

        }
    }
    free(d);
    free(a);
    free(da);
    return w;
}

/*calculate the optimal H matrix*/
double *calc_symnmf(double *h, double *w, int k, int N)
{
    int iter, i, j, m;
    double *h_t, *wh, *hht, *hhth, *h_i1, *tmp;
    double sum, f_norm;
    double beta = 0.5;
    int max_iter = 300;
    double epsilon = 0.0001;

    h_t = (double *)calloc(N * k, sizeof(double));
    if (h_t == NULL)
    {
        return NULL;
    }

    wh = (double *)calloc(N * k, sizeof(double));
    if (wh == NULL)
    {
        free(h_t);
        return NULL;
    }

    hht = (double *)calloc(N * N, sizeof(double));
    if (hht == NULL)
    {
        free(h_t);
        free(wh);
        return NULL;
    }

    hhth = (double *)calloc(N * k, sizeof(double));
    if (hhth == NULL)
    {
        free(h_t);
        free(wh);
        free(hht);
        return NULL;
    }

    h_i1 = (double *)calloc(N * k, sizeof(double));
    if (h_i1 == NULL)
    {
        free(h_t);
        free(wh);
        free(hht);
        free(hhth);
        return NULL;
    }

    for (iter = 0; iter < max_iter; iter++)
    {

        /*calc h traspose*/
        for (i = 0; i < k; i++)
        {
            for (j = 0; j < N; j++)
            {
                h_t[i * N + j] = h[j * k + i];
            }
        }

        /*calc W*H  */
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < k; j++)
            {
                sum = 0;
                for (m = 0; m < N; m++)
                {
                    sum += w[i * N + m] * h[m * k + j];
                }
                wh[i * k + j] = sum;
            }
        }

        /* calc H*(H_T)*H  in two steps */
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                sum = 0;
                for (m = 0; m < k; m++)
                {
                    sum += h[i * k + m] * h_t[m * N + j];
                }
                hht[i * N + j] = sum;
            }
        }
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < k; j++)
            {
                sum = 0;
                for (m = 0; m < N; m++)
                {
                    sum += hht[i * N + m] * h[m * k + j];
                }
                hhth[i * k + j] = sum;
            }
        }

        /* calc H(i+1) */
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < k; j++)
            {
                h_i1[i * k + j] = h[i * k + j] * (1 - beta + beta * (wh[i * k + j] / hhth[i * k + j]));
            }
        }

        /* calc frobenius norm*/
        f_norm = 0;
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < k; j++)
            {
                f_norm += pow(h_i1[i * k + j] - h[i * k + j], 2);
            }
        }
        /*making h to be h_i+1 for the next iteration*/
        tmp = h;
        h = h_i1;
        h_i1 = tmp;

        if (f_norm < epsilon)
        {
            break;
        }
    }

    free(h_t);
    free(wh);
    free(hht);
    free(hhth);
    free(h_i1);

    return h;
}



/*
int main()
{

}
*/



