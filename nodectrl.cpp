#include "nodectrl.h"

#include "mainctrl.h"

typedef zodiac::PlugHandle PlugHandle;

NodeCtrl::NodeCtrl(MainCtrl* manager,  zodiac::NodeHandle node)
    : QObject(manager)
    , m_manager(manager)
    , m_node(node)
    , m_plugs(QHash<PlugHandle, QList<PlugHandle>>())
{
    // connect node signals
    connect(&m_node, SIGNAL(removalRequested()), this, SLOT(remove()));
    connect(&m_node, SIGNAL(inputConnected(zodiac::PlugHandle, zodiac::PlugHandle)),
            this, SLOT(inputConnected(zodiac::PlugHandle, zodiac::PlugHandle)));
    connect(&m_node, SIGNAL(outputConnected(zodiac::PlugHandle, zodiac::PlugHandle)),
            this, SLOT(outputConnected(zodiac::PlugHandle, zodiac::PlugHandle)));
    connect(&m_node, SIGNAL(inputDisconnected(zodiac::PlugHandle, zodiac::PlugHandle)),
            this, SLOT(inputDisconnected(zodiac::PlugHandle, zodiac::PlugHandle)));
    connect(&m_node, SIGNAL(outputDisconnected(zodiac::PlugHandle, zodiac::PlugHandle)),
            this, SLOT(outputDisconnected(zodiac::PlugHandle, zodiac::PlugHandle)));
}

void NodeCtrl::rename(const QString& name)
{
    m_node.rename(name);
}

QString NodeCtrl::renamePlug(const QString& oldName, const QString& newName)
{
    if(newName == oldName){
        return oldName;
    }

    PlugHandle plug = m_node.getPlug(oldName);
    if(!plug.isValid()){
        // return the empty string if there is no plug by the given name
        return "";
    }

    // disconnect all connected plugs
    if(plug.isIncoming()){
        for(PlugHandle otherPlug : m_plugs.value(plug)){
            m_manager->getCtrlForHandle(otherPlug.getNode())->outputDisconnected(otherPlug, plug);
         }
    } else {
        for(PlugHandle otherPlug : m_plugs.value(plug)){
            m_manager->getCtrlForHandle(otherPlug.getNode())->inputDisconnected(otherPlug, plug);
        }
    }

    // rename the plug
    QString actualName = plug.rename(newName);

    // reconnect other plugs
    if(plug.isIncoming()){
        for(PlugHandle otherPlug : m_plugs.value(plug)){
            m_manager->getCtrlForHandle(otherPlug.getNode())->outputConnected(otherPlug, plug);
        }
    } else {
        for(PlugHandle otherPlug : m_plugs.value(plug)){
            m_manager->getCtrlForHandle(otherPlug.getNode())->inputConnected(otherPlug, plug);
        }
    }

    return actualName;
}

bool NodeCtrl::togglePlugDirection(const QString& name)
{
    PlugHandle plug = m_node.getPlug(name);
    if(!plug.isValid() || !plug.toggleDirection()){
        return false;
    }
    Q_ASSERT(m_plugs.contains(plug));
    Q_ASSERT(m_plugs[plug].size() == 0);

    return true;
}

bool NodeCtrl::removePlug(const QString& name)
{
    // get the plug that is about to be removed
    PlugHandle plug = m_node.getPlug(name);
    if(!plug.isValid() || !plug.isRemovable()){
        return false;
    }

    // remove all references to the plug
    Q_ASSERT(m_plugs.contains(plug));
    Q_ASSERT(m_plugs[plug].size() == 0);
    m_plugs.remove(plug);

    // remove the plug
    bool result = plug.remove();
    Q_ASSERT(result);
    return result;
}

void NodeCtrl::setSelected(bool isSelected)
{
    m_node.setSelected(isSelected);
}

bool NodeCtrl::remove()
{
    return m_manager->deleteNode(this);
}

zodiac::PlugHandle NodeCtrl::addPlug(const QString& name, bool incoming)
{
    PlugHandle newPlug;
    if(incoming){
        newPlug = m_node.createIncomingPlug(name);
    } else {
        newPlug = m_node.createOutgoingPlug(name);
    }
    Q_ASSERT(newPlug.isValid());
    m_plugs.insert(newPlug, QList<PlugHandle>());
    return newPlug;
}

void NodeCtrl::inputConnected(PlugHandle myInput, PlugHandle otherOutput)
{
    m_plugs[myInput].append(otherOutput);
}

void NodeCtrl::outputConnected(PlugHandle myOutput, PlugHandle otherInput)
{
    m_plugs[myOutput].append(otherInput);
}

void NodeCtrl::inputDisconnected(PlugHandle myInput, PlugHandle otherOutput)
{
    m_plugs[myInput].removeOne(otherOutput);
    Q_ASSERT(m_plugs[myInput].count(otherOutput) == 0);
}

void NodeCtrl::outputDisconnected(PlugHandle myOutput, PlugHandle otherInput)
{
    m_plugs[myOutput].removeOne(otherInput);
    Q_ASSERT(m_plugs[myOutput].count(otherInput) == 0);
}
