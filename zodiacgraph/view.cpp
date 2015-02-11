#include "view.h"

///
/// Set to 1 to enable fps output to std out.
///
#define PRINT_REDRAW_SPEED 0

#include <QApplication>
#include <QDebug>
#include <QPanGesture>
#include <QPinchGesture>
#include <QScrollBar>
#include <QWheelEvent>
#if PRINT_REDRAW_SPEED
#include <QElapsedTimer>
#endif

#include "scene.h"

namespace zodiac {

QColor View::s_backgroundColor = QColor("#191919");
qreal View::s_zoomSpeed = 0.001;
Qt::MouseButton View::s_dragMoveButton = Qt::RightButton;
Qt::MouseButton View::s_selectionButton  = Qt::LeftButton;
Qt::MouseButton View::s_removalButton = Qt::MiddleButton;
int View::s_activationKey = Qt::Key_Return;
qreal View::s_minZoomFactor = 0.1;
qreal View::s_maxZoomFactor = 2.0;

View::View(QWidget *parent)
    : QGraphicsView(parent)
    , m_zoomFactor(1.0)
{
    setBackgroundBrush(QBrush(s_backgroundColor));
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::RubberBandDrag);

    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::PanGesture);
    grabGesture(Qt::PinchGesture);
}

void View::updateStyle()
{
    // update all affected members
    setBackgroundBrush(QBrush(s_backgroundColor));

    // force a redraw for good measure
    resetCachedContent();
}

bool View::event(QEvent* event)
{
    event->accept();

    switch(event->type()){

        case QEvent::Gesture:
            setDragMode(QGraphicsView::NoDrag);
            return gestureEvent(static_cast<QGestureEvent*>(event));

        case QEvent::TouchEnd:
            setDragMode(QGraphicsView::RubberBandDrag);
            break;

        default:
            break;
    }
    return QGraphicsView::event(event);
}

bool View::gestureEvent(QGestureEvent* event)
{   
    //
    // pinch has precedence
    if (QGesture *pinchEvent = event->gesture(Qt::PinchGesture)) {
        QPinchGesture* pinch = static_cast<QPinchGesture*>(pinchEvent);

        //
        // only pinch if the fingers have already moved a significant amount
        qreal totalScaleFactor = pinch->totalScaleFactor();
        if((totalScaleFactor < 0.66) || (totalScaleFactor > 1.5)){
            qreal zoomDelta = pinch->scaleFactor();
            qreal resultZoom = m_zoomFactor * zoomDelta;
            if(resultZoom > s_maxZoomFactor){
                zoomDelta = s_maxZoomFactor / m_zoomFactor;
            }else if(resultZoom < s_minZoomFactor){
                zoomDelta = s_minZoomFactor / m_zoomFactor;
            }

            // scale the view
            scale(zoomDelta,zoomDelta);
            m_zoomFactor *= zoomDelta;

            return true;
        }
    }

    //
    // pan
    if (QGesture *panEvent = event->gesture(Qt::PanGesture)) {
        QPanGesture* pan = static_cast<QPanGesture*>(panEvent);
        QPointF delta = pan->delta();
        qreal factor = (1.0 / m_zoomFactor) * 0.9;

        QScrollBar* vScrollBar = verticalScrollBar();
        vScrollBar->setValue(vScrollBar->value() - int(delta.y()/factor));

        QScrollBar* hScrollBar = horizontalScrollBar();
        hScrollBar->setValue(hScrollBar->value() - int(delta.x()/factor));
    }

    return true;
}

bool View::viewportEvent(QEvent *event)
{
    if((event->type()==QEvent::Leave) && (qApp->mouseButtons()!=Qt::NoButton)){
        // leaving the window while dragging must not trigger a dragRelease event
        return true;
    }
    return QGraphicsView::viewportEvent(event);
}

void View::mousePressEvent(QMouseEvent* event)
{
    static const QTransform nullTransform = QTransform();

    if(event->button() == s_dragMoveButton){
        // only allow scroll dragging if no item is clicked
        if(!scene()->itemAt(mapToScene(event->pos()), nullTransform)){
            setDragMode(QGraphicsView::ScrollHandDrag);
            QMouseEvent fakeEvent(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
            QGraphicsView::mousePressEvent(&fakeEvent);
        }
        return;

    } else if (event->button() != s_selectionButton){
        // do not allow rubberband selection with any button other than the selection button
        setDragMode(QGraphicsView::NoDrag);
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == s_dragMoveButton){
        // disable scroll dragging, if it was enabled
        if(dragMode()==QGraphicsView::ScrollHandDrag){
            QMouseEvent fakeEvent(event->type(), event->pos(), Qt::LeftButton, Qt::LeftButton, event->modifiers());
            QGraphicsView::mouseReleaseEvent(&fakeEvent);
        }
    }

    // make sure to reset the drag mode
    setDragMode(QGraphicsView::RubberBandDrag);

    QGraphicsView::mouseReleaseEvent(event);
}

void View::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->buttons() & s_selectionButton){
        // double clicking into empty space collapse all nodes
        if(!itemAt(event->pos())){
            static_cast<Scene*>(scene())->collapseAllNodes();
        }
    }
    QGraphicsView::mouseDoubleClickEvent(event);
}

void View::wheelEvent(QWheelEvent *event)
{
    // calculate the zoom factor and make sure it does not exceed its range
    qreal zoomDelta = 1. + (event->angleDelta().y() * s_zoomSpeed);
    qreal resultZoom = m_zoomFactor * zoomDelta;
    if(resultZoom > s_maxZoomFactor){
        zoomDelta = s_maxZoomFactor / m_zoomFactor;
    }else if(resultZoom < s_minZoomFactor){
        zoomDelta = s_minZoomFactor / m_zoomFactor;
    }

    // scale the view
    scale(zoomDelta,zoomDelta);
    m_zoomFactor *= zoomDelta;

    // do not call QGraphicsView::wheelEvent here, because it will scroll up or down as well as zoom
    return;
}

void View::paintEvent(QPaintEvent* event)
{
#if PRINT_REDRAW_SPEED
    static quint64 total=0;
    static quint64 divisor=1;
    QElapsedTimer timer;
    timer.start();
#endif

    QGraphicsView::paintEvent(event);

#if PRINT_REDRAW_SPEED
    qint64 duration = timer.nsecsElapsed();
    total+=duration;
    if(divisor%100==0){
        qDebug() << (total/divisor) * 0.000001 << "ms";
        total=0;
        divisor=0;
    }
    divisor++;
#endif
}

void View::setScene(Scene *scene)
{
    QGraphicsView::setScene(scene);
}

} // namespace zodiac
