#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#include "dijkstra.h"
#include "utility.h"


shared MatrixData matrixData;

int main(int argc, char *argv[]) {
   
    // init
    upc_barrier;

    int sourceVertex;
    if (MYTHREAD == 0){
        char * fileName = getInputFileName(argc, argv);
        sourceVertex = getSourceVertex(argc, argv);
        matrixData = readDataAsContinuousMemory(fileName);

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

    //  allocate memory and copy matrix values
    colsPerProcess.data = calloc(colsPerProcess.numberOfColumns, sizeof(ColumnData));

    for (int i = 0; i < colsPerProcess.numberOfColumns; ++i) {
        int columnIndex = MYTHREAD + i*THREADS;
        colsPerProcess.data[i].index = columnIndex ;
        colsPerProcess.data[i].begin = matrixData.matrixValues 
                + columnIndex*matrixData.matrixDimention;  

    }

    printf("Thread %d has %d columns\n", MYTHREAD, colsPerProcess.numberOfColumns);

    upc_barrier;

    if (colsPerProcess.numberOfColumns > 0) {
        run(colsPerProcess);
    }

    // save results
    if (MYTHREAD == 0){
        printResultToFile(matrixData.matrixDimention, sourceVertex);
    }

    // finalize
    free(colsPerProcess.data);

    if (MYTHREAD == 0){
        upc_all_free(matrixData.matrixValues);
        freeSharedData();
    }

    return 0;
}
