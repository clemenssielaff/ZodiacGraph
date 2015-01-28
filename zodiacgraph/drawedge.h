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

#ifndef ZODIAC_DRAWEDGE_H
#define ZODIAC_DRAWEDGE_H

/// \file drawedge.h
///
/// \brief Contains the definition of the zodiac::DrawEdge class.
///

#include "bezieredge.h"

namespace zodiac {

class Scene;

///
/// \brief Edge used to draw a new edge.
///
/// It is a specialized BezierEdge, as it does not have an end Plug and is only used to visually create a PlugEdge in
/// the graph.
///
/// The end point and second control point of the DrawEdge splin eare placed at the same position (usually the mouse
/// cursor), giving the edge a "pointy" end.
///
class DrawEdge : public BezierEdge
{

    Q_OBJECT

public:

    ///
    /// \brief Constructor.
    ///
    /// \param [in] scene   Scene containing this DrawEdge.
    ///
    explicit DrawEdge(Scene* scene);

    ///
    /// \brief Reverses the display of the DrawEdge to accomodate drawing from an \ref zodiac::PlugDirection::IN
    /// "incoming" Plug.
    ///
    /// \param [in] isReverse   <i>true</i> if the DrawEdge originates at an \ref zodiac::PlugDirection::IN "incoming"
    /// Plug -- <i>false</i> otherwise.
    ///
    inline void setReverse(bool isReverse){m_isReverse=isReverse;}

    ///
    /// \brief The DrawEdge is said to be <i>reversed</i>, if it originates in an \ref zodiac::PlugDirection::IN
    /// "incoming" Plug of a Node.
    ///
    /// \return <i>true</i> if the DrawEdge is currently reversed -- <i>false</i> if not.
    ///
    inline bool isReversed() const {return m_isReverse;}

public: // methods

    ///
    /// \brief Used for creating a PlugEdge when its end is not connected to a Plug yet.
    ///
    /// The endPoint is usually the mouse cursor.
    ///
    /// \param [in] plug        Plug, from where the DrawEdge originates.
    /// \param [in] endPoint    Manually defined position of the end point and 2nd ctrl point of this spline.
    ///
    void fromPlugToPoint(Plug* plug, const QPointF& endPoint);

private: // members

    ///
    /// \brief <i>true</i> if this DrawEdge originates in a \ref zodiac::PlugDirection::IN "incoming" Plug.
    ///
    bool m_isReverse;

};

} // namespace zodiac

#endif // ZODIAC_DRAWEDGE_H
