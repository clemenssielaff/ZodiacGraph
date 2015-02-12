<p align="center">
  <a href="http://vimeo.com/clemenssielaff/zodiacgraph" target="_blank">
    <img src="http://www.clemens-sielaff.com/wp-content/uploads/vimeo_thumbnail.jpg" alt="ZodiacGraph Teaser Video"/>
  </a>
  A general-purpose, circular node graph GUI using Qt
</p>

# About
The ZodiacGraph is a node graph user interface module with a unique visual and conceptual design.
It is written in modern C++, using C++11 features, on top of the Qt framework.
Sources are released under the LGPL version 3.

This repository contains the ZodiacGraph embedded in a showcase application to demonstrate the UI from a user's 
perspective as well as to serve as an example for using the code in your own project.<br>
The complete ZodiacGraph module is contained within the "zodiacgraph" subfolder and nested in the "zodiac" namespace.
Code outside this folder is part of the showcase application and only serves demonstration purposes.

The ZodiacGraph is a pure user user interface that does not contain any business logic in itself.
Therefore, it should fit most use-cases that require a node graph-like GUI.
However, to fully utilize its potential, the showcase application does contain two controller classes that connect the
node graph UI elements to the rest of the application

# Installation
Building the code in this repository was tested on Windows and Linux (Ubuntu) with Qt 5.4.
Other configurations should work -- please let me know if you had any errors on your system.

## Using Qt-Creator
This should be easy.<br>
Pull the repo, open QtCreator and open the project file "ZodiacGraph_Showcase.pro" in QtCreator.
After configuring the build directories, click the play button and after a few seconds the showcase app should open.

## Using qmake
Since building the ZodiacGraph requires Qt anyway, why not make use of the excellent qmake?<br>
On windows, open the Qt command line to make sure that all Qt-libraries are available on the path, call
vcvarsall.bat and define the target machine type (32 or 64 bit) and finally head over the local ZodiacGraph repo.
Then simply run:
~~~~
qmake
nmake release
release\ZodiacGraph_Showcase.exe
~~~~

# Documentation
All code in the "zodiacgraph" subfolder is fully documented with doxygen comments.
To generate the ZodiacGraph html documentation, use the provided doxyfile or visit: 
http://www.clemens-sielaff.com/zodiacgraph-doc/

# Integration into your own project.
Find below a short introduction on how to use the public interface.
For a full example implementation, take a look at the showcase application.
It demonstrates how to integrate the ZodiacGraph into a full-fledged 3rd party Qt application.

Somewhere, for example in the constructor of your MainWidget, create the zodiac::Scene and zodiac::View:
~~~~
zodiac::Scene* zodiacScene = new zodiac::Scene(this);
zodiac::View* zodiacView = new zodiac::View(this);
zodiacView->setScene(zodiacScene);
~~~~

Use the <i>zodiacScene</i> object, to create a zodiac::SceneHandle, that will act as the main interface between your 
business logic and the ZodiacGraph UI:
~~~~
zodiac::SceneHandle sceneHandle = zodiac::SceneHandle(zodiacScene);
~~~~

SceneHandes, NodeHandles and PlugHandles are thin wrapper around a pointer to a zodiac::Scene, zodiac::Node and zodiac::Plug respectively.
They expose only high-level functionality, while keeping the user from leaving the graph in an inconsistent state.
They also <b>do no imply ownership</b> of the object pointed to, we leave that to Qt and the ZodiacGraph internal 
logic.
This means, you are free to create, copy, store and delete handles without affecting the data.<br>
All handles are connected to the QObject::destroyed()-signal of the object they point to and will become
<i>invalid</i>, once the reference object is destroyed.
Calling functions on invalid handles will raise Q_ASSERT errors in debug builds and do nothing in release builds.<br>
See <i>Note on the use of Handles</i> at the end of this section, on the usage of handles in a multi-threaded UI 
environment.

Next, use the <i>sceneHandle</i> to create two Nodes in the Scene:
~~~~
zodiac::NodeHandle fooNode = sceneHandle.createNode("Foo");
zodiac::NodeHandle barNode = sceneHandle.createNode("Bar");
~~~~

Then, through the NodeHandles, create one Plug for each Node -- one incoming, the other outgoing:
~~~~
zodiac::PlugHandle outPlug = fooNode.createOutgoingPlug("fooOut");
zodiac::PlugHandle inPlug = barNode.createIncomingPlug("barIn");
~~~~

Lastly, connect the two Plugs:
~~~~
outPlug.connectPlug(inPlug);
~~~~

And that's it!
You've just created a simple node graph consisting of two Nodes with one Plug each and an Edge between them.<br>
You can find all other functionality of the zodiac::SceneHandle, zodiac::NodeHandle and zodiac::PlugHandle classes in 
the documentation.

<b>Note on the use of Handles</b>:<br>
While the current implementation of handles works in the general case, it is far from being fool-proof.
You should strife to organize your own code in such a way that you are never required to call isValid(),
because you know which handles are valid at what point and when they become invalid.
I must assume (even though I never encountered the problem), that using handles in a multi-threaded environment would 
be subject to race-conditions, where one handle causes the internal object to be destroyed and another handle to 
access it before the <i>destroyed()</i>-signal had a chance of being emitted.<br>
The alternative of using weak references and smart pointers would clash with the currently used model of ownership: 
Qt's parent-child mechanism.
Changing this model requires a ground-up rewrite of the architecture and as this has been a non-issue for me so far,
I feel save to postpone this issue for a future release 2.0.

# Contribution
The ZodiacGraph is a standalone UI module of a larger application under active development.
As such, I expect the code to remain alive and updated regularly for a fair amount of time to come.

<b>Contributions through pull requests are welcome and encouraged!</b>
There are no official guidelines, but please try to be consistent with the existing code and make sure to update 
and create documentation for your changes.

If you happen to encounter a bug or any missing / outdated / wrong documentation, please report it in the issues
section.

# Future Features
- Node Colors<br>
You can already alter the global appearance of the ZodiacGraph.
One of the first next steps is to add the ability to assign colors to individual nodes or node-groups.
- Node Groups<br>
Combines a sub-network of nodes into an new node that exposes loose ends as input and outputs.
- Touch Navigation<br>
Version 1.0.0 already contains rudimentary touch handling but I haven't got the necessary hardware to deploy (and test) a 
serious touch-interface application.
- Node Core Symbols<br>
In addition to Node Colors and instead (?) of a title, one should be able to assign an icon to a node to ease the 
understanding of its meaning in the graph.
- Progress Animation<br>
An optional feature to use when the process described by a Node has a measurable progress and a defined end.
In that case, the arrows of all outgoing edges could wander from their outgoing start plug towards their incoming 
end-plug to visualize the progress within the graph.
