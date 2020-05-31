#pragma once

#include <stdio.h>
#include "utility.h"
#include "vertexData.h"

// allocates memory and fill the arrays
//  should be called by just one process
void initData(int sourceVertex, int size);


void freeSharedData();

VertexData findVertexWithMinimalDistance(ColumnsToProcess cols);

int checkIfAllVerticesHaveBeenProcessed(int numberOfColumns);

// run the algorithm
void run(ColumnsToProcess cols);

// find new distances
void performInnerLoop(VertexData closestVertex, ColumnsToProcess cols);

// debug printout
void printState();

// save results to file
void printResultToFile(int numberOfColumns, int sourceVertex);

void printPaths(FILE * fp, int numberOfColumns, int sourceVertex);

void printDistances(FILE * fp, int numberOfColumns, int sourceVertex);
