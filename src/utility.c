#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#include "utility.h"

MatrixData readDataAsContinuousMemory(const char * fileName){
    FILE * fp = fopen(fileName, "r");

    MatrixData m  = {-1, NULL};

    if (fp == NULL) {
        printf("Brak pliku %s !\n", fileName);
        return m;
    }

    int matrixSize = -1;
    fscanf(fp, "%d\n", &matrixSize);

    printf("Rozmiar macierzy = %d\n", matrixSize);

    m.matrixDimention = matrixSize;
    m.matrixValues = upc_global_alloc(matrixSize * matrixSize, sizeof(double));

    // scan by rows
    shared double * matrixPtr = m.matrixValues;
    for (int i = 0; i < matrixSize*matrixSize; ++i) {
        // save by columns not by rows
        int idx = i/matrixSize + matrixSize*(i%matrixSize);
        
        char tmp[250];
        fscanf(fp, "%s ", tmp);
        matrixPtr[idx] = atof(tmp);
    }

    fclose(fp);

    return m;
}

char * getInputFileName(int argc, char *argv[]) {
    if (argc < 3) {
        puts("Nie podano pliku - domyślny");
        return "data/graph.dat";
    }
    return argv[2];
}

int getSourceVertex(int argc, char *argv[]) {
    if (argc < 2) {
        puts("Nie podano wierzchołka - domyślny");
        return 0;
    }

    return atoi(argv[1]);
}   

void printColumn( ColumnsToProcess col, int columnIndex ){
    char str [250];
    sprintf(str, "Poszukiwanie kolumny %d", columnIndex);

    for (int i = 0; i < col.numberOfColumns; ++i){
        if (col.data[i].index == columnIndex ) {

            for (int j = 0; j < col.columnSize; ++j){
                sprintf(str, "%s \n %f", str, col.data[i].begin[j]);
            }

            puts(str);
            return;
        }
    }

    strcat(str, "\n  Nie znaleziono!");
    puts(str);
}
