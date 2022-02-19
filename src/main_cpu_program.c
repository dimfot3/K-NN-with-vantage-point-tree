/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the main program for knn implementation with vantage tree that runs on cpu exclusivly.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <serial_vp_tree.h>
#include <sys/time.h>
#include <float.h>
#include <search_knn.h>
#define VERBOSE 0


int main(int argc, char** argv)
{
    struct ses_args args;
    struct points_struct points;

    parse_arguments(argc, argv, &args);
    read_points(args.path, &points, VERBOSE);
    
    int* idxs = malloc(sizeof(int)*points.num);
    for(int i = 0; i < points.num; i++)
        idxs[i] = i;
    
    //serial vp creation. When we run in mode 0 its time is saved, when we run in other mode this is used for validation
    struct timeval t0, t1, tk0, tk1;
    double creation_time, knn_time;
    gettimeofday(&t0, 0);
    struct vp_point *root = serial_vp_create(&points, idxs, points.num, NULL);
    gettimeofday(&t1, 0);
    creation_time = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
    struct int_vector pre_arr_serial;
    read_preorder(root, 1, &pre_arr_serial, points.num);
    printf("Vantage-point tree created serial in %0.3fms\n", creation_time);
    if(VERBOSE)
    {
        printf("Preorder form: ");
        print_int_vector(&pre_arr_serial);
    }

    //finding all kneigbors and saving the idxs serial [n11,n12,...,n21,...n2k...,nkk]
    int k = MIN(points.num, 2);   //max number of neibhors for each point
    struct queue neibs;
    neibs.queue = (int*) malloc(sizeof(int) * k);
    int* total_neibs = (int*) malloc(sizeof(int) * k * points.num);
    gettimeofday(&tk0, 0);
    for(int i = 0; i < points.num; i++)
    {
        neibs.next_idx = 0;
        neibs.num = 0;
        neibs.tau = FLT_MAX;
        knn_search(root, &points, i, k, &neibs);
        memcpy(total_neibs + i * k, neibs.queue, sizeof(int)*k);
        print_knn(neibs.queue, k);
    }
    gettimeofday(&tk1, 0);
    free(neibs.queue);
    knn_time = (tk1.tv_sec - tk0.tv_sec) * 1000.0 + (tk1.tv_usec - tk0.tv_usec) / 1000.0;
    printf("K-NN task: %d nearest neighbors for each point calculated in %0.3fms\n", k,  knn_time);


    switch(args.mode)
    {
        case 0:
            save_times(args.mode, points.num, points.dim, creation_time, knn_time);
            break;
    }

    reallocate_tree(root);  //deallocation of serial created tree
    return 0;
}