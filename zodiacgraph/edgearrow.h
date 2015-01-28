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

#ifndef ZODIAC_EDGEARROW_H
#define ZODIAC_EDGEARROW_H

///
/// \file edgearrow.h
///
/// \brief Contains the definition of the zodiac::EdgeArrow class and zodiac::ArrowKind enum.
///

#include <QGraphicsObject>

namespace zodiac {

class EdgeLabel;
class BaseEdge;

///
/// \brief There are currently 2 visually different types of EdgeArrow%s used in the graph.
///
/// Even though they are drawn differently, both are functionally the same.
/// The only difference is the original arrow shape they use for generating their own. <br>
/// This enum is a numeration of these original arrow shapes.
///
enum class ArrowKind {
    SINGLE = 0, ///< A single triangle pointing into the direction of the edge.
    DOUBLE = 1, ///< Two mirrored triangles, pointing along different directions of the edge.
};

///
/// \brief The EdgeArrow is an additional item of the BaseEdge, identifying its direction.
///
/// It is placed at the middle of the BaseEdge and comes in two flavors: \ref zodiac::ArrowKind::SINGLE "single" and
/// \ref zodiac::ArrowKind::DOUBLE "double".
///
class EdgeArrow : public QGraphicsObject
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    explicit EdgeArrow(BaseEdge* edge);

    ///
    /// \brief Sets the transformation of this arrow.
    ///
    /// \param [in] pos     Position.
    /// \param [in] angle   Angle in radians.
    ///
    void setTransformation(const QPointF& pos, qreal angle);

    ///
    /// \brief Assings a label to this arrow.
    ///
    /// If a BaseEdge has an EdgeLabel, it transforms with the EdgeArrow.
    /// This way, the somewhat expensive calculation of a position along a spline is only performed once.
    ///
    /// \param [in] label   EdgeLabel assigned to this EdgeArrow, can be <i>nullptr</i> to remove an existing label.
    ///
    inline void setLabel(EdgeLabel* label) {m_label=label;}

    ///
    /// \brief Sets the ArrowKind of this EdgeArrow.
    ///
    /// \param [in] kind    New kind of arrow.
    ///
    inline void setKind(ArrowKind kind) {m_kind=kind;}

public: // static methods

    ///
    /// \brief The length of the EdgeArrow in pixels.
    ///
    /// Is used for the calculation of the \ref zodiac::ArrowKind::SINGLE "single" and \ref zodiac::ArrowKind::DOUBLE
    /// "double"  arrow.
    ///
    /// \return Arrow length in pixels.
    ///
    inline static qreal getArrowLength() {return s_arrowHalfLength * 2;}

    ///
    /// \brief The width of the EdgeArrow in pixels.
    ///
    /// Is used for the calculation of the \ref zodiac::ArrowKind::SINGLE "single" and \ref zodiac::ArrowKind::DOUBLE
    /// "double"  arrow.
    ///
    /// \return Arrow width in pixels.
    ///
    inline static qreal getArrowWidth() {return s_arrowHalfWidth * 2;}

    ///
    /// \brief (Re-)defines the two original arrows.
    ///
    /// \param [in] length  Length of the arrow in pixels (unscaled).
    /// \param [in] width   Width of the arrow in pixels (unscaled).
    ///
    static void defineArrow(qreal length, qreal width);

    ///
    /// \brief The fill color of all EdgeArrow%s.
    ///
    /// \return The arrow's fill color.
    ///
    inline static QColor getArrowColor() {return s_arrowColor;}

    ///
    /// \brief Sets a new color for all EdgeArrow%s.
    ///
    /// \param [in] color   New arrow fill color.
    ///
    inline static void setArrowColor(const QColor& color) {s_arrowColor=color;}

protected: // methods

    ///
    /// \brief Rectangular outer bounds of the item, used for redraw testing.
    ///
    /// \return Boundary rectangle of the item.
    ///
    QRectF boundingRect() const;

    ///
    /// \brief Paints this item.
    ///
    /// \param [in] painter Painter used to paint the item.
    /// \param [in] option  Provides style options for the item.
    /// \param [in] widget  Optional widget that this item is painted on.
    ///
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    ///
    /// \brief Exact boundary of the item used for collision detection among other things.
    ///
    /// \return         Shape in local coordinates.
    ///
    QPainterPath shape() const;

    ///
    /// \brief Called when this item is double-clicked.
    ///
    /// Manually passes the event on to the BaseEdge parenting this EdgeArrow.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private: // members

    ///
    /// \brief BaseEdge owning this EdgeArrow.
    ///
    BaseEdge* m_edge;

    ///
    /// \brief The polygon used to draw the arrow.
    ///
    QPolygonF m_arrowPolygon;

    ///
    /// \brief The kind of this EdgeArrow, defaults to \ref zodiac::ArrowKind::SINGLE "single".
    ///
    ArrowKind m_kind;

    ///
    /// \brief EdgeLabel assigned to this arrow or <i>nullptr</i>.
    ///
    EdgeLabel* m_label;

private: // static members

    ///
    /// \brief Size of the gap between the two lines of a \ref zodiac::ArrowKind::DOUBLE "double" arrow.
    ///
    static qreal s_doubleGap;

    ///
    /// \brief Half the length of the edge arrow along the direction it is pointing.
    ///
    static qreal s_arrowHalfLength;

    ///
    /// \brief Half the width of the arrow perpendicular to the direction it is pointing.
    ///
    static qreal s_arrowHalfWidth;

    ///
    /// \brief Fill color of the edge arrow.
    ///
    static QColor s_arrowColor;

    ///
    /// \brief Original \ref zodiac::ArrowKind::SINGLE "single" arrow.
    ///
    /// Is shared by all edge arrows as base for their own.
    ///
    static QPolygonF s_originalArrow;

    ///
    /// \brief Original \ref zodiac::ArrowKind::DOUBLE "double" arrow.
    ///
    /// Is shared by all edge arrows as base for their own.
    ///
    static QPolygonF s_originalDoubleArrow;

};

} // namespace zodiac

#endif // ZODIAC_EDGEARROW_H
