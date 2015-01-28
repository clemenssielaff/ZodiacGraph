#include "edgearrow.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QtMath>

#include "edgelabel.h"
#include "baseedge.h"

namespace zodiac {

qreal EdgeArrow::s_doubleGap = 2.5; // same as BaseEdge::s_width
qreal EdgeArrow::s_arrowHalfLength = 7.5;
qreal EdgeArrow::s_arrowHalfWidth = 6.;
QColor EdgeArrow::s_arrowColor = QColor("#cc5d4e");
QPolygonF EdgeArrow::s_originalArrow = QPolygonF()
        << QPointF(s_arrowHalfLength, 0.)
        << QPointF(-s_arrowHalfLength, -s_arrowHalfWidth)
        << QPointF(-s_arrowHalfLength, s_arrowHalfWidth);
QPolygonF EdgeArrow::s_originalDoubleArrow = QPolygonF()
        << QPointF(s_arrowHalfLength, s_doubleGap)
        << QPointF(-s_arrowHalfLength, s_doubleGap)
        << QPointF(-s_arrowHalfLength, s_arrowHalfWidth + s_doubleGap)
        << QPointF(s_arrowHalfLength, s_doubleGap)
        << QPointF(-s_arrowHalfLength, -s_doubleGap)
        << QPointF(s_arrowHalfLength, -s_arrowHalfWidth - s_doubleGap)
        << QPointF(s_arrowHalfLength, -s_doubleGap)
        << QPointF(-s_arrowHalfLength, -s_doubleGap);

EdgeArrow::EdgeArrow(BaseEdge* edge)
    : QGraphicsObject(edge)
    , m_edge(edge)
    , m_arrowPolygon(QPolygonF())
    , m_kind(ArrowKind::SINGLE)
    , m_label(nullptr)
{
    // doesn't do anything with it but needs this flag in order to pass the event on to the edge
    setAcceptHoverEvents(true);

    // like edges, arrows change too often to be cached meaningfully
    setCacheMode(NoCache);
}

void EdgeArrow::setTransformation(const QPointF &pos, qreal angle)
{
    prepareGeometryChange();

    // use a transformed original as the new arrow
    QTransform arrowTransform;
    arrowTransform.translate(pos.x(), pos.y());
    arrowTransform.rotate(qRadiansToDegrees(angle));

    // swap in a copy of the original arrow shape
    QPolygonF tempArrow;
    if(m_kind==ArrowKind::SINGLE){
        tempArrow=arrowTransform.map(s_originalArrow);
    } else if(m_kind==ArrowKind::DOUBLE){
        tempArrow=arrowTransform.map(s_originalDoubleArrow);
    }
    m_arrowPolygon.swap(tempArrow);

    // update the label
    if(m_label){
        QPointF scenePos = mapToScene(pos);
        m_label->setPos(scenePos.x(), scenePos.y());
    }

    // force an update here, otherwise the arrow seems to "swim" on top of the edge
    update();
}

void EdgeArrow::defineArrow(qreal length, qreal width)
{
    // update static members
    s_arrowHalfLength = qMax(0.,length/2.);
    s_arrowHalfWidth = qMax(0.,width/2.);

    // SINGLE arrow
    s_originalArrow = QPolygonF()
            << QPointF(s_arrowHalfLength, 0.)
            << QPointF(-s_arrowHalfLength, -s_arrowHalfWidth)
            << QPointF(-s_arrowHalfLength, s_arrowHalfWidth);

    // DOUBLE arrow
    s_originalDoubleArrow = QPolygonF()
            << QPointF(s_arrowHalfLength, s_doubleGap)
            << QPointF(-s_arrowHalfLength, s_doubleGap)
            << QPointF(-s_arrowHalfLength, s_arrowHalfWidth + s_doubleGap)
            << QPointF(s_arrowHalfLength, s_doubleGap)
            << QPointF(-s_arrowHalfLength, -s_doubleGap)
            << QPointF(s_arrowHalfLength, -s_arrowHalfWidth - s_doubleGap)
            << QPointF(s_arrowHalfLength, -s_doubleGap)
            << QPointF(-s_arrowHalfLength, -s_doubleGap);
}

QRectF EdgeArrow::boundingRect() const
{
    return m_arrowPolygon.boundingRect();
}

void EdgeArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /* widget */)
{
    painter->setClipRect(option->exposedRect);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(s_arrowColor));
    painter->drawConvexPolygon(m_arrowPolygon);
}

QPainterPath EdgeArrow::shape() const
{
    QPainterPath shape;
    shape.addPolygon(m_arrowPolygon);
    return shape;
}

void EdgeArrow::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    // manually forward the double-click event to the parenting BaseEdge
    return m_edge->mouseDoubleClickEvent(event);
}

} // namespace zodiac
