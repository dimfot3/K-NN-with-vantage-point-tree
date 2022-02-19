/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * @brief Here there is the implementation of basic utilities
 **/

#include <stdlib.h>
#include <stdio.h>
#include <utils.h>
#include <math.h>
#include <omp.h>

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
        printf( "Error loading the file with path %s\n",  path) ;
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

void print_point(float* p, int d)
{
    for(int i = 0; i < d; i++)
        printf("%f ", p[i]);
    printf("\n");
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
        sum += fabs(p1[i] - p2[i]);
    }
    return sum;
}

void swap(float* a, float* b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

float partition(float *arr, int l, int r)
{
    float lst = arr[r];
    int i = l, j = l;
    while (j < r) {
        if (arr[j] < lst) {
            swap(&arr[i], &arr[j]);
            i++;
        }
        j++;
    }
    swap(&arr[i], &arr[r]);
    return i;
}

float quickselect(float* arr, int left, int right, int k)
{
    float b = -1, a = -1;
    int n = right - left + 1;
    if(n == 0)
        return 0;
    float* temp_arr = malloc(sizeof(float) * n);
    memcpy(temp_arr, arr, n * sizeof(float));

    while (left <= right) {
        int pivotIndex = partition(temp_arr, left, right);
        if (pivotIndex == k)
            b = temp_arr[pivotIndex];
        else if(pivotIndex == k-1)
            a = temp_arr[pivotIndex];
        if (pivotIndex >= k)
            right = pivotIndex - 1;
        else
            left = pivotIndex + 1;
        if(b != -1 && a != -1)
            break;
    }
    free(temp_arr);
    //if the number of elements is odd return the k-1 element
    if(n % 2 == 1)
    {
        return a;
    }
    //if the number of elements is even return the mean of k-1 and k.
    return (b+a)/2;
}

float* calculateDistances(struct points_struct *points, float *pivot, int* idxs, int n)
{
    int d = points->dim;
    float* dist_arr = (float*)malloc(sizeof(float) * n);
    for(int i = 0; i < n; i++)
    {
        dist_arr[i] = calculate_euk_distance(pivot, &(points->points_arr[idxs[i] * d]), points->dim);
    }
    return dist_arr;
}

float* calculateDistancesParallel(struct points_struct *points, float *pivot, int* idxs, int n)
{
    int d = points->dim;
    float* dist_arr = (float*)malloc(sizeof(float) * n);
    #pragma omp parallel for
    for(int i = 0; i < n; i++)
    {
        dist_arr[i] = calculate_euk_distance(pivot, &(points->points_arr[idxs[i] * d]), points->dim);
    }
    return dist_arr;
}

void split_idxs(int* idxs, float* dists_arr, int n, float median, int **left_idxs, int **right_idxs, int *n_l, int *n_r)
{
    *right_idxs = (int*) malloc(sizeof(int) * n);
    *left_idxs = (int*) malloc(sizeof(int) * n);
    *n_l = 0;
    *n_r = 0;
    for(int i = 0; i < n; i++)
    {
        if(dists_arr[i] <= median)
            (*left_idxs)[(*n_l)++] = idxs[i];
        else
            (*right_idxs)[(*n_r)++] = idxs[i];
    }
    //reallocing extra memory space for left and right idxes
    *left_idxs = (int*) realloc(*left_idxs, sizeof(int) * *n_l);
    *right_idxs = (int*) realloc(*right_idxs, sizeof(int) * *n_r);
}

void read_preorder(struct vp_point *node, int root)
{
    if (node == NULL)
        return;
    if(root)
        printf("Preorder: ");
    printf("%d ", node->idx);
    read_preorder(node->left, 0);
    read_preorder(node->right, 0);
    //creates a new line after whole printing
    if(root)
        printf("\n");
}


void reallocate_tree(struct vp_point *node)
{
    if(node == NULL)
        return;
    reallocate_tree(node->left);
    reallocate_tree(node->right);
    free(node);
}
