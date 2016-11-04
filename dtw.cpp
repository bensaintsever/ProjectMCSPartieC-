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
#include <vecto>
#include <cmath>
#include <cfloat>
using namespace std;

typedef int INF = 65535;

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

float dtw(int n_ck, int n_cunk, int dim_mfcc, float* c_k, float* c_unk) {


    int w0 = 1;
    int w2 = 1;
    int w3 = 1;

    int _I = n_ck+1;
    int _J = n_cunk+1;


    float g[_I * _J];
    g[0] = 0;


    //Premiere colonne a INF
    for(int j = 1; j < _J; ++j){
        g[j*n_ck] = INF;
    }


    for(int i = 1; i < _I; ++i){
        //Premiere ligne a INF
        g[i] = INF;

        for(int j = 1; j < _J; ++j){
            int d = abs(c_k[i-1], c_unk[j-1]);

            g[i+j*_I] = min(g[(i-1)+j * n_ck] + w0 * d,
                              g[(i-1)+(j-1) * n_ck] + w1 * d,
                         g[i + (j-1) * n_ck] + w2 * d);
        }
    }

    return g[(_I*_J)-1]/(n_ck + n_cunk);

/* Code matlab


    I = size(sequence1,2);
    J = size(sequence2,2);

    g = zeros(I+1, J+1);
    chemins = zeros(I, J);
    if dDiagonale < abs(I-J)+1,
        dDiagonale = abs(I-J)+1;
        'Erreur : dDiagonale trop petite'
    end
    %g(1,1) = 0
    for j=2:J+1,
        g(1,j) = +inf;
    end
    for i=2:I+1,
        g(i,1) = +inf;
        for j=2:J+1,
            if j-i < dDiagonale && i-j < dDiagonale,
                d = feval(distance, sequence1, sequence2, i-1, j-1);
                g(i,j) = min([g(i-1,j)+w0*d , g(i-1,j-1)+w1*d , g(i,j-1)+w2*d]);
            else
                g(i,j) = +inf;
            end

            %marquage de +1000 si on vient du haut, -1000 si on vient de
            %la gauche
            if g(i,j) == g(i-1,j-1)+w1*d,
                chemins(i-1,j-1) = 1;
            else
                if g(i,j) == g(i, j-1)+w2*d,
                    chemins(i-1,j-1) = 2;
                else
                    if g(i,j) == g(i-1, j) + w0 * d,
                        chemins(i-1,j-1) = 0;
                    end
                end
            end
        end
    end
    %g
    %AfficheChemin(chemins, g)
    D=g(I+1,J+1)/(I+J);
end
        */

    
    
}
