/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here is the main program for knn implementation with vantage tree that runs on cpu exclusivly.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <serial_vp_tree.h>

int main(int argc, char** argv)
{
    struct ses_args args;
    struct points_struct points;

    parse_arguments(argc, argv, &args);
    read_points(args.path, &points);
    
    print_points(&points);

    int* idxs = malloc(sizeof(int)*points.num);
    for(int i = 0; i < points.num; i++)
        idxs[i] = i;
    struct vp_point *root = serial_vp_create(&points, idxs, points.num, NULL);
    read_preorder(root, 1);
    return 0;
}