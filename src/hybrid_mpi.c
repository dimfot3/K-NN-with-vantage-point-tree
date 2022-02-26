#include <stdlib.h>
#include <stdio.h>
#include <mixed_tree.h>
#include <utils.h>
#include <mpi.h>
#define THRES 140000


struct vp_point* hybrid_mpi_vp_create(struct points_struct* points, int* idxs, int id)
{
    //leader process creates the first node and send the right to second process
    if(id == 0)
    {
        int n = points->num;
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
        node->parent = NULL;
        node->thresshold = median;
        free(idxs); 

        //send right idxs to other node
        MPI_Send(&n_r, 1, MPI_INT, 1, 1, MPI_COMM_WORLD); 
        MPI_Send(right_idxs, n_r, MPI_INT, 1, 1, MPI_COMM_WORLD); 

        //find the left node
        node->left = mixed_vp_create(points, left_idxs, n_l, node);

        //wait the right node in preorder form and tf to tree structure
        struct int_vector preorder;
        MPI_Status stat;
        MPI_Recv(&(preorder.n), 1, MPI_INT, 1, 2, MPI_COMM_WORLD, &stat);
        preorder.arr = (int*) malloc(sizeof(int) * preorder.n);
        preorder.thres = (float*) malloc(sizeof(float) * preorder.n);
        node->right = NULL;
        MPI_Recv(preorder.arr, preorder.n, MPI_INT, 1, 2, MPI_COMM_WORLD, &stat);
        MPI_Recv(preorder.thres, preorder.n, MPI_FLOAT, 1, 2, MPI_COMM_WORLD, &stat);
        int idx = 0;
        preorder_to_tree(&preorder, &(node->right), &idx);
        node->right->parent = node;   
        return node;
    }
    else
    {
        //wait right idxs
        int n_r;
        MPI_Status stat;
        MPI_Recv(&n_r, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat); 
        int *right_idxs = (int*) malloc(sizeof(int) * n_r);
        MPI_Recv(right_idxs, n_r, MPI_INT, 0, 1, MPI_COMM_WORLD, &stat);

        //calculate right node
        struct vp_point *node = mixed_vp_create(points, right_idxs, n_r, NULL);

        //transform the right tree to preorder and send to leader
        struct int_vector preorder;
        read_preorder(node, 1, &preorder, n_r); 
        MPI_Send(&(preorder.n), 1, MPI_INT, 0, 2, MPI_COMM_WORLD); 
        MPI_Send(preorder.arr, preorder.n, MPI_INT, 0, 2, MPI_COMM_WORLD); 
        MPI_Send(preorder.thres, preorder.n, MPI_FLOAT, 0, 2, MPI_COMM_WORLD); 
    }
    return NULL;
}