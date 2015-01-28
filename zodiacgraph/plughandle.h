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

#ifndef ZODIAC_PLUGHANDLE_H
#define ZODIAC_PLUGHANDLE_H

/// \file plughandle.h
///
/// \brief Contains the definition of the zodiac::PlugHandle class.
///

#include <QObject>
#include <QHash>

namespace zodiac {

class NodeHandle;
class Plug;
class SceneHandle;

///
/// \brief A handle object for a zodiac::Plug.
///
/// Is a thin wrapper around a pointer but with a much nicer, outward facing interface than the real Plug.
///
/// See \ref zodiac::SceneHandle "SceneHandle" for more details on handles.
///
class PlugHandle : public QObject
{

    Q_OBJECT

public: // methods

    ///
    /// \brief Constructor.
    ///
    /// \param [in] plug    Plug to manage through this handle
    ///
    explicit PlugHandle(Plug* plug = nullptr);

    ///
    /// \brief Copy constructor.
    ///
    /// \param [in] other   Other PlugHandle to copy.
    ///
    PlugHandle(const PlugHandle& other)
        : PlugHandle(other.data()) {}

    ///
    /// \brief Assignment operator.
    ///
    /// \param [in] other   Other PlugHandle to copy from.
    ///
    /// \return             This.
    ///
    PlugHandle& operator = (const PlugHandle& other);

    ///
    /// \brief Equality operator.
    ///
    /// \param [in] other   Other PlugHandle to test against.
    ///
    /// \return             <i>true</i> if both handles handle the same object -- <i>false</i> otherwise.
    ///
    bool operator == (const PlugHandle& other) const {return other.data() == data();}

    ///
    /// \brief Direct pointer access.
    ///
    /// \return The pointer managed by this handle.
    ///
    inline Plug* data() const {return m_plug;}

    ///
    /// \brief Used for testing, whether the handle is still alive or not.
    ///
    /// \return <i>true</i> if the PlugHandle is still managing an existing Plug -- <i>false</i> otherwise.
    ///
    inline bool isValid() const {return m_isValid;}

    ///
    /// \brief Only Plug%s with no connections can be removed.
    ///
    /// \return <i>true</i> if the Plug could be removed -- <i>false</i> otherwise.
    ///
    bool isRemovable() const;

    ///
    /// \brief Removes the managed Plug, if it has no connections.
    ///
    /// \return <i>true</i> if the Plug could be removed -- <i>false</i> otherwise.
    ///
    bool remove();

    ///
    /// \brief Returns the unique name of the managed Plug.
    ///
    /// \return Name of the managed Plug.
    ///
    QString getName() const;

    ///
    /// \brief Changes the direction of a Plug from incoming to outgoing or vice-versa.
    ///
    /// This works only on Plug%s that have no existing connections.
    ///
    /// \return <i>true</i> if the Plug was able to change its direction -- <i>false</i> otherwise.
    ///
    bool toggleDirection();

    ///
    /// \brief Tests, whether the managed Plug is an incoming one.
    ///
    /// \return <i>true</i> if the Plug is incoming -- <i>false</i> if it is an outgoing one.
    ///
    bool isIncoming() const;

    ///
    /// \brief Tests, whether the managed Plug is an outgoing one.
    ///
    /// \return <i>true</i> if the Plug is outgoing -- <i>false</i> if it is an incoming one.
    ///
    bool isOutgoing() const;

    ///
    /// \brief The number of connections of the managed Plug.
    ///
    /// Incoming Plugs have a maximal connection count of 1.
    ///
    /// \return Connection count.
    ///
    int connectionCount() const;

    ///
    /// \brief Returns a list of all Plug%s that are connected to this one.
    ///
    /// \return Handles of all Plugs connected to this one.
    ///
    QList<PlugHandle> getConnectedPlugs() const;

    ///
    /// \brief Creates a new connection between the Plug managed by this handle and another.
    ///
    /// You can only connect incoming Plug%s to outgoing Plug%s and vice versa.
    /// Also, incoming Plug%s with an existing connection cannot be connected again until you remove the existing one.
    ///
    /// \param [in] other   Handle of the Plug to connect to.
    ///
    /// \return             <i>true</i> if the connection was created -- <i>false</i> otherwise.
    ///
    bool connectPlug(PlugHandle other);

    ///
    /// \brief Disconnects the managed Plug from another one.
    ///
    /// This method returns false, if there is no connection from this to the other Plug.
    ///
    /// \param [in] other   Plug to disconnect from.
    ///
    /// \return             <i>true</i> if a connection was removed -- <i>false</i> otherwise.
    ///
    bool disconnectPlug(PlugHandle other);

    ///
    /// \brief Disconnects all connected edges from this Plug.
    ///
    /// After calling this method, the number of connections to this Plug is always zero.
    ///
    void disconnectAll();

    ///
    /// \brief A handle of the Node of the Plug managed by this.
    ///
    /// \return Handle of the plug's Node.
    ///
    NodeHandle getNode() const;

    ///
    /// \brief Handle of the Scene containing this Plug and its Node.
    ///
    /// \return Scene handle.
    ///
    SceneHandle getScene() const;

public slots:

    ///
    /// \brief Renames this Plug to a new name.
    ///
    /// Name is suffixed with an integer, if the proposed name is not unique in the Node.
    ///
    /// \param [in] name    Proposed new name of the plug.
    ///
    /// \return             Actual new name of the plug.
    ///
    QString rename(const QString& name);

signals:

    ///
    /// \brief Is emitted, when the plug was renamed.
    ///
    /// \param [out] name   New name of the plug.
    ///
    void plugRenamed(const QString& name);

private: // methods

    ///
    /// \brief Connects the handle to its managed object.
    ///
    void connectSignals();

private slots:

    ///
    /// \brief Called, when the mangaged Plug was destroyed.
    ///
    void plugWasDestroyed();

private: // member

    ///
    /// \brief Managed plug.
    ///
    Plug* m_plug;

    ///
    /// \brief Validity flag.
    ///
    bool m_isValid;

};

} // namespace zodiac

///
/// \brief Returns the hash of a PlugHandle instance.
///
/// The hash is calculated by taking the address of the Plug-pointer.
/// This is also, why the pointer adress in the handle is never changed or set to <i>nullptr</i>.
///
/// \param [in] key PlugHandle instance to hash.
///
/// \return         Unique identifier of a PlugHandle that can be used to determine equality.
///
inline uint qHash(const zodiac::PlugHandle& key)
{
    return qHash(size_t(key.data()));
}

#endif // ZODIAC_PLUGHANDLE_H
