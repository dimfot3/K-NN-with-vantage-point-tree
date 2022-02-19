/**
 * Author: Andreas Efstathios Eleftheriadis
 * AEM: 9649
 * @brief Here are the declaration of parallel vp tree creating utilities
 **/


/**
 * @brief this function create vp tree parallel with recursive procedure
 * 
 * @param points  the structure for points
 * @param idxs  the idxs of points for current iteration
 * @param n the number of points for current itteration
 * @return return the root point for current itteration
 **/
struct vp_point* parallel_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent);
