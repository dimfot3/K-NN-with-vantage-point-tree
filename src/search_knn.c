/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the implementation of KNN search in vantage point tree
 **/

#include <stdlib.h>
#include <utils.h>
#include <search_knn.h>

void knn_search(struct vp_point *node, struct points_struct *points, int idx, int k, struct queue *neibs)
{
    if ( node == NULL ) 
        return;
    
    float dist = calculate_euk_distance(&(points->points_arr[idx * points->dim]), &(points->points_arr[node->idx * points->dim]), points->dim);
    if ( dist < neibs->tau ) {
        neibs->queue[((neibs->next_idx)++)%k] = node->idx;
        neibs->num = MIN(neibs->num + 1, k);
        if ( neibs->num == k ) neibs->tau = dist;
    }
    
    if ( node->left == NULL && node->right == NULL )
        return;

    if ( dist < node->thresshold ) 
    {
        if ( dist - neibs->tau <= node->thresshold ) 
            knn_search(node->left, points, idx, k, neibs);

        if ( dist + neibs->tau >= node->thresshold )
            knn_search(node->right, points, idx, k, neibs);
    } 
    else 
    {
        if ( dist + neibs->tau >= node->thresshold )
            knn_search(node->right, points, idx, k, neibs);

        if ( dist - neibs->tau <= node->thresshold )
            knn_search(node->left, points, idx, k, neibs);
    }
}
