/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the declaration of KNN search of vantage point tree
 **/
#ifndef _KSEARCH_H
#define _KSEARCH_H

/**
 * @brief this functions searches a vp tree for k nearest neighbors of a point.
 * 
 * @param node pointer to an entry node of the tree
 * @param points pointer to the struct of points
 * @param idx the idx of point of interest
 * @param k how many nearest neighbors we need to find
 * @param neib_arr the array where idxs of nearest neighbors will be saved
 * @return void
 **/
void knn_search(struct vp_point *node, struct points_struct *points, int idx, int k, struct queue *neibs);

#endif