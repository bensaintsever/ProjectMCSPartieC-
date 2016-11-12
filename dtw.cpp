/*******************************************************************************
 *
 * Drone control through voice recognition -- PC to drone communication
 * Team GYTAM, feb. 2016
 *
 *
 ******************************************************************************/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // for memcmp
#include <stdint.h> // for int16_t and int32_t
#include <math.h>
#include <iostream>
#include "dtw.h"
#include <cmath>

using namespace std;

int INF = 65535;

/**
* Dtw function that given two matrix of cep coefficient computes distance
* between those two signals.
*  @param n_ck      Dimension of know signal
*  @param n_cunk    Dimension of unknow signal
*  @param dim_mfcc  Size of nfcc decomposition base
*  @param c_k       Matrix of know signal
*  @param c_unk     Matrix of unknow signal
*  @return Distance between the two signals
*/

float dtw(int n_ck, int n_cunk, int dim_mfcc, float *c_k, float *c_unk) {


    int w0 = 1;
    int w1 = 1;
    int w2 = 1;

    int _I = n_ck + 1; //10
    int _J = n_cunk + 1; //7


    float g[_I * _J];
    g[0] = 0;

    //Premiere ligne a INF
    for (int j = 1; j < _I; ++j) {
        g[j * _J] = INF;
    }

    //TRAITEMENT COLONNE
    for (int i = 1; i < _J; ++i) {
        //Premier element Colonne a INF
        g[i] = INF;

        //TRAITEMENT PAR LIGNE
        for (int j = 1; j < _I; ++j) {
            int d = abs(c_unk[i - 1] - c_k[j - 1]);
            g[i + j * _J] = min(
                    min(g[(i - 1) + j * _J] + w0 * d, g[(i - 1) + (j - 1) * _J] + w1 * d),
                    g[i + (j - 1) * _J] + w2 * d);
        }
    }

    for (int i = 0; i < _J; i++) {

        for (int j = 0; j < _I; ++j) {
            cout << g[i + j * _J] << " ";
        }
        cout << endl;
    }

    return g[(_I * _J) - 1] / (n_ck + n_cunk);
}

