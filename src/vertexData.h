/**
*	@file vertexData.h
*	@brief Structure concerning single vertex.
*/

#pragma once

/// <summary>
/// This structure represents single vertex.
/// </summary>
typedef struct vertexData {
    /// <summary>
    /// Integer that represents index of vertex in the graph.
    /// </summary>
    int index;
    /// <summary>
    /// Current distance to vertex.
    /// </summary>
    double distance;
} VertexData;
