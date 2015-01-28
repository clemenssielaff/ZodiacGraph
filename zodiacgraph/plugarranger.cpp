#include "plugarranger.h"

#include <cfloat>       // DBL_MAX

int getProblemRows(QVector<int>& problemRows, const QVector<int>& guess, const int rowCount)
{
    int count = 0;
    for(int guessIndex = 0; guessIndex < rowCount; ++guessIndex){
        if(guess.count(guess[guessIndex]) > 1){
            problemRows[count++] = guessIndex;
        }
    }
    return count;
}

namespace zodiac {

QVector<int> arrangePlugs(const QVector<qreal>& costTable, const int rowCount, const int columnCount)
{
    //
    // create best possible solution as first guess - is most likely invalid
    QVector<int> guess = QVector<int>(rowCount);
    for(int row = 0; row < rowCount; ++row){
        qreal minCost = DBL_MAX;
        for(int col = 0; col < columnCount; ++col){
            qreal cell = costTable[(columnCount*row)+col];
            if(cell < minCost){
                minCost = cell;
                guess[row] = col;
            }
        }
    }

    //
    // find rows that have the smallest cost in the same column
    QVector<int> problemRows = QVector<int>(rowCount);
    int problemRowCount = getProblemRows(problemRows, guess, rowCount);

    //
    // find the empty columns
    QVector<int> emptyColumns = QVector<int>(columnCount-1);
    int emptyColumnCount = 0;
    for(int col = 0; col < columnCount; ++col){
        if(!guess.contains(col)){
            emptyColumns[emptyColumnCount++] = col;
        }
    }

    QVector<qreal> deltaTable = QVector<qreal>(rowCount * (columnCount-1));
    while(problemRowCount > 0){

        //
        // create the delta table
        for(int rowIndex = 0; rowIndex < problemRowCount; ++rowIndex){
            int row = problemRows[rowIndex];
            for(int colIndex = 0; colIndex < emptyColumnCount; ++colIndex){
                int col = emptyColumns[colIndex];
                deltaTable[(emptyColumnCount*rowIndex)+colIndex] =
                        costTable[(columnCount*row)+col] - costTable[(columnCount*row)+guess[row]];
            }
        }

        //
        // improve the guess by one problem row
        {
            qreal minCost = DBL_MAX;
            int inRow;
            int inCol;
            for(int row = 0; row < problemRowCount; ++row){
                for(int col = 0; col < emptyColumnCount; ++col){
                    qreal cell = deltaTable[(emptyColumnCount*row)+col];
                    if(cell < minCost){
                        minCost = cell;
                        inRow = row;
                        inCol = col;
                    }
                }
            }
            guess[problemRows[inRow]] = emptyColumns[inCol];

            // remove the empty column
            --emptyColumnCount;
            for(int col = inCol; col < emptyColumnCount; ++col){
                emptyColumns[col] = emptyColumns[col+1];
            }
        }

        // update the problemRows
        problemRowCount = getProblemRows(problemRows, guess, rowCount);
    }

    //
    // trivial optimization by swapping where favorable
    for(int leftIndex = 0; leftIndex < rowCount; ++leftIndex){
        for(int rightIndex = leftIndex+1; rightIndex < rowCount; ++rightIndex){
            qreal currentSum = costTable[(columnCount*leftIndex)+guess[leftIndex]] +
                    costTable[(columnCount*rightIndex)+guess[rightIndex]];
            qreal swapSum = costTable[(columnCount*leftIndex)+guess[rightIndex]] +
                    costTable[(columnCount*rightIndex)+guess[leftIndex]];
            if(swapSum < currentSum){
                qreal temp = guess[leftIndex];
                guess[leftIndex] = guess[rightIndex];
                guess[rightIndex] = temp;
            }
        }
    }

    return guess;
}


} // namespace zodiac
