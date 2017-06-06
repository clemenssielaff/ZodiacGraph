#
#    ZodiacGraph - A general-purpose, circular node graph UI module.
#    Copyright (C) 2015  Clemens Sielaff
#
#    The MIT License
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy of
#    this software and associated documentation files (the "Software"), to deal in
#    the Software without restriction, including without limitation the rights to
#    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
#    of the Software, and to permit persons to whom the Software is furnished to do so,
#    subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#    SOFTWARE.
#

QT          += core gui widgets
CONFIG      += c++11 static
DEFINES     *= QT_USE_QSTRINGBUILDER

static {
    QT  += svg
}

unix {
    QMAKE_CXX = ccache g++
}

TARGET = ZodiacGraph_Showcase
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    collapsible.cpp \
    mainctrl.cpp \
    nodectrl.cpp \
    nodeproperties.cpp \
    propertyeditor.cpp \
    zodiacgraph/baseedge.cpp \
    zodiacgraph/bezieredge.cpp \
    zodiacgraph/drawedge.cpp \
    zodiacgraph/edgearrow.cpp \
    zodiacgraph/edgegroup.cpp \
    zodiacgraph/edgegroupinterface.cpp \
    zodiacgraph/edgegrouppair.cpp \
    zodiacgraph/edgelabel.cpp \
    zodiacgraph/labeltextfactory.cpp \
    zodiacgraph/node.cpp \
    zodiacgraph/nodehandle.cpp \
    zodiacgraph/nodelabel.cpp \
    zodiacgraph/perimeter.cpp \
    zodiacgraph/plug.cpp \
    zodiacgraph/plugarranger.cpp \
    zodiacgraph/plugedge.cpp \
    zodiacgraph/plughandle.cpp \
    zodiacgraph/pluglabel.cpp \
    zodiacgraph/scene.cpp \
    zodiacgraph/scenehandle.cpp \
    zodiacgraph/straightdoubleedge.cpp \
    zodiacgraph/straightedge.cpp \
    zodiacgraph/view.cpp

HEADERS  += mainwindow.h \
    collapsible.h \
    mainctrl.h \
    nodectrl.h \
    nodeproperties.h \
    propertyeditor.h \
    zodiacgraph/baseedge.h \
    zodiacgraph/bezieredge.h \
    zodiacgraph/drawedge.h \
    zodiacgraph/edgearrow.h \
    zodiacgraph/edgegroup.h \
    zodiacgraph/edgegroupinterface.h \
    zodiacgraph/edgegrouppair.h \
    zodiacgraph/edgelabel.h \
    zodiacgraph/labeltextfactory.h \
    zodiacgraph/node.h \
    zodiacgraph/nodehandle.h \
    zodiacgraph/nodelabel.h \
    zodiacgraph/perimeter.h \
    zodiacgraph/plug.h \
    zodiacgraph/plugarranger.h \
    zodiacgraph/plugedge.h \
    zodiacgraph/plughandle.h \
    zodiacgraph/pluglabel.h \
    zodiacgraph/scene.h \
    zodiacgraph/scenehandle.h \
    zodiacgraph/straightdoubleedge.h \
    zodiacgraph/straightedge.h \
    zodiacgraph/utils.h \
    zodiacgraph/view.h

RESOURCES += \
    res/icons.qrc
