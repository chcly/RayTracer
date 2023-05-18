# Math

Is a basic linear math library for 2D & 3D Graphics with OpenGL.

## Building

It uses cmake to generate its make files it, and has a dependency on the [Utils](https://github.com/CharlesCarley/Utils) module.


The file [gitupdate.py](gitupdate.py) will clone the the dependency and update the sub-module to the head of the master branch.

It has been tested with CMake's VisualStudio and Unix Makefile generators.   


### Defines 

Optional definitions used in this project.

| Option             | Description                   |  Default   |
|:-------------------|:------------------------------|:----------:|
| Math_SCALAR_DOUBLE | Define scalar type as double. | OFF(float) |


## References

Some portions were initially derived from:

+ Dunn, Fletcher. “3D Math Primer for Graphics and Game Development.” Amazon, CRC Press, 2012, http://www.amazon.com/Math-Primer-Graphics-Game-Development/dp/1568817231.
    + <b>Notably</b> portions of skEuler, skQuaternion, skMatrix3, skMatrix4 and skMath.
+   https://en.wikipedia.org/w/index.php?title=HSL_and_HSV&oldid=941280606
    +   HSV Reference
