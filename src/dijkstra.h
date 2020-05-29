#pragma once

#include <stdio.h>
#include "utility.h"
#include "vertexData.h"

// allocates memory and fill the arrays
//  should be called by just one process
void initData ( int sourceVertex, int size );

// clenup shared memory concerning algorithm
//  should be called by just one process
void freeSharedData ();

VertexData findVertexWithMinimalDistance(ColumnsToProcess cols);

int checkIfAllVerticesHaveBeenProcessed(int numberOfColumns);

// run the algorithm
void run (ColumnsToProcess cols);

// find new distances
void performInnerLoop(VertexData closestVertex, ColumnsToProcess cols);

// print states of processed data
void printState();
