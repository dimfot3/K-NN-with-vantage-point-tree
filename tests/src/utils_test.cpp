/**
 * Author: Fotiou Dimitrios
 * AEM: 9650
 * Here there are tests to evaluate correctness of basic utilities
 **/

#include <gtest/gtest.h>

extern "C"
{
   #include "utils.h"
}

/**
 * @brief this tests the point loader with loading a known set of points
 **/
TEST(load_points, t1) {
    struct points_struct points;
    char path[] = "../../data/dt_2_10_3.dat";
    read_points(path, &points);
        float gr_arr[] = {-241.9691,     139.0561,     268.2338,   -1228.0676,    -101.13516,
                            -664.4158,   -2442.0344,     -96.27045,   -484.76395,    408.71097,
                            -2087.13 ,      849.50684,  -986.0619 ,   2019.4124,    -680.8635,
                            -822.56335,   -394.05826,    935.4332,   -1435.3661,     910.64056,
                            2564.1982,    -922.347,     1724.9014,     113.36945,   -481.6732,
                            -31.877008,  -624.2047,     440.09164,   -151.25363,   -995.9362};
    
    bool valid = false;
    float tolerance = 0.000001;
    for(int i = 0; i < points.num*points.dim; i++)
    {
        valid = true;
        if(abs(points.points_arr[i] - gr_arr[i]) > tolerance)
        {
            valid = false;
            break;
        }
            
    }
    ASSERT_EQ(valid, true);
};

TEST(distance, manhattan) {
    int d = 7;
    float p1[] = {2, -4, 2, 1, -3, 2, 2};
    float p2[] = {2, 5, 2, -1, 3, 3, 2};
    float dist = calculate_man_distance(p1, p2, d);
    EXPECT_FLOAT_EQ(dist, 18);
     
};

TEST(distance, eucledean) {
    int d = 7;
    float p1[] = {2, -4, 2, 1, -3, 2, 2};
    float p2[] = {2, 5, 2, -1, 3, 3, 2};
    float dist = calculate_euk_distance(p1, p2, d);
    EXPECT_FLOAT_EQ(dist, 11.04536102);
     
};

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}