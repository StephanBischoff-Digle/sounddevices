cmake_minimum_required(VERSION 2.6)
project(sounddevices)

# set standard to c++11
set(CMAKE_CXX_STANDARD 11)

# link with ALSA
set(CMAKE_CXX_FLAGS "-lasound")

# add sourcefiles to sourcelist
set(exe_src
  src/main.cpp
  src/device.cpp
  )
  
# add headerfiles to headerlist
set(exe_hdr
  src/device.h
  )

# set executable name and it's sourcefiles
add_executable(sounddevices ${exe_src} ${exe_hdr})

install(TARGETS sounddevices RUNTIME DESTINATION bin)
