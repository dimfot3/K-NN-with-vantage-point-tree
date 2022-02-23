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



"""


idxs = np.arange(0, n, dtype=int)

tree = create_vantage_tree(data, idxs)

pre_order = np.array([x.value for x in tree.preorder], dtype='int32')
real_pre = np.fromfile('./preorder.txt', sep=' ', dtype=int).reshape(1,-1)[0]
print(f'VP tree: {(pre_order == real_pre).all()}')
"""

files = []
os.chdir("../data/")
for file in glob.glob("*.dat"):
    files.append(file)

times = np.array([], dtype=float)
print(files)
for i in range(len(files)):
    data = np.fromfile(f'../data/{files[i]}', dtype='float32')
    n, d = data[0:2].astype('int')
    data = data[2:].reshape(n, d)
    kdt = KDTree(data, leaf_size=40, metric='euclidean')
    for j in range(1,256):
        start = time.time()
        dist, ind = kdt.query(data, k=min(j, n))
        end = time.time()
        times = np.append(times, end - start)
    print('.....')
print(times)



