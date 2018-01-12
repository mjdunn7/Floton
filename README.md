# Floton, a simple C++ Ray-Tracer

### Introduction

Floton (pronounced flow-ton, rhymes with photon) is a simple, lightweight, CPU ray tracer written in c++. Its current purpose is mainly for use as an educational tool for those looking to learn more about how a ray tracer works, though more fully flushed out features may come in the future.

### How it works
Floton uses a technique called ray tracing to render realistic graphical images. Taken from Wikipedia,"ray tracing is a rendering technique for generating an image by tracing the path of light as pixels in an image plane and simulating the effects of its encounters with virtual objects. The technique is capable of producing a very high degree of visual realism, usually higher than that of typical scanline rendering methods, but at a greater computational cost. This makes ray tracing best suited for applications where the image can be rendered slowly ahead of time, such as in still images and film and television visual effects, and more poorly suited for real-time applications like video games where speed is critical."

Currently, images can only be outputted in ppm format, an ascii based image encoding that is very simple but produces very large file sizes. Additional image formats may be sopported in the future.

### Getting Started
To use, first download the source code from GitHub to your local machine:
```
git clone https://github.com/mjdunn7/Floton.git
cd Floton
```
  
Then, compile the source (you must have cmake installed):
```
cmake -B./build -H. -DCMAKE_BUILD_TYPE=Release
cmake --build ./build -- install
```
  
An executable binary file should then appear in the top directory. To render an example image, you can run the following command:
```
./Floton.sh ExampleRenderFiles\CircularReality.txt testRender.ppm
```
  


