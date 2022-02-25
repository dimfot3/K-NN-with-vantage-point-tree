/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here there is the implementation of basic utilities
 **/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <utils.h>

void parse_arguments(int argc, char** argv, struct ses_args *args)
{
    
    if(argc < 3)
    {
        printf("Usage: `./main_cpu_program path mode num_thr` where path is the binay file with points and mode 0:sequential, 1:parallel in cpu, 2:mixed serial/parallel with limits 3: hybrid mpi/openmp and num_thr is the thread limit\n");
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
        printf("Sessios's mode (0:sequential, 1:parallel in cpu, 2:mixed serial/parallel with limits 3: hybrid mpi/openmp): %d\nPoints path: %s\n", args->mode, args->path);
        if(argc == 4)
            args->max_threads = atoi(argv[3]);
        else
            args->max_threads = -1;
        
    }
}

void read_points(char *path, struct points_struct *points, int verbose)
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
    
    points->num = (int) info[0];
    points->dim = (int) info[1];
    points->points_arr = (float*) malloc(sizeof(float*)*info[0]*info[1]);
    fread(points->points_arr, sizeof(float),info[0]*info[1], fp);
    fclose(fp);
    if(verbose)
    {   
        print_points(points);
        printf("Number of points: %1.0f\nDimension of points: %1.0f\n", info[0], info[1]);
    }
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
    if(n_l > 0)
        *left_idxs = (int*) realloc(*left_idxs, sizeof(int) * (*n_l));
    if(n_r > 0)
    *right_idxs = (int*) realloc(*right_idxs, sizeof(int) * (*n_r));
}

void read_preorder(struct vp_point *node, int root, struct int_vector* pre_arr, int n)
{
    if (node == NULL)
    {
        //this saves the null child. This makes it possible to make 1-1 represantation of vantage point tree
        pre_arr->arr[pre_arr->n] = -1;
        pre_arr->thres[(pre_arr->n)++] = -1;
        return;
    }
        
    if(root)
    {
        pre_arr->arr = (int*) malloc(sizeof(int) * n*n);
        pre_arr->thres = (float*) malloc(sizeof(float) * n*n);
        pre_arr->n = 0;
    }
    pre_arr->arr[pre_arr->n] = node->idx;
    pre_arr->thres[(pre_arr->n)++] = node->thresshold;
    read_preorder(node->left, 0, pre_arr, n);
    read_preorder(node->right, 0, pre_arr, n);
    //free unecessary space
    if(root)
    {
        pre_arr->arr = (int*) realloc(pre_arr->arr, sizeof(int) * pre_arr->n);
        pre_arr->thres = (float*) realloc(pre_arr->thres, sizeof(float) * pre_arr->n);
    }
}

void preorder_to_tree(struct int_vector* pre_arr, struct vp_point **node, int *idx)
{
    if(pre_arr->arr[*idx] == -1)
    {
        (*node) = NULL;
        return;
    }
    *node = (struct vp_point*) malloc(sizeof(struct vp_point));
    (*node)->idx = pre_arr->arr[*idx];
    (*node)->thresshold = pre_arr->thres[*idx];
    (*idx)++;
    preorder_to_tree(pre_arr, &((*node)->left), idx);
    (*idx)++;
    preorder_to_tree(pre_arr, &((*node)->right), idx);
}


void reallocate_tree(struct vp_point *node)
{
    if(node == NULL)
        return;
    reallocate_tree(node->left);
    reallocate_tree(node->right);
    free(node);
}

int compare_int_vectors(struct int_vector* arr1, struct int_vector* arr2)
{
    if(arr1->n != arr2->n)
    {
        printf("WARNING! The two vectors are have different dimensions.\n");
        return 0;
    }
    int same = 1;
    for(int i = 0; i < arr1->n; i++)
    {
        if(arr1->arr[i] != arr2->arr[i] || abs(arr1->thres[i] - arr2->thres[i]) > 0.01)     //check the idx and the thresshold(here accepts minor percision diffs)
        {
            same = 0;
            break;
        }
    }
    return same;
}

void print_int_vector(struct int_vector* vec)
{
    for(int i = 0; i < vec->n; i++)
    {
        printf("%d ", vec->arr[i]);
    }
    printf("\n");
}

void save_times(int mode, int num, int dim, double creation_time, double knn_time, int max_threads)
{
    FILE *fp;
    int file_exists = 0;
    if( access("results.csv", F_OK ) == 0 )
        file_exists = 1;
    fp = fopen("results.csv", "a+");
    if(file_exists!=1)
        fprintf(fp, "mode,num,dim,vp_creation_time,knn_search_time,max_threads\n");
    fprintf(fp, "%d,%d,%d,%lf,%lf,%d\n", mode, num, dim, creation_time, knn_time, max_threads);
    fclose(fp);
}

void save_knn(int *neibs, int n)
{
    FILE *fp;
    
    fp = fopen("neibs.txt", "w");
    for(int i=0; i < n; i++)
        fprintf(fp, "%d ", neibs[i]);
    fclose(fp);
}
