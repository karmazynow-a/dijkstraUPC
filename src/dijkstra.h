#pragma once

#include <stdio.h>
#include "utility.h"
#include "vertexData.h"

// allocates memory and fill the arrays
//  should be called by just one process
void initData(const int sourceVertex, const int size);

void freeSharedData();

VertexData findVertexWithMinimalDistance(const ColumnsToProcess cols);

int checkIfAllVerticesHaveBeenProcessed(const int numberOfColumns);

// run the algorithm
void run(const ColumnsToProcess cols);

// find new distances
void performInnerLoop(const VertexData closestVertex, const ColumnsToProcess cols);

// debug printout
void printState();

// save results to file
void printResultToFile(const int numberOfColumns, const int sourceVertex);

void printPaths(FILE * fp, const int numberOfColumns, const int sourceVertex);

void printDistances(FILE * fp, const int numberOfColumns, const int sourceVertex);
