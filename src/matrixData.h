#pragma once

#include <upc.h>


// helper structure to store data about matrix
// possibly to be removed from shared
typedef struct matrixData {
    int matrixDimention;
    shared double * matrixValues;
} MatrixData;
