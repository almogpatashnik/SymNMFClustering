# SYMNMF-clustering-project

The project introduces the SYMNMF Clustering algorithm, designed to calculate clusters for a given dataset.

The core of the project involves implementing the SYMNMF algorithm for data clustering. It offers flexibility by enabling the computation of essential matrices necessary for clustering or the execution of the full algorithm. The outcome is a matrix that represents the relationship between clusters and vectors.

**Key Features:**
- Development in both C and Python.
- Inclusion of a Python-C API for seamless integration and extendibility.
- Implementation of the K-means algorithm, another popular clustering method, in Python.
- Comparison between SYMNMF and the K-Means algorithm using the silhouette score. This comparison assesses the quality of clusters for a given dataset and a specified number of required clusters, utilizing Python's scikit-learn library


The project's technique, is thoroughly discussed in the paper titled ["Symmetric Nonnegative Matrix Factorization for Graph Clustering.pdf."](https://github.com/almogpatashnik/SymNMF_Clustering/blob/master/Symmetric%20Nonnegative%20Matrix%20Factorization%20for%20Graph%20Clustering.pdf)

For detailed project requirements, please refer to ["project description.pdf."](https://github.com/almogpatashnik/SymNMF_Clustering/blob/master/project_description.pdf)

## C Interface
### How to build
```bash
make
```
### How to execute
```bash
./my_program <goal> <input_file>
```
**goal (string):** Can get the following values:
- sym: Calculate and output the Similarity Matrix.
- ddg: Calculate and output the Diagonal Degree Matrix.
- norm: Calculate and output the Normalized Similarity Matrix.

**input_file:**    
The path to the Input file, it will contain N data points for all
above goals, the file extension is .txt

## Python Interface
### Requirements
 - numpy
 - pandas
### How to build
```bash
python3 setup.py build_ext --inplace
```
### How to execute
```bash
python3 symnmf.py <k> <goal> <input_file>
```
**k (int, < N):** Number of required clusters.

**goal (string):** Can get the following values:
- symnmf: Perform full the symNMF and output H (as described at the project descreption).
- sym: Calculate and output the Similarity Matrix.
- ddg: Calculate and output the Diagonal Degree Matrix.
- norm: Calculate and output the Normalized Similarity Matrix.

**input_file:**    
The path to the Input file, it will contain N data points for all
above goals, the file extension is .txt


## Analysis
### Requirements
 - numpy
 - scikit-learn

### How to execute
```bash
python3 analysis.py <k> <input_file>
```
**k (int, < N):** Number of required clusters.

**input_file:**    
The path to the Input file, it will contain N data points for all
above goals, the file extension is .txt


**For additional information about the project and how it works use the [project description file](project%20description.pdf).**

***Made by [@almogpatashnik](https://github.com/almogpatashnik) && [@maibendayan](https://github.com/maibendayan)***
# SymNMFClustering
