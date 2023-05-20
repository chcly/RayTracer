# RayTracer

Is a small custom ray tracer for educational purposes.

It runs either on the CPU or GPU.

+ The GPU implementation is written with CUDA.
+ The CPU implementation is threaded using maximum number of CPU cores.
+ Scene information is provided with .bascii files.
+ The [Viewer](https://github.com/chcly/RayTracer/tree/master/Samples/Viewer) in the Samples directory is the main entry point.
+ [Extras/Example.md](Extras/Example.md) provides a usage example.

## Testing

The Test directory is setup to work with [googletest](https://github.com/google/googletest).

## Building

Building with CMake and Make.

```sh
mkdir build
cd build
cmake .. -DRayTracer_BUILD_TEST=ON -DRayTracer_AUTO_RUN_TEST=ON
make
```

### Optional defines

| Option                       | Description                                          | Default |
| :--------------------------- | :--------------------------------------------------- | :-----: |
| RayTracer_BUILD_TEST         | Build the unit test program.                         |   ON    |
| RayTracer_AUTO_RUN_TEST      | Automatically run the test program.                  |   OFF   |
| RayTracer_USE_STATIC_RUNTIME | Build with the MultiThreaded(Debug) runtime library. |   ON    |
