#
#    ZodiacGraph - A general-purpose, circular node graph UI module.
#    Copyright (C) 2015  Clemens Sielaff
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU Lesser General Public License as published
#    by the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU Lesser General Public License for more details.
#
#    You should have received a copy of the GNU Lesser General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
