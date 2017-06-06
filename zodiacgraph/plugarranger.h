//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    The MIT License
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy of
//    this software and associated documentation files (the "Software"), to deal in
//    the Software without restriction, including without limitation the rights to
//    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//    of the Software, and to permit persons to whom the Software is furnished to do so,
//    subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
//

#ifndef ZODIAC_PLUGARRANGER_H
#define ZODIAC_PLUGARRANGER_H

/// \file plugarranger.h
///
/// \brief Contains the definition of the zodiac::arrangePlugs function.
///

#include <QVector>

namespace zodiac {

///
/// \brief Calculates the cheapest path through the cost-table.
///
/// For example:
/// With the given table:
/// ~~~
/// |----|----|----|
/// | 87 | 15 | 75 |
/// | 41 | 32 | 68 |
/// | 93 | 54 | 21 |
/// |----|----|----|
/// ~~~
///
/// This function would produce the Vector [1, 0, 2], indicating that the cheapest path is:
/// ~~~
/// |----|----|----|
/// |    | 15 |    |
/// | 41 |    |    |
/// |    |    | 21 |
/// |----|----|----|
/// ~~~
///
/// \param [in] costTable   Reference to the cost table.
/// \param [in] rowCount    Numer of rows in the cost table.
/// \param [in] columnCount Numer of columns in the cost table.
///
/// \return Vector of column indices, ordered by row.
///
QVector<int> arrangePlugs(const QVector<qreal>& costTable, const int rowCount, const int columnCount);

} // namespace zodiac

#endif // ZODIAC_PLUGARRANGER_H
