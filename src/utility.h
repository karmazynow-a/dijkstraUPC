#pragma once

#include "columnsToProcess.h"

typedef shared [] double * sdblptr;

int readMatrixSize(const char * fileName);

void readDataAsContinuousMemory(const char * fileName, shared sdblptr columnData [THREADS]);

// filename should be specified as second arguent
char * getInputFileName(const int argc, char *argv[]);

// start node should be specified as first argument
int getSourceVertex(const int argc, char *argv[]);

// print column by index
void printColumn( const ColumnsToProcess * col, const int columnIndex );

// print column data assignment
void printColumnInfo( const ColumnsToProcess * col );
