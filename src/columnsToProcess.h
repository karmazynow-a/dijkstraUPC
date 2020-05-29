#pragma once

// helper structures for processing the columns

// data concerning single column
typedef struct columnData {
    // index in global matrix
    int index;

    // pointer to the beginning of column in matrix
    //TODO change to local
    shared double * begin;
} ColumnData;

// columns for each process
typedef struct colsToProcess {
    // number of columns to process by process
    int numberOfColumns;

    // size of single column - is equal to number of columns
    int columnSize;

    // list of beginnings of columns in matrix
    ColumnData * data;
} ColumnsToProcess;
