# Floton, a simple C++ Ray-Tracer

<p align="center">
<img src="https://static1.squarespace.com/static/5a4eaf126957da87f6d3de00/t/5a4eb48fe2c4837fbba347f2/1515107560454/CircularRealityWithDoF.png?format=2500w" width="50%" >
</p>

### Introduction

Floton (pronounced flow-ton, rhymes with photon) is a simple, lightweight, CPU ray tracer written in C++. In the project's current state, it is best used as educational tool for those looking to learn more about how a ray tracer works by exploring the source code, but more fancy features will hopefully be added soon.

### How it works
Floton uses a technique called ray tracing to render realistic graphical images. Taken from [Wikipedia](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)),"ray tracing is a rendering technique for generating an image by tracing the path of light as pixels in an image plane and simulating the effects of its encounters with virtual objects. The technique is capable of producing a very high degree of visual realism, usually higher than that of typical scanline rendering methods, but at a greater computational cost. This makes ray tracing best suited for applications where the image can be rendered slowly ahead of time, such as in still images and film and television visual effects, and more poorly suited for real-time applications like video games where speed is critical."

Currently, images can only be outputted in [ppm format](https://en.wikipedia.org/wiki/Netpbm_format), an ascii based image encoding that is very simple but produces very large file sizes. Additional image formats may be supported in the future.

### Getting Started (Linux)
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
  
An executable binary file should then appear in the top directory. To render an example image, you can run the following command (the first argument indicates the render driver file, the second indicates the name of the output file):
```
./Floton ./ExampleRenderFiles/CircularReality.txt testRender.ppm
```

### Current Capabilities
  * Rendering of spheres and [wavefront obj](https://en.wikipedia.org/wiki/Wavefront_.obj_file) files with triangular faces 
  * Multithreading support
  * Ambient, specular, and diffuse illumination
  * Reflection
  * Shadows
  * Depth of field (image is in focus at a certain distance)
  * Refraction of light through spheres
  * Image output in .ppm format
  
### Development Guide
Floton was originally made using the C-Lion IDE, but the use of cmake allows for versitile development styles. To get started with C-Lion, simply click file->open in the IDE and navigate to the cloned Floton Github repository. Otherwise, the source code and be found in the SRC folder and edited with your text editor of choice. To compile, run the following commands from the top directory, the first command is only necessary the first time you run:
```
cmake -B./debug-build -H. -DCMAKE_BUILD_TYPE=Debug
cmake --build ./debug-build -- install
```


  


