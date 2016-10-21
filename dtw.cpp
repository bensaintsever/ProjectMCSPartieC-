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
*  @param n_ck      Dimension of unknow signal
*  @param n_cunk    Dimension of know signal
*  @param dim_mfcc  Size of nfcc decompostion base
*  @param c_k       Matrix of know signal
*  @param c_unk     Matrix of unknow signal
*  @return Distance between the two signals
*/

float dtw(int n_ck, int n_cunk, int dim_mfcc, float* c_k, float* c_unk) {

    //d vaut quoi ici ? dim_mffcc?

    int w0 = 1;
    int w2 = 1;
    int w3 = 1;



    vector<int> I;//A Initialiser (c_k)
    vector<int> J;//A Initialiser (c_unk)



    vector<int><int> g;
    g[0][0] = INF;


    for(int j = 1; j < J.size(); ++j){
        g[0][j] = INF;
    }


    for(int i = 1; j < I.size(); ++j){
        g[i][0] = INF;

        for(int j = 1; j < J.size(); ++j){
            g[i,j] = min(g[i-1][j]+w0*d, g[i-1][j-1]+w1*d, g[i][j-1]+w2*d);
        }
    }
    return g[I][J]/(I+J);


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
