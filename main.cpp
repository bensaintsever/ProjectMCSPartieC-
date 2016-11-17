#include <iostream>
#include "dtw.h"
#include "WavToMfcc.h"
#include <string>
#include <cstring>
#include <stdlib.h>


using namespace std;

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

    float res = dtw(9, 6, 0, sequence1, sequence2);

    std::cout << std::to_string(res) << std::endl;


    /*Seconde partie*/

    string chemin = "/corpus/dronevolant_nonbruite/";
    int nbmots = 13;
    string vocabulaire[nbmots]={"arretetoi", "atterrissage", "avance", "decollage", "droite", "etatdurgence", "faisunflip", "gauche", "plusbas", "plushaut", "recule", "tournedroite", "tournegauche"};

    string locuteur = "M01";
    string nomfichier;


    FILE** refFILE [nbmots];
    wavfile* header_fichier[nbmots];

    char* c_mfcName[nbmots];

    /* VARIABLES PARAMETRISATION */
    struct parametrisation{
        float* X_mfcc;
        int length_xmfcc;
    };
    struct parametrisation par;


    for (int mot = 0; mot < nbmots; ++mot){
        nomfichier = chemin + locuteur + "_" + vocabulaire[mot] + ".wav";
        char* c_nomfichier = new char[nomfichier.length() + 1];
        strcpy(c_nomfichier, nomfichier.c_str());

        /** LECTURE **/
        wavRead(refFILE[mot], c_nomfichier, header_fichier[mot]);


        /* Deplacement du curseur après l'entete wav, directement sur les données (44 octets d'entete) */
        fseek(refFILE[mot],SEEK_SET, sizeof(struct wavfile));


        int nbreOctetsAudio = header_fichier[mot]->bytes_in_data;
        int nbreOctetsRestant = nbreOctetsAudio;
        char *buffer = new char[nbreOctetsAudio];
        int16_t donnee[nbreOctetsAudio];
        int indice = 0;

        /** LECTURE ET COPIE DES DONNEES AUDIO **/
        while(fread(buffer, sizeof(int16_t),1,refFILE[mot]) > 0 && (nbreOctetsRestant > 0)){
            donnee[indice] = buffer[indice];
            indice++;
            nbreOctetsRestant--;
        }

        /** REMOVE SILENCE **/

        int16_t* signalSansSilence;
        int taille_signal;
        float threshold = 1/100; //Sensibilité de détection du silence
        removeSilence(donnee,nbreOctetsAudio, &signalSansSilence, &taille_signal, threshold);



        /* fichier en mfc A quoi cela peut il servir ?
         * nameWavToMfc(c_nomfichier, c_mfcName);
         */


        /** PARAMETRISATION **/

        /* IMPLEMENTATION BASE SUR UNE SOURCE */
        computeMFCC(&par.X_mfcc, &par.length_xmfcc, signalSansSilence, nbreOctetsAudio, header_fichier[mot]->frequency,
                    512, 256, 13, 26 );
        //computeMFCC(float **X_mfcc, int *length_xmfcc, int16_t *x, int Nx, int frequency, int frame_length, int frame_step,
        //               int dim_mfcc, int num_filter);



        delete [] c_nomfichier;

    }


    string hypotheses[12]={"M02","M03","M04","M05","M06","M07","M08", "M09", "M10", "M11", "M12", "M13"};






        return 0;
}