#include "edgegroup.h"

#include "edgegrouppair.h"
#include "labeltextfactory.h"
#include "node.h"
#include "plug.h"
#include "plugedge.h"
#include "straightedge.h"
#include "scene.h"

namespace zodiac {

EdgeGroup::EdgeGroup(Scene* scene,
                     Node* fromNode, Node* toNode,
                     EdgeGroupPair* pair)
    : QObject(nullptr)
    , EdgeGroupInterface()
    , m_scene(scene)
    , m_fromNode(fromNode)
    , m_toNode(toNode)
    , m_pair(pair)
    , m_edges(QSet<PlugEdge*>())
    , m_straightEdge(nullptr)
    , m_bentEdgesCount(0)
{
    // create an invisible StraightEdge
    m_straightEdge = new StraightEdge(m_scene, this, fromNode, toNode);
    m_straightEdge->setVisible(false);

    // let the StraightEdge request removal of this group
    connect(m_straightEdge, SIGNAL(removalRequested()), this, SLOT(removalRequested()));
}

EdgeGroup::~EdgeGroup()
{
    // As EdgeGroups are always deleted before the rest of the QGraphicsView, this also work on shutdown

    m_straightEdge->getFromNode()->removeStraightEdge(m_straightEdge);
    m_straightEdge->getToNode()->removeStraightEdge(m_straightEdge);

    m_scene->removeItem(m_straightEdge);
    delete m_straightEdge; // valgrind seems to mind a call to deleteLater() here... I've read it's not real but this works also
    m_straightEdge = nullptr;
}

void EdgeGroup::addEdge(PlugEdge* edge)
{
#ifdef QT_DEBUG
    Q_ASSERT(edge->getStartPlug()->getNode() == m_fromNode);
    Q_ASSERT(edge->getEndPlug()->getNode() == m_toNode);
#else
    if((edge->getStartPlug()->getNode() != m_fromNode) || (edge->getEndPlug()->getNode() != m_toNode)){
        return;
    }
#endif
    m_edges.insert(edge);

    // update the labels
    m_straightEdge->updateLabel();
    m_pair->updateLabel();
}

void EdgeGroup::removeEdge(PlugEdge* edge)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_edges.contains(edge));
#else
    if(!m_edges.contains(edge)){
        return;
    }
#endif
    m_edges.remove(edge);

    // if the edge to be removed is the last one in the group, it might be 'unbent'
    // to avoid confusion, temporarily set the bent count to 1 before decreasing it again
    if((m_edges.count()==0) && (m_bentEdgesCount==0)){
        m_bentEdgesCount = 1;
    }
    decreaseBentCount();

    updateLabelText();
}

void EdgeGroup::increaseBentCount()
{
    ++m_bentEdgesCount;
    m_pair->hideDoubleEdge();
}

void EdgeGroup::decreaseBentCount()
{
    --m_bentEdgesCount;
    Q_ASSERT(m_bentEdgesCount>=0);
    updateVisibility();
    m_pair->updateDoubleEdgeVisibility();
}

void EdgeGroup::updateVisibility()
{
    bool visibility = m_bentEdgesCount!=0;
    for(PlugEdge* edge : m_edges){
        edge->setVisible(visibility);
    }
    if(m_edges.size()>0){
        m_straightEdge->setVisible(!visibility);
    }
}

uint EdgeGroup::getHash() const
{
    return getHashOf(m_fromNode, m_toNode);
}

bool EdgeGroup::isVisible() const
{
    return m_straightEdge->isVisible();
}

void EdgeGroup::setVisibility(bool visibility)
{
    m_straightEdge->setVisible(visibility);
}

QString EdgeGroup::getLabelText()
{
    return LabelTextFactory(m_edges).produceLabel();
}

void EdgeGroup::updateLabelText()
{
    m_straightEdge->updateLabel();
    m_pair->updateLabel();
}

void EdgeGroup::updateStyle()
{
    m_straightEdge->updateStyle();
}

void EdgeGroup::removalRequested()
{
    // if there is only one edge in this group, tell the Scene to remove it
    if(m_edges.count()==1){
        increaseBentCount();
        m_scene->removeEdge((*m_edges.begin()));
    }
    // otherwise do nothing
}

} // namespace zodiac
