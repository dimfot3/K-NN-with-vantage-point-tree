import numpy as np
import pandas as pd
from binarytree import Node


def create_vantage_tree(points, orig_points):
    n, d = points.shape
    if(n == 0):
        return None
    #pic the vantage point
    vp = points[0, :]
    idx = int(np.where((orig_points == vp).all(axis=1))[0][0])
    root = Node(idx)
    distances = np.zeros(n)
    for i in range(n):
        distances[i] = np.linalg.norm(points[i, :] - vp)
    median = np.median(distances)

    left_idx = np.full(n, False)
    left_idx[np.where((distances[:] <= median))] = True

    right_idx = np.full(n, False)
    right_idx[np.where((distances[:] > median))] = True
    print(left_idx, right_idx)
    root.left = create_vantage_tree((points[left_idx, :])[1:], orig_points)
    root.right = create_vantage_tree((points[right_idx, :])[0:], orig_points)
    return root


data = np.fromfile('../data/dt_2_10_3.dat', dtype='float32')
n, d = data[0:2].astype('int')
data = data[2:].reshape(n, d)
tree = create_vantage_tree(data, data)

print(tree)
pre_order = np.array([x.value for x in tree.preorder], dtype='int32')
f = open(f"./grount_truth.dat", 'wb')
pre_order.tofile(f)
f.close()

print(np.fromfile('grount_truth.dat', dtype='int32'))
