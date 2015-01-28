#include "straightdoubleedge.h"

#include <QVector2D>

#include "edgearrow.h"
#include "edgegroupinterface.h"

namespace zodiac {

StraightDoubleEdge::StraightDoubleEdge(Scene* scene, EdgeGroupInterface* group,
                       Node* fromNode, Node* toNode)
    : StraightEdge(scene, group, fromNode, toNode)
{
    m_arrow->setKind(ArrowKind::DOUBLE);

    // initialize the shape
    // the StraightEdge Constructor does so as well, but at that time this part of the instance is not constructed yet
    // so the wrong function is called (it calls StraightEdge::updateShape instead).
    updateShape();
}

void StraightDoubleEdge::updateLabel()
{
    setLabelText(m_group->getLabelText());
    placeArrowAt(.5);
}

void StraightDoubleEdge::updateShape()
{
    prepareGeometryChange();

    // calculate the perpendicular edge offset
    QVector2D direction = QVector2D(m_endPoint-m_startPoint);
    direction.normalize();
    QPointF offset = QPointF(-direction.y(), direction.x()) * s_width;

    // update the path
    QPainterPath doubleLine;
    doubleLine.moveTo(m_startPoint+offset);
    doubleLine.lineTo(m_endPoint+offset);

    doubleLine.moveTo(m_startPoint-offset);
    doubleLine.lineTo(m_endPoint-offset);

    m_path.swap(doubleLine);

    // update the arrow
    placeArrowAt(.5);
}

} // namespace zodiac
