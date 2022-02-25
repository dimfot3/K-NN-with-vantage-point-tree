/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the implementation of Vantage point creation with hybrid parallel and serial execution
 **/

/**
 * @brief this function create vp tree parallel with recursive procedure and using of both openmp and mpi
 * 
 * @param points  the structure for points
 * @param idxs  the idxs of points for current iteration
 * @param n the number of points for current itteration
 * @param id the mpi id
 * @return return the root point for current itteration
 **/
struct vp_point* hybrid_mpi_vp_create(struct points_struct* points, int* idxs, int id);
