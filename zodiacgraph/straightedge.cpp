#include "straightedge.h"

#include <QGraphicsSceneMouseEvent>
#include <QtMath>

#include "edgearrow.h"
#include "edgegroupinterface.h"
#include "node.h"
#include "view.h"

namespace zodiac {

StraightEdge::StraightEdge(Scene* scene, EdgeGroupInterface* group, Node* fromNode, Node* toNode)
    : BaseEdge(scene)
    , m_group(group)
    , m_fromNode(fromNode)
    , m_toNode(toNode)
    , m_startPoint(QPointF())
    , m_endPoint(QPointF())
{
    Q_ASSERT(fromNode!=toNode);

    // register with the nodes
    fromNode->addStraightEdge(this);
    toNode->addStraightEdge(this);

    // initialize the shape of the edge
    nodePositionHasChanged();
}

void StraightEdge::nodePositionHasChanged()
{
    // return early, if the shape has not changed
    QPointF startPoint = m_fromNode->scenePos();
    QPointF endPoint = m_toNode->scenePos();
    if((startPoint==m_startPoint)&&(endPoint==m_endPoint)){
        return;
    }

    // update the edge's ctrl points
    m_startPoint = startPoint;
    m_endPoint = endPoint;

    updateShape();
}

void StraightEdge::updateLabel()
{
    setLabelText(m_group->getLabelText());
    placeArrowAt(.5);
}

void StraightEdge::placeArrowAt(qreal fraction)
{
    QPointF delta = m_endPoint-m_startPoint;
    QPointF centerPoint = m_startPoint + (delta*fraction);
    qreal angle = qAtan2(delta.y(), delta.x());
    m_arrow->setTransformation(centerPoint, angle);
}

void StraightEdge::updateShape()
{
    prepareGeometryChange();

    // update the path
    QPainterPath straightLine;
    straightLine.moveTo(m_startPoint);
    straightLine.lineTo(m_endPoint);
    m_path.swap(straightLine);

    // update the arrow
    placeArrowAt(.5);
}

void StraightEdge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->buttons() & View::getRemovalButton()){
        event->accept();
        emit removalRequested();
    } else {
        BaseEdge::mousePressEvent(event);
    }
}

void StraightEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & View::getSelectionButton()){
        m_fromNode->softSetExpansion(NodeExpansion::BOTH);
        m_toNode->softSetExpansion(NodeExpansion::BOTH);
    }
    BaseEdge::mouseDoubleClickEvent(event);
}

} // namespace zodiac
