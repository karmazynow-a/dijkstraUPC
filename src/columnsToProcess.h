#pragma once

// helper structures for processing the columns

// data concerning single column
typedef struct columnData {
    // index in global matrix
    int columnIndex;

    // pointer to the beginning of column in matrix
    shared [] double * column;
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

// get matrix value from column
double getValueFromColumn ( const ColumnData * col, const int rowIndex, const int rowSize );
