/**
*	@file columnsToProcess.h
*	@brief This file contains helper structures manage processed columns.
*/

#pragma once

/// <summary>
/// This structure contains data concerning single column.
/// </summary>
typedef struct columnData {
    /// <summary>
    /// Index of the column in original adjacency matrix. Its value is the same as
    /// the index of vertex in the graph.
    /// </summary>
    int columnIndex;

    /// <summary>
    /// Pointer to the beggining of column in shared directory.
    /// </summary>
    shared [] double * column;
} ColumnData;


/// <summary>
/// The structure that holds all columns processed by the process.
/// </summary>
typedef struct colsToProcess {
    /// <summary>
    /// The amount of processed columns.
    /// </summary>
    int numberOfColumns;

    /// <summary>
    /// The size of a single column.
    /// </summary>
    int columnSize;

    /// <summary>
    /// The list of columns that will be processed.
    /// </summary>
    ColumnData * data;
} ColumnsToProcess;


/// <summary>
/// Helper function to retrieve distance in column for given vertex.
/// </summary>
/// <returns>
/// Distance in given column to given vertex.
/// </returns>
/// <param name="column">
/// Pointer to column wrapped in ColumnData structure from which the distance 
/// will be retrieved.
/// </param>
/// <param name="rowIndex">
/// Index of the vertex to which the distance will be found.
/// </param>
double getValueFromColumn ( const ColumnData * column, const int rowIndex );
