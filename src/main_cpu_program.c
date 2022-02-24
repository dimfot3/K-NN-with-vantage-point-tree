/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here is the main program for knn implementation with vantage tree
 **/

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <sys/time.h>
#include <utils.h>
#include <omp.h>
#include <mpi.h>
#include <serial_vp_tree.h>
#include <open_mp_tree.h>
#include <mixed_tree.h>
#include <search_knn.h>
#include <hybrid_mpi.h>
#include <Queue.h>


#define VERBOSE 0


int main(int argc, char** argv)
{
    struct ses_args args;
    struct points_struct points;

    parse_arguments(argc, argv, &args);
    read_points(args.path, &points, VERBOSE);
    
    int world_size, world_rank=0, name_len;

    //MPI basic operations
    MPI_Init(NULL, NULL);
    
    if(args.mode == 3)
    {
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        char processor_name[MPI_MAX_PROCESSOR_NAME];
        MPI_Get_processor_name(processor_name, &name_len);
        if(world_rank == 0)
            printf("Name of device: %s, Rank: %d, Total processors: %d \n", processor_name, world_rank, world_size); 
    }           
    struct timeval t0, t1, tk0, tk1;
    double creation_time, knn_time;
    struct int_vector pre_arr_serial;
    int* idxs;
    struct vp_point *root;
    //--------------------------------SEQUENTIAL SECTION----------------------------------------//
    //serial vp creation. When we run in mode 0 its time is saved, when we run in other mode this is used for validation
    if(world_rank == 0)
    {
        idxs = malloc(sizeof(int)*points.num);
        for(int i = 0; i < points.num; i++)
            idxs[i] = i;
        gettimeofday(&t0, 0);
        root = serial_vp_create(&points, idxs, points.num, NULL);
        gettimeofday(&t1, 0);
        creation_time = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
        read_preorder(root, 1, &pre_arr_serial, points.num);
        printf("Vantage-point tree created serial in %0.3fms\n", creation_time);
        if(VERBOSE)
        {
            printf("Preorder form: ");
            print_int_vector(&pre_arr_serial);
        }
    }
    //------------------------END OF SEQUENTIAL------------------------------------------

    /*
    //--------------------------------KNN SECTION----------------------------------------//
    //finding all kneigbors and saving the idxs serial [n11,n12,...,n21,...n2k...,nkk]
    int k = MIN(points.num, 256);   //max number of neibhors for each point
    int* total_neibs = (int*) malloc(sizeof(int) * k * points.num);
    gettimeofday(&tk0, 0);

    #pragma omp parallel for schedule(dynamic, 1)
    for(int i = 0; i < points.num; i++)
    {
        float tau = FLT_MAX;
        struct queue_node *queue_neibs = NULL;
        knn_search(root, &points, i, k, &tau, &queue_neibs);
        queue_to_arr(queue_neibs, total_neibs + i * k, k);
        free_queue(&queue_neibs);
    }
    gettimeofday(&tk1, 0);
    save_knn(total_neibs, k * points.num);
    knn_time = (tk1.tv_sec - tk0.tv_sec) * 1000.0 + (tk1.tv_usec - tk0.tv_usec) / 1000.0;
    printf("K-NN task: %d nearest neighbors for each point calculated in %0.3fms\n", k,  knn_time);
    //--------------------------------END OF KNN----------------------------------------//
    */
            
    switch(args.mode)
    {
        case 0:
            save_times(args.mode, points.num, points.dim, creation_time, knn_time, args.max_threads);
            break;
        case 1:
            //--------------------------------OPENMP----------------------------------------//
            idxs = (int*) malloc(sizeof(int)*points.num);
            for(int i = 0; i < points.num; i++)
                idxs[i] = i;
            gettimeofday(&t0, 0);
            struct vp_point *opmp_root = openmp_vp_create(&points, idxs, points.num, NULL);
            gettimeofday(&t1, 0);
            creation_time = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
            struct int_vector pre_arr_openmp;
            read_preorder(opmp_root, 1, &pre_arr_openmp, points.num);
            printf("Vantage-point tree created with OpenMP in %0.3fms\n", creation_time);
            int comp = compare_int_vectors(&pre_arr_openmp, &pre_arr_serial);
            if(comp == 1)
                printf("Parallel validation with sequential were successful.\n");
            reallocate_tree(opmp_root);  //deallocation of openmp created tree
            save_times(args.mode, points.num, points.dim, creation_time, knn_time, args.max_threads);
            break;
            //--------------------------------END OF OPENMP----------------------------------------//
        case 2:
            //--------------------------------MIXED VERSION (Serial and OpenMP)----------------------------------------//            
            omp_set_max_active_levels(args.max_threads);
            omp_set_nested(1);
            idxs = (int*) malloc(sizeof(int)*points.num);
            for(int i = 0; i < points.num; i++)
                idxs[i] = i;        
            gettimeofday(&t0, 0);
            struct vp_point *mixed_root = mixed_vp_create(&points, idxs,  points.num, NULL);
            gettimeofday(&t1, 0);
            creation_time = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
            struct int_vector pre_arr_mixed;
            read_preorder(mixed_root, 1, &pre_arr_mixed, points.num);
            printf("Vantage-point tree created with Mixed implementation(Serial/OpenMP with limited threads) in %0.3fms\n", creation_time);
            comp = compare_int_vectors(&pre_arr_mixed, &pre_arr_serial);
            if(comp == 1)
                printf("Mixed validation with sequential were successful.\n");
            reallocate_tree(mixed_root);  //deallocation of openmp created tree
            save_times(args.mode, points.num, points.dim, creation_time, knn_time, args.max_threads);
            break;
            //--------------------------------END OF MIXED VERSION (Serial and OpenMP)----------------------------------------//
        case 3:
            //--------------------------------MPI with OpenMP VERSION----------------------------------------//
            
            omp_set_max_active_levels(args.max_threads);
            omp_set_nested(1);

            idxs = malloc(sizeof(int)*points.num);
            for(int i = 0; i < points.num; i++)
                idxs[i] = i;  
              
            gettimeofday(&t0, 0);
            struct vp_point *mpi_root = hybrid_mpi_vp_create(&points, idxs, world_rank);
            MPI_Barrier(MPI_COMM_WORLD);  
            gettimeofday(&t1, 0);
            if(world_rank == 0)
            {
                creation_time = (t1.tv_sec - t0.tv_sec) * 1000.0 + (t1.tv_usec - t0.tv_usec) / 1000.0;
                //validation
                struct int_vector pre_arr_mpi;
                read_preorder(mpi_root, 1, &pre_arr_mpi, points.num);
                printf("Vantage-point tree created with Mixed implementation(Serial/OpenMP with limited threads) in %0.3fms\n", creation_time);
                comp = compare_int_vectors(&pre_arr_mpi, &pre_arr_serial);
                if(comp == 1)
                    printf("Hybrid mpi-limited openmp implementation were successful.\n");
                reallocate_tree(mpi_root);  //deallocation of openmp created tree
                save_times(args.mode, points.num, points.dim, creation_time, knn_time, args.max_threads);
            }
            MPI_Finalize();
            break;
            //--------------------------------END OF MPI with OpenMP VERSION----------------------------------------//
    }
    if(world_rank == 0)
        reallocate_tree(root);  //deallocation of serial created tree
    return 0;
}
