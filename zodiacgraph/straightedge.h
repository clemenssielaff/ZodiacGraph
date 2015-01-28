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

#ifndef ZODIAC_STRAIGHTEDGE_H
#define ZODIAC_STRAIGHTEDGE_H

///
/// \file straightedge.h
///
/// \brief Contains the definition of the zodiac::StraightEdge class.
///

#include "baseedge.h"

namespace zodiac {

class Node;
class Scene;
class EdgeGroupInterface;

///
/// \brief Connects two nodes in a straight line.
///
/// StraightEdge%s are used by EdgeGroup%s.
///
class StraightEdge : public BaseEdge
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// Registers this StraightEdge with its connected Node instances.
    ///
    /// \param [in] scene      Scene containing this edge.
    /// \param [in] group      EdgeGroup or EdgeGroupPair containing this edge.
    /// \param [in] fromNode   Start Node of this edge.
    /// \param [in] toNode     End Node of this edge.
    ///
    explicit StraightEdge(Scene* scene, EdgeGroupInterface* group, Node* fromNode, Node* toNode);

    ///
    /// \brief Is called from a Node to notify a connected StraightEdge of a change in position.
    ///
    void nodePositionHasChanged();

    ///
    /// \brief Generates and updates the text for this edge's EdgeLabel.
    ///
    void updateLabel();

    ///
    /// \brief The Node from which the PlugEdge%s in the EdgeGroup originate.
    ///
    /// \return Start Node of this StraightEdge.
    ///
    inline Node* getFromNode() const {return m_fromNode;}

    ///
    /// \brief The Node to which the PlugEdge%s in the EdgeGroup lead.
    ///
    /// \return End Node of this StraightEdge.
    ///
    inline Node* getToNode() const {return m_toNode;}

    ///
    /// \brief Places the EdgeArrow along the edge to a given fraction of the arclength.
    ///
    /// \param [in] fraction    Fraction of arclength at which to place the arrow.
    ///
    void placeArrowAt(qreal fraction) override;

signals:

    ///
    /// \brief Clicking this edge with the removal button emits this signal to be caught by the appropriate EdgeGroup.
    ///
    void removalRequested();

protected: // methods

    ///
    /// \brief Updates the shape of the StraightEdge.
    ///
    virtual void updateShape() override;

    ///
    /// \brief Called, when the mouse is pressed as the cursor is on this item.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    ///
    /// \brief Called when this item is double-clicked.
    ///
    /// The user can expand both Node%s connected through this StraightEdge by double-clicking it.
    ///
    /// \param [in] event   Qt event object.
    ///
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

protected: // members

    ///
    /// \brief EdgeGroup that this StraightEdge represents.
    ///
    EdgeGroupInterface* m_group;

    ///
    /// \brief The start Node of this StraightEdge.
    ///
    Node* m_fromNode;

    ///
    /// \brief The end Node of this StraightEdge.
    ///
    Node* m_toNode;

    ///
    /// \brief Start point of the StraightEdge in scene coordinates.
    ///
    QPointF m_startPoint;

    ///
    /// \brief End point of the StraightEdge in scene coordinates.
    ///
    QPointF m_endPoint;

};

} // namespace zodiac

#endif // ZODIAC_STRAIGHTEDGE_H
