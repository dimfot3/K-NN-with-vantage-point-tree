/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the implementation of Vantage point creation with OpenMP and serial execution
 **/

/**
 * @brief this function create vp tree parallel with recursive procedure using openMP or sequential code where needed
 * 
 * @param points  the structure for points
 * @param idxs  the idxs of points for current iteration
 * @param n the number of points for current iteration
 * @param parrent pointer to the parent node
 * @return return the root point for current iteration
 **/
struct vp_point* mixed_vp_create(struct points_struct* points, int* idxs,  int n, struct vp_point* parrent);
