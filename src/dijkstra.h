/**
*	@file dijkstra.h
*	@brief Operations and data concerning Dijkstra algorithm.
*/

#pragma once

#include <stdio.h>
#include "utility.h"
#include "vertexData.h"


/// <summary>
/// Helper function to allocate memory for shared structures and fill them with 
/// initial values. It should be called just by one process.
/// </summary>
/// <param name="sourceVertex">
/// Index of the source vertex.
/// </param>
/// <param name="size">
/// Size of a single column. It is equal to adjacency matrix dimention.
/// </param>
void initData(const int sourceVertex, const int size);


/// <summary>
/// Helper function to free memory of shared structure. Since the upc_free is 
/// collective it should be called by all processes.
/// </summary>
void freeSharedData();


/// <summary>
/// Function responsible for finding vertex with minimal distance in given columns.
/// </summary>
/// <returns>
/// Found vertex with minimal distance wrapped in VertexData structure.
/// </returns>
/// <param name="cols">
/// Structure of columns that will be searched.
/// </param>
VertexData findVertexWithMinimalDistance(const ColumnsToProcess cols);


/// <summary>
/// Function that checks if all vertices have been processed. Used to determine 
/// the end of algorithm execution.
/// </summary>
/// <returns>
/// Boolean value if vertices have been processed.
/// </returns>
/// <param name="numberOfColumns">
/// Number of columns that are processed.
/// </param>
int checkIfAllVerticesHaveBeenProcessed(const int numberOfColumns);


/// <summary>
/// Function used to run the algorithm by process for given columns.
/// </summary>
/// <param name="cols">
/// Structure of columns that will be processed during algorithm execution by 
/// this process.
/// </param>
void run(const ColumnsToProcess cols);


/// <summary>
/// Function responsible for calculating new distances for processed columns. 
/// If the distance from new chosen vertex to processed vertices is shorter,
/// the new distance and predecessor are saved.
/// </summary>
/// <param name="closestVertex">
/// Vertex with minimal distance wrapped in closestVertex structure. It contains
/// its index and the distance.
/// </param>
/// <param name="cols">
/// Structure of columns that will be searched for new distances.
/// </param>
void performInnerLoop(const VertexData closestVertex, const ColumnsToProcess cols);


/// <summary>
/// Helper function with debug printout with the state of algorithm
/// </summary>
void printState();


/// <summary>
/// Saves algorithm results to file 'results.UPC.txt'
/// </summary>
/// <param name="numberOfColumns">
/// Number of columns that are processed.
/// </param>
/// <param name="sourceVertex">
/// Index of the source vertex.
/// </param>
void printResultToFile(const int numberOfColumns, const int sourceVertex);


/// <summary>
/// Saves found paths to given file.
/// </summary>
/// <param name="fp">
/// Pointer to file to which paths should be saved.
/// </param>
/// <param name="numberOfColumns">
/// Number of columns that are processed.
/// </param>
/// <param name="sourceVertex">
/// Index of the source vertex.
/// </param>
void printPaths(FILE * fp, const int numberOfColumns, const int sourceVertex);


/// <summary>
/// Saves found distances to given file.
/// </summary>
/// <param name="fp">
/// Pointer to file to which distances should be saved.
/// </param>
/// <param name="numberOfColumns">
/// Number of columns that are processed.
/// </param>
/// <param name="sourceVertex">
/// Index of the source vertex.
/// </param>
void printDistances(FILE * fp, const int numberOfColumns, const int sourceVertex);
