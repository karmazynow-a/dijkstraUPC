
#include "columnsToProcess.h"

double getValueFromColumn ( const ColumnData * col, const int rowIndex, const int rowSize ){
    return *(col->column + rowIndex);
}
