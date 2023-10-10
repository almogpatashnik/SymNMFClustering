# ifndef SYMNMFHEADER_H_
# define SYMNMFHEADER_H_

double* calc_sym(double *vectors, int N, int dim);
double* calc_ddg(double *vectors, int N, int dim);
double* calc_norm(double *vectors, int N, int dim);
double* calc_symnmf(double *h, double *w, int k, int N);

# endif