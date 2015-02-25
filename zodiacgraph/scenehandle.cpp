#include "scenehandle.h"

#include "node.h"
#include "scene.h"

namespace zodiac {

SceneHandle::SceneHandle(Scene* scene)
    : QObject(nullptr)
    , m_scene(scene)
    , m_isValid(scene!=nullptr)
{
    connectSignals();
}

SceneHandle& SceneHandle::operator = (const SceneHandle& other)
{
    if(m_scene){
        m_scene->disconnect(this);
    }
    m_scene = other.data();
    m_isValid = m_scene != nullptr;
    connectSignals();
    return *this;
}

NodeHandle SceneHandle::createNode(const QString& name, const QUuid& uuid)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return NodeHandle();
    }
#endif
    return NodeHandle(m_scene->createNode(name, uuid));
}

QList<NodeHandle> SceneHandle::getNodes() const
{
    QList<NodeHandle> result;
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return result;
    }
#endif
    QList<Node*> nodes = m_scene->getNodes();
    result.reserve(nodes.size());
    for(Node* node : nodes){
        result.append(NodeHandle(node));
    }
    return result;
}

void SceneHandle::deselectAll() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return;
    }
#endif
    for(QGraphicsItem* item : m_scene->selectedItems()){
        item->setSelected(false);
    }
}

void SceneHandle::connectSignals()
{
    if(!m_isValid){
        return;
    }
    connect(m_scene, SIGNAL(destroyed()), this, SLOT(sceneWasDestroyed()));
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(updateSelection()));
}

void SceneHandle::updateSelection()
{
    QList<NodeHandle> selection;
    for(QGraphicsItem* item : m_scene->selectedItems()){
        Node* selectedNode = qobject_cast<Node*>(item->toGraphicsObject());
        if(selectedNode){
            selection.append(NodeHandle(selectedNode));
        }
    }
    emit selectionChanged(selection);
}

void SceneHandle::sceneWasDestroyed()
{
    m_isValid = false;
}

} // namespace zodiac
