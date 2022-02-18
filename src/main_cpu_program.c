/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here is the main program for knn implementation with vantage tree that runs on cpu exclusivly.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <serial_vp_tree.h>
#include <sys/time.h>
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
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
    struct vp_point *root = serial_vp_create(&points, idxs, points.num, NULL);
    gettimeofday(&t1, 0);
    double elapsed = (t1.tv_sec-t0.tv_sec)*1000.0 + (t1.tv_usec-t0.tv_usec)/1000.0;

    printf("Vantage-point tree created serial in %0.3fms\n", elapsed);

    struct int_vector pre_arr;
    read_preorder(root, 1, &pre_arr, points.num);

    if(VERBOSE)
    {
        printf("Preorder form: ");
        print_int_vector(&pre_arr);
    }
    reallocate_tree(root);
    return 0;
}