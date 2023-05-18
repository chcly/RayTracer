﻿# RayTracer

Is a small custom ray tracer for educational purposes that runs either on the CPU or GPU. The GPU implementation is written with CUDA. It provides a small code base to work with, and an area to experiment with different ray tracing techniques and optimization tactics. The primary goal is to strike a balance between quality and performance in real-time, as well as to understand the performance limitations of the ray tracing algorithm.


## File formats

Scene information is provided with [.bascii](https://github.com/CharlesCarley/bAscii) files. 

The [Viewer](https://github.com/CharlesCarley/RayTracer/tree/master/Samples/Viewer) in the Samples directory is the main entry point. It provides the means to load a file, specify a backend, and display a scene. 

## Example

This [link](Extras/Example.md) provides a usage example.


## Building
This project builds with CMake and has the following external git repositories as dependencies.

+ [bAscii](https://github.com/CharlesCarley/bAscii)
+ [FileTools](https://github.com/CharlesCarley/FileTools)
+ [FreeImage](https://github.com/CharlesCarley/FreeImage)
+ [Image](https://github.com/CharlesCarley/Image)
+ [Math](https://github.com/CharlesCarley/Math)
+ [SDL](https://github.com/CharlesCarley/SDL)
+ [Window](https://github.com/CharlesCarley/Window)
+ [Utils](https://github.com/CharlesCarley/Utils)


It has been tested with CMake's VisualStudio and Unix Makefile generators.   

### Defines 

Optional definitions used in this project.

| Option                  | Description                         | Default |
|:------------------------|:------------------------------------|:-------:|
| RayTracer_BUILD_TEST    | Build the unit test program.        |   OFF   |
| RayTracer_AUTO_RUN_TEST | Automatically run the test program. |   OFF   |

