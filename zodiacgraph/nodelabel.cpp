#include "nodelabel.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

#include "node.h"

namespace zodiac {

QColor NodeLabel::s_textColor = QColor("#ffffff");
QColor NodeLabel::s_backgroundColor = QColor("#426998");
QColor NodeLabel::s_lineColor = QColor("#cdcdcd");
qreal NodeLabel::s_outlineWidth = 1.5;
uint NodeLabel::s_roundEdgeRadius = 8;
qreal NodeLabel::s_verticalMargin = 2.;
qreal NodeLabel::s_horizontalMargin = 4.;
QFont NodeLabel::s_font = QFont("DejaVu Sans Mono", 9, QFont::DemiBold);
QPen NodeLabel::s_linePen = QPen(QBrush(s_lineColor), s_outlineWidth);

NodeLabel::NodeLabel(Node* parent)
    : QGraphicsObject(parent)
{
    // core only expands if you hover above it, not above the label
    setAcceptHoverEvents(false);

    // cache the label
    setCacheMode(DeviceCoordinateCache);

    // get the text from the node
    setText(parent->getDisplayName());
}

void NodeLabel::setText(const QString& text)
{
    // update the text
    m_text = QStaticText(text);
    m_text.setTextFormat(Qt::PlainText);
    QTextOption textOption = QTextOption(Qt::AlignHCenter | Qt::AlignBaseline);
    textOption.setUseDesignMetrics(false);
    m_text.setTextOption(textOption);
    updateStyle();
}

void NodeLabel::updateStyle()
{
    prepareGeometryChange();

    // update the text position
    m_text.prepare(QTransform(), s_font);
    QSizeF textSize = m_text.size();
    m_textPos = QPointF(textSize.width()/-2., textSize.height()/-2.);

    // update the label
    QSizeF labelSize = QSizeF(qMax(textSize.width(), Node::getCoreRadius()*2.), textSize.height());
    m_outlineRect = QRectF((labelSize.width()/-2.)-s_horizontalMargin, m_textPos.y()-s_verticalMargin,
                        labelSize.width()+(s_horizontalMargin*2.), textSize.height()+(s_verticalMargin*2.));

    // update bounding rectangle
    qreal halfLine = s_outlineWidth*0.5;
    m_boundingRect = m_outlineRect.marginsAdded(QMarginsF(halfLine, halfLine, halfLine, halfLine));

    update();
}

QRectF NodeLabel::boundingRect() const
{
    return m_boundingRect;
}

void NodeLabel::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* /* widget */)
{
    painter->setClipRect(option->exposedRect);

    // draw the background
    painter->setPen(s_linePen);
    painter->setBrush(s_backgroundColor);
    painter->drawRoundedRect(m_outlineRect, s_roundEdgeRadius, s_roundEdgeRadius, Qt::AbsoluteSize);

    // draw the text
    painter->setFont(s_font);
    painter->setPen(s_textColor);
    painter->drawStaticText(m_textPos, m_text);
}

QPainterPath NodeLabel::shape() const
{
    QPainterPath path;
    path.addRect(m_boundingRect);
    return path;
}

void NodeLabel::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QPointF p = event->pos();
    if((p.x()*p.x())+(p.y()*p.y())<=(Node::getCoreRadius() * Node::getCoreRadius())){
        // if the event is label was clicked inside the core, let the parent handle it
        event->ignore();

    } else {
        // otherwise adjust the current selection "on-foot", since we cannot fall back on Qt's own selection mechanism
        if(event->modifiers() & Qt::ControlModifier){
            parentItem()->setSelected(!parentItem()->isSelected()); // multi-selection
        } else {
            for(QGraphicsItem* item : scene()->selectedItems()){    // single selection
                item->setSelected(false);
            }
            parentItem()->setSelected(true);
        }
        event->accept();
    }
}

} // namespace zodiac
