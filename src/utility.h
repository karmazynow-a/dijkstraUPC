#pragma once

#include "matrixData.h"
#include "columnsToProcess.h"

MatrixData readDataAsContinuousMemory(const char * fileName);

// filename should be specified as second arguent
char * getInputFileName(int argc, char *argv[]);

// start node should be specified as first argument
int getSourceVertex(int argc, char *argv[]);

// print column by index
void printColumn( const ColumnsToProcess * col, const int columnIndex );

// print column data assignment
void printColumnInfo( const ColumnsToProcess * col );
