# Utils

Utils is a small general purpose utility library in C++. 

## Building

It uses CMake to generate its make files. 

It has been tested with CMake's VisualStudio and Unix Makefile generators.   



## Definitions 


Optional defines used in this project.


| Option                    | Description                                                           | Default |
|:--------------------------|:----------------------------------------------------------------------|:-------:|
| Utils_USE_DEBUG_ASSERT    | Enable or disable `assert` in debug builds.                           |   ON    |
| Utils_USE_ITERATOR_DEBUG  | Enable or disable iterator checks in debug builds.                    |   OFF   |
| Utils_USE_COMPILER_CHECKS | Enable or disable compile asserts.                                    |   OFF   |
| Utils_NO_DEBUGGER         | Send calls to `skPrintf` to `printf` opposed to `skDebugger::report`. |   ON    |
| Utils_BUILD_TESTS         | Builds tests.                                                         |   OFF   |
| Utils_AUTO_RUN_TESTS      | Adds a custom build target that executes the tests on build.          |   OFF   |
