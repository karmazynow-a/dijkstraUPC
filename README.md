# Dijkstra's algorithm using UPC

This repository contains Dijkstra's Shortest Path algorithm implemented using UPC (Unified Parallel C).


### Contributors
 * [Aleksandra Poręba](https://github.com/karmazynow-a)
 * [Arkadiusz Kasprzak](https://github.com/arokasprz100)

### Repository content
 * `src` - contains UPC implementation of Dijkstra's algorithm
 * `data` -  contains sample input file
 * `example_results` - contains files with example results generated using UPC implementation of Dijkstra's algorithm
 * `report` and `seminars` - contain documentation and presentation (in polish, since it was a part of university project)
 * `build_uni` - contains setup used for building this project on university infrastructure


### Usage
To build this software using AGH UST infrastructure (Faculty of Physics and Applied Computer Science), use prepared `Makefile` and `setup.sh` files located in `build_uni` directory:

```
cd build_uni
source setup.sh
make 
```
To run the program, use prepared `Makefile`. Possible options:
```
make run                                        runs UPS algorithm with default parameters: 
                                                    VERTEX=0 FILE="../data/graph.dat" PROC_COUNT=1 HOSTS="nodes"
make run VERTEX=V FILE=F PROC_COUNT=P HOSTS=H   runs UPC algorithm with specified parameters 
                                                    (all of them are optional)
make docs                                       creates Doxygen documentation
make install                                    move executables to build directory
make clean                                      remove all created files and directories
```