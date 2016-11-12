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


    std::cout << dtw(9, 6, 0, sequence1, sequence2) << std::endl;


    /*Seconde partie*/

    //LECTURE AUDIO ( enregistrer les sons sur l'applications puis les lires ici)

    //Transformer le signal ? (avec la transformé de fourier : fft)
    //FFTReal(const long length);

    /*==========================================================================*/
    /*      Name: do_fft                                                        */
    /*      Description: Compute the FFT of the array.                          */
    /*      Input parameters:                                                   */
    /*        - x: pointer on the source array (time).                          */
    /*      Output parameters:                                                  */
    /*        - f: pointer on the destination array (frequencies).              */
    /*             f [0...length(x)/2] = real values,                           */
    /*             f [length(x)/2+1...length(x)-1] = imaginary values of        */
    /*               coefficents 1...length(x)/2-1.                             */
    /*      Throws: Nothing                                                     */
    /*==========================================================================*/

    //FFTReal::do_fft(flt_t f[], const flt_t x[]) const



    /**
* Compute MFCC of a signal
*
* @param X_mfcc (OUT) Adress of a buffer to store MFCC coefficient
* @param length_xmfcc (OUT) Pointer to the length of the buffer
* @param x (IN) The signal to process
* @param Nx The (IN) length of the signal
* @param header (IN) wave header structure
* @param frame_length (IN) Frame length
* @param frame_step (IN) Step between each frame to allow overlapping. 160frame step = 10ms frames
* @param dim_mfcc (IN) Number of MFCC coefficient kept
* @param num_filter (IN) Number of filter for the mfcc algorithm
* @return none
*/
    //computeMFCC(float **X_mfcc, int *length_xmfcc, int16_t *x, int Nx, int frequency, int frame_length, int frame_step,
      //               int dim_mfcc, int num_filter);

        return 0;
}