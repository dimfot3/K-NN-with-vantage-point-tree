/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
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
 * @param left  pointer to left child (for points with distance less or equal than median)
 * @param right pointer to right child (for points with distance bigger than median)
 * @param idx the idx to original array
 * @param thesshold the median of left and right childs
 **/
struct vp_point
{
    struct vp_point *left;
    struct vp_point *right;
    int idx;
    int thresshold;
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
void read_points(char *path, struct points_struct *points);


/**
 * @brief this functions prints the points
 * 
 * @param points basic structure that hold the points
 **/
void print_points(struct points_struct *points);

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

#endif