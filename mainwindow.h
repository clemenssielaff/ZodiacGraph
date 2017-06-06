//
//    ZodiacGraph - A general-purpose, circular node graph UI module.
//    Copyright (C) 2015  Clemens Sielaff
//
//    The MIT License
//
//    Permission is hereby granted, free of charge, to any person obtaining a copy of
//    this software and associated documentation files (the "Software"), to deal in
//    the Software without restriction, including without limitation the rights to
//    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
//    of the Software, and to permit persons to whom the Software is furnished to do so,
//    subject to the following conditions:
//
//    The above copyright notice and this permission notice shall be included in all
//    copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//    SOFTWARE.
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
