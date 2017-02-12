#include <iostream>
#include <vector>

#include "device.h"


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
  frame = mic->receive(10);
  mic->close();
  
  // print recorded data to stdout
  for(auto f : frame) {
    std::cout << f << std::endl;
  }
  
  // open speaker, replay recorded sound and close the device
  speaker->open();
  speaker->send(frame);
  speaker->close();
  
  // free all the memory
  delete(mic);
  delete(speaker);
  
  return 0;
}
