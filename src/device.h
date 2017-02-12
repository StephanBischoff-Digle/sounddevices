#ifndef DEVICE_H
#define DEVICE_H

#include <alsa/asoundlib.h>
#include <string>
#include <vector>

class Device {
public:
  enum Type {MICROPHONE, SPEAKER};
private:
  snd_pcm_t * handler_;
  std::string devicename_;
  unsigned int samples_;
  unsigned int latency_;
  Type devicetype_;
  
  
public:
  Device(std::string name, unsigned int samples, unsigned int latency, Type devicetype) :
    handler_(nullptr),
    devicename_(name),
    samples_(samples),
    latency_(latency),
    devicetype_(devicetype)
    {}
    
  bool open();
  void close();
  void send(const std::vector<float>& frame);
  std::vector<float> receive(float duration);

};
#endif 
