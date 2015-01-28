#include "bezieredge.h"

#include "edgearrow.h"
#include "node.h"
#include "plug.h"

namespace zodiac {

qreal BezierEdge::s_maxCtrlDistance = 150.;
qreal BezierEdge::s_ctrlExpansionFactor = 0.4;

BezierEdge::BezierEdge(Scene* scene)
    : BaseEdge(scene)
    , m_startPoint(QPointF())
    , m_ctrlPoint1(QPointF())
    , m_ctrlPoint2(QPointF())
    , m_endPoint(QPointF())
{
    // initialize the shape of the edge
    updateShape();
}

void BezierEdge::placeArrowAt(qreal fraction)
{
    /// Technically, you cannot place the arrow all the way at the end of the edge because at that point, the spline
    /// would not have a tangent to orient the arrow to.
    /// We could of course say that if <code>fraction >= 1.0-VERY_SMALL_DELTA</code>, the direction would have to be
    /// calculated using a point before the position ... but why bother?
    /// It doesn't make a visual difference and would only introduce unnecessary code branching.
    static const qreal VERY_SMALL_DELTA = 0.00001;

    qreal pos = qMin(1.0-VERY_SMALL_DELTA, qMax(0.0, fraction));
    QPointF edgeCenter = m_path.pointAtPercent(pos);
    QPointF edgeDirection = m_path.pointAtPercent(qMin(pos+VERY_SMALL_DELTA, 1.))-edgeCenter;
    m_arrow->setTransformation(edgeCenter, atan2(edgeDirection.y(), edgeDirection.x()));
}

void BezierEdge::updateShape()
{
    prepareGeometryChange();

    // create the path
    QPainterPath bezierPath;
    bezierPath.moveTo(m_startPoint);
    bezierPath.cubicTo(m_ctrlPoint1, m_ctrlPoint2, m_endPoint);
    m_path.swap(bezierPath);

    placeArrowAt(0.5);
}

QPointF BezierEdge::getCtrlPointFor(Plug* plug)
{
    qreal factor;
    switch (plug->getDirection()) {
    case PlugDirection::IN:
        factor = plug->getNode()->getIncomingExpansionFactor();
        break;
    case PlugDirection::OUT:
        factor = plug->getNode()->getOutgoingExpansionFactor();
        break;
    case PlugDirection::BOTH:
        factor = qMax(plug->getNode()->getIncomingExpansionFactor(), plug->getNode()->getOutgoingExpansionFactor());
        break;
    default:
        factor = 0.0;       //
        Q_ASSERT(false);    // we shouldn't ever reach these lines..
    }

    // calculate the control point distance
    qreal manhattanLength = (m_endPoint-m_startPoint).manhattanLength();
    qreal ctrlDistance = qMin(s_maxCtrlDistance, manhattanLength*s_ctrlExpansionFactor);
    return plug->scenePos()+((plug->getNormal()*ctrlDistance).toPointF()*factor);
}

} // namespace zodiac
