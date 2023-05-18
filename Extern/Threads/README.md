# Threads

Threads is a basic cross-platform thread library. It provides a thin abstraction 
layer over the specific OS platform. It is written using the Win32 API for windows and posix threads for Unix.
 
## Building

It uses cmake to generate its make files it, and has a dependency on the [Utils](https://github.com/CharlesCarley/Utils) module.


The file [gitupdate.py](gitupdate.py) will clone the the dependency and update the sub-module to the head of the master branch.

It has been tested with CMake's VisualStudio and Unix Makefile generators. 


### Defines 

Optional definitions used in this project.

| Option                 | Description                         | Default |
|:-----------------------|:------------------------------------|:-------:|
| Threads_BUILD_TEST     | Build the unit test program.        |   OFF   |
| Threads_AUTO_RUN_TEST  | Automatically run the test program. |   OFF   |



## Testing 

The testing directory is setup to work with [googletest](https://github.com/google/googletest). 

It also contains the initial setup for testing the standalone module using GitHub actions.
