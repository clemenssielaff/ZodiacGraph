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

#ifndef ZODIAC_NODEHANDLE_H
#define ZODIAC_NODEHANDLE_H

/// \file nodehandle.h
///
/// \brief Contains the definition of the zodiac::NodeHandle class.
///

#include <QObject>
#include <QHash>

#include "plughandle.h"

namespace zodiac {

class Node;
class Plug;
class SceneHandle;

///
/// \brief A handle object for a zodiac::Node.
///
/// Is a thin wrapper around a pointer but with a much nicer, outward facing interface than the real Node.
///
/// See \ref zodiac::SceneHandle "SceneHandle" for more details on handles.
///
class NodeHandle : public QObject
{

    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] node    Node to manage through this handle
    ///
    explicit NodeHandle(Node* node = nullptr);

    ///
    /// \brief Copy constructor.
    ///
    /// \param [in] other   Other NodeHandle to copy.
    ///
    NodeHandle(const NodeHandle& other)
        : NodeHandle(other.data()) {}

    ///
    /// \brief Assignment operator.
    ///
    /// \param [in] other   Other NodeHandle to copy from.
    ///
    /// \return             This.
    ///
    NodeHandle& operator = (const NodeHandle& other);

    ///
    /// \brief Equality operator.
    ///
    /// \param [in] other   Other NodeHandle to test against.
    ///
    /// \return             <i>true</i> if both handles handle the same object -- <i>false</i> otherwise.
    ///
    bool operator == (const NodeHandle& other) const {return other.data() == data();}

    ///
    /// \brief Direct pointer access.
    ///
    /// \return The pointer managed by this handle.
    ///
    inline Node* data() const {return m_node;}

    ///
    /// \brief Used for testing, whether the handle is still alive or not.
    ///
    /// \return <i>true</i>, if the NodeHandle is still managing an existing Node -- <i>false</i> otherwise.
    ///
    inline bool isValid() const {return m_isValid;}

    ///
    /// \brief Only Node%s with no connections can be removed.
    ///
    /// \return <i>true</i> if the Node could be removed -- <i>false</i> otherwise.
    ///
    bool isRemovable() const;

    ///
    /// \brief Tries to remove the Node managed by this handler.
    ///
    /// \return <i>true</i> if the Node could be removed -- <i>false</i> otherwise.
    ///
    bool remove();

    ///
    /// \brief Queries and returns the id of the Node.
    ///
    /// \return Unique id of the Node.
    ///
    const QUuid &getId() const;

    ///
    /// \brief Queries and returns the name of the Node.
    ///
    /// \return Name of the Node.
    ///
    QString getName() const;

    ///
    /// \brief Sets s new display name for the managed Node.
    ///
    /// \param [in] name    New display name of this Node.
    ///
    void rename(const QString& name);

    ///
    /// \brief Creates a new incoming Plug for the managed Node.
    ///
    /// \param [in] name        The proposed name of the plug.
    ///
    /// \return                 Handle of the created Plug.
    ///
    PlugHandle createIncomingPlug(const QString& name);

    ///
    /// \brief Creates a new outgoing Plug for the managed Node.
    ///
    /// \param [in] name        The proposed name of the plug.
    ///
    /// \return                 Handle of the created Plug.
    ///
    PlugHandle createOutgoingPlug(const QString& name);

    ///
    /// \brief Returns handles for all Plugs of the managed Node.
    ///
    /// \return All Plugs of the managed Node.
    ///
    QList<PlugHandle> getPlugs() const;

    ///
    /// \brief Returns a single Plug of the managed Node by name.
    ///
    /// \param [in] name    Name of the requested Plug.
    ///
    /// \return             PlugHandle for the requested Plug, or an invalid one -- if there is no Plug with the name.
    ///
    PlugHandle getPlug(const QString& name) const;

    ///
    /// \brief setSelected
    ///
    /// \param [in] isSelected  <i>true</i> to select the Node -- <i>false</i> to unselect it.
    ///
    void setSelected(bool isSelected);

    ///
    /// \brief SceneHandle of the Scene containing the managed Node.
    ///
    /// \return Scene that contains the managed Node.
    ///
    SceneHandle getScene() const;

    ///
    /// \brief Returns the position of the Node in the Scene.
    ///
    /// \return The position of the Node in the Scene.
    ///
    QPointF getPos() const;

    ///
    /// \brief Returns the position of the Node in the Scene.
    ///
    /// \param [in] x   The position of the Node along the x axis.
    /// \param [in] y   The position of the Node along the y axis.
    ///
    void setPos(qreal x, qreal y);

signals:

    ///
    /// \brief Emitted, when the \ref zodiac::View::s_activationKey "activation key" is pressed while the managed Node
    /// is selected.
    ///
    void nodeActivated();

    ///
    /// \brief Emitted, when the display name of the Node was changed.
    ///
    /// \param [out] name   New display name of the Node.
    ///
    void nodeRenamed(const QString& name);

    ///
    /// \brief Emitted, when the \ref zodiac::View::s_removalButton "removal button" is pressed on the managed Node.
    ///
    void removalRequested();

    ///
    /// \brief Is emitted, when an incoming plug of the managed Node has been connected to an outgoing plug of another
    /// Node.
    ///
    /// \param [out] myInput        The input plug of the managed Node.
    /// \param [out] otherOutput    The output plug of the source Node.
    ///
    void inputConnected(zodiac::PlugHandle myInput, zodiac::PlugHandle otherOutput);

    ///
    /// \brief Is emitted, when an outgoing plug of the managed Node has been connected to an incoming plug of another
    /// Node.
    ///
    /// \param [out] myOutput   The output plug of the managed Node.
    /// \param [out] otherInput The input plug of the target Node.
    ///
    void outputConnected(zodiac::PlugHandle myOutput, zodiac::PlugHandle otherInput);

    ///
    /// \brief Is emitted, when a connection from an incoming plug of the managed Node was removed.
    ///
    /// \param [out] myInput        The input plug of the managed Node.
    /// \param [out] otherOutput    The output plug of the source Node.
    ///
    void inputDisconnected(zodiac::PlugHandle myInput, zodiac::PlugHandle otherOutput);

    ///
    /// \brief Is emitted, when a connection from an outgoing plug of the managed Node was removed.
    ///
    /// \param [out] myOutput   The output plug of the managed Node.
    /// \param [out] otherInput The input plug of the target Node.
    ///
    void outputDisconnected(zodiac::PlugHandle myOutput, zodiac::PlugHandle otherInput);

private: // methods

    ///
    /// \brief Connects the handle to its managed object.
    ///
    void connectSignals();

private slots:

    ///
    /// \brief Passes the 'inputConnected'-signal from the managed Node.
    ///
    /// \param [in] myInput     The input Plug of the managed Node.
    /// \param [in] otherOutput The output Plug of the source Node.
    ///
    void passInputConnected(Plug* myInput, Plug* otherOutput);

    ///
    /// \brief Passes the 'outputConnected'-signal from the managed Node.
    ///
    /// \param [in] myOutput    The output Plug of the managed Node.
    /// \param [in] otherInput  The input Plug of the target Node.
    ///
    void passOutputConnected(Plug* myOutput, Plug* otherInput);

    ///
    /// \brief Passes the 'inputDisconnected'-signal from the managed Node.
    ///
    /// \param [in] myInput     The input Plug of the managed Node.
    /// \param [in] otherOutput The output Plug of the source Node.
    ///
    void passInputDisconnected(Plug* myInput, Plug* otherOutput);

    ///
    /// \brief Passes the 'outputDisconnected'-signal from the managed Node.
    ///
    /// \param [in] myOutput    The output Plug of the managed Node.
    /// \param [in] otherInput  The input Plug of the target Node.
    ///
    void passOutputDisconnected(Plug* myOutput, Plug* otherInput);

    ///
    /// \brief Called, when the mangaged Node was destroyed.
    ///
    void nodeWasDestroyed();

private: // member

    ///
    /// \brief Managed node.
    ///
    Node* m_node;

    ///
    /// \brief Validity flag.
    ///
    bool m_isValid;

};

} // namespace zodiac

///
/// \brief Returns the hash of a NodeHandle instance.
///
/// The hash is calculated by taking the address of the Node-pointer.
/// This is also, why the pointer adress in the handle is never changed or set to <i>nullptr</i>.
///
/// \param [in] key NodeHandle instance to hash.
///
/// \return         Unique identifier of a NodeHandle that can be used to determine equality.
///
inline uint qHash(const zodiac::NodeHandle& key)
{
    return qHash(size_t(key.data()));
}

#endif // ZODIAC_NODEHANDLE_H
