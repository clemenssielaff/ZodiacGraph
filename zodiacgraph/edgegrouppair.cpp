#include "edgegrouppair.h"

#include <QGraphicsScene>
#include <QSet>

#include "edgegroup.h"
#include "labeltextfactory.h"
#include "node.h"
#include "plugedge.h"
#include "scene.h"
#include "straightdoubleedge.h"

namespace zodiac {

EdgeGroupPair::EdgeGroupPair(Scene* scene, Node* nodeA, Node* nodeB)
    : EdgeGroupInterface()
    , m_scene(scene)
    , m_firstGroup(new EdgeGroup(scene, nodeA, nodeB, this))
    , m_secondGroup(new EdgeGroup(scene, nodeB, nodeA, this))
    , m_edge(new StraightDoubleEdge(m_scene, this, nodeA, nodeB))
{
    // upon creation, the PlugEdge creating the first EdgeGroup is still bent and visible.
    m_edge->setVisible(false);
}

EdgeGroupPair::~EdgeGroupPair()
{
    // delete the groups
    delete m_firstGroup;
    delete m_secondGroup;

    m_firstGroup=nullptr;
    m_secondGroup=nullptr;

    // As EdgeGroupPairs are always deleted before the rest of the QGraphicsView, this also work on shutdown

    // delete the double edge
    m_edge->getFromNode()->removeStraightEdge(m_edge);
    m_edge->getToNode()->removeStraightEdge(m_edge);

    m_scene->removeItem(m_edge);
    delete m_edge; // valgrind seems to mind a call to deleteLater() here... I've read it's not real but this works also
    m_edge=nullptr;
}

bool EdgeGroupPair::isEmpty() const
{
    return((m_firstGroup->getEdgeCount()==0) && (m_secondGroup->getEdgeCount()==0));
}

void EdgeGroupPair::updateDoubleEdgeVisibility()
{
    // return early, if any of the two group edges is not visible
    if(!m_firstGroup->isVisible() || !m_secondGroup->isVisible()){
        return;
    }

    // if both are visible, hide them and show the double edge in their place
    m_firstGroup->setVisibility(false);
    m_secondGroup->setVisibility(false);
    m_edge->setVisible(true);
}

void EdgeGroupPair::hideDoubleEdge()
{
    // hide the double edge
    m_edge->setVisible(false);

    // let the edge groups determine by themselves if they need to become visible or not
    m_firstGroup->updateVisibility();
    m_secondGroup->updateVisibility();
}

void EdgeGroupPair::updateLabel()
{
    m_edge->updateLabel();
}

QString EdgeGroupPair::getLabelText()
{
    LabelTextFactory firstLabelGroup(m_firstGroup->getEdges());
    LabelTextFactory secondLabelGroup(m_secondGroup->getEdges());
    int maxNameLength = qMax(firstLabelGroup.getMaxNameLength(), secondLabelGroup.getMaxNameLength());
    int labelCount = firstLabelGroup.getLabelCount() + secondLabelGroup.getLabelCount() + 1;

    QStringList labelStrings;
    labelStrings.reserve(labelCount);
    labelStrings << firstLabelGroup.produceLabel(maxNameLength);
    labelStrings << LabelTextFactory::getHorizontalLine(maxNameLength);
    labelStrings << secondLabelGroup.produceLabel(maxNameLength);
    return labelStrings.join(LabelTextFactory::getNewlineChar());

}

void EdgeGroupPair::updateStyle()
{
    m_edge->updateStyle();
    m_firstGroup->updateStyle();
    m_secondGroup->updateStyle();
}

} // namespace zodiac
