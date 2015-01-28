#include "collapsible.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

const QString Collapsible::s_downArrow = QString::fromUtf8(" \xE2\x96\xBC   ");
const QString Collapsible::s_upArrow = QString::fromUtf8(" \xE2\x96\xB2   ");
const int Collapsible::s_maxWidth = 400;

Collapsible::Collapsible(QWidget *parent)
    : QFrame(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_displayWidget(nullptr)
    , m_titleButton(new QPushButton(this))
    , m_title(QString())
{
    setFrameShape(QFrame::NoFrame);
    setMaximumWidth(s_maxWidth);
    setStyleSheet("Collapsible { border: 1px solid #808080; border-radius: 2px; }");

    m_titleButton->setFlat(true);
    m_titleButton->setStatusTip("Click to collapse / expand the section.");
    connect(m_titleButton, SIGNAL(clicked()), this, SLOT(toggleCollapse()));

    m_mainLayout->setMargin(0);
    m_mainLayout->setSpacing(0);
    m_mainLayout->addWidget(m_titleButton);
    setLayout(m_mainLayout);
}

void Collapsible::setWidget(QWidget* displayWidget)
{
    // remove any existing widget
    if(m_displayWidget){
        m_mainLayout->removeWidget(m_displayWidget);
        m_displayWidget->deleteLater();
        m_displayWidget = nullptr;
    }

    // take possession of the widget
    if(displayWidget->parent() != this){
        displayWidget->setParent(this);
    }
    m_displayWidget = displayWidget;
    m_mainLayout->addWidget(m_displayWidget);
}

void Collapsible::toggleCollapse()
{
    m_displayWidget->setHidden(!m_displayWidget->isHidden());
    updateTitle();
}

void Collapsible::updateTitle(const QString& title)
{
    QFontMetrics fontMetrics = QFontMetrics(m_titleButton->font());
    qreal arrowWidth = fontMetrics.boundingRect(s_downArrow).width();

    if(!title.isEmpty()){
        m_title = fontMetrics.elidedText(title, Qt::ElideMiddle, s_maxWidth - arrowWidth);
    }

    // display the correct arrow
    // ... or if there is no display widget yet, always the up arrow, because display widgets are opened by default
    if(m_displayWidget && m_displayWidget->isHidden()){
        m_titleButton->setText(s_downArrow + m_title);
    } else {
        m_titleButton->setText(s_upArrow + m_title);
    }
}
