/*
-------------------------------------------------------------------------------
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would be
	 appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
	 misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _bAsciiMain_h_
#define _bAsciiMain_h_

#include <cstdio>
#include "bApi.h"
#include "bAsciiCommon.h"

template <typename T>
class bAsciiApiStorage
{
public:
    typedef skHashTable<bString, T*> Table;
    typedef typename Table::Iterator Iterator;

private:
    Table m_table;

public:
    bAsciiApiStorage() = default;

    ~bAsciiApiStorage()
    {
        clear();
    }

    void clear()
    {
        Iterator it = m_table.iterator();
        while (it.hasMoreElements())
            delete it.getNext().second;
        m_table.clear();
    }

    /// <summary>
    /// Searches for the storage type by the supplied name.
    /// </summary>
    /// <returns>Returns null if the storage type was not found.</returns>
    T* find(const bString& name)
    {
        SKsize pos;
        if ((pos = m_table.find(name)) != SK_NPOS)
            return m_table.at(pos);
        return nullptr;
    }

    /// <summary>
    /// Stores a storage type object by a lookup name.
    /// </summary>
    /// <param name="name">A unique name to reference the object.</param>
    /// <param name="data">The object that should be stored.</param>
    /// <returns>true if the data was stored otherwise false.</returns>
    bool insert(const bString& name, T* data)
    {
        bool result = true;
        if (m_table.find(name) == SK_NPOS)
            m_table.insert(name, data);
        else
        {
            delete data;
            result = false;
            printf("duplicate object '%s' was not added.\n", name.c_str());
        }
        return result;
    }
};

/// <summary>
/// bAsciiMain is the primary data access API.
/// </summary>
class bAsciiMain
{
public:
    typedef bAsciiApiStorage<bCamera>   CameraLibrary;
    typedef bAsciiApiStorage<bLight>    LightLibrary;
    typedef bAsciiApiStorage<bObject>   ObjectLibrary;
    typedef bAsciiApiStorage<bScene>    SceneLibrary;
    typedef bAsciiApiStorage<bMaterial> MaterialLibrary;
    typedef bAsciiApiStorage<bShape>    ShapeLibrary;

private:
    bScene* m_scene;
    bString m_creator;
    SKint32 m_version;

    friend class bAsciiLinker;

    CameraLibrary   m_cameras;
    LightLibrary    m_lights;
    ObjectLibrary   m_objects;
    SceneLibrary    m_scenes;
    MaterialLibrary m_materials;
    ShapeLibrary    m_shapes;

public:
    explicit bAsciiMain() :
        m_scene(nullptr),
        m_version(0)
    {
    }

    ~bAsciiMain() = default;

    SK_INLINE CameraLibrary& getCameraLibrary()
    {
        return m_cameras;
    }

    SK_INLINE LightLibrary& getLightLibrary()
    {
        return m_lights;
    }

    SK_INLINE ObjectLibrary& getObjectLibrary()
    {
        return m_objects;
    }

    SK_INLINE SceneLibrary& getSceneLibrary()
    {
        return m_scenes;
    }

    SK_INLINE MaterialLibrary& getMaterialLibrary()
    {
        return m_materials;
    }

    SK_INLINE ShapeLibrary& getShapeLibrary()
    {
        return m_shapes;
    }

    SK_INLINE bScene* getActiveScene() const
    {
        return m_scene;
    }

    SK_INLINE SKint32 getVersion() const
    {
        return m_version;
    }

    SK_INLINE const bString& getCreator() const
    {
        return m_creator;
    }
};

#endif  //_bAsciiMain_h_
