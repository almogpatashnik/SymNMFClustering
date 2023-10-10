import pandas as pd
import numpy as np
import sys
try:
    import symnmfmodule
except Exception as e:
    # Exception occurred during module import, handle it here
    print(str(e))
    sys.exit(1)

# this function prints our matrices in requested format
def matrix_printer(matrix, rows, columns):
    for i in range(rows):
        for j in range(columns-1):
            print("{:.4f}".format(float(matrix[i*columns+j])), end=",")
        print("{:.4f}".format(float(matrix[i*columns+columns-1])))
    return


def symnmf_from_python(argv):

    np.random.seed(0)
    # define our arguments
    k = int(argv[0])
    goal = argv[1]
    input_path = argv[2]
    from_analysis = argv[3]

    # reading file vectors here and find out N using pandas:
    vectors = pd.read_csv(input_path, header=None)


    #defining the parameters
    N = vectors.shape[0]   # N is number of vectors
    dim = vectors.shape[1] #counts how many columns there are . which means how many coordinates in here from the vector
 

    # create the lists to be delivered to c module
    matrix = np.mat(vectors.values)
    vectors = []
    for i in range(N):
        vectors += matrix[i, :].tolist()[0]


    try:
        if goal == 'symnmf':
            w = symnmfmodule.norm(vectors , N , dim)
            m = sum(w) / len(w)
            init_h = np.random.uniform(0, 2 * np.sqrt(m / k), size=(1, N*k))
            init_h = init_h[0].tolist()
            h = symnmfmodule.symnmf(init_h,w, k, N)
            if from_analysis == "True": # in case that the clustering request comes from analysis, we want to return those arguments
                return N, dim, vectors, h
            matrix_printer(h, N, k)
        elif goal == 'sym':
            a = symnmfmodule.sym(vectors, N, dim)
            matrix_printer(a, N, N)
        elif goal == 'ddg':
            d = symnmfmodule.ddg(vectors, N, dim)
            matrix_printer(d, N, N)
        else:# assuming the goal entered is legal - the left goal is norm
            w = symnmfmodule.norm(vectors, N , dim)
            matrix_printer(w, N, N)

    except Exception as e:
        print(str(e))
        return 1


    return 0



if __name__ == '__main__':
    params_from_user = sys.argv[1:]
    params_from_user += ["False"] #indicate that the clustering goal request comes from user and not analysis
    symnmf_from_python(params_from_user)