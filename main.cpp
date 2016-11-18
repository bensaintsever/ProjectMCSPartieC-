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

int nbmots = 13;
string vocabulaire[13] = {"arretetoi", "atterrissage", "avance", "decollage", "droite", "etatdurgence", "faisunflip",
                          "gauche", "plusbas", "plushaut", "recule", "tournedroite", "tournegauche"};


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

    cout << to_string(res) <<endl;


    /********************************************
    ***********  Reconnaisance vocale  **********
    ********************************************/

    //FILE **refFILE;
    wavfile header_fichier[nbmots];
    struct parametrisation ref[1]; //1 le nombre de locuteur de ref
    string locuteur = "M01";

    /** Fichiers de reférences **/
    for (int mot = 0; mot < nbmots; ++mot) {
        string nomfichier = chemin + locuteur + "_" + vocabulaire[mot] + ".wav";
        char *c_nomfichier = new char[nomfichier.length() + 1];
        strcpy(c_nomfichier, nomfichier.c_str());

        FILE *refFILE;


        /** LECTURE **/
        wavRead(&refFILE, c_nomfichier, &header_fichier[mot]);


        /* Deplacement du curseur après l'entete wav, directement sur les données (44 octets d'entete) */
        //fseek(refFILE, SEEK_SET, sizeof(struct wavfile));


        int nbreOctetsAudio = header_fichier[mot].bytes_in_data;
        int nbreOctetsRestant = nbreOctetsAudio;
        int16_t *buffer = new int16_t[nbreOctetsAudio + sizeof(struct wavfile)];
        int16_t donnee[nbreOctetsAudio];
        int indice = 0;



        if (mot == 0) {
            //FILE* fichierALire;
            FILE* fichierAEcrire;
            fichierAEcrire = fopen(
                    "/Users/benjamin.saint-sever/Documents/Education/Master 1.2/Semestre 1/MCS/ProjectMCSPartieC-/test",
                    "w");
            if (fichierAEcrire == NULL) {
                cerr<<"pb allocation"<<endl;
            }
            /*fichierALire = fopen(c_nomfichier, "rb+");
            if (fichierALire == NULL) {
                cerr<<"pb allocation"<<endl;
            }*/

            /** LECTURE ET COPIE DES DONNEES AUDIO **/
            while (int nblu = fread(buffer, sizeof(int16_t), 1, refFILE) > 0 && (nbreOctetsRestant > 0)) {
                fwrite(buffer,sizeof(int16_t),nblu,fichierAEcrire);


                donnee[indice] = buffer[indice];

                indice++;
                nbreOctetsRestant--;
            }


            fclose(fichierAEcrire);
            //fclose(fichierALire);
        }


      /*  if (mot > 0) {
        /** LECTURE ET COPIE DES DONNEES AUDIO **/
      /*  while (fread(buffer, sizeof(int16_t), 1, refFILE) > 0 && (nbreOctetsRestant > 0)) {


            //donnee[indice] = buffer[indice];

            indice++;
            nbreOctetsRestant--;
        }
        cout << buffer << endl;
    }*/

        /** REMOVE SILENCE **/

       /* int16_t *signalSansSilence;
        int taille_signal;
        float threshold = 1 / 100; //Sensibilité de détection du silence
        removeSilence(donnee, nbreOctetsAudio, &signalSansSilence, &taille_signal, threshold);



        /** PARAMETRISATION **/

        /* IMPLEMENTATION BASE SUR UNE SOURCE */
      /*  computeMFCC(&ref[0].X_mfcc, &ref[0].length_xmfcc, signalSansSilence, nbreOctetsAudio,
                    header_fichier[mot].frequency,
                    512, 256, 13, 26);
*/

        delete[] c_nomfichier;

    }


    int nbLocuteur = 12;
    string hypotheses[nbLocuteur] = {"M02", "M03", "M04", "M05", "M06", "M07", "M08", "M09", "M10", "M11", "M12",
                                     "M13"};

    int** matriceConfusion;
    int nbMotsRec = 0;

    struct parametrisation paramHyp[nbLocuteur];
    for (int noLocuteur = 0; noLocuteur < nbLocuteur; ++noLocuteur) {

        locuteur = hypotheses[noLocuteur];
        for (int mot = 1; mot < nbmots; ++mot) {

            FILE **ref_FILE[nbmots];
            wavfile *head_file[nbmots];

            /** Fichiers de reférences **/
            for (int mot = 0; mot < nbmots; ++mot) {
                string nomfichier = chemin + locuteur + "_" + vocabulaire[mot] + ".wav";
                char *c_nomfichier = new char[nomfichier.length() + 1];
                strcpy(c_nomfichier, nomfichier.c_str());

                /** LECTURE **/
                wavRead(ref_FILE[mot], c_nomfichier, head_file[mot]);


                /* Deplacement du curseur après l'entete wav, directement sur les données (44 octets d'entete) */
                fseek(*ref_FILE[mot], SEEK_SET, sizeof(struct wavfile));


                int nbreOctetsAudio = head_file[mot]->bytes_in_data;
                int nbreOctetsRestant = nbreOctetsAudio;
                char *buffer = new char[nbreOctetsAudio];
                int16_t donnee[nbreOctetsAudio];
                int indice = 0;

                /** LECTURE ET COPIE DES DONNEES AUDIO **/
                while (fread(buffer, sizeof(int16_t), 1, *ref_FILE[mot]) > 0 && (nbreOctetsRestant > 0)) {
                    donnee[indice] = buffer[indice];
                    indice++;
                    nbreOctetsRestant--;
                }

                /** REMOVE SILENCE **/

                int16_t *signalSansSilence;
                int taille_signal;
                float threshold = 1 / 100; //Sensibilité de détection du silence
                removeSilence(donnee, nbreOctetsAudio, &signalSansSilence, &taille_signal, threshold);



                /** PARAMETRISATION **/

                /* IMPLEMENTATION BASE SUR UNE SOURCE */
                computeMFCC(&paramHyp[mot].X_mfcc, &paramHyp[mot].length_xmfcc, signalSansSilence, nbreOctetsAudio,
                            head_file[mot]->frequency,
                            512, 256, 13, 26);


                delete[] c_nomfichier;

            }


        }


        for (int motRef = 0; motRef < nbmots; ++motRef) {
            //Dmots = zeros(1,nbmots); ?
            float *dMots = new float[nbmots];
            for (int motHyp = 0; motHyp < nbmots; ++motHyp) {
                float distance = dtw(paramHyp[motHyp].length_xmfcc, ref[motRef].length_xmfcc, 0,
                                     paramHyp[motHyp].X_mfcc, ref[motRef].X_mfcc);
                dMots[motHyp] = distance;
            }

            //A la recherche du min de dMots
            float motMin = 0.0;
            int motRec = 0;
            for(int i = 0; i < nbmots-1; ++i ){
                if (dMots[i] < motMin){
                    motMin = dMots[i];
                    motRec = i;
                }
            }
            matriceConfusion[motRef][motRec] = matriceConfusion[motRef][motRec]+1;
            if(motRef == motRec)
                nbMotsRec = nbMotsRec+1;


        }


    }

    /** AFFICHAGE MATRICE DE CONFUSION **/
    for(int i = 0; i < sizeof(matriceConfusion); ++i){
        for(int j = 0; j < sizeof(matriceConfusion[i]); ++j)
            cout<< matriceConfusion[i][j]<<endl;
    }

    return 0;
}