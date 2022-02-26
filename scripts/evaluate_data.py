import numpy as np
import pandas as pd
from binarytree import Node
from sklearn.neighbors import NearestNeighbors
from sklearn.neighbors import KDTree
import time
import glob, os

def create_vantage_tree(orig_points, idxs):
    n = len(idxs)
    if (n == 0):
        return None
    # pic the vantage point
    vp = orig_points[idxs[0], :]
    root = Node(int(idxs[0]))
    n = n - 1
    if(n == 0):
        return root

    idxs = idxs[1:]
    distances = np.zeros(n)
    for i in range(n):
        distances[i] = np.linalg.norm(orig_points[idxs[i], :] - vp)
    median = np.median(distances)
    #split the idxes
    left_idx = idxs[np.argwhere(distances <= median).reshape(1, -1)[0]]
    right_idx = idxs[np.argwhere(distances > median).reshape(1, -1)[0]]

    root.left = create_vantage_tree(orig_points, left_idx)
    root.right = create_vantage_tree(orig_points, right_idx)
    return root



file = '../data/dt_2_400_200.dat'
data = np.fromfile(f'../data/{file}', dtype='float32')
n, d = data[0:2].astype('int')
data = data[2:].reshape(n, d)


#Vantage point tree
idxs = np.array(range(0, n))
root = create_vantage_tree(data, idxs)
print(root)
print(root.preorder)

#KNN
kdt = KDTree(data, leaf_size=40, metric='euclidean')
dist, ind = kdt.query(data, k=min(256, n))

print(ind)



