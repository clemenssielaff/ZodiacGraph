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

#ifndef ZODIAC_NODEUTILS_H
#define ZODIAC_NODEUTILS_H

///
/// \file utils.h
///
/// \brief Common utils and enums that are used throughout the code.
///

#include <QRectF>

namespace zodiac {

///
/// \brief Z-Positions of items in the node view.
///
/// Is not an enum class, because this enum is often used with QGraphicItem's setZValue(), which requires an integer.
/// And using a pure enum is easier.
///
enum zStack {
    EDGE            = -10,  ///< Edges are all the way in the background.
    NODE_CLOSED     = 0,    ///< A closed Node is the base line depth at zero.
    NODE_EXPANDED   = 10,   ///< An expanded Node automatically overlays a closed one.
    NODE_ACTIVE     = 20,   ///< The active Node (the last selected one) overlays other expanded Nodes.
    EDGE_LABEL      = 30,   ///< EdgeLabel%s overlay all Node%s.
    DRAW_EDGE       = 40    ///< The DrawEdge is drawn in front of overthing.
};

} // namespace zodiac

///
/// \brief Constructs a quadrat with a given side length.
///
/// Solely a convenience function as the Zodiac Graph design requires many quadratic QRectF%s of various sidelengths.
///
/// \param sidelength   Length of one side of the quadrat
///
/// \return             Rectangle around zero with given side length.
///
inline QRectF quadrat(qreal sidelength){
    return QRectF(-sidelength, -sidelength, sidelength*2, sidelength*2);
}

#endif // ZODIAC_NODEUTILS_H
