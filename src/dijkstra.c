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


void initData ( int sourceVertex, int size ){
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

      // all local minimums should be set to INFINITY, in any other case
      // the algorithm could use previous value
      upc_forall(int i = 0; i < cols.columnSize; ++i; &localMin[i]) {
          localMin[i] = INFINITY;
      }

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


int checkIfAllVerticesHaveBeenProcessed(int numberOfColumns) {
    upc_barrier;
    upc_all_reduceI(&numberOfProcessedColumns, processedColumns, UPC_ADD, numberOfColumns, 1, NULL, UPC_IN_ALLSYNC);
    upc_barrier;
    return numberOfProcessedColumns == numberOfColumns;
}


VertexData findVertexWithMinimalDistance(ColumnsToProcess cols){
    
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


void performInnerLoop(VertexData closestVertex, ColumnsToProcess cols){
    //  distance between columns for one process
    int columnsDist = THREADS;

        for (int i = 0; i < cols.numberOfColumns; ++i) {
            int currentIndex = cols.data[i].index;

            if( processedColumns[currentIndex] == 1){
               continue;
            }
            
            double valueInMatrix = cols.data[i].begin[closestVertex.index];

            valueInMatrix = valueInMatrix < 0.001
                ? INFINITY
                : valueInMatrix;

            double newDistance = valueInMatrix + closestVertex.distance;
            
            if( newDistance < distances[currentIndex] ) {
               distances[currentIndex] = newDistance;
               predecessors[currentIndex] = closestVertex.index;

            }
        }
}


void printState( int numberOfColumns ){
    char str[1024] = "";

    for (int i = 0; i < numberOfColumns; ++i){
        sprintf(str, "%s\nVertex: %d processed? %d predecessor: %d distance: %.2f", 
            str, i, processedColumns[i], predecessors[i], distances[i]);
    }

    puts(str);
}


void printResultToFile(int numberOfColumns, int sourceVertex){
    FILE * fp = fopen("resultsUPC.txt", "w");

    printDistances( fp, numberOfColumns, sourceVertex );
    printPaths( fp, numberOfColumns, sourceVertex );

    fclose(fp);
}


void printPaths( FILE * fp, int numberOfColumns, int sourceVertex ){
    fprintf(fp, "============= PATHS =============\n");

    for (int i = 0; i < numberOfColumns; ++i){
        char path [100] = "";
        char buff [100];
        int vertex = i;

        while (vertex != sourceVertex && vertex >= 0) {
            strcpy(buff, path);
            sprintf(path, "%d, %s", vertex, buff);
            vertex = predecessors[vertex];
        }
       
        strcpy(buff, path);
        sprintf(path, "%d, %s", vertex, buff);

        fprintf(fp, "%s\n", path);
    }
}


void printDistances( FILE * fp, int numberOfColumns, int sourceVertex ){
    fprintf(fp, "============ RESULTS ============\n");

    for (int i = 0; i < numberOfColumns; ++i){
        fprintf(fp, "Distance from vertex %d to %d: %.2f \n", 
            sourceVertex, i, distances[i]);
    }
}

