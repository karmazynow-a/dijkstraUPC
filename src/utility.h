/**
*	@file utility.h
*	@brief File with helper functions to manage program execution.
*/

#pragma once

#include "columnsToProcess.h"

/// <summary>
/// Typedef for shared pointer, used in memory directory.
/// </summary>
typedef shared [] double * sdblptr;


/// <summary>
/// Function responsible for reading matrix dimention from input file.
/// If file does not exist it returns -1.
/// </summary>
/// <returns>
/// Matrix dimention.
/// </returns>
/// <param name="fileName">
/// Name of input file.
/// </param>
int readMatrixSize(const char * fileName);


/// <summary>
/// Function responsible for reading adjacency matrix values from input file. 
/// They are saved as continuous memory by columns into arrays for corresponding
/// processes. The index of process is determined by vertex index.
/// </summary>
/// <param name="fileName">
/// Name of input file.
/// </param>
/// <param name="columnData">
/// Memory dictionary, that helds data for corresponding threads. Memory should 
/// already be allocated.
/// </param>
void readDataAsContinuousMemory(const char * fileName, shared sdblptr columnData [THREADS]);


/// <summary>
/// Retrieve filename from input arguments. If it is not present
/// function returns NULL.
/// </summary>
/// <returns>
/// Filename.
/// </returns>
/// <param name="argc">
/// Number of input arguments.
/// </param>
/// <param name="argv">
/// Array of input arguments.
/// </param>
char * getInputFileName(const int argc, char *argv[]);


/// <summary>
/// Retrieve source vertex from input arguments. If it is not present
/// function returns -1.
/// </summary>
/// <returns>
/// Index of source vertex.
/// </returns>
/// <param name="argc">
/// Number of input arguments.
/// </param>
/// <param name="argv">
/// Array of input arguments.
/// </param>
int getSourceVertex(const int argc, char *argv[]);


/// <summary>
/// Helper function to print the column from ColumnsToProcess with given index.
/// </summary>
/// <param name="cols">
/// Structure of columns that will be processed during algorithm execution by 
/// this process.
/// </param>
/// <param name="columnIndex">
/// Index of column in original adjacency matrix.
/// </param>
void printColumn( const ColumnsToProcess * cols, const int columnIndex );


/// <summary>
/// Function responsible for printing data concerning ColumnsToProcess structure: 
/// number of columns and its indexes for particular process.
/// </summary>
/// <param name="cols">
/// Structure of columns that will be processed during algorithm execution by 
/// this process.
/// </param>
void printColumnInfo( const ColumnsToProcess * cols );
