// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2001 Alistair Riddoch

#ifndef RULESETS_WORLD_H
#define RULESETS_WORLD_H

#include "Thing.h"

namespace Mercator {
    class Terrain;
}

typedef Thing World_parent;

typedef std::map<int, std::set<int> > PointSet;

/// This is the in-game entity class used to represent the world.
///
/// I added this because I was not happy with the way the old object model
/// used an out of game object of type WorldRouter to represent the world.
class World : public World_parent {
  protected:
    /// Terrain manager for the world.
    Mercator::Terrain & m_terrain;
    /// Set of terrain points which have been changed.
    PointSet m_modifiedTerrain;
    /// Set of terrain points which have been added.
    PointSet m_createdTerrain;

    void getTerrain(MapType &) const;
    void setTerrain(const MapType &);
  public:
    explicit World(const std::string & id);
    virtual ~World();

    const Mercator::Terrain & terrain() {
        return m_terrain;
    }

    float getHeight(float x, float y);

    virtual bool get(const std::string &, Element &) const;
    virtual void set(const std::string &, const Element &);

    virtual void addToMessage(MapType &) const;

    virtual void LookOperation(const Look & op, OpVector &);
    virtual void BurnOperation(const Burn & op, OpVector &);
    virtual void DeleteOperation(const Delete & op, OpVector &);
    virtual void MoveOperation(const Move & op, OpVector &);
    virtual void SetOperation(const Set & op, OpVector &);
};

#endif // RULESETS_WORLD_H
