// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2002 Alistair Riddoch

#include "Restorer_impl.h"

#include "rulesets/Creator.h"
#include "rulesets/World.h"
#include "rulesets/Plant.h"
#include "rulesets/Food.h"
#include "rulesets/Area.h"
#include "rulesets/Line.h"
#include "rulesets/Structure.h"
#include "rulesets/Stackable.h"
#include "rulesets/World.h"

#include "common/terrain_utils.h"

template class Restorer<Entity>;
template class Restorer<Thing>;
template class Restorer<Character>;
template class Restorer<Creator>;
template class Restorer<Plant>;
template class Restorer<Food>;
template class Restorer<Area>;
template class Restorer<Line>;
template class Restorer<Stackable>;
template class Restorer<Structure>;
template class Restorer<World>;

template <> Persistor<Entity> Restorer<Entity>::m_persist(true);
template <> Persistor<Thing> Restorer<Thing>::m_persist(true);
template <> Persistor<Character> Restorer<Character>::m_persist(true);
template <> Persistor<Creator> Restorer<Creator>::m_persist(true);
template <> Persistor<Plant> Restorer<Plant>::m_persist(true);
template <> Persistor<Food> Restorer<Food>::m_persist(true);
template <> Persistor<Area> Restorer<Area>::m_persist(true);
template <> Persistor<Line> Restorer<Line>::m_persist(true);
template <> Persistor<Stackable> Restorer<Stackable>::m_persist(true);
template <> Persistor<Structure> Restorer<Structure>::m_persist(true);
template <> Persistor<World> Restorer<World>::m_persist(true);

void Restorer<Character>::rCharacter(DatabaseResult::const_iterator & dr)
{
    dr.readColumn("drunkness", m_drunkness);
    dr.readColumn("sex", m_sex);
    dr.readColumn("food", m_food);
}

void Restorer<Character>::populate(DatabaseResult::const_iterator & dr)
{
    rEntity(dr);
    rCharacter(dr);
}

void Restorer<Plant>::rPlant(DatabaseResult::const_iterator & dr)
{
    dr.readColumn("fruits", m_fruits);
    dr.readColumn("sizeAdult", m_sizeAdult);
    dr.readColumn("fruitChance", m_fruitChance);
    dr.readColumn("fruitName", m_fruitName);
}

void Restorer<Plant>::populate(DatabaseResult::const_iterator & dr)
{
    rEntity(dr);
    rPlant(dr);
}

void Restorer<World>::populate(DatabaseResult::const_iterator & dr)
{
    rEntity(dr);

    // Restore the terrain
    loadTerrain(getId(), m_terrain);
}
