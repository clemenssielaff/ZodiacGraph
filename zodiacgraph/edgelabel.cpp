#include "edgelabel.h"

#include <QBrush>

#include "utils.h"

namespace zodiac {

QFont EdgeLabel::s_font = QFont("DejaVu Sans Mono", 10, 75);
QColor EdgeLabel::s_color = QColor(200,200,200,180);
qreal EdgeLabel::s_verticalOffset = 0.5;

EdgeLabel::EdgeLabel()
    : QGraphicsSimpleTextItem(nullptr)
{
    // the label does not react to mouse events
    setAcceptHoverEvents(false);

    // cache the label
    setCacheMode(DeviceCoordinateCache);

    // define settings
    setOpacity(0.); // start out hidden
    setZValue(zStack::EDGE_LABEL);
    updateStyle();
}

void EdgeLabel::setPos(qreal x, qreal y)
{
    // apply offsets to the given position
    x -= boundingRect().width() * .5;
    y -= boundingRect().height() * (s_verticalOffset+.5);
    QGraphicsItem::setPos(x,y);
}

void EdgeLabel::updateStyle()
{
    // update the font, if the static label font has changed
    if(font()!=s_font){
        setFont(s_font);
    }

    // update the color, if the static label color has changed
    if(brush().color()!=s_color){
        setBrush(s_color);
    }
}

} // namespace zodiac
