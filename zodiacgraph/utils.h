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
