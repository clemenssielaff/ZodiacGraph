# ZodiacGraph
A general-purpose, circular nodegraph GUI

# About
The ZodiacGraph is a nodegraph user interface module with a unique visual and conceptual design.
It is written in C++, using C++11 features, on top of the Qt framework.
Sources are released under the LGPL version 3.

This repository contains the ZodiadGraph embedded in a showcase application to demonstrate the UI from a user's perspective as well as to serve as an example for using the code in your own project.<br>
Everything outside the subfolder "zodiacgraph" is code directly related to the showcase application, while everthing inside "zodiacgraph" is independent of the app.
In the code, the distinction is made clear by the use of the "zodiac" namespace, that is used exclusively for classes inside the "zodiacgraph" subfolder.

The ZodiacGraph is a pure user user interface that does not contain any business logic in itself.
Therefore, it should fit most use-cases like require a nodegraph-like GUI.

# Installation
Building the code in this repository was tested on Windows and Linux (Ubuntu) with Qt 5.4.
Other configurations should work -- please let me know if you had any errors on your system.

## Using Qt-Creator
This should be easy.<br>
Pull the repo, open QtCreator and open the project file in QtCreator.
After configuring the build directories, click the play button and after a few seconds the showcase app should open.

## Using qmake
Since building the ZodiacGraph requires Qt anyway, why not make use of the excellent qmake.<br>
On windows, open the Qt command line call vcvarsall.bat with the head over to where you pulled the repo to.
Then simply run:
~~~~
qmake
nmake release
release\ZodiacGraph_Showcase.exe
~~~~

# Documentation
Everything inside the "zodiacgraph" subfolder is 100% doxygen documented.
To generate the documentation, use the doxyfile inside the "zodiacgraph" subfolder or visit: http://www.clemens-sielaff.com/zodiacgraph-doc/

# Integration into your own project.
Best use the "handle" classes to interface with the ZodiacGraph, as demonstrated in the showcase appliation.
There are handles for the Scene, each Node and its individual Plugs.
Handles are slim wrappers around a pointer to the internals of into the ZodiacGraph, exposing only high-level functionality, while keeping the user from leaving the graph in an inconsistent state.

# Future Features
- Node Colors<br>
You can already alter the global appearance of the ZodiacGraph.
One of the first next steps is to add the ability to assign colors to individual nodes or node-groups.
- Node Groups<br>
Combines a sub-network of nodes into an new node that exposes loose ends as input and outputs.
- Touch Navigation<br>
The test application is already a lot of fun on touch devices, but missing navigation functionality like zooming or panning.
- Node Core Symbols<br>
In addition to Node Colors and instead (?) of a title, one should be able to assign an icon to a node to ease the understanding of its meaning in the graph.

# Contribution
Feel free to fork the code and implement any changes or updates that you feel fit.
Please report any bugs that you encounter or any missing / outdated / plain wrong documentation.
