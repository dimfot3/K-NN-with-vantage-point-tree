/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here are the implementation of serial vp tree creating utilities
 **/
#include <stdlib.h>
#include <utils.h>


struct vp_point* serial_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent)
{
    if(n == 0)
        return NULL;
    int d = points->dim;

    //pick vantage point
    float * vp = &(points->points_arr[ idxs[0] * d]);
    
    float *dists_arr = calculateDistances(points, vp, idxs + 1, n-1);       //in idx and n parameters we exclude current root

    float median = quickselect(dists_arr, 0, n-2, ceil((n-1)/2.0));
    int *left_idxs, *right_idxs, n_l, n_r;
    split_idxs(idxs + 1, dists_arr, n-1, median, &left_idxs, &right_idxs, &n_l, &n_r);      //in idx and n parameters we exclude current root
    
    //create node
    struct vp_point *node = (struct vp_point*) malloc(sizeof(struct vp_point));
    node->idx = idxs[0];
    node->parent = parrent;
    node->thresshold = median;
    node->left = serial_vp_create(points, left_idxs, n_l, node);
    node->right = serial_vp_create(points, right_idxs,  n_r, node);

    return node;
}