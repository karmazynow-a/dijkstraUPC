#include <stdio.h> 
#include <math.h>
#include <string.h>
#include <upc.h>
#include <upc_collective.h>

#include "dijkstra.h"

// data concerning algorithm
// shared ptrs for shared data
shared int * shared processedColumns;
shared double * shared distances;
shared int * shared predecessors;

shared VertexData globalMin;
shared int numberOfProcessedColumns;
shared double * shared localMin;


void initData ( const int sourceVertex, const int size ){
    processedColumns = upc_global_alloc( size, sizeof(int));
    distances = upc_global_alloc( size, sizeof(double));
    predecessors = upc_global_alloc( size, sizeof(int));
    localMin = upc_global_alloc( size, sizeof(double));

    for (int i = 0; i < size; ++i){
        processedColumns[i] = 0;
        distances[i] = INFINITY;
        predecessors[i] = -1;
        localMin[i] = INFINITY;
    }

    distances[sourceVertex] = 0;
}


void freeSharedData (){
    upc_all_free(processedColumns);
    upc_all_free(predecessors);
    upc_all_free(distances);
    upc_all_free(localMin);
}


void run (ColumnsToProcess cols){

   while( !checkIfAllVerticesHaveBeenProcessed(cols.columnSize) ){

      upc_notify;

      // reset global minimum value
      if (MYTHREAD == 0) {
          globalMin.index = -1;
          globalMin.distance = INFINITY;
      }

      upc_wait;

      VertexData local = findVertexWithMinimalDistance(cols);
      localMin[local.index] = local.distance;

      upc_barrier;
      upc_all_reduceD(&globalMin.distance, localMin, UPC_MIN, cols.columnSize, 1, NULL, UPC_IN_ALLSYNC);
      upc_barrier;

      // find index of global value
      if (MYTHREAD == 0){
        for(int i = 0; i < cols.columnSize; ++i){
            if( fabs(localMin[i] - globalMin.distance) < 0.001){
                globalMin.index = i;
                break;
            }
        }
      }

      upc_barrier;

      if (globalMin.index == -1) {
        break;
      }

      // mark vertex as processed
      upc_notify;
      if (MYTHREAD == 0){  
        processedColumns[globalMin.index] = 1;

        // reset chosen local minimum value
        localMin[globalMin.index ] = INFINITY;
      }
      upc_wait;

      // calculate new distances
      upc_notify;
      performInnerLoop(globalMin, cols);
      upc_wait;
   }

    // TODO to be deleted before release
    if(MYTHREAD == 0){
        puts("\n\nFINAL STATE");
        printState(cols.columnSize);
    }
}


int checkIfAllVerticesHaveBeenProcessed(const int numberOfColumns) {
    upc_barrier;
    upc_all_reduceI(&numberOfProcessedColumns, processedColumns, UPC_ADD, numberOfColumns, 1, NULL, UPC_IN_ALLSYNC);
    upc_barrier;
    return numberOfProcessedColumns == numberOfColumns;
}


VertexData findVertexWithMinimalDistance(const ColumnsToProcess cols){
    
    VertexData closestVertex = { -1, INFINITY };

    // for each processed column
    upc_forall(int i = 0; i < cols.columnSize; ++i; &processedColumns[i] ) {

        // we check if it was already calculated
        if (processedColumns[i] == 0) {

            // if not, we look for closest vertex to it
            if ( distances[i] < closestVertex.distance ) {
                closestVertex.index = i;
                closestVertex.distance = distances[i];
            }
        }
    }
    return closestVertex;
}


void performInnerLoop(const VertexData closestVertex, const ColumnsToProcess cols){
    //  distance between columns for one process
    int columnsDist = THREADS;

        for (int i = 0; i < cols.numberOfColumns; ++i) {
            
            int currentColumnIndex = cols.data[i].columnIndex;

            double valueInMatrix = getValueFromColumn(cols.data + i, closestVertex.index );

            valueInMatrix = valueInMatrix < 0.001
                ? INFINITY
                : valueInMatrix;

            double newDistance = valueInMatrix + closestVertex.distance;
           
            if( newDistance < distances[currentColumnIndex] ) {
               distances[currentColumnIndex] = newDistance;
               predecessors[currentColumnIndex] = closestVertex.index;
            }
        }
}


void printState( const int numberOfColumns ){
    char str[1024] = "";

    for (int i = 0; i < numberOfColumns; ++i){
        sprintf(str, "%s\nVertex: %d processed? %d predecessor: %d distance: %.2f", 
            str, i, processedColumns[i], predecessors[i], distances[i]);
    }

    puts(str);
}


void printResultToFile( const int numberOfColumns, const int sourceVertex ){
    FILE * fp = fopen("resultsUPC.txt", "w");

    printDistances( fp, numberOfColumns, sourceVertex );
    printPaths( fp, numberOfColumns, sourceVertex );

    fclose(fp);
}


void printPaths( FILE * fp, const int numberOfColumns, const int sourceVertex ){
    fprintf(fp, "============= PATHS =============\n");

    char placeholderVertex [100] = "";
    sprintf(placeholderVertex, "%d\0", -1);

    for (int i = 0; i < numberOfColumns; ++i){
        char path [100] = "";
        char buff [100] = "";
        int vertex = i;

        while (vertex != sourceVertex && vertex >= 0) {
            strcpy(buff, path);
            sprintf(path, "%d, %s", vertex, buff);
            vertex = predecessors[vertex];
        }
       
        strcpy(buff, path);
        sprintf(path, "%d, %s", vertex, buff);

        if (strstr(path, placeholderVertex) == NULL) {
            fprintf(fp, "%s\n", path);
        }
        else {
            fprintf(fp, "Vertex %d unreachable from source vertex.\n", i);
        }

        
    }
}


void printDistances( FILE * fp, const int numberOfColumns, const int sourceVertex ){
    fprintf(fp, "============ RESULTS ============\n");

    for (int i = 0; i < numberOfColumns; ++i){
        fprintf(fp, "Distance from vertex %d to %d: %.2f \n", 
            sourceVertex, i, distances[i]);
    }
}
