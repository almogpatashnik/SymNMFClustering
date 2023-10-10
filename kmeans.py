
class Vector:
    def __init__(self, coordinates):
        self.coordinates = coordinates
        self.cluster = None #points to the cluster it belongs to


class Cluster:
    def __init__(self):
        self.vectors = [] #each entry will contain the coordinate of the appropriate vector
        self.prev_miu = None
        self.curr_miu = None

def calc_miu(cluster, vec_len):
    """this func calcs the current miu of a cluster"""
    miu_curr = [0 for i in range(vec_len)]
    for vector in cluster.vectors:
        cordi = vector
        miu_curr = [miu_curr[i] + cordi[i] for i in range(vec_len)] #sum per cordinate all vectors in the cluster

    miu_curr = [miu_curr[i] / len(cluster.vectors) for i in range(vec_len)] #calculate the miu
    return miu_curr


def calc_distance(vector1, centroid):
    """ this func calculates the distance between vector and the centroid of a cluster"""
    res = [(vector1[i] - centroid[i]) ** 2 for i in range(len(vector1))]
    return (sum(res)) ** 0.5

def kmeans_py(argv):
    # parsing parameters
    vectors_array =  argv[1]
    dim = argv[2]
    N = argv[3]
    k = argv[0] # here it is already comes as an int. 
    epsilon = 0.0001 # as mentioned on the instructions
    iter = 300 # same as above

    # organizing our vectors 
    vectors = [] # a list that will contail all of our vectors
    for i in range(N):
        vector = Vector(vectors_array[i*dim:(i+1)*dim])
        vectors.append(vector)

    # initialization of the Clusters
    clusters = [Cluster() for i in range(k)]
    #at the begining each cluster i is datapoint i
    for i in range(k):
        # initialize pointers from cluster to vector and vector to cluster
        clusters[i].vectors.append(vectors[i].coordinates)    # init: cluster contains vector cordinates
        vectors[i].cluster = clusters[i]   # vector has pointer to its curr cluster
        clusters[i].curr_miu = vectors[i].coordinates   # at the beginning we set the only vector to be the centroid = curr_miu

    # body of the algorithm
    flag = True  # whether we still need to update the clusters
    for i in range(iter):
        if flag is False:
            break

        for vector in vectors:
            minimum = float('inf')
            tmp = vector.cluster
            for cluster in clusters:
                d = calc_distance(vector.coordinates, cluster.curr_miu)
                if minimum > d:
                    tmp = cluster
                    minimum = d

            if tmp != vector.cluster:  # didn't stay at the same cluster - update them
                if vector.cluster is not None: # will be None only for the first iteration and for the N-k last ones.
                    vector.cluster.vectors.remove(vector.coordinates)
                tmp.vectors.append(vector.coordinates) #adding the vector to the new cluster it belongs to
                vector.cluster = tmp # updating 

        flag = False
        for cluster in clusters:
            # update miu
            cluster.prev_miu = cluster.curr_miu
            cluster.curr_miu = calc_miu(cluster, dim)  # calc the curr miu
            if calc_distance(cluster.curr_miu, cluster.prev_miu) > epsilon:
                flag = True 

    # creating the belongs array
    belongs = []
    for i in range(N):
        belongs.append(clusters.index(vectors[i].cluster))

    return belongs


# if __name__ == "__main__":
#     main(sys.argv[1:])


