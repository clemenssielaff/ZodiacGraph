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

#ifndef ZODIAC_PERIMETER_H
#define ZODIAC_PERIMETER_H

///
/// \file perimeter.h
///
/// \brief Contains the definition of the zodiac::Perimeter class.
///

#include <QGraphicsObject>

namespace zodiac {

class Node;
class Plug;

///
/// \brief Perimeter around the Node, only appears once mouse has entered the Node.
///
class Perimeter : public QGraphicsObject
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor
    ///
    /// \param [in] parent  Node to which this Perimeter belongs.
    ///
    explicit Perimeter(Node* parent);

    ///
    /// \brief The radius of this Perimeter.
    ///
    /// \return Perimeter radius.
    ///
    inline qreal getRadius() const {return m_radius;}

    ///
    /// \brief Sets a new radius for this Perimeter.
    ///
    /// If the radius is less then the minimal radius, the minimal radius is used instead.
    /// See \ref zodiac::Perimeter::setMinRadius() "setMinRadius()" for details.
    ///
    /// \param [in] radius  New radius.
    ///
    inline void setRadius(qreal radius) {prepareGeometryChange(); m_radius=qMax(s_minRadius, radius);}

    ///
    /// \brief Applies style changes in the class' static members to this instance.
    ///
    /// Is part of the scene-wide cascade of %updateStyle()-calls after a re-styling of the ZodiacGraph.
    ///
    inline void updateStyle() {update();}

public: // static methods

    ///
    /// \brief The minimal radius of a Perimeter.
    ///
    /// \return Minimal Perimeter radius.
    ///
    inline static qreal getMinRadius() {return s_minRadius;}

    ///
    /// \brief Sets a new minimal radius for Perimeter%s.
    ///
    /// No matter how few Plug%s are on the Node, the Perimeter will not shrink past this radius.
    ///
    /// \param [in] radius   New minimal radius of a Perimeter.
    ///
    inline static void setMinRadius(qreal radius) {s_minRadius = qMax(0., radius);}

    ///
    /// \brief The final opacity [0 -> 1] of the Perimeter after fading in.
    ///
    /// \return Final opacity [0 -> 1].
    ///
    inline static qreal getMaxOpacity() {return s_maxOpacity;}

    ///
    /// \brief Sets a new final opacity [0 -> 1] of the Perimeter after fading in.
    ///
    /// \param [in] opacity  New final opacity [0 -> 1].
    ///
    inline static void setMaxOpacity(qreal opacity) {s_maxOpacity = qMax(0., qMin(1., opacity));}

    ///
    /// \brief Color used to fill the Perimeter.
    ///
    /// \return Fill color.
    ///
    inline static QColor getColor() {return s_color;}

    ///
    /// \brief Sets a new color used to draw the Perimeter.
    ///
    /// \param [in] color   New color used to draw the Perimeter.
    ///
    inline static void setColor(QColor color) {s_color=color;}

    ///
    /// \brief The currently closest Plug to the mouse cursor of this Perimeter, if there is any -- otherwise
    /// <i>nullptr</i>.
    ///
    /// \return The closest Plug to the mouse or <i>nullptr</i>.
    ///
    inline static const Plug* getClosestPlugToMouse() {return s_closestPlugToMouse;}

    ///
    /// \brief Sets the pointer holding the closest Plug to the mouse cursor to <i>nullptr</i>.
    ///
    /// Useful, when deleting a Plug to ensure that no dangling pointers are left.
    ///
    inline static void clearClosestPlugToMouse() {s_closestPlugToMouse=nullptr;}

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
    /// \return Shape in local coordinates.
    ///
    QPainterPath shape() const;

    ///
    /// \brief Called when the mouse moves while hovering the shape of the item.
    ///
    /// \param [in] event   Qt event object
    ///
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);

    ///
    /// \brief Called when the mouse leaves the item after hovering over it.
    ///
    /// \param [in] event   Qt event object
    ///
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    ///
    /// \brief Is called, when the mouse is pressed while the cursor is on this item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    ///
    /// \brief Called, when the mouse is moved after clicking on the item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called if the mouse is released after a click on this item.
    ///
    /// Is always called after \ref zodiac::Perimeter::mousePressEvent "mousePressEvent" and possibly
    /// \ref zodiac::Perimeter::mousePressEvent "mousePressEvent".
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    ///
    /// \brief Called when this item is doubleclicked.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private: // members

    ///
    /// \brief Node owning this Perimeter.
    ///
    Node* m_node;

    ///
    /// \brief Radius of this Perimeter.
    ///
    qreal m_radius;

private: // static members

    ///
    /// \brief Minimal radius for all Perimeter%s in pixels.
    ///
    static qreal s_minRadius;

    ///
    /// \brief Maximal opacity of all Perimeter%s.
    ///
    static qreal s_maxOpacity;

    ///
    /// \brief Color used to fill the Perimeter.
    ///
    static QColor s_color;

    ///
    /// \brief The closest Plug to the position of the mouse cursor.
    ///
    static Plug* s_closestPlugToMouse;

    ///
    /// \brief Used to determine whether a mouse click was a click or a drag.
    ///
    static bool s_mouseWasDragged;

};

} // namespace zodiac

#endif // ZODIAC_PERIMETER_H
