/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * Here are the basic utilities of this project
 **/

#ifndef _UTILS_H
#define _UTILS_H

/**
 * @brief this is the basic structure session arguments
 * 
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
 * @brief this functions reads from argv the arguments for the session
 * 
 * @param argc number of of arguments
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


#endif