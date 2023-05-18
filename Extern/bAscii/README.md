# bAscii

This project is a repurposed update to an earlier ASCII scene loader initially written for [OgreKit](https://github.com/gamekit-developers/gamekit/tree/OgreKit/Sandbox/Ascii). 

Its purpose is to provide a convenient way to supply simple scene data.

## Building

It uses cmake to generate its make files it, and has a dependency on the [Utils](https://github.com/CharlesCarley/Utils) module.


The file [gitupdate.py](gitupdate.py) will clone the the dependency and update the sub-module to the head of the master branch.

It has been tested with CMake's VisualStudio and Unix Makefile generators.   

### Defines 

Optional cmake definitions used in this project.

| Option               | Description                         | Default |
|:---------------------|:------------------------------------|:-------:|
| bAscii_BUILD_TEST    | Build the unit test program.        |   OFF   |
| bAscii_AUTO_RUN_TEST | Automatically run the test program. |   OFF   |


## Testing

The testing directory is setup to work with [googletest](https://github.com/google/googletest). 

It also contains the initial setup for testing the standalone module using GitHub actions.

