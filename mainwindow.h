//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published
//    by the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/// \file mainwindow.h
///
/// \brief Documentation for the mainwindow file.
///
///

#include <QMainWindow>

class MainCtrl;
class PropertyEditor;
class QSplitter;

///
/// \brief A single instance of this class contains all other widgets of the application.
///
/// It also contains the node Manager that in turn controls the business logic.
///
class MainWindow : public QMainWindow
{
    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param parent   Qt parent.
    ///
    MainWindow(QWidget *parent=0);

protected: // methods

    ///
    /// \brief Called upon closing.
    ///
    /// \param event    Qt event.
    ///
    void closeEvent(QCloseEvent* event);

private slots:

    ///
    /// \brief Displays the About-window of the application.
    ///
    void displayAbout();

private: // methods

    ///
    /// \brief Reads GUI settings stored by QSettings.
    ///
    /// In Ubuntu the location of the settings file is:  ~/.config/clemens-sielaff/ZodiacGraph_ExampleApp.ini
    /// In Windows the location of the sittings file is: %APPDATA%\Roaming\clemens-sielaff\ZodiacGraph_ExampleApp.ini
    ///
    void readSettings();

    ///
    /// \brief Writes out GUI settings for the next instance of the application to read.
    ///
    /// For details, see readSettings().
    ///
    void writeSettings();

private: // members

    ///
    /// \brief Main controller used for controlling the nodes (both visual and logical) of the graph.
    ///
    MainCtrl* m_mainCtrl;

    ///
    /// \brief Main splitter between the Zodiac Graph and the Property editor.
    ///
    QSplitter* m_mainSplitter;

};

#endif // MAINWINDOW_H
