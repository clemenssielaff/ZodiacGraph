#include "nodehandle.h"

#include "node.h"
#include "plug.h"
#include "scene.h"
#include "scenehandle.h"

namespace zodiac {

NodeHandle::NodeHandle(Node* node)
    : QObject(nullptr)
    , m_node(node)
    , m_isValid(node!=nullptr)
{
    connectSignals();
}

NodeHandle& NodeHandle::operator = (const NodeHandle& other)
{
    if(m_node){
        m_node->disconnect(this);
    }
    m_node = other.data();
    m_isValid = m_node != nullptr;
    connectSignals();
    return *this;
}

bool NodeHandle::isRemovable() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    return m_node->isRemovable();
}

bool NodeHandle::remove()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    if(m_node->getScene()->removeNode(m_node)){
        m_isValid = false;
        return true;
    } else {
        return false;
    }
}

const QUuid& NodeHandle::getId() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        QUuid();
    }
#endif
    return m_node->getUniqueId();
}

QString NodeHandle::getName() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return "";
    }
#endif
    return m_node->getDisplayName();
}

void NodeHandle::rename(const QString& name)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return;
    }
#endif
    m_node->setDisplayName(name);
}

PlugHandle NodeHandle::createIncomingPlug(const QString& name)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        PlugHandle();
    }
#endif
    return PlugHandle(m_node->createPlug(name, PlugDirection::IN));
}

PlugHandle NodeHandle::createOutgoingPlug(const QString& name)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        PlugHandle();
    }
#endif
    return PlugHandle(m_node->createPlug(name, PlugDirection::OUT));
}

QList<PlugHandle> NodeHandle::getPlugs() const
{
    QList<PlugHandle> result;
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return result;
    }
#endif
    QList<Plug*> plugs = m_node->getPlugs();
    result.reserve(plugs.size());
    for(Plug* plug : plugs){
        result.append(PlugHandle(plug));
    }
    return result;
}

PlugHandle NodeHandle::getPlug(const QString& name) const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return PlugHandle();
    }
#endif
    return PlugHandle(m_node->getPlug(name));
}

void NodeHandle::setSelected(bool isSelected)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return;
    }
#endif
    m_node->setSelected(isSelected);
}

SceneHandle NodeHandle::getScene() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return SceneHandle();
    }
#endif
    return SceneHandle(m_node->getScene());
}

QPointF NodeHandle::getPos() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return QPointF();
    }
#endif
    return m_node->pos();
}

void NodeHandle::setPos(qreal x, qreal y)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return;
    }
#endif
    m_node->setPos(x, y);
}

void NodeHandle::connectSignals()
{
    if(!m_isValid){
        return;
    }
    connect(m_node, SIGNAL(destroyed()), this, SLOT(nodeWasDestroyed()));
    connect(m_node, SIGNAL(nodeActivated()), this, SIGNAL(nodeActivated()));
    connect(m_node, SIGNAL(nodeRenamed(QString)), this, SIGNAL(nodeRenamed(QString)));
    connect(m_node, SIGNAL(removalRequested()), this, SIGNAL(removalRequested()));
    connect(m_node, SIGNAL(inputConnected(Plug*,Plug*)), this, SLOT(passInputConnected(Plug*,Plug*)));
    connect(m_node, SIGNAL(inputDisconnected(Plug*,Plug*)), this, SLOT(passInputDisconnected(Plug*,Plug*)));
    connect(m_node, SIGNAL(outputConnected(Plug*,Plug*)), this, SLOT(passOutputConnected(Plug*,Plug*)));
    connect(m_node, SIGNAL(outputDisconnected(Plug*,Plug*)), this, SLOT(passOutputDisconnected(Plug*,Plug*)));
}

void NodeHandle::passInputConnected(Plug* myInput, Plug* otherOutput)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    emit inputConnected(PlugHandle(myInput), PlugHandle(otherOutput));
}

void NodeHandle::passOutputConnected(Plug* myOutput, Plug* otherInput)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    emit outputConnected(PlugHandle(myOutput), PlugHandle(otherInput));
}

void NodeHandle::passInputDisconnected(Plug* myInput, Plug* otherOutput)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    emit inputDisconnected(PlugHandle(myInput), PlugHandle(otherOutput));
}

void NodeHandle::passOutputDisconnected(Plug* myOutput, Plug* otherInput)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    emit outputDisconnected(PlugHandle(myOutput), PlugHandle(otherInput));
}

void NodeHandle::nodeWasDestroyed()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    m_isValid = false;
    this->disconnect();
}

} // namespace zodiac
