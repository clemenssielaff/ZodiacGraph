#include "plughandle.h"

#include "node.h"
#include "nodehandle.h"
#include "plug.h"
#include "plugedge.h"
#include "scene.h"
#include "scenehandle.h"

namespace zodiac {

PlugHandle::PlugHandle(Plug* plug)
    : QObject(nullptr)
    , m_plug(plug)
    , m_isValid(plug!=nullptr)
{
    connectSignals();
}

PlugHandle& PlugHandle::operator = (const PlugHandle& other)
{
    if(m_plug){
        m_plug->disconnect(this);
    }
    m_plug = other.data();
    m_isValid = m_plug != nullptr;
    connectSignals();
    return *this;
}

bool PlugHandle::isRemovable() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    return m_plug->isRemovable();
}

bool PlugHandle::remove()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    if(m_plug->getNode()->removePlug(m_plug)){
        m_isValid = false;
        return true;
    } else {
        return false;
    }
}

QString PlugHandle::getName() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return "";
    }
#endif
    return m_plug->getName();
}

QString PlugHandle::rename(const QString& name)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return "";
    }
#endif
    return m_plug->getNode()->renamePlug(m_plug, name);
}

bool PlugHandle::toggleDirection()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    return m_plug->getNode()->togglePlugDirection(m_plug);
}

bool PlugHandle::isIncoming() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    return m_plug->getDirection() == PlugDirection::IN;
}

bool PlugHandle::isOutgoing() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    return m_plug->getDirection() == PlugDirection::OUT;
}

int PlugHandle::connectionCount() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return 0;
    }
#endif
    return m_plug->getEdgeCount();
}

QList<PlugHandle> PlugHandle::getConnectedPlugs() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return QList<PlugHandle>();
    }
#endif
    QList<PlugHandle> result;
    for(Plug* plug : m_plug->getConnectedPlugs()){
        result.append(PlugHandle(plug));
    }
    return result;
}

bool PlugHandle::connectPlug(PlugHandle other)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return false;
    }
#endif
    if(m_plug->getDirection() == PlugDirection::OUT){
        return m_plug->getNode()->getScene()->createEdge(m_plug, other.data()) != nullptr;
    } else {
        return m_plug->getNode()->getScene()->createEdge(other.data(), m_plug) != nullptr;
    }
}


bool PlugHandle::disconnectPlug(PlugHandle other)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid && other.isValid());
#else
    if(!m_isValid || !other.isValid()){
        return false;
    }
#endif
    PlugEdge* edge;
    Scene* scene = m_plug->getNode()->getScene();
    if(m_plug->getDirection() == PlugDirection::OUT){
        edge = scene->getEdge(m_plug, other.data());
    } else {
        edge = scene->getEdge(other.data(), m_plug);
    }
    if(!edge){
        return false;
    }
    scene->removeEdge(edge);
    return true;
}

void PlugHandle::disconnectAll()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return;
    }
#endif
    Scene* scene = m_plug->getNode()->getScene();
    for(Plug* plug : m_plug->getConnectedPlugs()){
        PlugEdge* edge;
        if(m_plug->getDirection() == PlugDirection::OUT){
            edge = scene->getEdge(m_plug, plug);
        } else {
            edge = scene->getEdge(plug, m_plug);
        }
        Q_ASSERT(edge);
        scene->removeEdge(edge);
    }
}

NodeHandle PlugHandle::getNode() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return NodeHandle();
    }
#endif
    return NodeHandle(m_plug->getNode());
}

SceneHandle PlugHandle::getScene() const
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#else
    if(!m_isValid){
        return SceneHandle();
    }
#endif
    return SceneHandle(m_plug->getNode()->getScene());
}

void PlugHandle::connectSignals()
{
    if(!m_isValid){
        return;
    }
    connect(m_plug, SIGNAL(destroyed()), this, SLOT(plugWasDestroyed()));
    connect(m_plug, SIGNAL(plugRenamed(QString)), this, SIGNAL(plugRenamed(QString)));
}

void PlugHandle::plugWasDestroyed()
{
#ifdef QT_DEBUG
    Q_ASSERT(m_isValid);
#endif
    m_isValid = false;
    this->disconnect();
}

} // namespace zodiac

