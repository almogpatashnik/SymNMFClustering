import sys
import numpy as np
from sklearn.metrics import silhouette_score
from symnmf import symnmf_from_python
from kmeans import kmeans_py

#this method recives matrix h (was calculated at python) , k , N and returns a mapping between vectors and clusters
def belongs_gen(h , k , N):
    belongs = []
    for i in range(N):
        max = -1
        max_index = -1
        for j in range(k):
            if h[i*k + j] > max:
                max = h[i*k + j]
                max_index = j
        belongs.append(max_index)
    return belongs

if __name__ == '__main__':
    lst_params = sys.argv[1:] #parameters recived from user: lst_params[0] = k , [1] = filepath
    k = int(lst_params[0])

    try: # we are doing try-except inorder to handel exception from c that may occur.
        N, dim, vectors, h = symnmf_from_python([k, "symnmf", lst_params[1], "True"]) # call to the symnmf from python. True is indicator for being called from analysis.
        vectors_np = np.array(vectors).reshape(N, dim) #organize the vectors for the silhouette
        
        belongs = belongs_gen(h, k, N) # creating the map between vector and cluster for the symnmf
        belongs_kmeans = kmeans_py([k, vectors, dim, N]) # creating the map between vector and cluster for the kmeans, from hw1


        print("nmf: " + "{:.4f}".format(float(silhouette_score(vectors_np, belongs))))
        print("kmeans: " + "{:.4f}".format(float(silhouette_score(vectors_np, belongs_kmeans))))
    
    except Exception as e:
        sys.exit()
