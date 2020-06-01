#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#include "dijkstra.h"
#include "utility.h"


typedef shared [] double * sdblptr;
shared sdblptr columnData [THREADS];

shared MatrixData matrixData;

int main(int argc, char *argv[]) {
   
    // init
    upc_barrier;

    // read matrix data
    int sourceVertex;
    if (MYTHREAD == 0){
        char * fileName = getInputFileName(argc, argv);
        sourceVertex = getSourceVertex(argc, argv);
        matrixData = readDataAsContinuousMemory(fileName);
    }

    upc_barrier;
    if (matrixData.matrixDimention == -1) return 0;

    if (MYTHREAD == 0){
        initData(sourceVertex, matrixData.matrixDimention);
        printf("\n");
    }

    upc_barrier;

    // divide columns per processes
    // structure to hold data    
    ColumnsToProcess colsPerProcess;

    //  number of columns per process
    colsPerProcess.numberOfColumns = matrixData.matrixDimention / THREADS;
    colsPerProcess.columnSize = matrixData.matrixDimention;

    // add columns that left
    if ( matrixData.matrixDimention % THREADS > MYTHREAD) {
        ++colsPerProcess.numberOfColumns;
    }

    // prepare directory of columns
    columnData[MYTHREAD] = upc_alloc(colsPerProcess.numberOfColumns * 
            colsPerProcess.columnSize * sizeof(double) );

    // save pointer to the beggining of column
    colsPerProcess.data = calloc(colsPerProcess.numberOfColumns, sizeof(ColumnData));

    for (int i = 0; i < colsPerProcess.numberOfColumns; ++i) {
        int columnIndex = MYTHREAD + i*THREADS;

        // copy column from the matrix
        // TODO it copies memory from affinity
        /*
        upc_memcpy(columnData[MYTHREAD] + i*colsPerProcess.columnSize,
                   matrixData.matrixValues + columnIndex * colsPerProcess.columnSize, 
                   colsPerProcess.columnSize * sizeof(double));
        */
        for(int j = 0; j < colsPerProcess.columnSize; ++j){
            columnData[MYTHREAD][i*colsPerProcess.columnSize + j] = 
                matrixData.matrixValues[columnIndex * colsPerProcess.columnSize + j];
        }

        // save data from column to structure
        colsPerProcess.data[i].columnIndex = columnIndex;
        colsPerProcess.data[i].column = columnData[MYTHREAD] + i*colsPerProcess.columnSize;  
    }

    upc_barrier;
    printColumnInfo(&colsPerProcess);

    // run algorithm
    run(colsPerProcess);

    // save results
    if (MYTHREAD == 0){
        printResultToFile(matrixData.matrixDimention, sourceVertex);
    }

    // finalize
    free(colsPerProcess.data);
    upc_all_free(columnData[MYTHREAD]);
    upc_all_free(matrixData.matrixValues);
    freeSharedData();

    return 0;
}
