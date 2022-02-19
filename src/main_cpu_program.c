/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here is the main program for knn implementation with vantage tree that runs on cpu exclusivly.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <utils.h>
#include <serial_vp_tree.h>
#include <parallel_vp_tree.h>

int main(int argc, char** argv)
{
    struct ses_args args;
    struct points_struct points;

    struct timeval startwtime, endwtime;
    double seq_time;

    parse_arguments(argc, argv, &args);
    read_points(args.path, &points);

    print_points(&points);

    int* idxs = malloc(sizeof(int)*points.num);
    for(int i = 0; i < points.num; i++)
        idxs[i] = i;

    gettimeofday (&startwtime, NULL);
    //struct vp_point *root = serial_vp_create(&points, idxs, points.num, NULL);
    struct vp_point *root = parallel_vp_create(&points, idxs, points.num, NULL);
    gettimeofday (&endwtime, NULL);
    read_preorder(root, 1);

    reallocate_tree(root);
    seq_time = (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6
  		      + endwtime.tv_sec - startwtime.tv_sec);
    printf("Finished in %f seconds\n", seq_time);
    return 0;
}
