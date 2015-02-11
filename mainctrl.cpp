#include "mainctrl.h"

#include <QDebug>
#include <cstdlib>

#include "nodectrl.h"
#include "propertyeditor.h"
#include "zodiacgraph/nodehandle.h"

QString MainCtrl::s_defaultName = "Node ";

MainCtrl::MainCtrl(QObject *parent, zodiac::Scene* scene, PropertyEditor* propertyEditor)
    : QObject(parent)
    , m_scene(zodiac::SceneHandle(scene))
    , m_propertyEditor(propertyEditor)
    , m_nodes(QHash<zodiac::NodeHandle, NodeCtrl*>())
    , m_nodeIndex(1)            // name suffixes start at 1
{
    m_propertyEditor->setMainCtrl(this);

    connect(&m_scene, SIGNAL(selectionChanged(QList<zodiac::NodeHandle>)),
            this, SLOT(selectionChanged(QList<zodiac::NodeHandle>)));
}

NodeCtrl* MainCtrl::createNode(const QString& name)
{
    // the newly created Node is the only selected one to avoid confusion
    m_scene.deselectAll();

    // use the given name or construct a default one
    QString nodeName = name;
    if(nodeName.isEmpty()){
        nodeName = s_defaultName + QString::number(m_nodeIndex++);
    }

    // create the node
    NodeCtrl* nodeCtrl = new NodeCtrl(this, m_scene.createNode(nodeName));
    m_nodes.insert(nodeCtrl->getNodeHandle(), nodeCtrl);

    return nodeCtrl;
}

bool MainCtrl::deleteNode(NodeCtrl* node)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_nodes.contains(node->getNodeHandle()));
#else
    if(!m_nodes.contains(node->getNodeHandle())){
        return false;
    }
#endif

    if(!node->isRemovable()){
        // nodes with connections cannot be deleted
        return false;
    }

    // disconnect and delete the node
    node->disconnect();
    zodiac::NodeHandle handle = node->getNodeHandle();
    m_nodes.remove(handle);
    bool result = handle.remove();
    Q_ASSERT(result);
    return result;
}

void MainCtrl::printZodiacScene()
{
    QList<zodiac::NodeHandle> allNodes = m_nodes.keys();
    for(zodiac::NodeHandle node : allNodes){
        int number = node.getName().right(2).trimmed().toInt();
        QString nodeCtrl = "nodeCtrl" + QString::number(number);
        QPointF pos = node.getPos();

        qDebug() << "NodeCtrl* nodeCtrl" + QString::number(number) + " = mainCtrl->createNode(\"" + node.getName() + "\");";
        qDebug() << nodeCtrl + "->getNodeHandle().setPos(" + QString::number(pos.x()) + ", " + QString::number(pos.y()) + ");";

        for(zodiac::PlugHandle plug : node.getPlugs()){
            if(plug.isIncoming()){
                qDebug() << nodeCtrl + "->addIncomingPlug(\"" + plug.getName() + "\");";
            } else {
                qDebug() << nodeCtrl + "->addOutgoingPlug(\"" + plug.getName() + "\");";
            }
        }

        qDebug() << ""; // newline
    }

    for(zodiac::NodeHandle node : allNodes){
        int number = node.getName().right(2).trimmed().toInt();
        QString nodeCtrl = "nodeCtrl" + QString::number(number);
        for(zodiac::PlugHandle plug : node.getPlugs()){
            if(plug.isIncoming()) continue;
            for(zodiac::PlugHandle otherPlug : plug.getConnectedPlugs()){
                int otherNumber = otherPlug.getNode().getName().right(2).trimmed().toInt();
                QString otherNodeCtrl = "nodeCtrl" + QString::number(otherNumber);
                qDebug() << nodeCtrl + "->getNodeHandle().getPlug(\"" + plug.getName() + "\").connectPlug(" + otherNodeCtrl + "->getNodeHandle().getPlug(\"" + otherPlug.getName() + "\"));";
            }
        }
    }
}

bool MainCtrl::shutdown()
{
    // do not receive any more signals from the scene handle
    m_scene.disconnect();

    return true;
}


void MainCtrl::createDefaultNode()
{
    NodeCtrl* newNode = createNode();

//    int plugCount = (qreal(qrand())/qreal(RAND_MAX))*12;
//    for(int i = 0; i < plugCount + 4; ++i){
//        if((qreal(qrand())/qreal(RAND_MAX))<0.5){
//            newNode->addIncomingPlug("plug");
//        } else {
//            newNode->addOutgoingPlug("plug");
//        }
//    }

    newNode->setSelected(true);
}

void MainCtrl::selectionChanged(QList<zodiac::NodeHandle> selection)
{
    m_propertyEditor->showNodes(selection);
}
