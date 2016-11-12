#include <iostream>
#include "dtw.h"

int main() {


    float sequence1[9];
    float sequence2[6];

    sequence1[0] = -2.0;
    sequence1[1] = 10.0;
    sequence1[2] = -10.0;
    sequence1[3] = 15.0;
    sequence1[4] = -13.0;
    sequence1[5] = 20.0;
    sequence1[6] = -5.0;
    sequence1[7] = 14.0;
    sequence1[8] = 2.0;


    sequence2[0] = 3.0;
    sequence2[1] = -13.0;
    sequence2[2] = 14.0;
    sequence2[3] = -7.0;
    sequence2[4] = 9.0;
    sequence2[5] = -2.0;



    std::cout << dtw(9,6,0,sequence1,sequence2) << std::endl;

    return 0;
}