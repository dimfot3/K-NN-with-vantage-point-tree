/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here are the basic utilities of this project
 **/

#ifndef _UTILS_H
#define _UTILS_H

/**
 * @brief this is the basic structure session arguments
 * @param path char pointer to data path
 * @param mode 0:sequential, 1:prallel in cpu, 2:fair work per thread
 **/
struct ses_args
{
    char *path;
    int mode;
};

/**
 * @brief this is the basic structure for n nd points
 * 
 * @param num  number of points
 * @param dim dimension of points
 * @param points_arr float arr that contains the points in contigius form
 **/
struct points_struct
{
    int num;
    int dim;
    float* points_arr;
};

/**
 * @brief this is the basic structure vantage point tree
 * 
 * @param left  pointer to parent node
 * @param left  pointer to left child (for points with distance less or equal than median)
 * @param right pointer to right child (for points with distance bigger than median)
 * @param idx the idx to original array
 * @param thesshold the median of left and right childs
 **/
struct vp_point
{
    struct vp_point *parent;
    struct vp_point *left;
    struct vp_point *right;
    int idx;
    int thresshold;
};

/**
 * @brief this is a C implementation of vector for int
 * 
 * @param arr  pointer array of integers
 * @param n  number of elements
 **/
struct int_vector
{
    int *arr;
    int n;
};

/**
 * @brief this functions reads from argv the arguments for the session
 * 
 * @param argc number of points of arguments
 * @param argv points to the program arguments
 * @param args pointer to structure for the session arguments
 **/
void parse_arguments(int argc, char** argv, struct ses_args *args);

/**
 * @brief this functions reads the points from a binary file
 * 
 * @param path path to binary file
 * @param points basic structure that hold the points
 * @return void
 **/
void read_points(char *path, struct points_struct *points, int verbose);


/**
 * @brief this functions prints the points
 * 
 * @param points basic structure that hold the points
 * @return void
 **/
void print_points(struct points_struct *points);

/**
 * @brief prints a specific point
 * 
 * @param p point pointer
 * @param d dimension of point
 * @return void
 **/
void print_point(float* p, int d);

/**
 * @brief this functions free the allocated memory
 * 
 * @param root the pointer to the root vp point
 **/
void free_vp_tree(struct vp_point *root);

/**
 * @brief calculates eucledian distance
 * 
 * @param p1 pointer to first point
 * @param p2 pointer to first point
 * @param d dimension of points
 * @return a float fistance between the two points
 **/
float calculate_euk_distance(float *p1, float *p2, int d);

/**
 * @brief calculates manhattan distance
 * 
 * @param p1 pointer to first point
 * @param p2 pointer to first point
 * @param d dimension of points
 * @return a float fistance between the two points
 **/
float calculate_man_distance(float *p1, float *p2, int d);

/**
 * @brief quick select that finds the kth bigger element in a float array
 * 
 * @param arr array to find the kth bigger element
 * @param left first index of the array
 * @param right last index of array
 * @param k kth bigger element to search
 * @return the kth bigger element
 */
float quickselect(float* arr, int left, int right, int k);

/**
 * @brief consider last element as pivot and moves all the smaller to left and greater to right
 * 
 * @param arr array to find the kth bigger element
 * @param left first index of the array
 * @param right last index of array
 * @return the final position of pivot
 */
float partition(float *arr, int l, int r);

/**
 * @brief swap two elements
 * 
 * @param a first element
 * @param b second element
 * @return void
 */
void swap(float* a, float* b);


/**
 * @brief calculate the distances from a vantage point
 * 
 * @param points data stracture where holds all the data
 * @param vp vantage point
 * @param idxs idxs of points to get the distances
 * @param n number of points
 * @return void
 */
float* calculateDistances(struct points_struct *points, float *pivot, int* idxs, int n);


/**
 * @brief split the idxs less and more than median
 * 
 * @param idxs pointer to idxs
 * @param dists_arr pointer to array of distances
 * @param n the number of distances
 * @param median median for which the split will be made
 * @param left_idxs pointer to array of the left idxs
 * @param rights pointer to array of the right idxs
 * @param n_l pointer to the length of left idxs
 * @param n_r pointer to the length of right idxs
 * @return void
 */
void split_idxs(int* idxs, float* dists_arr, int n, float median, int **left_idxs, int **right_idxs, int *n_l, int *n_r);

/**
 * @brief function that reads binary tree in preorder and saves them in int_vector struct
 * 
 * @param node pointer to the node of tree
 * @param root this is a c boolean 0:node, 1:root
 * @param pre_arr the struct where to save the preorder form
 * @param n the number of elements the struct must have
 * @return void
 */
void read_preorder(struct vp_point *node, int root, struct int_vector* pre_arr, int n);

/**
 * @brief function that compares two vectors
 * 
 * @param arr1 first array
 * @param arr2 second array
 * @return 0 if they are different else returns 1
 */
int compare_int_vectors(struct int_vector* arr1, struct int_vector* arr2);

/**
 * @brief function that prints an int vector
 * 
 * @param vec vecttor of intergers to print
 * @return void
 */
void print_int_vector(struct int_vector* vec);


/**
 * @brief this functions reallocates the tree
 * 
 * @param node an node of the tree
 * @return void
 */
void reallocate_tree(struct vp_point *node);

#endif