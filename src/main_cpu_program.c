/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here is the main program for knn implementation with vantage tree that runs on cpu exclusivly.
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>

int main(int argc, char** argv)
{
    struct ses_args args;
    struct points_struct points;

    parse_arguments(argc, argv, &args);
    read_points(args.path, &points);

    print_points(&points);
    return 0;
}