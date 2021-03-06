# Mandelbrot

Mandelbrot is a program written is C++, using SDL2 and parallelization with OpenMP. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

The basic requirements for building the executable are:

* CMake 2.8+
* a c++ compiler
* make
* SDL2

#### Installation on Ubuntu

```
sudo apt-get install build-essentials cmake
sudo apt-get install libsdl2-2.0 libsdl2-dev
```

### Installing

This program have been tested on Ubuntu 16.04 but should work under any systems that fulfills the aforementioned requirements.

#### Installation on Ubuntu

If you succesfully cloned the source files and you are currently in the project directory, you can generate the Makefile using the following command:

```
mkdir build
cd build/
cmake ..
```
Now you can build the binary by running:

```
make
```
Now, you should see the executable in the build file. If you run the executable, the application should look like this:
![](https://github.com/biroeniko/mandelbrot/blob/master/img/demo.png)

## Built With

* [SDL2](https://www.libsdl.org/) - used for display and control
* [OpenMP](https://www.openmp.org/) - used for creating multiple threads for the tasks (pixel color calculation)

## Authors

* **Biró Enikő** - [BiroEniko](https://github.com/biroeniko)
