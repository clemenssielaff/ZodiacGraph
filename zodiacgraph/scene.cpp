#include "scene.h"

#include <time.h>

#include "drawedge.h"
#include "edgegroup.h"
#include "edgegrouppair.h"
#include "node.h"
#include "plug.h"
#include "plugedge.h"

namespace zodiac {

Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
    , m_drawEdge(nullptr)
    , m_nodes(QSet<Node*>())
    , m_edges(QHash<QPair<Plug*, Plug*>, PlugEdge*>())
    , m_edgeGroups(QHash<uint, EdgeGroup*>())
    , m_edgeGroupPairs(QSet<EdgeGroupPair*>())
{
    // add the draw edge to the scene
    m_drawEdge = new DrawEdge(this);
    m_drawEdge->setVisible(false);
}

Scene::~Scene()
{
    // most members are implicitly removed through Qt's parent-child mechanism
    m_drawEdge = nullptr;
    m_nodes.clear();
    m_edges.clear();

    // EdgeGroups belong to EdgeGroupPairs, which we need to delete manually
    m_edgeGroups.clear();
    for(EdgeGroupPair* edgeGroupPair : m_edgeGroupPairs){
        delete edgeGroupPair;
    }
    m_edgeGroupPairs.clear();
}

Node* Scene::createNode(const QString &name, const QUuid& uuid)
{
    Node* newNode = new Node(this, name, uuid);
    m_nodes.insert(newNode);
    addItem(newNode);
    return newNode;
}

bool Scene::removeNode(Node* node)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_nodes.contains(node));
#else
    if(!m_nodes.contains(node)){
        return false;
    }
#endif

    // return early if the node cannot be removed
    if(!node->isRemovable()){
        return false;
    }

    // delete all references to the node and finally the node itself
    m_nodes.remove(node);
    removeItem(node);
    node->deleteLater();

    return true;
}

PlugEdge* Scene::createEdge(Plug* fromPlug, Plug* toPlug)
{
    // only allow edges between different plugs of different nodes
    Node* fromNode = fromPlug->getNode();
    Node* toNode = toPlug->getNode();
    if(fromNode==toNode){
        return nullptr;
    }

    // do not create the same edge twice
    if(fromPlug->isConnectedWith(toPlug)){
        return nullptr;
    }

    // make sure an outgoing plug is connecting with an incoming plug
    if((fromPlug->getDirection() != PlugDirection::OUT) || (toPlug->getDirection() != PlugDirection::IN)){
        return nullptr;
    }

    // make sure that the incoming edge has no connections yet
    if(toPlug->getEdgeCount() != 0){
        return nullptr;
    }

    // find the edge group for this edge, if it exists
    uint edgeGroupHash = EdgeGroup::getHashOf(fromNode, toNode);
    EdgeGroup* edgeGroup;
    if(m_edgeGroups.contains(edgeGroupHash)){
        edgeGroup = m_edgeGroups[edgeGroupHash];
    } else {

        // ... or create a new edge group pair for it
        EdgeGroupPair* newGroupPair = new EdgeGroupPair(this, fromNode, toNode);
        m_edgeGroupPairs.insert(newGroupPair);

        edgeGroup = newGroupPair->getFirstGroup();
        m_edgeGroups.insert(edgeGroupHash, edgeGroup);

        EdgeGroup* oppositeEdgeGroup = newGroupPair->getSecondGroup();
        m_edgeGroups.insert(oppositeEdgeGroup->getHash(), oppositeEdgeGroup);
    }

    // create the new edge
    PlugEdge* newEdge = new PlugEdge(this, fromPlug, toPlug, edgeGroup);
    m_edges.insert(QPair<Plug*, Plug*>(fromPlug, toPlug), newEdge);

    // emit signals
    emit fromNode->outputConnected(fromPlug, toPlug);
    emit toNode->inputConnected(toPlug, fromPlug);

    return newEdge;
}

void Scene::removeEdge(PlugEdge* edge)
{
    Plug* fromPlug = edge->getStartPlug();
    Plug* toPlug = edge->getEndPlug();
    QPair<Plug*, Plug*> edgeKey(fromPlug, toPlug);
#ifdef QT_DEBUG
    Q_ASSERT(m_edges.contains(edgeKey));
#else
    if(!m_edges.contains(edgeKey)){
        return;
    }
#endif

    // unregister from the connected plugs
    fromPlug->removeEdge(edge);
    toPlug->removeEdge(edge);

    // remove the edge from the Scene's register
    m_edges.remove(edgeKey);

    // remove the edge from its group
    EdgeGroup* edgeGroup  = edge->getGroup();
    edgeGroup->removeEdge(edge);

    // if the group is now empty, we can only delete it if the other group in the pair is also empty
    EdgeGroupPair* edgeGroupPair = edgeGroup->getEdgeGroupPair();
    if(edgeGroupPair->isEmpty()){
        uint firstHash = edgeGroupPair->getFirstGroup()->getHash();
        uint secondHash = edgeGroupPair->getSecondGroup()->getHash();
        Q_ASSERT(m_edgeGroups.contains(firstHash));
        Q_ASSERT(m_edgeGroups.contains(secondHash));
        m_edgeGroups.remove(firstHash);
        m_edgeGroups.remove(secondHash);
        m_edgeGroupPairs.remove(edgeGroupPair);
        delete edgeGroupPair; // also deletes the EdgeGroups
        edgeGroupPair = nullptr;
    }

    // lastly, remove the QGraphicsItem from the scene, thereby taking possession of the last pointer to the edge
    removeItem(edge);

    // delete the edge from memory (automatically deletes all Qt-children as well)
    edge->deleteLater();

    // emit signals
    emit fromPlug->getNode()->outputDisconnected(fromPlug, toPlug);
    emit toPlug->getNode()->inputDisconnected(toPlug, fromPlug);
}

PlugEdge* Scene::getEdge(Plug* fromPlug, Plug* toPlug)
{
    QPair<Plug*, Plug*> edgeKey(fromPlug, toPlug);
    return m_edges.value(edgeKey, nullptr);
}

void Scene::collapseAllNodes()
{
    for(Node* node : m_nodes){
        node->forceCollapse();
    }
}

void Scene::updateStyle()
{
    for(Node* node : m_nodes){
        node->updateStyle();
    }
    for(EdgeGroupPair* pair : m_edgeGroupPairs){
        pair->updateStyle();
    }
    m_drawEdge->updateStyle();
}

} // namespace zodiac
