/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the implementation of KNN search in vantage point tree
 **/

#include <stdlib.h>
#include <utils.h>
#include <Queue.h>
#include <search_knn.h>

void knn_search(struct vp_point *node, struct points_struct *points, int idx, int k, float* tau, struct queue_node **neibs_queue)
{
    if ( node == NULL ) 
        return;
    float dist = calculate_euk_distance(&(points->points_arr[idx * points->dim]), &(points->points_arr[node->idx * points->dim]), points->dim);
    if ( dist < *tau ) 
    {
        if (get_heap_size(neibs_queue) == k) 
            pop(neibs_queue);
        push(neibs_queue, node->idx, dist);
        if (get_heap_size(neibs_queue) == k) 
            *tau = top(neibs_queue)->priority;
    }
    if ( node->left == NULL && node->right == NULL )
        return;

    if ( dist < node->thresshold ) 
    {
        if ( dist - *tau <= node->thresshold ) 
            knn_search(node->left, points, idx, k, tau, neibs_queue);

        if ( dist + *tau >= node->thresshold )
            knn_search(node->right, points, idx, k, tau, neibs_queue);
    } 
    else 
    {
        if ( dist + *tau >= node->thresshold )
            knn_search(node->right, points, idx, k, tau, neibs_queue);

        if ( dist - *tau <= node->thresshold )
            knn_search(node->left, points, idx, k, tau, neibs_queue);
    }
    
}
