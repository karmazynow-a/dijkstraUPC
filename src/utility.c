#include <stdio.h>
#include <stdlib.h>
#include <upc.h>

#include "utility.h"

int readMatrixSize(const char * fileName){
    FILE * fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Brak pliku %s !\n", fileName);
        return -1;
    }

    int matrixSize = -1;
    fscanf(fp, "%d\n", &matrixSize);

    printf("Rozmiar macierzy = %d\n", matrixSize);

    fclose(fp);

    return matrixSize;
}


void readDataAsContinuousMemory(const char * fileName, shared sdblptr columnData [THREADS]){
    FILE * fp = fopen(fileName, "r");

    if (fp == NULL) {
        printf("Brak pliku %s !\n", fileName);
        return;
    }

    int matrixSize = -1;
    fscanf(fp, "%d\n", &matrixSize);

    shared sdblptr * dirPtr = columnData;
    for (int i = 0; i < matrixSize; ++i){
        for (int j = 0; j < matrixSize; ++j){
            char tmp[10];
            fscanf(fp, "%s ", tmp);
            dirPtr[j%THREADS][i + (j/THREADS) * matrixSize] = atof(tmp);
        }
    }

    fclose(fp);
}

char * getInputFileName(const int argc, char *argv[]) {
    if (argc < 3) {
        puts("Nie podano pliku - domyślny");
        return "data/graph.dat";
    }
    return argv[2];
}


int getSourceVertex(const int argc, char *argv[]) {
    if (argc < 2) {
        puts("Nie podano wierzchołka - domyślny");
        return 0;
    }

    return atoi(argv[1]);
}   


void printColumn( const ColumnsToProcess * col, const int columnIndex ){
    char str [250];
    sprintf(str, "Poszukiwanie kolumny %d", columnIndex);

    for (int i = 0; i < col->numberOfColumns; ++i){
        if (col->data[i].columnIndex == columnIndex ) {

            for (int j = 0; j < col->columnSize; ++j){
                sprintf(str, "%s \n %f", str, col->data[i].column[j]);
            }

            puts(str);
            return;
        }
    }

    strcat(str, "\n  Nie znaleziono!");
    puts(str);
}


void printColumnInfo( const ColumnsToProcess * col ){
    char str [250];
    sprintf(str, "Proces %d posiada %d kolumn o indeksach: ", MYTHREAD, col->numberOfColumns);
    for (int i = 0; i < col->numberOfColumns; ++i){
        sprintf(str, "%s %d, ", str, col->data[i].columnIndex);
    }

    puts(str);
}
