#include "mainwindow.h"

#include <QApplication>
#include <QAction>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QSplitter>
#include <QStyleFactory>
#include <QToolBar>

#include "nodectrl.h"
#include "mainctrl.h"
#include "propertyeditor.h"
#include "zodiacgraph/baseedge.h"
#include "zodiacgraph/edgearrow.h"
#include "zodiacgraph/edgelabel.h"
#include "zodiacgraph/node.h"
#include "zodiacgraph/nodelabel.h"
#include "zodiacgraph/perimeter.h"
#include "zodiacgraph/plug.h"
#include "zodiacgraph/pluglabel.h"
#include "zodiacgraph/scene.h"
#include "zodiacgraph/view.h"

void createZodiacLogo(MainCtrl* mainCtrl);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("ZodiacGraph - Showcase Application");
    setWindowIcon(QIcon(":/icons/zodiac_logo.png"));

    // create the main toolbar
    QToolBar* mainToolBar = new QToolBar(this);
    mainToolBar->setStyleSheet("QToolBar {border: 0px;}");
    mainToolBar->setIconSize(QSize(12,12));
    mainToolBar->setMovable(false);
    mainToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addToolBar(Qt::TopToolBarArea, mainToolBar);

    // create the status bar
    statusBar();

    // create the Zodiac graph
    zodiac::Scene* zodiacScene = new zodiac::Scene(this);
    zodiac::View* zodiacView = new zodiac::View(this);
    zodiacView->setScene(zodiacScene);

    // create the Property Editor
    PropertyEditor* propertyEditor = new PropertyEditor(this);

    // create the Main Controller
    m_mainCtrl = new MainCtrl(this, zodiacScene, propertyEditor);

    // setup the main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    m_mainSplitter->addWidget(propertyEditor);
    m_mainSplitter->addWidget(zodiacView);
    m_mainSplitter->setSizes({100, 900});

    // create global actions
    QAction* newNodeAction = new QAction(QIcon(":/icons/plus.svg"), tr("&Add Node"), this);
    newNodeAction->setShortcuts(QKeySequence::New);
    newNodeAction->setStatusTip(tr("Create a new Node"));
    mainToolBar->addAction(newNodeAction);
    connect(newNodeAction, SIGNAL(triggered()), m_mainCtrl, SLOT(createDefaultNode()));

    QWidget* emptySpacer = new QWidget();
    emptySpacer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    mainToolBar->addWidget(emptySpacer);

    QAction* aboutAction = new QAction(QIcon(":/icons/questionmark.svg"), tr("&About"), this);
    aboutAction->setStatusTip(tr("Show the about box"));
    mainToolBar->addAction(aboutAction);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(displayAbout()));

    // initialize the GUI
    setCentralWidget(m_mainSplitter);
    readSettings();
    zodiacScene->updateStyle();
    zodiacView->updateStyle();

    createZodiacLogo(m_mainCtrl);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_mainCtrl->shutdown()) {
        // if the user accepted, close the application
        writeSettings();
        event->accept();

    } else {
        // if the user objected, do nothing
        event->ignore();
    }
}

void MainWindow::displayAbout()
{
    QMessageBox aboutBox;
    aboutBox.setWindowTitle("About the ZodiacGraph Showcase Application");
    aboutBox.setText(
        "<h3>About this Application</h3>"
        "This example demonstrates how to use the <b>ZodiacGraph</b> both as a user and (on the source level) as a "
        "module of your own application."

        "<h3>License</h3>"
        "<b>ZodiacGraph</b> is developed by <a href=\"http://www.clemens-sielaff.com\" title=\"clemens-sielaff.com\">Clemens Sielaff</a> "
        "and released under the terms of the <a href=\"https://opensource.org/licenses/MIT\" title=\"MIT License</a>."

        "<h3>Icons</h3>"
        "Icons from <a href=\"http://www.flaticon.com\" title=\"Flaticon\">www.flaticon.com</a>, licensed under <a href=\"http://creativecommons.org/licenses/by/3.0/\" title=\"Creative Commons BY 3.0\">CC BY 3.0</a><br>"
        "&#8594; Arrows, Plus, Minus & Bucket Icons made by <a href=\"http://www.freepik.com\" title=\"Freepik\">Freepik</a><br>"
        "&#8594; Door Icons made by <a href=\"http://www.icomoon.io\" title=\"Icomoon\">Icomoon</a><br>"
        "&#8594; Questionmark Icon made by <a href=\"http://www.danielbruce.se\" title=\"Daniel Bruce\">Daniel Bruce</a><br>"
        "&#8594; Play Icon made by <a href=\"http://yanlu.de\" title=\"Yannick\">Yannick</a>"
         );
    aboutBox.exec();
}

void MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

    // apply custom palette and style
    QPalette palette;
    settings.beginGroup("palette");
    palette.setColor(QPalette::Window, settings.value("window", "#353535").toString());
    palette.setColor(QPalette::WindowText, settings.value("windowText", "#E0E2E4").toString());
    palette.setColor(QPalette::Base, settings.value("base", "#191919").toString());
    palette.setColor(QPalette::AlternateBase, settings.value("alternateBase", "#353535").toString());
    palette.setColor(QPalette::ToolTipBase, settings.value("toolTipBase", "#000000").toString());
    palette.setColor(QPalette::ToolTipText, settings.value("toolTipText", "#808080").toString());
    palette.setColor(QPalette::Text, settings.value("text", "#E0E2E4").toString());
    palette.setColor(QPalette::Button, settings.value("button", "#353535").toString());
    palette.setColor(QPalette::ButtonText, settings.value("buttonText", "#E0E2E4").toString());
    palette.setColor(QPalette::BrightText, settings.value("brightText", "#ffffff").toString());
    palette.setColor(QPalette::Link, settings.value("link", "#2a82da").toString());
    palette.setColor(QPalette::Highlight, settings.value("highlight", "#2a82da").toString());
    palette.setColor(QPalette::HighlightedText, settings.value("highlightedText", "#000000").toString());
    settings.endGroup();
    qApp->setPalette(palette);
    qApp->setStyle(QStyleFactory::create(settings.value("style", QString("Fusion")).toString()));

    // apply window position and size
    settings.beginGroup("mainWindow");
    resize(settings.value("size", QSize(1080, 600)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    m_mainSplitter->restoreState(settings.value("propertySplitter").toByteArray());
    settings.endGroup();

    // apply ZodiacGraph settings
    settings.beginGroup("zodiac");

    settings.beginGroup("baseEdge");
    zodiac::BaseEdge::setBaseWidth(settings.value("width", 2.5).toReal());
    zodiac::BaseEdge::setBaseColor(settings.value("color", "#cc5d4e").toString());
    settings.endGroup();

    settings.beginGroup("edgeArrow");
    zodiac::EdgeArrow::defineArrow(settings.value("length", 12).toReal(), settings.value("width", 15).toReal());
    zodiac::EdgeArrow::setArrowColor(settings.value("color", "#cc5d4e").toString());
    settings.endGroup();

    settings.beginGroup("edgeLabel");
    zodiac::EdgeLabel::setFontFamily(settings.value("fontFamily", "DejaVu Sans Mono").toString());
    zodiac::EdgeLabel::setPointSize(settings.value("pointSize", 10).toReal());
    zodiac::EdgeLabel::setWeight(QFont::Weight(settings.value("weight", 75).toInt()));
    zodiac::EdgeLabel::setColor(settings.value("color", "#c8c8c8").toString());
    zodiac::EdgeLabel::setTransparency(settings.value("transparency", 0.7).toReal());
    zodiac::EdgeLabel::setVerticalOffset(settings.value("verticalOffset", 0.5).toReal());
    settings.endGroup();

    settings.beginGroup("node");
    zodiac::Node::setCoreRadius(settings.value("coreRadius", 25).toReal());
    zodiac::Node::setIdleColor(settings.value("idleColor", "#4b77a7").toString());
    zodiac::Node::setSelectedColor(settings.value("selectedColor", "#62abfa").toString());
    zodiac::Node::setOutlineColor(settings.value("outlineColor", "#cdcdcd").toString());
    zodiac::Node::setOutlineWidth(settings.value("outlineWidth", 3).toReal());
    zodiac::Node::setPlugSweep(settings.value("plugSweep", 32.5).toReal());
    zodiac::Node::setGapSweep(settings.value("gapSweep", 8.125).toReal());
    settings.endGroup();

    settings.beginGroup("nodeLabel");
    zodiac::NodeLabel::setTextColor(settings.value("textColor", "#ffffff").toString());
    zodiac::NodeLabel::setBackgroundColor(settings.value("backgroundColor", "#426998").toString());
    zodiac::NodeLabel::setLineColor(settings.value("lineColor", "#cdcdcd").toString());
    zodiac::NodeLabel::setLineWidth(settings.value("lineWidth", 1.5).toReal());
    zodiac::NodeLabel::setCornerRadius(settings.value("cornerRadius", 8).toReal());
    zodiac::NodeLabel::setVerticalMargin(settings.value("verticalMargin", 2).toReal());
    zodiac::NodeLabel::setHorizontalMargin(settings.value("horizontalMargin", 4).toReal());
    zodiac::NodeLabel::setFontFamily(settings.value("fontFamily", "DejaVu Sans Mono").toString());
    zodiac::NodeLabel::setPointSize(settings.value("pointSize", 9).toReal());
    zodiac::NodeLabel::setWeight(QFont::Weight(settings.value("weight", 63).toInt()));
    settings.endGroup();

    settings.beginGroup("perimeter");
    zodiac::Perimeter::setColor(settings.value("color", "#2b517d").toString());
    settings.endGroup();

    settings.beginGroup("plug");
    zodiac::Plug::setWidth(settings.value("width", 12).toReal());
    zodiac::Plug::setInColor(settings.value("inColor", "#728872").toString());
    zodiac::Plug::setOutColor(settings.value("outColor", "#887272").toString());
    zodiac::Plug::setHighlightColor(settings.value("highlightColor", "#d1d7db").toString());
    settings.endGroup();

    settings.beginGroup("plugLabel");
    zodiac::PlugLabel::setFontFamily(settings.value("fontFamily", "DejaVu Sans Mono").toString());
    zodiac::PlugLabel::setPointSize(settings.value("pointSize", 10).toReal());
    zodiac::PlugLabel::setWeight(QFont::Weight(settings.value("weight", 75).toInt()));
    zodiac::PlugLabel::setColor(settings.value("color", "#828688").toString());
    zodiac::PlugLabel::setLabelDistance(settings.value("distance", 15).toReal());
    settings.endGroup();

    settings.beginGroup("view");
    zodiac::View::setBackgroundColor(settings.value("backgroundColor", "#191919").toString());
    settings.endGroup();

    settings.endGroup(); // zodiac
}

void MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, qApp->organizationName(), qApp->applicationName());

    // write out current palette
    QPalette palette = qApp->palette();
    settings.beginGroup("palette");
    settings.setValue("window", palette.color(QPalette::Window).name());
    settings.setValue("windowText", palette.color(QPalette::WindowText).name());
    settings.setValue("base", palette.color(QPalette::Base).name());
    settings.setValue("alternateBase", palette.color(QPalette::AlternateBase).name());
    settings.setValue("toolTipBase", palette.color(QPalette::ToolTipBase).name());
    settings.setValue("toolTipText", palette.color(QPalette::ToolTipText).name());
    settings.setValue("text", palette.color(QPalette::Text).name());
    settings.setValue("button", palette.color(QPalette::Button).name());
    settings.setValue("buttonText", palette.color(QPalette::ButtonText).name());
    settings.setValue("brightText", palette.color(QPalette::BrightText).name());
    settings.setValue("link", palette.color(QPalette::Link).name());
    settings.setValue("highlight", palette.color(QPalette::Highlight).name());
    settings.setValue("highlightedText", palette.color(QPalette::HighlightedText).name());
    settings.endGroup();

    // write out MainWindow position and size
    settings.beginGroup("mainWindow");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("propertySplitter", m_mainSplitter->saveState());
    settings.endGroup();

    // write out ZodiacGraph settings
    settings.beginGroup("zodiac");

    settings.beginGroup("baseEdge");
    settings.setValue("width", zodiac::BaseEdge::getBaseWidth());
    settings.setValue("color", zodiac::BaseEdge::getBaseColor().name());
    settings.endGroup();

    settings.beginGroup("edgeArrow");
    settings.setValue("width", zodiac::EdgeArrow::getArrowWidth());
    settings.setValue("length", zodiac::EdgeArrow::getArrowLength());
    settings.setValue("color", zodiac::EdgeArrow::getArrowColor().name());
    settings.endGroup();

    settings.beginGroup("edgeLabel");
    settings.setValue("fontFamily", zodiac::EdgeLabel::getFontFamily());
    settings.setValue("pointSize", zodiac::EdgeLabel::getPointSize());
    settings.setValue("weight", zodiac::EdgeLabel::getWeight());
    settings.setValue("color", zodiac::EdgeLabel::getColor().name());
    settings.setValue("transparency", zodiac::EdgeLabel::getTransparency());
    settings.setValue("verticalOffset", zodiac::EdgeLabel::getVerticalOffset());
    settings.endGroup();

    settings.beginGroup("node");
    settings.setValue("coreRadius", zodiac::Node::getCoreRadius());
    settings.setValue("idleColor", zodiac::Node::getIdleColor().name());
    settings.setValue("selectedColor", zodiac::Node::getSelectedColor().name());
    settings.setValue("outlineColor", zodiac::Node::getOutlineColor().name());
    settings.setValue("outlineWidth", zodiac::Node::getOutlineWidth());
    settings.setValue("plugSweep", zodiac::Node::getPlugSweep());
    settings.setValue("gapSweep", zodiac::Node::getGapSweep());
    settings.endGroup();

    settings.beginGroup("nodeLabel");
    settings.setValue("textColor", zodiac::NodeLabel::getTextColor().name());
    settings.setValue("backgroundColor", zodiac::NodeLabel::getBackgroundColor().name());
    settings.setValue("lineColor", zodiac::NodeLabel::getLineColor().name());
    settings.setValue("lineWidth", zodiac::NodeLabel::getLineWidth());
    settings.setValue("cornerRadius", zodiac::NodeLabel::getCornerRadius());
    settings.setValue("verticalMargin", zodiac::NodeLabel::getVerticalMargin());
    settings.setValue("horizontalMargin", zodiac::NodeLabel::getHorizontalMargin());
    settings.setValue("fontFamily", zodiac::NodeLabel::getFontFamily());
    settings.setValue("pointSize", zodiac::NodeLabel::getPointSize());
    settings.setValue("weight", zodiac::NodeLabel::getWeight());
    settings.endGroup();

    settings.beginGroup("perimeter");
    settings.setValue("color", zodiac::Perimeter::getColor().name());
    settings.endGroup();

    settings.beginGroup("plug");
    settings.setValue("width", zodiac::Plug::getWidth());
    settings.setValue("inColor", zodiac::Plug::getInColor().name());
    settings.setValue("outColor", zodiac::Plug::getOutColor().name());
    settings.setValue("highlightColor", zodiac::Plug::getHighlightColor().name());
    settings.endGroup();

    settings.beginGroup("plugLabel");
    settings.setValue("fontFamily", zodiac::PlugLabel::getFontFamily());
    settings.setValue("pointSize", zodiac::PlugLabel::getPointSize());
    settings.setValue("weight", zodiac::PlugLabel::getWeight());
    settings.setValue("color", zodiac::PlugLabel::getColor().name());
    settings.setValue("distance", zodiac::PlugLabel::getLabelDistance());
    settings.endGroup();

    settings.beginGroup("view");
    settings.setValue("backgroundColor", zodiac::View::getBackgroundColor().name());
    settings.endGroup();

    settings.endGroup(); // zodiac
}

void createZodiacLogo(MainCtrl* mainCtrl)
{
    NodeCtrl* nodeCtrl12 = mainCtrl->createNode("Node 12");
    nodeCtrl12->getNodeHandle().setPos(-1360.86, 265.708);
    nodeCtrl12->addOutgoingPlug("plug");
    nodeCtrl12->addIncomingPlug("plug_10");
    nodeCtrl12->addOutgoingPlug("plug_11");
    nodeCtrl12->addOutgoingPlug("plug_12");
    nodeCtrl12->addOutgoingPlug("plug_13");
    nodeCtrl12->addIncomingPlug("plug_2");
    nodeCtrl12->addIncomingPlug("plug_3");
    nodeCtrl12->addOutgoingPlug("plug_4");
    nodeCtrl12->addIncomingPlug("plug_5");
    nodeCtrl12->addIncomingPlug("plug_6");
    nodeCtrl12->addOutgoingPlug("plug_7");
    nodeCtrl12->addOutgoingPlug("plug_8");
    nodeCtrl12->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl8 = mainCtrl->createNode("Node 8");
    nodeCtrl8->getNodeHandle().setPos(-1533.75, 121.789);
    nodeCtrl8->addOutgoingPlug("plug");
    nodeCtrl8->addIncomingPlug("plug_10");
    nodeCtrl8->addOutgoingPlug("plug_11");
    nodeCtrl8->addIncomingPlug("plug_12");
    nodeCtrl8->addOutgoingPlug("plug_2");
    nodeCtrl8->addIncomingPlug("plug_3");
    nodeCtrl8->addIncomingPlug("plug_4");
    nodeCtrl8->addOutgoingPlug("plug_5");
    nodeCtrl8->addIncomingPlug("plug_6");
    nodeCtrl8->addIncomingPlug("plug_7");
    nodeCtrl8->addOutgoingPlug("plug_8");
    nodeCtrl8->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl16 = mainCtrl->createNode("Node 16");
    nodeCtrl16->getNodeHandle().setPos(-628.364, 351.884);
    nodeCtrl16->addOutgoingPlug("plug");
    nodeCtrl16->addOutgoingPlug("plug_2");
    nodeCtrl16->addOutgoingPlug("plug_3");
    nodeCtrl16->addIncomingPlug("plug_4");
    nodeCtrl16->addOutgoingPlug("plug_5");
    nodeCtrl16->addIncomingPlug("plug_6");

    NodeCtrl* nodeCtrl15 = mainCtrl->createNode("Node 15");
    nodeCtrl15->getNodeHandle().setPos(-858.166, -276.48);
    nodeCtrl15->addOutgoingPlug("plug");
    nodeCtrl15->addOutgoingPlug("plug_10");
    nodeCtrl15->addIncomingPlug("plug_11");
    nodeCtrl15->addOutgoingPlug("plug_12");
    nodeCtrl15->addIncomingPlug("plug_2");
    nodeCtrl15->addOutgoingPlug("plug_3");
    nodeCtrl15->addOutgoingPlug("plug_4");
    nodeCtrl15->addIncomingPlug("plug_5");
    nodeCtrl15->addOutgoingPlug("plug_6");
    nodeCtrl15->addOutgoingPlug("plug_7");
    nodeCtrl15->addIncomingPlug("plug_8");
    nodeCtrl15->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl3 = mainCtrl->createNode("Node 3");
    nodeCtrl3->getNodeHandle().setPos(-1562.21, -291.632);
    nodeCtrl3->addIncomingPlug("plug");
    nodeCtrl3->addIncomingPlug("plug_2");
    nodeCtrl3->addIncomingPlug("plug_3");
    nodeCtrl3->addIncomingPlug("plug_4");

    NodeCtrl* nodeCtrl4 = mainCtrl->createNode("Node 4");
    nodeCtrl4->getNodeHandle().setPos(-1732.21, -11.0822);
    nodeCtrl4->addIncomingPlug("plug");
    nodeCtrl4->addIncomingPlug("plug_10");
    nodeCtrl4->addOutgoingPlug("plug_11");
    nodeCtrl4->addIncomingPlug("plug_12");
    nodeCtrl4->addIncomingPlug("plug_13");
    nodeCtrl4->addIncomingPlug("plug_14");
    nodeCtrl4->addOutgoingPlug("plug_15");
    nodeCtrl4->addIncomingPlug("plug_2");
    nodeCtrl4->addIncomingPlug("plug_3");
    nodeCtrl4->addIncomingPlug("plug_4");
    nodeCtrl4->addIncomingPlug("plug_5");
    nodeCtrl4->addOutgoingPlug("plug_6");
    nodeCtrl4->addOutgoingPlug("plug_7");
    nodeCtrl4->addOutgoingPlug("plug_8");
    nodeCtrl4->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl31 = mainCtrl->createNode("Node 31");
    nodeCtrl31->getNodeHandle().setPos(725.312, -323.159);
    nodeCtrl31->addIncomingPlug("plug");
    nodeCtrl31->addOutgoingPlug("plug_10");
    nodeCtrl31->addIncomingPlug("plug_2");
    nodeCtrl31->addOutgoingPlug("plug_3");
    nodeCtrl31->addOutgoingPlug("plug_4");
    nodeCtrl31->addOutgoingPlug("plug_5");
    nodeCtrl31->addIncomingPlug("plug_6");
    nodeCtrl31->addIncomingPlug("plug_7");
    nodeCtrl31->addIncomingPlug("plug_8");
    nodeCtrl31->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl17 = mainCtrl->createNode("Node 17");
    nodeCtrl17->getNodeHandle().setPos(-517.054, 100.538);
    nodeCtrl17->addOutgoingPlug("plug");
    nodeCtrl17->addIncomingPlug("plug_2");
    nodeCtrl17->addOutgoingPlug("plug_3");
    nodeCtrl17->addIncomingPlug("plug_4");
    nodeCtrl17->addIncomingPlug("plug_5");

    NodeCtrl* nodeCtrl38 = mainCtrl->createNode("Node 38");
    nodeCtrl38->getNodeHandle().setPos(1317.77, 359.065);
    nodeCtrl38->addOutgoingPlug("plug");
    nodeCtrl38->addOutgoingPlug("plug_2");
    nodeCtrl38->addOutgoingPlug("plug_3");
    nodeCtrl38->addOutgoingPlug("plug_4");
    nodeCtrl38->addOutgoingPlug("plug_5");
    nodeCtrl38->addOutgoingPlug("plug_6");
    nodeCtrl38->addOutgoingPlug("plug_7");
    nodeCtrl38->addIncomingPlug("plug_8");

    NodeCtrl* nodeCtrl6 = mainCtrl->createNode("Node 6");
    nodeCtrl6->getNodeHandle().setPos(-1969.34, 253.69);
    nodeCtrl6->addOutgoingPlug("plug");
    nodeCtrl6->addIncomingPlug("plug_10");
    nodeCtrl6->addIncomingPlug("plug_11");
    nodeCtrl6->addOutgoingPlug("plug_12");
    nodeCtrl6->addIncomingPlug("plug_13");
    nodeCtrl6->addOutgoingPlug("plug_2");
    nodeCtrl6->addOutgoingPlug("plug_3");
    nodeCtrl6->addOutgoingPlug("plug_4");
    nodeCtrl6->addIncomingPlug("plug_5");
    nodeCtrl6->addIncomingPlug("plug_6");
    nodeCtrl6->addIncomingPlug("plug_7");
    nodeCtrl6->addOutgoingPlug("plug_8");
    nodeCtrl6->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl44 = mainCtrl->createNode("Node 44");
    nodeCtrl44->getNodeHandle().setPos(-1371.63, 75.4037);
    nodeCtrl44->addOutgoingPlug("plug");
    nodeCtrl44->addIncomingPlug("plug_10");
    nodeCtrl44->addIncomingPlug("plug_11");
    nodeCtrl44->addOutgoingPlug("plug_12");
    nodeCtrl44->addIncomingPlug("plug_13");
    nodeCtrl44->addOutgoingPlug("plug_14");
    nodeCtrl44->addOutgoingPlug("plug_15");
    nodeCtrl44->addOutgoingPlug("plug_2");
    nodeCtrl44->addOutgoingPlug("plug_3");
    nodeCtrl44->addOutgoingPlug("plug_4");
    nodeCtrl44->addIncomingPlug("plug_5");
    nodeCtrl44->addIncomingPlug("plug_6");
    nodeCtrl44->addIncomingPlug("plug_7");
    nodeCtrl44->addOutgoingPlug("plug_8");
    nodeCtrl44->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl35 = mainCtrl->createNode("Node 35");
    nodeCtrl35->getNodeHandle().setPos(1019.75, -75.4037);
    nodeCtrl35->addOutgoingPlug("plug");
    nodeCtrl35->addOutgoingPlug("plug_2");
    nodeCtrl35->addOutgoingPlug("plug_3");
    nodeCtrl35->addOutgoingPlug("plug_4");
    nodeCtrl35->addOutgoingPlug("plug_5");

    NodeCtrl* nodeCtrl1 = mainCtrl->createNode("Node 1");
    nodeCtrl1->getNodeHandle().setPos(-2151.8, -364.222);
    nodeCtrl1->addOutgoingPlug("plug");
    nodeCtrl1->addIncomingPlug("plug_2");
    nodeCtrl1->addOutgoingPlug("plug_3");
    nodeCtrl1->addOutgoingPlug("plug_4");

    NodeCtrl* nodeCtrl39 = mainCtrl->createNode("Node 39");
    nodeCtrl39->getNodeHandle().setPos(1123.87, 111.31);
    nodeCtrl39->addIncomingPlug("plug");
    nodeCtrl39->addIncomingPlug("plug_10");
    nodeCtrl39->addIncomingPlug("plug_11");
    nodeCtrl39->addIncomingPlug("plug_2");
    nodeCtrl39->addIncomingPlug("plug_3");
    nodeCtrl39->addOutgoingPlug("plug_4");
    nodeCtrl39->addIncomingPlug("plug_5");
    nodeCtrl39->addIncomingPlug("plug_6");
    nodeCtrl39->addOutgoingPlug("plug_7");
    nodeCtrl39->addIncomingPlug("plug_8");
    nodeCtrl39->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl37 = mainCtrl->createNode("Node 37");
    nodeCtrl37->getNodeHandle().setPos(1687.61, 290.843);
    nodeCtrl37->addIncomingPlug("plug");
    nodeCtrl37->addOutgoingPlug("plug_2");
    nodeCtrl37->addIncomingPlug("plug_3");
    nodeCtrl37->addIncomingPlug("plug_4");

    NodeCtrl* nodeCtrl30 = mainCtrl->createNode("Node 30");
    nodeCtrl30->getNodeHandle().setPos(466.785, -441.65);
    nodeCtrl30->addOutgoingPlug("plug");
    nodeCtrl30->addOutgoingPlug("plug_10");
    nodeCtrl30->addIncomingPlug("plug_11");
    nodeCtrl30->addOutgoingPlug("plug_12");
    nodeCtrl30->addOutgoingPlug("plug_13");
    nodeCtrl30->addIncomingPlug("plug_14");
    nodeCtrl30->addIncomingPlug("plug_15");
    nodeCtrl30->addOutgoingPlug("plug_2");
    nodeCtrl30->addIncomingPlug("plug_3");
    nodeCtrl30->addOutgoingPlug("plug_4");
    nodeCtrl30->addIncomingPlug("plug_5");
    nodeCtrl30->addIncomingPlug("plug_6");
    nodeCtrl30->addOutgoingPlug("plug_7");
    nodeCtrl30->addOutgoingPlug("plug_8");
    nodeCtrl30->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl13 = mainCtrl->createNode("Node 13");
    nodeCtrl13->getNodeHandle().setPos(-1066.42, 319.568);
    nodeCtrl13->addIncomingPlug("plug");
    nodeCtrl13->addOutgoingPlug("plug_10");
    nodeCtrl13->addOutgoingPlug("plug_11");
    nodeCtrl13->addOutgoingPlug("plug_12");
    nodeCtrl13->addIncomingPlug("plug_2");
    nodeCtrl13->addIncomingPlug("plug_3");
    nodeCtrl13->addOutgoingPlug("plug_4");
    nodeCtrl13->addIncomingPlug("plug_5");
    nodeCtrl13->addOutgoingPlug("plug_6");
    nodeCtrl13->addOutgoingPlug("plug_7");
    nodeCtrl13->addIncomingPlug("plug_8");
    nodeCtrl13->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl23 = mainCtrl->createNode("Node 23");
    nodeCtrl23->getNodeHandle().setPos(-93.3569, 402.153);
    nodeCtrl23->addIncomingPlug("plug");
    nodeCtrl23->addOutgoingPlug("plug_2");
    nodeCtrl23->addIncomingPlug("plug_3");
    nodeCtrl23->addIncomingPlug("plug_4");
    nodeCtrl23->addOutgoingPlug("plug_5");
    nodeCtrl23->addOutgoingPlug("plug_6");
    nodeCtrl23->addIncomingPlug("plug_7");

    NodeCtrl* nodeCtrl26 = mainCtrl->createNode("Node 26");
    nodeCtrl26->getNodeHandle().setPos(-46.6785, -430.878);
    nodeCtrl26->addOutgoingPlug("plug");
    nodeCtrl26->addOutgoingPlug("plug_10");
    nodeCtrl26->addOutgoingPlug("plug_11");
    nodeCtrl26->addOutgoingPlug("plug_2");
    nodeCtrl26->addIncomingPlug("plug_3");
    nodeCtrl26->addOutgoingPlug("plug_4");
    nodeCtrl26->addOutgoingPlug("plug_5");
    nodeCtrl26->addOutgoingPlug("plug_6");
    nodeCtrl26->addOutgoingPlug("plug_7");
    nodeCtrl26->addOutgoingPlug("plug_8");
    nodeCtrl26->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl14 = mainCtrl->createNode("Node 14");
    nodeCtrl14->getNodeHandle().setPos(-840.212, 3.59065);
    nodeCtrl14->addIncomingPlug("plug");
    nodeCtrl14->addIncomingPlug("plug_2");
    nodeCtrl14->addOutgoingPlug("plug_3");
    nodeCtrl14->addIncomingPlug("plug_4");
    nodeCtrl14->addIncomingPlug("plug_5");
    nodeCtrl14->addIncomingPlug("plug_6");
    nodeCtrl14->addOutgoingPlug("plug_7");
    nodeCtrl14->addIncomingPlug("plug_8");

    NodeCtrl* nodeCtrl5 = mainCtrl->createNode("Node 5");
    nodeCtrl5->getNodeHandle().setPos(-2109.21, 232.368);
    nodeCtrl5->addOutgoingPlug("plug");
    nodeCtrl5->addIncomingPlug("plug_10");
    nodeCtrl5->addIncomingPlug("plug_11");
    nodeCtrl5->addOutgoingPlug("plug_12");
    nodeCtrl5->addIncomingPlug("plug_13");
    nodeCtrl5->addOutgoingPlug("plug_2");
    nodeCtrl5->addIncomingPlug("plug_3");
    nodeCtrl5->addOutgoingPlug("plug_4");
    nodeCtrl5->addOutgoingPlug("plug_5");
    nodeCtrl5->addOutgoingPlug("plug_6");
    nodeCtrl5->addOutgoingPlug("plug_7");
    nodeCtrl5->addOutgoingPlug("plug_8");
    nodeCtrl5->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl34 = mainCtrl->createNode("Node 34");
    nodeCtrl34->getNodeHandle().setPos(520.644, -3.59065);
    nodeCtrl34->addIncomingPlug("plug");
    nodeCtrl34->addOutgoingPlug("plug_2");
    nodeCtrl34->addOutgoingPlug("plug_3");
    nodeCtrl34->addOutgoingPlug("plug_4");
    nodeCtrl34->addIncomingPlug("plug_5");
    nodeCtrl34->addIncomingPlug("plug_6");
    nodeCtrl34->addIncomingPlug("plug_7");
    nodeCtrl34->addOutgoingPlug("plug_8");

    NodeCtrl* nodeCtrl25 = mainCtrl->createNode("Node 25");
    nodeCtrl25->getNodeHandle().setPos(161.579, -86.1756);
    nodeCtrl25->addOutgoingPlug("plug");
    nodeCtrl25->addIncomingPlug("plug_10");
    nodeCtrl25->addOutgoingPlug("plug_11");
    nodeCtrl25->addOutgoingPlug("plug_12");
    nodeCtrl25->addOutgoingPlug("plug_2");
    nodeCtrl25->addIncomingPlug("plug_3");
    nodeCtrl25->addIncomingPlug("plug_4");
    nodeCtrl25->addOutgoingPlug("plug_5");
    nodeCtrl25->addOutgoingPlug("plug_6");
    nodeCtrl25->addOutgoingPlug("plug_7");
    nodeCtrl25->addIncomingPlug("plug_8");
    nodeCtrl25->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl20 = mainCtrl->createNode("Node 20");
    nodeCtrl20->getNodeHandle().setPos(-104.129, -71.813);
    nodeCtrl20->addIncomingPlug("plug");
    nodeCtrl20->addOutgoingPlug("plug_10");
    nodeCtrl20->addIncomingPlug("plug_2");
    nodeCtrl20->addIncomingPlug("plug_3");
    nodeCtrl20->addOutgoingPlug("plug_4");
    nodeCtrl20->addOutgoingPlug("plug_5");
    nodeCtrl20->addOutgoingPlug("plug_6");
    nodeCtrl20->addIncomingPlug("plug_7");
    nodeCtrl20->addIncomingPlug("plug_8");
    nodeCtrl20->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl24 = mainCtrl->createNode("Node 24");
    nodeCtrl24->getNodeHandle().setPos(86.1756, 319.568);
    nodeCtrl24->addOutgoingPlug("plug");
    nodeCtrl24->addOutgoingPlug("plug_2");
    nodeCtrl24->addOutgoingPlug("plug_3");
    nodeCtrl24->addOutgoingPlug("plug_4");
    nodeCtrl24->addOutgoingPlug("plug_5");
    nodeCtrl24->addOutgoingPlug("plug_6");
    nodeCtrl24->addOutgoingPlug("plug_7");
    nodeCtrl24->addIncomingPlug("plug_8");
    nodeCtrl24->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl22 = mainCtrl->createNode("Node 22");
    nodeCtrl22->getNodeHandle().setPos(-438.06, 344.703);
    nodeCtrl22->addOutgoingPlug("plug");
    nodeCtrl22->addOutgoingPlug("plug_10");
    nodeCtrl22->addIncomingPlug("plug_11");
    nodeCtrl22->addIncomingPlug("plug_12");
    nodeCtrl22->addOutgoingPlug("plug_13");
    nodeCtrl22->addIncomingPlug("plug_14");
    nodeCtrl22->addOutgoingPlug("plug_15");
    nodeCtrl22->addOutgoingPlug("plug_2");
    nodeCtrl22->addIncomingPlug("plug_3");
    nodeCtrl22->addIncomingPlug("plug_4");
    nodeCtrl22->addIncomingPlug("plug_5");
    nodeCtrl22->addOutgoingPlug("plug_6");
    nodeCtrl22->addIncomingPlug("plug_7");
    nodeCtrl22->addOutgoingPlug("plug_8");
    nodeCtrl22->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl29 = mainCtrl->createNode("Node 29");
    nodeCtrl29->getNodeHandle().setPos(373.428, -111.31);
    nodeCtrl29->addIncomingPlug("plug");
    nodeCtrl29->addOutgoingPlug("plug_10");
    nodeCtrl29->addIncomingPlug("plug_2");
    nodeCtrl29->addIncomingPlug("plug_3");
    nodeCtrl29->addIncomingPlug("plug_4");
    nodeCtrl29->addOutgoingPlug("plug_5");
    nodeCtrl29->addIncomingPlug("plug_6");
    nodeCtrl29->addIncomingPlug("plug_7");
    nodeCtrl29->addOutgoingPlug("plug_8");
    nodeCtrl29->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl41 = mainCtrl->createNode("Node 41");
    nodeCtrl41->getNodeHandle().setPos(1508.07, -402.153);
    nodeCtrl41->addOutgoingPlug("plug");
    nodeCtrl41->addOutgoingPlug("plug_10");
    nodeCtrl41->addIncomingPlug("plug_11");
    nodeCtrl41->addIncomingPlug("plug_12");
    nodeCtrl41->addOutgoingPlug("plug_13");
    nodeCtrl41->addOutgoingPlug("plug_2");
    nodeCtrl41->addIncomingPlug("plug_3");
    nodeCtrl41->addOutgoingPlug("plug_4");
    nodeCtrl41->addIncomingPlug("plug_5");
    nodeCtrl41->addOutgoingPlug("plug_6");
    nodeCtrl41->addIncomingPlug("plug_7");
    nodeCtrl41->addIncomingPlug("plug_8");
    nodeCtrl41->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl42 = mainCtrl->createNode("Node 42");
    nodeCtrl42->getNodeHandle().setPos(1684.02, -262.118);
    nodeCtrl42->addIncomingPlug("plug");
    nodeCtrl42->addIncomingPlug("plug_2");
    nodeCtrl42->addOutgoingPlug("plug_3");
    nodeCtrl42->addIncomingPlug("plug_4");
    nodeCtrl42->addIncomingPlug("plug_5");
    nodeCtrl42->addIncomingPlug("plug_6");
    nodeCtrl42->addOutgoingPlug("plug_7");
    nodeCtrl42->addIncomingPlug("plug_8");
    nodeCtrl42->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl27 = mainCtrl->createNode("Node 27");
    nodeCtrl27->getNodeHandle().setPos(204.667, 387.79);
    nodeCtrl27->addOutgoingPlug("plug");
    nodeCtrl27->addIncomingPlug("plug_2");
    nodeCtrl27->addIncomingPlug("plug_3");
    nodeCtrl27->addIncomingPlug("plug_4");
    nodeCtrl27->addOutgoingPlug("plug_5");
    nodeCtrl27->addOutgoingPlug("plug_6");
    nodeCtrl27->addOutgoingPlug("plug_7");

    NodeCtrl* nodeCtrl33 = mainCtrl->createNode("Node 33");
    nodeCtrl33->getNodeHandle().setPos(937.16, 362.656);
    nodeCtrl33->addOutgoingPlug("plug");
    nodeCtrl33->addIncomingPlug("plug_2");
    nodeCtrl33->addIncomingPlug("plug_3");
    nodeCtrl33->addOutgoingPlug("plug_4");
    nodeCtrl33->addOutgoingPlug("plug_5");
    nodeCtrl33->addIncomingPlug("plug_6");
    nodeCtrl33->addOutgoingPlug("plug_7");
    nodeCtrl33->addOutgoingPlug("plug_8");
    nodeCtrl33->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl19 = mainCtrl->createNode("Node 19");
    nodeCtrl19->getNodeHandle().setPos(-341.112, -337.521);
    nodeCtrl19->addIncomingPlug("plug");
    nodeCtrl19->addIncomingPlug("plug_2");
    nodeCtrl19->addIncomingPlug("plug_3");
    nodeCtrl19->addOutgoingPlug("plug_4");
    nodeCtrl19->addOutgoingPlug("plug_5");
    nodeCtrl19->addIncomingPlug("plug_6");
    nodeCtrl19->addIncomingPlug("plug_7");

    NodeCtrl* nodeCtrl21 = mainCtrl->createNode("Node 21");
    nodeCtrl21->getNodeHandle().setPos(-211.848, 219.03);
    nodeCtrl21->addIncomingPlug("plug");
    nodeCtrl21->addIncomingPlug("plug_2");
    nodeCtrl21->addOutgoingPlug("plug_3");
    nodeCtrl21->addOutgoingPlug("plug_4");
    nodeCtrl21->addOutgoingPlug("plug_5");
    nodeCtrl21->addIncomingPlug("plug_6");
    nodeCtrl21->addOutgoingPlug("plug_7");

    NodeCtrl* nodeCtrl7 = mainCtrl->createNode("Node 7");
    nodeCtrl7->getNodeHandle().setPos(-1643.57, 318.695);
    nodeCtrl7->addOutgoingPlug("plug");
    nodeCtrl7->addIncomingPlug("plug_2");
    nodeCtrl7->addIncomingPlug("plug_3");
    nodeCtrl7->addOutgoingPlug("plug_4");

    NodeCtrl* nodeCtrl28 = mainCtrl->createNode("Node 28");
    nodeCtrl28->getNodeHandle().setPos(315.977, 280.071);
    nodeCtrl28->addIncomingPlug("plug");
    nodeCtrl28->addIncomingPlug("plug_2");
    nodeCtrl28->addIncomingPlug("plug_3");
    nodeCtrl28->addOutgoingPlug("plug_4");
    nodeCtrl28->addOutgoingPlug("plug_5");

    NodeCtrl* nodeCtrl10 = mainCtrl->createNode("Node 10");
    nodeCtrl10->getNodeHandle().setPos(-1235.18, -348.293);
    nodeCtrl10->addIncomingPlug("plug");
    nodeCtrl10->addIncomingPlug("plug_10");
    nodeCtrl10->addIncomingPlug("plug_11");
    nodeCtrl10->addIncomingPlug("plug_2");
    nodeCtrl10->addOutgoingPlug("plug_3");
    nodeCtrl10->addOutgoingPlug("plug_4");
    nodeCtrl10->addIncomingPlug("plug_5");
    nodeCtrl10->addOutgoingPlug("plug_6");
    nodeCtrl10->addIncomingPlug("plug_7");
    nodeCtrl10->addOutgoingPlug("plug_8");
    nodeCtrl10->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl9 = mainCtrl->createNode("Node 9");
    nodeCtrl9->getNodeHandle().setPos(-1360.86, -89.7663);
    nodeCtrl9->addOutgoingPlug("plug");
    nodeCtrl9->addOutgoingPlug("plug_10");
    nodeCtrl9->addIncomingPlug("plug_11");
    nodeCtrl9->addIncomingPlug("plug_12");
    nodeCtrl9->addOutgoingPlug("plug_2");
    nodeCtrl9->addIncomingPlug("plug_3");
    nodeCtrl9->addIncomingPlug("plug_4");
    nodeCtrl9->addIncomingPlug("plug_5");
    nodeCtrl9->addIncomingPlug("plug_6");
    nodeCtrl9->addOutgoingPlug("plug_7");
    nodeCtrl9->addOutgoingPlug("plug_8");
    nodeCtrl9->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl11 = mainCtrl->createNode("Node 11");
    nodeCtrl11->getNodeHandle().setPos(-973.067, -362.656);
    nodeCtrl11->addOutgoingPlug("plug");
    nodeCtrl11->addOutgoingPlug("plug_2");
    nodeCtrl11->addIncomingPlug("plug_3");
    nodeCtrl11->addOutgoingPlug("plug_4");

    NodeCtrl* nodeCtrl32 = mainCtrl->createNode("Node 32");
    nodeCtrl32->getNodeHandle().setPos(800.715, 122.082);
    nodeCtrl32->addIncomingPlug("plug");
    nodeCtrl32->addOutgoingPlug("plug_10");
    nodeCtrl32->addOutgoingPlug("plug_2");
    nodeCtrl32->addOutgoingPlug("plug_3");
    nodeCtrl32->addOutgoingPlug("plug_4");
    nodeCtrl32->addIncomingPlug("plug_5");
    nodeCtrl32->addIncomingPlug("plug_6");
    nodeCtrl32->addOutgoingPlug("plug_7");
    nodeCtrl32->addIncomingPlug("plug_8");
    nodeCtrl32->addOutgoingPlug("plug_9");

    NodeCtrl* nodeCtrl2 = mainCtrl->createNode("Node 2");
    nodeCtrl2->getNodeHandle().setPos(-1994.16, -238.404);
    nodeCtrl2->addIncomingPlug("plug");
    nodeCtrl2->addOutgoingPlug("plug_2");
    nodeCtrl2->addOutgoingPlug("plug_3");
    nodeCtrl2->addOutgoingPlug("plug_4");
    nodeCtrl2->addIncomingPlug("plug_5");
    nodeCtrl2->addOutgoingPlug("plug_6");
    nodeCtrl2->addOutgoingPlug("plug_7");
    nodeCtrl2->addOutgoingPlug("plug_8");
    nodeCtrl2->addIncomingPlug("plug_9");

    NodeCtrl* nodeCtrl40 = mainCtrl->createNode("Node 40");
    nodeCtrl40->getNodeHandle().setPos(1159.78, -362.656);
    nodeCtrl40->addOutgoingPlug("plug");
    nodeCtrl40->addIncomingPlug("plug_2");
    nodeCtrl40->addIncomingPlug("plug_3");
    nodeCtrl40->addOutgoingPlug("plug_4");
    nodeCtrl40->addOutgoingPlug("plug_5");

    NodeCtrl* nodeCtrl18 = mainCtrl->createNode("Node 18");
    nodeCtrl18->getNodeHandle().setPos(-710.949, -347.521);
    nodeCtrl18->addOutgoingPlug("plug");
    nodeCtrl18->addOutgoingPlug("plug_2");
    nodeCtrl18->addOutgoingPlug("plug_3");
    nodeCtrl18->addOutgoingPlug("plug_4");
    nodeCtrl18->addOutgoingPlug("plug_5");

    NodeCtrl* nodeCtrl0 = mainCtrl->createNode("www.clemens-sielaff.com");
    nodeCtrl0->getNodeHandle().setPos(1645.45, 445.963);

    nodeCtrl12->getNodeHandle().getPlug("plug_7").connectPlug(nodeCtrl13->getNodeHandle().getPlug("plug_3"));
    nodeCtrl16->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl17->getNodeHandle().getPlug("plug_4"));
    nodeCtrl15->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl14->getNodeHandle().getPlug("plug_4"));
    nodeCtrl4->getNodeHandle().getPlug("plug_11").connectPlug(nodeCtrl3->getNodeHandle().getPlug("plug_2"));
    nodeCtrl4->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl5->getNodeHandle().getPlug("plug_9"));
    nodeCtrl31->getNodeHandle().getPlug("plug_5").connectPlug(nodeCtrl32->getNodeHandle().getPlug("plug"));
    nodeCtrl17->getNodeHandle().getPlug("plug_3").connectPlug(nodeCtrl16->getNodeHandle().getPlug("plug_4"));
    nodeCtrl38->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl37->getNodeHandle().getPlug("plug"));
    nodeCtrl38->getNodeHandle().getPlug("plug_2").connectPlug(nodeCtrl39->getNodeHandle().getPlug("plug_3"));
    nodeCtrl6->getNodeHandle().getPlug("plug_8").connectPlug(nodeCtrl7->getNodeHandle().getPlug("plug_3"));
    nodeCtrl44->getNodeHandle().getPlug("plug_9").connectPlug(nodeCtrl12->getNodeHandle().getPlug("plug_10"));
    nodeCtrl35->getNodeHandle().getPlug("plug_4").connectPlug(nodeCtrl32->getNodeHandle().getPlug("plug_6"));
    nodeCtrl1->getNodeHandle().getPlug("plug_4").connectPlug(nodeCtrl2->getNodeHandle().getPlug("plug_5"));
    nodeCtrl39->getNodeHandle().getPlug("plug_9").connectPlug(nodeCtrl40->getNodeHandle().getPlug("plug_3"));
    nodeCtrl30->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl31->getNodeHandle().getPlug("plug_2"));
    nodeCtrl13->getNodeHandle().getPlug("plug_10").connectPlug(nodeCtrl14->getNodeHandle().getPlug("plug_6"));
    nodeCtrl23->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl24->getNodeHandle().getPlug("plug_9"));
    nodeCtrl5->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl4->getNodeHandle().getPlug("plug_5"));
    nodeCtrl34->getNodeHandle().getPlug("plug_2").connectPlug(nodeCtrl32->getNodeHandle().getPlug("plug_5"));
    nodeCtrl25->getNodeHandle().getPlug("plug_12").connectPlug(nodeCtrl26->getNodeHandle().getPlug("plug_9"));
    nodeCtrl25->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl24->getNodeHandle().getPlug("plug_8"));
    nodeCtrl20->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl21->getNodeHandle().getPlug("plug_6"));
    nodeCtrl24->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl25->getNodeHandle().getPlug("plug_9"));
    nodeCtrl29->getNodeHandle().getPlug("plug_10").connectPlug(nodeCtrl30->getNodeHandle().getPlug("plug_11"));
    nodeCtrl29->getNodeHandle().getPlug("plug_8").connectPlug(nodeCtrl28->getNodeHandle().getPlug("plug"));
    nodeCtrl41->getNodeHandle().getPlug("plug_9").connectPlug(nodeCtrl42->getNodeHandle().getPlug("plug_2"));
    nodeCtrl27->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl28->getNodeHandle().getPlug("plug_3"));
    nodeCtrl19->getNodeHandle().getPlug("plug_5").connectPlug(nodeCtrl20->getNodeHandle().getPlug("plug_8"));
    nodeCtrl21->getNodeHandle().getPlug("plug_5").connectPlug(nodeCtrl22->getNodeHandle().getPlug("plug_9"));
    nodeCtrl7->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl8->getNodeHandle().getPlug("plug_7"));
    nodeCtrl10->getNodeHandle().getPlug("plug_6").connectPlug(nodeCtrl9->getNodeHandle().getPlug("plug_12"));
    nodeCtrl10->getNodeHandle().getPlug("plug_9").connectPlug(nodeCtrl11->getNodeHandle().getPlug("plug_3"));
    nodeCtrl9->getNodeHandle().getPlug("plug_9").connectPlug(nodeCtrl10->getNodeHandle().getPlug("plug_2"));
    nodeCtrl32->getNodeHandle().getPlug("plug_4").connectPlug(nodeCtrl34->getNodeHandle().getPlug("plug_5"));
    nodeCtrl32->getNodeHandle().getPlug("plug_7").connectPlug(nodeCtrl33->getNodeHandle().getPlug("plug_6"));
    nodeCtrl2->getNodeHandle().getPlug("plug_2").connectPlug(nodeCtrl3->getNodeHandle().getPlug("plug_3"));
    nodeCtrl40->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl39->getNodeHandle().getPlug("plug"));
    nodeCtrl40->getNodeHandle().getPlug("plug_5").connectPlug(nodeCtrl41->getNodeHandle().getPlug("plug_12"));
    nodeCtrl18->getNodeHandle().getPlug("plug").connectPlug(nodeCtrl19->getNodeHandle().getPlug("plug_6"));
    nodeCtrl18->getNodeHandle().getPlug("plug_5").connectPlug(nodeCtrl17->getNodeHandle().getPlug("plug_2"));
}
