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

#ifndef ZODIAC_BEZIEREDGE_H
#define ZODIAC_BEZIEREDGE_H

/// \file bezieredge.h
///
/// \brief Contains the definition of the zodiac::BezierEdge class.
///

#include "baseedge.h"

namespace zodiac {

class Plug;
class Scene;

///
/// \brief The base class for round edges in the graph: PlugEdge and DrawEdge.
///
/// Other than the StraightEdge branch of the inheritance tree, this class uses a
/// <a href="http://en.wikipedia.org/wiki/B%C3%A9zier_curve/">cubic bezier curve</a> for drawing its edge.
///
/// The position of the two control points (the ones in the middle) of the cubic bezier edge are calculated using
/// the normal of their start and end Plug%s and the curve's arclength.
/// Depending on the arclength of the curve, the control points are moved out from their respective end point in the
/// direction of the Plug's normal until they have reached the \ref zodiac::BezierEdge::s_maxCtrlDistance
/// "maximal control point distance".
/// This is the distance between one of the end points of a BezierEdge and its nearest control point.
/// Smaller numbers produce a sharper kink of the edge near its Plug, larger numbers produce rounder, larger bends.
/// In the extremes, a value of zero would produce a straight edge, a very large value would produce very pronounced
/// "S-shapes".
///
/// The ratio between arclength and expansion is controlled using the \ref zodiac::BezierEdge::s_ctrlExpansionFactor
/// "control point expansion factor".
/// A value of 0 would mean that the control points never leave their respective end points.
/// A very high value would cause them to jump out to their maximal distance as soon as the edge is stretched to a
/// minimal length.
///
/// For the calculation of the spline's arlength, we are using the
/// <a href="https://en.wikipedia.org/wiki/Manhattan_distance/">Manhattan distance</a> between its start and end point.
/// If we were to use the actual spline arclength, the shape of the path would influence its length would influence its
/// shape etc.
/// This works eventually, but the spline requires a few iterations (=redraws) to settle down.
/// Using the manhattan distance is visually similar, faster and works immediately
///
/// Although the class is non-virtual, it is not intended to be instantiated as-is.
/// Therefore its constructor is protected, so only derived classes can use it.
///
class BezierEdge : public BaseEdge
{

    Q_OBJECT

public: // methods

    ///
    /// \brief Moves the EdgeArrow along the BezierEdge to a given fraction of the edge's arclength.
    ///
    /// \param [in] fraction    Fraction [0 -> 1] of the edges arclength at which to place the arrow.
    ///
    void placeArrowAt(qreal fraction) override;

public: // static methods

    ///
    /// \brief The maximal ctrl distance is the distance between one of the end points of a BezierEdge and its nearest
    /// control point.
    ///
    /// \return  Maximal distance from the ctrl point to the Plug in pixels.
    ///
    static inline qreal getMaxCtrlDistance() {return s_maxCtrlDistance;}

    ///
    /// \brief Sets a new maximal distance from the ctrl point to the plug in pixels.
    ///
    /// \param [in] distance    New maximal ctrl point distance to the plug, at least 0.
    ///
    static inline void setMaxCtrlDistance(qreal distance) {s_maxCtrlDistance=qMax(0.,distance);}

    ///
    /// \brief The ratio between the BezierEdge's arclength and the expansion of its control points.
    ///
    /// \return  Control point expansion factor.
    ///
    static inline qreal getCtrlExpansionFactor() {return s_ctrlExpansionFactor;}

    ///
    /// \brief Sets a new ratio between the BezierEdge's arclength and the expansion of its control points.
    ///
    /// \param [in] factor  New control point expansion factor.
    ///
    static inline void setCtrlExpansionFactor(qreal factor) {s_ctrlExpansionFactor=factor;}

protected: // methods

    ///
    /// \brief Constructor.
    ///
    /// Protected, so a BezierEdge cannot be instantiated, as it does not have any meaningful functionality by itself.
    ///
    /// \param [in] scene   Scene containing this edge.
    ///
    explicit BezierEdge(Scene *scene);

    ///
    /// \brief Updates the shape of this edge.
    ///
    virtual void updateShape() override;

    ///
    /// \brief Returns the position of the control point of the edge for a given Plug.
    ///
    /// This works regardless if it is the start or the end Plug of this edge.
    /// Calling this function requires the start and end point of the edge to be set correctly to calculate the spline's
    /// arclength.
    ///
    /// \param [in] plug    Plug to calculate the control point position for.
    ///
    /// \return             %Scene coordinates of the control point.
    ///
    QPointF getCtrlPointFor(Plug* plug);

protected: // members

    ///
    /// \brief Start point of the BezierEdge in scene coordinates.
    ///
    QPointF m_startPoint;

    ///
    /// \brief First control point of the BezierEdge in scene coordinates.
    ///
    QPointF m_ctrlPoint1;

    ///
    /// \brief Second control point of the BezierEdge in scene coordinates.
    ///
    QPointF m_ctrlPoint2;

    ///
    /// \brief End point of the BezierEdge in scene coordinates.
    ///
    QPointF m_endPoint;

private: // static members

    ///
    /// \brief Maximal distance of a control point from its respective end point.
    ///
    static qreal s_maxCtrlDistance;

    ///
    /// \brief Factor by which the control point of is moved outward along the normal of its Plug, based on the edge's
    /// arclengh.
    ///
    static qreal s_ctrlExpansionFactor;
};

} // namespace zodiac

#endif // ZODIAC_BEZIEREDGE_H
