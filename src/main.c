/** 
*   @mainpage Dijkstra algorithm implementations
*   Dijkstra algorithm implementations created using C and UPC.
*   @author Aleksandra Poreba 
*   @author Arkadiusz Kasprzak
*/


#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#include "dijkstra.h"
#include "utility.h"


shared sdblptr columnData [THREADS];

shared int matrixDimention;

int main(int argc, char *argv[]) {
   
    // init
    upc_barrier;

    // read matrix size to allocate memory
    int sourceVertex = -1;
    char * fileName = NULL;
    if (MYTHREAD == 0){
        fileName = getInputFileName(argc, argv);
        sourceVertex = getSourceVertex(argc, argv);
        matrixDimention = readMatrixSize(fileName);
    }

    upc_barrier;
    if (matrixDimention == -1) return 0;

    if (MYTHREAD == 0){
        initData(sourceVertex, matrixDimention);
        printf("\n");
    }

    upc_barrier;

    // divide columns per processes
    // local structure to hold data about processed columns
    ColumnsToProcess colsPerProcess;

    //  number of columns per process
    colsPerProcess.numberOfColumns = matrixDimention / THREADS;
    colsPerProcess.columnSize = matrixDimention;

    if ( matrixDimention % THREADS > MYTHREAD) {
        ++colsPerProcess.numberOfColumns;
    }

    // prepare directory of columns
    columnData[MYTHREAD] = upc_alloc(colsPerProcess.numberOfColumns * 
            colsPerProcess.columnSize * sizeof(double) );

    upc_barrier;

    // read matrix data to directory
    if (MYTHREAD == 0){
        readDataAsContinuousMemory(fileName, columnData);
    }

    upc_barrier;

    // save pointers to the begginings of columns
    colsPerProcess.data = calloc(colsPerProcess.numberOfColumns, sizeof(ColumnData));

    for (int i = 0; i < colsPerProcess.numberOfColumns; ++i) {
        int columnIndex = MYTHREAD + i*THREADS;

        colsPerProcess.data[i].columnIndex = columnIndex;
        colsPerProcess.data[i].column = columnData[MYTHREAD] + i*colsPerProcess.columnSize;  
    }

    upc_barrier;
    printColumnInfo(&colsPerProcess);

    // run algorithm
    run(colsPerProcess);

    // save results
    if (MYTHREAD == 0){
        printResultToFile(matrixDimention, sourceVertex);
    }

    // finalize
    free(colsPerProcess.data);
    upc_all_free(columnData[MYTHREAD]);
    freeSharedData();

    return 0;
}
