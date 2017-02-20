#include <iostream>
#include <fstream>
#include <vector>

#include <fftw3.h>
#include <cmath>

#include "device.h"


std::vector<std::vector<float>>
compute_fourier(std::vector<float> wave, unsigned int framesize) {
    std::cout << "\033[1;32m" << "Start Fouriertransform ..." << "\033[0m" << std::endl;
    // computing number of frames
    // note, if wave.size() % framesize is not 0 the end
    //  of the wave that does not fit into a full frame will be ignored!
    unsigned int numFrames = std::floor(wave.size() / framesize);
    std::vector<std::vector<float>> result(numFrames, std::vector<float>(framesize));

    fftw_complex *in, *out;
    fftw_plan p;

    std::cout << "\033[1;32m" << "Allocating In- and Outputbuffer" << "\033[0m" << std::endl;
    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * framesize);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * framesize);
    p = fftw_plan_dft_1d(framesize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    std::cout << "\033[1;32m" << "Starting to transform " << numFrames << " Frames" << "\033[0m" << std::endl;
    for(auto i = 0; i < numFrames; i++) {
        std::cout << "\033[1;32m" << "\tFrame " << i+1 << " | " << numFrames << "\tLoading Inputbuffer" << "\033[0m" << std::endl;
        // load complex inputbuffer with current frame
        for(auto j = 0; j < framesize; j++) {
            in[j][0] = wave[framesize * i + j];
        }

        std::cout << "\033[1;32m" << "\tFrame " << i+1 << " | " << numFrames << "\tExecuting Transformation" << "\033[0m" << std::endl;
        // execute transformation
        fftw_execute(p);

        std::cout << "\033[1;32m" << "\tFrame " << i+1 << " | " << numFrames << "\tCalculating Absolute Value" << "\033[0m" << std::endl;
        // compute absolute value
        for(auto j = 0; j < framesize; j++) {
            result[i][j] = std::sqrt(out[j][0] * out[j][0] + out[j][1] * out[j][1]);
        }
        std::cout << "\033[1;32m" << "\t- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << "\033[0m" << std::endl;
    }

    std::cout << "\033[1;32m" << "Done" << "\033[0m" << std::endl;
    return result;
}


int main(int argc, char **argv) {
    // create microphone and speaker devices
    // parameters are:
    // devicename
    // sampling rate
    // latency
    // device typ
    Device * mic = new Device("default", 40000, 500000, Device::Type::MICROPHONE);
    Device * speaker = new Device("default", 40000, 500000, Device::Type::SPEAKER);

    // framebuffer
    std::vector<float> frame;

    // open microphone, record some data and close the device
    mic->open();
    float time_in_sec = 10;
    std::cout << "\033[1;32m" << "Start recording for " << time_in_sec << "seconds" << "\033[0m" << std::endl;
    frame = mic->receive(time_in_sec);
    std::cout << "\033[1;32m" << "Done recording" << "\033[0m" << std::endl;
    mic->close();

    std::ofstream outfile;

    unsigned int framesize = 1000;
    auto fourier = compute_fourier(frame, framesize);

    outfile.open("fourier.csv");
    std::cout << "\033[1;32m" << "Writing CSV ..." << "\033[0m" << std::endl;

    for(std::size_t j = 0; j < framesize; j++) {
        for(std::size_t i = 0; i < fourier.size(); i++) {
            if(i != 0)
                outfile << ";";
            outfile << fourier[i][j];
        }
        outfile << std::endl;
    }
    outfile.close();
    std::cout << "\033[1;32m" << "Done Writing CSV" << "\033[0m" << std::endl;


    // free all the memory
    delete(mic);
    delete(speaker);

    return 0;
}
