//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
