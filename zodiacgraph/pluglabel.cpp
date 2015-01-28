#include "pluglabel.h"

#include <QtMath>                       // for M_PI
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "plug.h"

namespace zodiac {

QFont PlugLabel::s_font = QFont("DejaVu Sans Mono", 10, 75);
qreal PlugLabel::s_labelDistance = 15.;
QColor PlugLabel::s_color = QColor("#828688");

PlugLabel::PlugLabel(Plug* parent)
    : QGraphicsItem(parent)
    , m_plug(parent)
    , m_isHighlighted(false)
{
    // the label does not react to mouse events
    setAcceptHoverEvents(false);

    // cache the label
    setCacheMode(DeviceCoordinateCache);

    // initialize
    setOpacity(0.); // to avoid flicker, the label starts out fully transparent
    updateShape();
}

void PlugLabel::updateShape()
{
    prepareGeometryChange();

    // update the text
    m_text = QStaticText(m_plug->getName());
    m_text.setTextFormat(Qt::PlainText);
    m_text.prepare(QTransform(), s_font); // prepare once to get the dimensions of the text

    // update the label transformation
    QVector2D normal = m_plug->getNormal();
    qreal angle = atan2(-normal.y(), normal.x());
    QSizeF textSize = m_text.size();
    qreal xOffset;
    if(normal.x()>0){   // label is on the right
        xOffset = s_labelDistance;
    } else {            // label is on the left
        xOffset = -textSize.width()-s_labelDistance;
        angle+=M_PI;
    }
    m_transform.reset();
    m_transform.rotateRadians(-angle);
    m_transform.translate(xOffset, textSize.height()/-2.);

    m_text.prepare(m_transform, s_font);    // prepare a second time with the correct transform matrix

    // update the bounding rect
    m_boundingRect = m_transform.mapRect(QRectF(0, 0, textSize.width(), textSize.height()));
}

void PlugLabel::updateStyle()
{
    updateShape();
    update();
}

QRectF PlugLabel::boundingRect() const
{
    return m_boundingRect;
}

void PlugLabel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /*widget*/)
{
    painter->setClipRect(option->exposedRect);
    painter->setTransform(m_transform * painter->transform());
    painter->setFont(s_font);
    painter->setPen(QPen(m_isHighlighted?Plug::getHighlightColor():s_color));
    painter->drawStaticText(0,0,m_text);
}

QPainterPath PlugLabel::shape() const
{
    QPainterPath path;
    path.addRect(m_boundingRect);
    return path;
}

} // namespace zodiac
