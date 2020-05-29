
#include <math.h>
#include <upc.h>
#include <upc_collective.h>

#include "dijkstra.h"

// data concerning algorithm
// shared ptrs for shared data
shared int * shared processedColumns;
shared double * shared distances;
shared int * shared predecessors;

shared VertexData globalMin;
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

    printState(size);
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
      // reset minimum values
      if (MYTHREAD == 0) {
        // all local minimums should be inf, in other case
        //    algorithm can take previous minimum that got smaller value
        for (int i = 0; i < cols.columnSize; ++i){
            localMin[i] = INFINITY;
        }

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
        printf("global min %f \n", globalMin.distance);
        for(int i = 0; i < cols.columnSize; ++i){
            if( localMin[i] - globalMin.distance < 0.001){
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

   if(MYTHREAD == 0){
        printState(cols.columnSize);
   }
}

int checkIfAllVerticesHaveBeenProcessed(int numberOfColumns) {
   // FIXME: find more efficient way
   for (int i = 0;  i < numberOfColumns; ++i){
      if( processedColumns[i] == 0)
         return false;
   }
   return true;
}
:w

VertexData findVertexWithMinimalDistance(ColumnsToProcess cols){
    VertexData closestVertex = {-1, INFINITY};

    // for each column that is processed
    for (int i = 0; i < cols.numberOfColumns; ++i){
        int columnIndex = cols.data[i].index;
        
        // we check if it was already calculated
        if( processedColumns[columnIndex] == 0 ) {

            // if not, we look for closest vertex to it
                if ( distances[columnIndex] < closestVertex.distance ) {
                    closestVertex.index = columnIndex;
                    closestVertex.distance = distances[columnIndex];
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
        sprintf(str, "%s\nVertex: %d processed? %d predecessor: %d distance: %f", 
            str, i, processedColumns[i], predecessors[i], distances[i]);
    }

    puts(str);
}

