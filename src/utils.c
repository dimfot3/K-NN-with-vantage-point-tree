/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here there is the implementation of basic utilities
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <math.h>

void parse_arguments(int argc, char** argv, struct ses_args *args)
{
    
    if(argc < 3)
    {
        printf("Usage: `./main_cpu_program path mode` where path is the binay file with points and mode 0:sequential, 1:prallel in cpu, 2:fair work per thread\n");
        printf("WARNING! You have not given arguments, so default values path=../data/dt_2_10_3.dat, mode=0 will be used.\n\n");
        args->path = (char*) malloc(sizeof(char)*50);
        strcpy(args->path, "../data/dt_2_10_3.dat");
        args->mode = 0;
    }   
    else
    {
        args->path = (char*) malloc(sizeof(char)*50);
        strcpy(args->path, argv[1]);
        args->mode = atoi(argv[2]);
        printf("Sessios's mode (0:sequential, 1:prallel in cpu, 2:fair work per thread): %d\nPoints path: %s\n", args->mode, args->path);
    }
}

void read_points(char *path, struct points_struct *points)
{
    //opten a file that containes the points
    FILE *fp;
    float info[2];
    fp = fopen(path, "rb");
    if ( fp == NULL )
    {
        printf( "Error loading the file with paht %s\n",  path) ;
        exit(0);
    }
    fread(info,sizeof(float),2,fp); 
    printf("Number of points: %1.0f\nDimension of points: %1.0f\n", info[0], info[1]);
    points->num = (int) info[0];
    points->dim = (int) info[1];
    points->points_arr = (float*) malloc(sizeof(float*)*info[0]*info[1]);
    fread(points->points_arr, sizeof(float),info[0]*info[1], fp);
    fclose(fp);
}

void print_points(struct points_struct *points)
{
    int d = points->dim;
    printf("-------------------------- Points --------------------------\n");
    for(int i = 0; i < points->num; i++)
    {
        for(int j = 0; j < points->dim; j++)
            printf("%3.2f ", points->points_arr[i * d + j]);
        printf("\n");
    }   
    printf("-----------------------------------------------------------\n");
}

float calculate_euk_distance(float *p1, float *p2, int d)
{
    double sum = 0;
    for(int i = 0; i < d; i++)
    {
        sum += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(sum);
}

float calculate_man_distance(float *p1, float *p2, int d)
{
    double sum = 0;
    for(int i = 0; i < d; i++)
    {
        sum += abs(p1[i] - p2[i]);
    }
    return sum;
}