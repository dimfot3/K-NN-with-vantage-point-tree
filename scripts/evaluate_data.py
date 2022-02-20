import numpy as np
import pandas as pd
from binarytree import Node
from sklearn.neighbors import NearestNeighbors
from sklearn.neighbors import KDTree

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

data = np.fromfile('../data/dt_1_100_100.dat', dtype='float32')
n, d = data[0:2].astype('int')
data = data[2:].reshape(n, d)
idxs = np.arange(0, n, dtype=int)

tree = create_vantage_tree(data, idxs)

pre_order = np.array([x.value for x in tree.preorder], dtype='int32')
real_pre = np.fromfile('./preorder.txt', sep=' ', dtype=int).reshape(1,-1)[0]
print(f'VP tree: {(pre_order == real_pre).all()}')


real_k = np.fromfile('./neibs.txt', sep=' ', dtype=int).reshape(1,-1)[0]
kdt = KDTree(data, leaf_size=500, metric='euclidean')
dist, ind = kdt.query(data, k=min(256, n))
ind = ind.reshape(1,-1)[0]
dist = dist.reshape(1,-1)[0]
print(real_k)
print(ind)
print(f'KNN: {(ind == real_k).all()}')

print(dist[np.where(ind != real_k)])