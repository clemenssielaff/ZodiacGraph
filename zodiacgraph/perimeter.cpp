#include "perimeter.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneHoverEvent>

#include "plug.h"
#include "utils.h"
#include "node.h"
#include "view.h"

namespace zodiac {

qreal Perimeter::s_minRadius = 55;
qreal Perimeter::s_maxOpacity = 0.5;
QColor Perimeter::s_color = QColor("#2b517d");
Plug* Perimeter::s_closestPlugToMouse = nullptr;
bool Perimeter::s_mouseWasDragged = false;

Perimeter::Perimeter(Node* parent)
    : QGraphicsObject(parent)
    , m_node(parent)
    , m_radius(s_minRadius)
{
    // the perimeter needs to stack behind the parent
    setFlag(ItemStacksBehindParent);
    setCacheMode(DeviceCoordinateCache);

    // doesn't do anything with them, but needs to be enabled so they get send up to the node
    setAcceptHoverEvents(true);

    // only becomes visible as it fades in
    setOpacity(0.);
}

QRectF Perimeter::boundingRect() const
{
    return quadrat(m_radius);
}

void Perimeter::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /* widget */)
{
    painter->setClipRect(option->exposedRect);

    // draw perimeter
    painter->setBrush(s_color);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(quadrat(m_radius));
}

QPainterPath Perimeter::shape() const
{
    QPainterPath path;
    path.addEllipse(quadrat(m_radius));
    return path;
}

void Perimeter::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    Plug* closestPlug = m_node->getClosestPlugTo(event->pos(), PlugDirection::BOTH);
    if(closestPlug && closestPlug->isVisible()){
        if(closestPlug!=s_closestPlugToMouse){
            if(s_closestPlugToMouse){
                s_closestPlugToMouse->setHighlight(false);
            }
            closestPlug->setHighlight(true);
            s_closestPlugToMouse=closestPlug;
        }
    } else if(s_closestPlugToMouse){
        s_closestPlugToMouse->setHighlight(false);
        s_closestPlugToMouse = nullptr;
    }
    QGraphicsObject::hoverMoveEvent(event);
}

void Perimeter::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    if(s_closestPlugToMouse){
        s_closestPlugToMouse->setHighlight(false);
        s_closestPlugToMouse = nullptr;
    }
    QGraphicsObject::hoverLeaveEvent(event);
}

void Perimeter::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    s_mouseWasDragged = false;
    if((event->buttons() & View::getSelectionButton()) && (s_closestPlugToMouse)){
        s_closestPlugToMouse->aquireDrawEdge();
        return;
    }

    QGraphicsObject::mousePressEvent(event);
}

void Perimeter::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    s_mouseWasDragged = true;
    if(s_closestPlugToMouse){
        s_closestPlugToMouse->advanceDrawEdge(event->scenePos());
    }
    QGraphicsObject::mouseMoveEvent(event);
}

void Perimeter::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(s_closestPlugToMouse){
        // stop drawing the edge if the mouse was dragged
        s_closestPlugToMouse->releaseDrawEdge();
        s_closestPlugToMouse->setHighlight(false);
        s_closestPlugToMouse=nullptr;
    }

    if((!s_mouseWasDragged) && (event->button() & View::getSelectionButton())){
        // toggle node expansion on click
        m_node->toggleExpansion();
    }

    QGraphicsObject::mouseReleaseEvent(event);
}

void Perimeter::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if(event->button() == View::getSelectionButton()){
        m_node->toggleForcedExpansion();
    }

    // calling the superclass is mandatory here, to treat doubleclick like normal click PLUS something else.
    // othwerise the draw edge does not get aquired properly
    return QGraphicsObject::mouseDoubleClickEvent(event);
}

} // namespace zodiac
