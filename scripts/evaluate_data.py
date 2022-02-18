import numpy as np
import pandas as pd
from binarytree import Node


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


data = np.fromfile('../data/dt_2_10_3.dat', dtype='float32')
n, d = data[0:2].astype('int')
data = data[2:].reshape(n, d)
idxs = np.arange(0, n, dtype=int)
tree = create_vantage_tree(data, idxs)

print(tree)
pre_order = np.array([x.value for x in tree.preorder], dtype='int32')
f = open(f"./grount_truth.dat", 'wb')
pre_order.tofile(f)
f.close()

print(np.fromfile('grount_truth.dat', dtype='int32'))
