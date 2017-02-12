#include "device.h"

#include <cmath>

bool Device::open()
{
  int err;
  if(devicetype_ == Type::SPEAKER) {
    if((err = snd_pcm_open(&handler_,
            devicename_.data(),
            SND_PCM_STREAM_PLAYBACK, 0)) < 0)
      return false;
  } else if(devicetype_ == Type::MICROPHONE) {
    if((err = snd_pcm_open(&handler_,
            devicename_.data(),
           SND_PCM_STREAM_CAPTURE, 0)) < 0)
      return false;
  } else {
    return false;
  }
  
  if((err = snd_pcm_set_params(handler_,
          SND_PCM_FORMAT_FLOAT,
          SND_PCM_ACCESS_RW_INTERLEAVED,
          1,
          samples_,
          1,
          latency_)) < 0)
    return false;
  
  return true;
}

void Device::close()
{
  snd_pcm_close(handler_);
}

void Device::send(const std::vector<float>& frame)
{
  snd_pcm_writei(handler_, frame.data(), frame.size());
}

std::vector<float> Device::receive(float duration)
{
  int framesize = std::round(samples_ * duration);
  float buffer[framesize];
  int err;
  std::vector<float> ret;
  
  if((err = snd_pcm_readi(handler_, buffer, framesize)) == framesize) {
    ret.assign(buffer, buffer + sizeof(buffer) / sizeof(float));
  }
  
  return ret;
}
