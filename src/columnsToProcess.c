
#include "columnsToProcess.h"

double getValueFromColumn ( const ColumnData * column, const int rowIndex ){
    return *(column->column + rowIndex);
}
