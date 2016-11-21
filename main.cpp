#include <iostream>
#include "dtw.h"
#include "WavToMfcc.h"
#include "FFTReal.h"
#include "libmfccOptim.h"
#include <string>
#include <cstring>
#include <stdlib.h>


using namespace std;

string chemin = "/Users/benjamin.saint-sever/Documents/Education/Master 1.2/Semestre 1/MCS/ProjectMCSPartieC-/corpus/dronevolant_nonbruite/";

/*int nbmots = 13;
string vocabulaire[13] = {"arretetoi", "atterrissage", "avance", "decollage", "droite", "etatdurgence", "faisunflip",
                          "gauche", "plusbas", "plushaut", "recule", "tournedroite", "tournegauche"};
*/
int nbmots = 2;
string vocabulaire[2] = {"arretetoi", "atterrissage"/*, "avance", "decollage", "droite", "etatdurgence", "faisunflip",
                          "gauche", "plusbas", "plushaut", "recule", "tournedroite", "tournegauche"*/ };



/** VARIABLES PARAMETRISATION **/
struct parametrisation {
    float *X_mfcc;
    int length_xmfcc;
};


int main() {

    /*******************************************
     ***************  TEST DTW  ****************
     *******************************************/
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

    float res = dtw(9, 6, 0, sequence1, sequence2);

    cout << "distance :"<<to_string(res) << endl;


    /********************************************
    ***********  Reconnaisance vocale  **********
    ********************************************/


    wavfile header_fichier[nbmots];
    struct parametrisation ref[nbmots];
    string locuteur = "M01";

    /** Fichiers de reférences **/
    for (int mot = 0; mot < nbmots; ++mot) {
        string nomfichier = chemin + locuteur + "_" + vocabulaire[mot] + ".wav";
        char *c_nomfichier = new char[nomfichier.length() + 1];
        strcpy(c_nomfichier, nomfichier.c_str());

        FILE *refFILE;


        /** LECTURE **/
        wavRead(&refFILE, c_nomfichier, &header_fichier[mot]);


        int nbreOctetsAudio = header_fichier[mot].bytes_in_data;
        int16_t *buffer = new int16_t[nbreOctetsAudio];

        fread(buffer, sizeof(int16_t), nbreOctetsAudio, refFILE);
        fclose(refFILE);

        /** REMOVE SILENCE **/

        int16_t *signalSansSilence;
        int taille_signal;
        float threshold = 10 / 100; //Sensibilité de détection du silence
        removeSilence(buffer, nbreOctetsAudio, &signalSansSilence, &taille_signal, threshold);


        /** PARAMETRISATION **/

        /* IMPLEMENTATION BASE SUR UNE SOURCE */
        computeMFCC(&ref[mot].X_mfcc, &ref[mot].length_xmfcc, signalSansSilence, nbreOctetsAudio,
                    header_fichier[mot].frequency,
                    512, 256, 13, 26);


        delete[] c_nomfichier;

    }

    int nbLocuteur = 1;
    string hypotheses[nbLocuteur] = {"M01"/*, "M03", "M04", "M05", "M06", "M07", "M08", "M09", "M10", "M11", "M12",
                                     "M13"*/};

    /***** INIT MATRICE CONFUSION *****/
    int **matriceConfusion = new int *[nbmots];
    for (int i = 0; i < nbmots; ++i)
        matriceConfusion[i] = new int[nbmots];


    for (int i = 0; i < nbmots; i++)
        for (int j = 0; j < nbmots; j++)
            matriceConfusion[i][j] = 0;
    /**********************************/

    int nbMotsRec = 0;
    for (int noLocuteur = 0; noLocuteur < nbLocuteur; ++noLocuteur) {

        struct parametrisation paramHyp[nbmots];
        locuteur = hypotheses[noLocuteur];


        wavfile head_file[nbmots];

        /** Fichiers de reférences **/
        for (int mot = 0; mot < nbmots; ++mot) {
            FILE *ref_FILE;
            string nomfichier = chemin + locuteur + "_" + vocabulaire[mot] + ".wav";
            char *c_nomfichier = new char[nomfichier.length() + 1];
            strcpy(c_nomfichier, nomfichier.c_str());

            /** LECTURE **/
            wavRead(&ref_FILE, c_nomfichier, &head_file[mot]);


            int nbreOctetsAudio = head_file[mot].bytes_in_data;
            int16_t *buffer = new int16_t[nbreOctetsAudio];

            /** LECTURE ET COPIE DES DONNEES AUDIO **/
            fread(buffer, sizeof(int16_t), nbreOctetsAudio, ref_FILE);
            fclose(ref_FILE);

            /** REMOVE SILENCE **/

            int16_t *signalSansSilence;
            int taille_signal;
            float threshold = 1 / 100; //Sensibilité de détection du silence
            removeSilence(buffer, nbreOctetsAudio, &signalSansSilence, &taille_signal, threshold);



            /** PARAMETRISATION **/

            /* IMPLEMENTATION BASE SUR UNE SOURCE */
            computeMFCC(&paramHyp[mot].X_mfcc, &paramHyp[mot].length_xmfcc, signalSansSilence, nbreOctetsAudio,
                        head_file[mot].frequency,
                        512, 256, 13, 26);

            //cout << "taille :" <<paramHyp[mot].length_xmfcc<<endl;

            delete[] c_nomfichier;

        }


        for (int motRef = 0; motRef < nbmots; ++motRef) {

            //Dmots = zeros(1,nbmots); ?
            float *dMots = new float[nbmots];
            for (int motHyp = 0; motHyp < nbmots; ++motHyp) {
                float distance = dtw(paramHyp[motHyp].length_xmfcc, ref[motRef].length_xmfcc, 13,
                                     paramHyp[motHyp].X_mfcc, ref[motRef].X_mfcc);
                dMots[motHyp] = distance;

                cout<<"Mot ref N°" << motRef<<endl;
                cout<<"Mot hyp N°:"<<motHyp<<endl<<"distance entre eux :"<<dMots[motHyp]<<endl;
            }

            //A la recherche du min de dMots
            float motMin = dMots[0];
            int motRec = 0;
            for (int i = 1; i < nbmots; ++i) {
                if (dMots[i] < motMin) {
                    motMin = dMots[i];
                    motRec = i;
                }
            }
            matriceConfusion[motRef][motRec] = matriceConfusion[motRef][motRec] + 1;
            if (motRef == motRec)
                nbMotsRec = nbMotsRec + 1;


        }



    }

    /** AFFICHAGE MATRICE DE CONFUSION **/
   /* for (int j = 0; j < nbmots; ++j) {
        for (int i = 0; i < nbmots; ++i)
            cout << matriceConfusion[i][j] << " ";
        cout<<""<<endl;
    }
    float tauxReco = (nbMotsRec/nbmots)/nbLocuteur;
    cout<<"Taux de reconnaissance : " <<tauxReco*100<< "%"<<endl;
    */

    return 0;
}