//
// Created by Benjamin Saint-Sever on 21/10/2016.
//

#ifndef PROJETDTW_CALCULDISTANCEDTW_H
#define PROJETDTW_CALCULDISTANCEDTW_H
#include <vector>
using namespace std;

class dtw {
    dtw();
    void calculDistanceDtw(vector<int> sequence1, vector<int> sequence2, int dDiagonale);
};


#endif //PROJETDTW_CALCULDISTANCEDTW_H
