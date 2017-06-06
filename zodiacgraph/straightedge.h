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
