#include "plug.h"

#include <QGraphicsSceneMouseEvent>
#include <QtMath>                       // for M_PI, qRadiansToDegrees
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "drawedge.h"
#include "node.h"
#include "scene.h"
#include "utils.h"
#include "perimeter.h"
#include "plugedge.h"
#include "pluglabel.h"

static QGraphicsItem* getRootItemOf(QGraphicsItem* item);

namespace zodiac {

qreal Plug::s_width = 12.;
QColor Plug::s_inColor = QColor("#728872");
QColor Plug::s_outColor = QColor("#887272");
QColor Plug::s_highlightColor = QColor("#d1d7db");
bool Plug::s_toggleNodeExpansionOnEdgeCreation = true;
Qt::MouseButton Plug::s_drawEdgeButton = Qt::LeftButton;
Node* Plug::s_dragTargetNode = nullptr;
Plug* Plug::s_dragTargetPlug = nullptr;
Plug* Plug::s_edgeDrawingPlug = nullptr;

Plug::Plug(Node* parent, const QString& name, PlugDirection direction)
    : QGraphicsObject(parent)
    , m_name(name)
    , m_direction(direction)
    , m_node(parent)
    , m_arclength(0.1)
    , m_normal(QVector2D(1.,0.))
    , m_shape(QPainterPath())
    , m_isHighlighted(false)
    , m_edges(QSet<PlugEdge*>())
    , m_label(nullptr)
    , m_connectedPlugs(QSet<Plug*>())
{
    // the perimeter needs to stack behind the node core
    setFlag(ItemStacksBehindParent);
    setCacheMode(DeviceCoordinateCache);

    // the plug doesn't do anything with them, but needs to be enabled so they get send up to the node
    setAcceptHoverEvents(true);

    // only becomes visible as the node core expands
    setVisible(false);

    // create a label for this plug
    m_label = new PlugLabel(this);

    // initialize the members of this plug with default values
    setHighlight(false);
}

void Plug::addEdge(PlugEdge* edge)
{
    // make sure the edge actually connects to this plug
    Plug* startPlug = edge->getStartPlug();
    Plug* endPlug = edge->getEndPlug();
    bool isValid = (((startPlug==this) || (endPlug==this))                      // edge contains this plug
                    && (!m_edges.contains(edge))                                // edge is not already connected
                    && (!m_connectedPlugs.contains(startPlug))                  // other plug is not already connected
                    && (!m_connectedPlugs.contains(endPlug))                    //    "    -     "
                    && (m_direction==PlugDirection::IN?m_edges.count()==0:true));   // if incoming, not yet connected
#ifdef QT_DEBUG
    Q_ASSERT(isValid);
#else
    if(!isValid){
        return;
    }
#endif

    // store the edge and the other plug
    m_edges.insert(edge);
    if(startPlug==this){
        m_connectedPlugs.insert(endPlug);
    } else {
        m_connectedPlugs.insert(startPlug);
    }
}

void Plug::removeEdge(PlugEdge* edge)
{
#ifdef QT_DEBUG
    Q_ASSERT(m_edges.contains(edge));
#else
    if(!m_edges.contains(edge)){
        return;
    }
#endif

    // remove the edge
    m_edges.remove(edge);

    // remove the plug
    Plug* startPlug = edge->getStartPlug();
    Plug* endPlug = edge->getEndPlug();
    if(m_connectedPlugs.contains(startPlug)){
        m_connectedPlugs.remove(startPlug);
    } else {
        Q_ASSERT(m_connectedPlugs.contains(endPlug));
        m_connectedPlugs.remove(endPlug);
    }
}

void Plug::defineShape(QVector2D normal, qreal arclength)
{
    // return early, if nothing has changed
    if(qFuzzyCompare(normal.x(), m_normal.x()) && qFuzzyCompare(normal.y(), m_normal.y())
            && qFuzzyCompare(arclength, m_arclength)){
        return;
    }
    m_normal=normal;
    m_arclength=qAbs(arclength);
    updateShape();
}

void Plug::updateEdges() const
{
    foreach (PlugEdge* edge, m_edges){
        edge->plugHasChanged();
    }
}

void Plug::updateExpansion(qreal expansion)
{
    // use visibility toggle to adjust edge stretch if necessary
    setVisible(expansion!=0.0);

    // update position
    qreal targetDistance = m_node->getPerimeterRadius()-(m_direction==PlugDirection::IN?s_width:0.);
    QPointF pos = (m_normal*expansion*targetDistance).toPointF();
    setPos(pos);

    // update label tranparency
    m_label->setOpacity(expansion);

    // update any connected edges
    updateEdges();
}

void Plug::setHighlight(bool highlight)
{
    m_isHighlighted=highlight;
    m_label->setHighlight(highlight);
    update();
}

QVector2D Plug::getTargetNormal() const
{
    // reset and return early, if there are no plugs to target
    if(m_connectedPlugs.isEmpty()){
        return QVector2D(0,0);
    }

    // get the average direction to all connected plugs (plus their respective normal)
    QVector2D averageDirection(0,0);
    QVector2D thisPos(m_node->scenePos());
    for(Plug* plug : m_connectedPlugs){
        QVector2D otherPos(plug->scenePos());
        otherPos += plug->m_normal * plug->getNode()->getPerimeterRadius();
        averageDirection += (otherPos - thisPos).normalized();
    }
    return averageDirection.normalized();
}

void Plug::aquireDrawEdge()
{
#ifdef QT_DEBUG
    Q_ASSERT(s_edgeDrawingPlug==nullptr);
#else
    if(s_edgeDrawingPlug!=nullptr){
        return;
    }
#endif
    s_edgeDrawingPlug = this;
    m_node->getScene()->getDrawEdge()->setReverse(m_direction==PlugDirection::IN);
}

void Plug::advanceDrawEdge(const QPointF& scenePos)
{
    // due to some weird Qt event handling race conditions, this function is very occassionaly called on a Plug that is
    // no longer the edge drawing plug.
    // in this case, simply ignore the call.
    if(s_edgeDrawingPlug!=this){
        Q_ASSERT(s_edgeDrawingPlug==nullptr);
        return;
    }

    // make sure the draw edge is visible as soon as the mouse moves (but no sooner)
    DrawEdge* drawEdge = m_node->getScene()->getDrawEdge();
    drawEdge->setVisible(true);

    // find the topmost node under the mouse cursor
    Node* targetNode = nullptr;
    QGraphicsItem* plugNode = parentItem(); // the parent item of this plug is always its node
    for(QGraphicsItem* currentItem : scene()->items(scenePos)){
        QGraphicsItem* rootItem = getRootItemOf(currentItem);
        if((!rootItem) || (rootItem == plugNode)){
            continue;
        }
        targetNode = qobject_cast<Node*>(rootItem->toGraphicsObject());
        if(targetNode){
            break;
        }
    }

    // if we found a node and if it is not already the dragTargetNode, collapse the old node and expand the current
    if(targetNode){
        bool isDrawingReverse = drawEdge->isReversed();
        if(s_dragTargetNode!=targetNode){
            if(s_dragTargetNode){
                s_dragTargetNode->softResetExpansion();
            }
            s_dragTargetNode = targetNode;
            targetNode->softSetExpansion(isDrawingReverse ? NodeExpansion::OUT : NodeExpansion::IN);
        }

        // highlight the closest plug
        Plug* closestPlug = targetNode->getClosestPlugTo(targetNode->mapFromScene(scenePos),
                                                             isDrawingReverse ? PlugDirection::OUT : PlugDirection::IN);
        if(closestPlug){
            if((closestPlug!=this)&&(s_dragTargetPlug!=closestPlug)){
                closestPlug->setHighlight(true);
                if(s_dragTargetPlug){
                    s_dragTargetPlug->setHighlight(false);
                }
                s_dragTargetPlug = closestPlug;
            }

        // reset the target plug, if the node item has none
        } else if(s_dragTargetPlug){
            s_dragTargetPlug->setHighlight(false);
            s_dragTargetPlug = nullptr;
        }

    // if you found none but there is still an active dragTargetNode, collapse it
    } else if(s_dragTargetNode){
        s_dragTargetNode->softResetExpansion();
        s_dragTargetNode=nullptr;

        // also reset the target plug
        if(s_dragTargetPlug){
            s_dragTargetPlug->setHighlight(false);
            s_dragTargetPlug = nullptr;
        }
    }

    // update the draw edge
    drawEdge->fromPlugToPoint(this, scenePos);
}

void Plug::releaseDrawEdge()
{
    // due to some weird Qt event handling race conditions, this function is very occassionaly called on a Plug that is
    // no longer the edge drawing plug.
    // in this case, simply ignore the call.
    if(s_edgeDrawingPlug!=this){
        Q_ASSERT(s_edgeDrawingPlug==nullptr);
        return;
    }

    // if requested, expand the outgoing plugs of the target node if there is any
    if(s_toggleNodeExpansionOnEdgeCreation && s_dragTargetNode){
        s_dragTargetNode->softSetExpansion(NodeExpansion::OUT);
    }

    // release the draw edge
    Scene* nodeScene = m_node->getScene();
    DrawEdge* drawEdge = nodeScene->getDrawEdge();
    drawEdge->setVisible(false);
    s_edgeDrawingPlug = nullptr;
    s_dragTargetNode = nullptr;

    // if there is a plug targeted at the moment of release, add an edge
    if(s_dragTargetPlug){
        if(drawEdge->isReversed()){
            nodeScene->createEdge(s_dragTargetPlug, this);
        } else {
            nodeScene->createEdge(this, s_dragTargetPlug);
        }
        s_dragTargetPlug->setHighlight(false);
        s_dragTargetPlug = nullptr;
    }
}

void Plug::updateEdgeLabels()
{
    for(PlugEdge* edge : m_edges){
        edge->updateLabelText();
    }
}

void Plug::updateStyle()
{
    updateShape();

    m_label->updateStyle();

    update();
}

qreal Plug::getArrangementPriority()
{
    qreal factor=0.;
    for(Plug* connectedPlug : m_connectedPlugs){
        factor += connectedPlug->getEdgeCount();
    }
    factor *= 0.5;
    return factor + getEdgeCount();
}

QRectF Plug::boundingRect() const
{
    return m_shape.boundingRect();
}

void Plug::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /* widget */)
{
    painter->setClipRect(option->exposedRect);

    // define the pen to draw this plug
    QBrush brush(Qt::SolidPattern);

    if(m_isHighlighted){
        brush.setColor(s_highlightColor);
    } else {
        if(m_direction==PlugDirection::IN){
            brush.setColor(s_inColor);
        } else {
            brush.setColor(s_outColor);
        }
    }

    // draw the plug
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->drawPath(m_shape);
}

QPainterPath Plug::shape() const
{
    return m_shape;
}

void Plug::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
    // only highlight, if this is not a connected, incoming plug
    if(mayReceiveInput()){
        setHighlight(true);
    }
    QGraphicsObject::hoverEnterEvent(event);
}

void Plug::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
    setHighlight(false);
    QGraphicsObject::hoverLeaveEvent(event);
}

void Plug::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // only react to the draw edge button and only if this is not a connected incoming plug
    if((event->buttons() & s_drawEdgeButton) && mayReceiveInput()){
        aquireDrawEdge();
        event->accept();
    }
}

void Plug::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    advanceDrawEdge(event->scenePos());
    return QGraphicsObject::mouseMoveEvent(event);
}

void Plug::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    releaseDrawEdge();
    return QGraphicsItem::mouseReleaseEvent(event);
}

void Plug::setName(const QString& name)
{
    if(name == m_name){
        return;
    }
    m_name=name;
    updateEdgeLabels();
    updateStyle();
    emit plugRenamed(m_name);
}

void Plug::updateShape()
{
    prepareGeometryChange();

    // update the path traced by the plug
    // the path is created in-place, meaning it does not need to be rotated
    // as it turns out, rotating stuff (especially with labels attached that also rotate)
    // is a MAYOR performance killer
    qreal perimeterRadius = m_node->getPerimeterRadius()-(m_direction==PlugDirection::IN?s_width:0.);
    qreal arclength = qRadiansToDegrees(m_arclength);
    qreal arcpos = qRadiansToDegrees(atan2(-m_normal.y(), m_normal.x()));

    QPointF rectOffset = QPointF(-m_normal.x()*perimeterRadius, -m_normal.y()*perimeterRadius);
    QRectF outsideRect =quadrat(perimeterRadius+(s_width/2.));
    outsideRect.translate(rectOffset);
    QRectF insideRect =quadrat(perimeterRadius-(s_width/2.));
    insideRect.translate(rectOffset);

    QPainterPath path;
    path.arcMoveTo(outsideRect,arcpos+(arclength/2.));
    path.arcTo(outsideRect,arcpos+(arclength/2.),-arclength);
    path.arcTo(insideRect,arcpos-(arclength/2.), 0.);
    path.arcTo(insideRect,arcpos-(arclength/2.),arclength);
    path.closeSubpath();
    path = path.simplified();
    m_shape.swap(path);

    m_label->updateShape();
}

bool Plug::mayReceiveInput()
{
    const NodeExpansion nodeState = m_node->getExpansionState();
    if(m_direction==PlugDirection::IN){
        return ((m_edges.count()==0) && ((nodeState == NodeExpansion::IN) || (nodeState == NodeExpansion::BOTH)));
    } else {
        return ((nodeState == NodeExpansion::OUT) || (nodeState == NodeExpansion::BOTH));
    }
}

} // namespace zodiac

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// HELPER
//

///
/// \brief Finds the root item in the ancestry of this item
///
/// Ignores items that do not accept hover events.
///
/// \param [in] item    Item for which to search to root item.
///
/// \return             The pointer to the root item of the one given or the null pointer.
///
static QGraphicsItem* getRootItemOf(QGraphicsItem* item)
{
    for(QGraphicsItem* childItem = item; childItem!=nullptr; childItem=childItem->parentItem()){
        if(childItem->acceptHoverEvents()){
            item = childItem;
        } else {    // do not traverse past items that do not accept mouse hover events (like labels)
            return nullptr;
        }
    }
    return item;
}
