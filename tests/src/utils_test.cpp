/**
 * Authors: Fotiou Dimitrios(9650), Andreas Eleutheriadis(9649)
 * @brief Here there are Google tests to evaluate correctness of basic utilities
 **/

#include <gtest/gtest.h>

extern "C"
{
   #include "utils.h"
}

/**
 * @brief this tests the point loader with loading a known set of points. WARNING: If this tests output fail check the path to data!
 **/
TEST(load_points, t1) {
    struct points_struct points;
    char path[] = "../../tests/data/dt_2_10_3.dat";
    read_points(path, &points, 1);
    float gr_arr[] = {369.21, -454.55, -83.44, 
                    -1688.57, -853.15, 497.05, 
                    892.56, -564.89, 414.00, 
                    1401.83, 465.88, -375.56, 
                    1218.25, -39.79, 907.43, 
                    265.23, 280.28, -633.60,
                    -150.06, -493.86, 610.72, 
                    1204.43, -431.29, 359.60, 
                    -523.36, 1265.77, -124.49, 
                    1485.53, 1137.91, -1208.99 };
    
    bool valid = true;
    float tolerance = 0.01;
    if(points.num !=10 || points.dim != 3)
        valid = false;
    for(int i = 0; i < points.num*points.dim; i++)
    {
        if(abs(points.points_arr[i] - gr_arr[i]) > tolerance)
        {
            valid = false;
            break;
        }
            
    }
    ASSERT_EQ(valid, true);
};

/**
 * @brief this tests the manhattan distance between two points
 **/
TEST(distance, manhattan) {
    int d = 7;
    float p1[] = {2, -4, 2, 1, -3, 2, 2};
    float p2[] = {2, 5, 2, -1, 3, 3, 2};
    float dist = calculate_man_distance(p1, p2, d);
    EXPECT_FLOAT_EQ(dist, 18);
};

/**
 * @brief this tests the eucledian distance between two points
 **/
TEST(distance, eucledean) {
    int d = 7;
    float p1[] = {2, -4, 2, 1, -3, 2, 2};
    float p2[] = {2, 5, 2, -1, 3, 3, 2};
    float dist = calculate_euk_distance(p1, p2, d);
    EXPECT_FLOAT_EQ(dist, 11.04536102);
};

/**
 * @brief this test quick select when input is odd
 **/
TEST(quickselect, odd) {
    float arr[] = {2, -4, 2, 1, -3, 2, 5};
    float median = quickselect(arr, 0, 6, 4);
    EXPECT_FLOAT_EQ(median, 2);
};

/**
 * @brief this test quick select when input is even
 **/
TEST(quickselect, even) {
    float arr[] = {2, -4, 2.4, 1, -3, 2.4, 5, 6};
    float median = quickselect(arr, 0, 7, 4);
    EXPECT_FLOAT_EQ(median, 2.2);
};

/**
 * @brief this test tree to preorder transformation
 **/
TEST(tree_to_preorder, t1) {
    vp_point root = {NULL, NULL, NULL, 0, 10.5};
    vp_point left = {&root, NULL, NULL, 2, 3.1};
    vp_point right = {&root, NULL, NULL, 1, 2.71};
    vp_point left_left = {&left, NULL, NULL, 4, 5.3};
    vp_point right_left = {&right, NULL, NULL, 3, 22.1};
    root.left = &left;
    root.right = &right;
    root.left->left = &left_left;
    root.right->left = &right_left;
    struct int_vector prearr;
    read_preorder(&root, 1, &prearr, 5);
    int idx_truth[] = {0, 2, 4, -1, -1, -1, 1, 3, -1, -1, -1};
    float thrs_truth[] = {10.5, 3.1, 5.3, -1, -1, -1, 2.71, 22.1, -1, -1, -1};
    bool valid = true;
    for(int i = 0; i < prearr.n; i++)
    {
        if(idx_truth[i] != prearr.arr[i] || thrs_truth[i] != prearr.thres[i])
            valid = false;
    }

    ASSERT_EQ(valid, true);
};

/**
 * @brief this compares two preorder structs that are the same
 **/
TEST(compare_int_vectors, same) {
    struct int_vector v1, v2;
    int arr1[] = {1, 5, 3 , 1};
    int arr2[] = {1, 5, 3 , 1};
    float thess1[] = {1.22, 12.12, 44, 2.2};
    float thess2[] = {1.22, 12.12, 44, 2.2};
    v1.arr = (int*) &arr1;
    v1.thres = (float*) &thess1;
    v1.n = 4;
    v2.arr = (int*) &arr1;
    v2.thres = (float*) &thess2;
    v2.n = 4;
    int t = compare_int_vectors(&v1, &v2);   
    ASSERT_EQ(t, 1);
};

/**
 * @brief this compares two preorder structs that have different idxs  but same thresshold
 **/
TEST(compare_int_vectors, diff_idx) {
    struct int_vector v1, v2;
    int arr1[] = {1, 5, 3 , 1};
    int arr2[] = {1, 2, 3 , 1};
    float thess1[] = {1.22, 12.12, 44, 2.2};
    float thess2[] = {1.22, 12.12, 44, 2.2};
    v1.arr = (int*) arr1;
    v1.thres = (float*) thess1;
    v1.n = 4;
    v2.arr = (int*) arr2;
    v2.thres = (float*) thess2;
    v2.n = 4;
    int t = compare_int_vectors(&v1, &v2);   
    ASSERT_EQ(t, 0);
};

/**
 * @brief this compares two preorder structs that have different thresshold but same idxs
 **/
TEST(compare_int_vectors, diff_thress) {
    struct int_vector v1, v2;
    int arr1[] = {1, 5, 3 , 1};
    int arr2[] = {1, 5, 3 , 1};
    float thess1[] = {1.22, 12.12, 44, 2.2};
    float thess2[] = {2.22, 12.12, 44, 2.2};
    v1.arr = (int*) arr1;
    v1.thres = (float*) thess1;
    v1.n = 4;
    v2.arr = (int*) arr2;
    v2.thres = (float*) thess2;
    v2.n = 4;
    int t = compare_int_vectors(&v1, &v2);   
    ASSERT_EQ(t, 0);
};

/**
 * @brief this tests the preorder to tree transformation. It relies on preorder and comparison correctness!
 **/
TEST(preorder_to_tree, t1) {
    struct int_vector v, gr_v;
    int arr[] = {0, 5, 1, -1, -1, 3, -1, -1, 4, -1, 2, -1, -1};
    float thess[] = {1.54, 23.4, 22.2, -1, -1, 21, -1, -1, 52.5, -1, 12, -1, -1};
    v.arr = (int*) arr;
    v.thres = (float*) thess;
    v.n = 13;
    vp_point *root;
    int idx = 0;
    preorder_to_tree(&v, &root, &idx);
    read_preorder(root, 1, &gr_v, 6);
    int t = compare_int_vectors(&v, &gr_v);
    ASSERT_EQ(t, 1);
};


/**
 * @brief this tests the split idxs used in all sequential or parallel implementations
 **/
TEST(split_idxs, t1) {
    int idxs[] = {1, 13, 32, 611, 7, 4};
    float dists[] = {15.3, 23.2, 21.1, 64.2, 1.23, 500.2};
    int* left, *right;
    int n_l, n_r;
    split_idxs(idxs, dists, 6, 23.2, &left, &right, &n_l, &n_r);
    bool valid = true;
    //check number of elements
    if(n_l != 4 || n_r != 2)
        valid = false;
    
    int gr_left[] = {1, 13, 32, 7};
    int gr_right[] = {611, 4};

    //check left elements
    for(int i = 0; i < n_l; i++)
        if(gr_left[i] != left[i])
            valid = false;

    //check left elements
    for(int i = 0; i < n_r; i++)
        if(gr_right[i] != right[i])
            valid = false;
    
    ASSERT_EQ(valid, true);
};


/**
 * @brief this tests the calculate distances used in sequential, mixed and hybrid mpi implementation. We test with a precision of 0.001.
 **/
TEST(calculate_distances, t1) {
    bool valid = true;
    struct points_struct points;
    float arr[] = {1.3, 1.4, 2.1, 6.5, 3.6, 9.5, 3.4, 3.4, 5.3};
    points.num = 3;
    points.dim = 3;
    points.points_arr = (float*) arr;
    int idxs[] = {1, 2};
    float ref_point[] = {1.4, 2.1, 8.7};
    float* dist = calculateDistances(&points, ref_point, idxs, 2);
    float gr_dist[] = {5.375872002, 4.153311931};

    for(int i = 0; i < 2; i++)
        if(abs(gr_dist[i] - dist[i]) > 0.001)
           valid = false;
    
    ASSERT_EQ(valid, true);
};



int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}