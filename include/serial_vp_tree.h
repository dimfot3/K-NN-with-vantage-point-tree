/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here are the declaration of serial vp tree creating utilities
 **/


/**
 * @brief this function create vp tree serial with recursive procedure
 * 
 * @param points  the structure for points
 * @param idxs  the idxs of points for current iteration
 * @param n the number of points for current iteration
 * @return return the root point for current iteration
 **/
struct vp_point* serial_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent);
