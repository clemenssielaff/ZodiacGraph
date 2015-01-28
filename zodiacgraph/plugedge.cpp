#include "plugedge.h"

#include <QGraphicsSceneMouseEvent>

#include "edgegroup.h"
#include "labeltextfactory.h"
#include "node.h"
#include "scene.h"
#include "view.h"
#include "plug.h"

namespace zodiac {

PlugEdge::PlugEdge(Scene* scene, Plug* startPlug, Plug* endPlug, EdgeGroup* edgeGroup)
    :BezierEdge(scene)
    , m_startPlug(startPlug)
    , m_endPlug(endPlug)
    , m_group(edgeGroup)
    , m_isBent(false)
{
    // register with the plugs
    m_startPlug->addEdge(this);
    m_endPlug->addEdge(this);

    // register with your edge group
    m_group->addEdge(this);

    // create the label for this edge
    updateLabelText();

    // initialize
    plugHasChanged();
}

void PlugEdge::plugHasChanged()
{
    // update the count of bent edges in the group, if necessary
    bool isBent = m_startPlug->isVisible() || m_endPlug->isVisible();
    if(m_isBent!=isBent){
        m_isBent = isBent;
        if(m_isBent){
            m_group->increaseBentCount();
        }else{
            m_group->decreaseBentCount();
        }
    }

    // return early, if the shape of the edge has not changed
    QPointF startPoint = m_startPlug->scenePos();
    QPointF endPoint = m_endPlug->scenePos();
    if((startPoint==m_startPoint)&&(endPoint==m_endPoint)){
        return;
    }

    // update the edge's ctrl points
    m_startPoint = startPoint;
    m_endPoint = endPoint;
    m_ctrlPoint1 = getCtrlPointFor(m_startPlug);
    m_ctrlPoint2 = getCtrlPointFor(m_endPlug);

    // update the path
    updateShape();
}

QString PlugEdge::getLabelText()
{
    return LabelTextFactory(this).produceLabel();
}

void PlugEdge::setLabelText(const QString& text)
{
    BezierEdge::setLabelText(text);

    m_group->updateLabelText();
}

void PlugEdge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & View::getRemovalButton()){
        // remove the edge on removal click
        event->accept();
        m_scene->removeEdge(this);

    } else {
        // othwerise do whatever
        return BezierEdge::mousePressEvent(event);
    }
}

void PlugEdge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & View::getSelectionButton()){
        m_startPlug->getNode()->softSetExpansion(NodeExpansion::BOTH);
        m_endPlug->getNode()->softSetExpansion(NodeExpansion::BOTH);
    }
    BaseEdge::mouseDoubleClickEvent(event);
}

} // namespace zodiac
