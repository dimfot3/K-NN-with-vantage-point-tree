/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the declaration of Vantage point creation in OpenMP
 **/

/**
 * @brief this function create vp tree with recursive procedure using OpenMP
 * 
 * @param points  the structure for points
 * @param idxs  the idxs of points for current iteration
 * @param n the number of points for current iteration
 * @param parrent pointer to the parent node
 * @return return the root point for current iteration
 **/
struct vp_point* openmp_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent);
