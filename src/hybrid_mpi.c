#include <stdlib.h>
#include <stdio.h>
#include <mixed_tree.h>
#include <utils.h>
#include <mpi.h>
#define THRES 140000


struct vp_point* hybrid_mpi_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent, int id)
{
    if(id == 0)
    {
        if(n == 0)
            return NULL;
        int d = points->dim;

        //pick vantage point
        float * vp = &(points->points_arr[idxs[0] * d]);
        n = n - 1;
        float *dists_arr;
        if(n * d >= THRES)
            dists_arr = calculateDistancesParallel(points, vp, idxs + 1, n);
        else
            dists_arr = calculateDistances(points, vp, idxs + 1, n);
        float median = quickselect(dists_arr, 0, n-1, ceil(n/2.0));
        int *left_idxs, *right_idxs, n_l, n_r;
        split_idxs(idxs + 1, dists_arr, n, median, &left_idxs, &right_idxs, &n_l, &n_r);      //in idx and n parameters we exclude current root
        //create node
        struct vp_point *node = (struct vp_point*) malloc(sizeof(struct vp_point));
        node->idx = idxs[0];
        free(idxs);     //dealocating idxs to avoid dynamic memory corruption as they are not used anymore
        node->parent = parrent;
        node->thresshold = median;
        MPI_Send(&n_r, 1, MPI_INT, 1, 1, MPI_COMM_WORLD); 
        MPI_iSend(idxs+1, n_r, MPI_INT, 1, 1, MPI_COMM_WORLD); 
        node->left = mixed_vp_create(points, left_idxs, n_l, node);
        struct int_vector *preorder;
        //MPI_Recv(test, 128, MPI_CHAR, i, 4444, MPI_COMM_WORLD, &stat); 
        preorder_to_tree(preorder, &(node->right), 0);
        return node;
    }
    else
    {
        int n_r;
        MPI_Status stat; 
        MPI_Recv(&n_r, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat); 
        int *right_idxs = (int*)malloc(sizeof(int)*n_r);
        MPI_Recv(right_idxs, n_r, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat); 
        struct vp_point *node = mixed_vp_create(points, right_idxs, n_r, NULL);
        struct int_vector preorder;
        read_preorder(node, 1, &preorder, n_r);
        //MPI_Send(test, 128, MPI_CHAR, i, 4444, MPI_COMM_WORLD, &stat); 
    }
    return NULL;
}