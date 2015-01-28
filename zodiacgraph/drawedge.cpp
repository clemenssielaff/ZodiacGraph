#include "drawedge.h"

#include "utils.h"
#include "plug.h"

namespace zodiac {

DrawEdge::DrawEdge(Scene* scene)
    : BezierEdge(scene)
    , m_isReverse(false)
{
    // the draw edge is always in front of the nodes
    setZValue(zStack::DRAW_EDGE);

    // the draw edge does not need to react to hover events
    setAcceptHoverEvents(false);
}

void DrawEdge::fromPlugToPoint(Plug* plug, const QPointF& endPoint)
{
    // return early, if the shape of the edge has not changed
    QPointF startPoint = plug->scenePos();
    if((startPoint==m_startPoint)&&(endPoint==m_endPoint)){
        return;
    }

    // update the edge's ctrl points
    if(!m_isReverse){
        m_startPoint = startPoint;
        m_endPoint = endPoint;
        m_ctrlPoint1 = getCtrlPointFor(plug);
        m_ctrlPoint2 = m_endPoint;
    } else {
        m_endPoint = startPoint;
        m_startPoint = endPoint;
        m_ctrlPoint2 = getCtrlPointFor(plug);
        m_ctrlPoint1 = m_startPoint;
    }

    // update the path
    updateShape();
}

} // namespace zodiac
