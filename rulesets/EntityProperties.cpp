// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2004 Alistair Riddoch

#include "Entity.h"

#include "Container.h"

#include "common/Property_impl.h"
#include "common/type_utils.h"

#include "physics/BBox.h"

#include <wfmath/atlasconv.h>

template<>
void Property<BBox>::get(Atlas::Message::Element & e)
{
    e = m_data.toAtlas();
}

template<>
void Property<BBox>::set(const Atlas::Message::Element & e)
{
    if (e.isList() && (e.asList().size() > 2)) {
        m_data.fromAtlas(e.asList());
    }
}

template<>
void Property<BBox>::add(const std::string & s, Atlas::Message::MapType & ent)
{
    if (m_data.isValid()) {
        ent[s] = m_data.toAtlas();
    }
}

template<>
void Property<IdList>::get(Atlas::Message::Element & e)
{
    e = Atlas::Message::ListType();
    idListasObject(m_data, e.asList());
}

template<>
void Property<IdList>::set(const Atlas::Message::Element & e)
{
    if (e.isList()) {
        idListFromAtlas(e.asList(), m_data);
    }
}

template<>
void Property<IdList>::add(const std::string & s, Atlas::Message::MapType & ent)
{
    if (!m_data.empty()) {
        get(ent["s"]);
    }
}

template<>
void ImmutableProperty<EntitySet>::get(Atlas::Message::Element & e)
{
    e = Atlas::Message::ListType();
    Atlas::Message::ListType & contlist = e.asList();
    EntitySet::const_iterator Iend = m_data.end();
    for (EntitySet::const_iterator I = m_data.begin(); I != Iend; ++I) {
        contlist.push_back((*I)->getId());
    }
}

template<>
void ImmutableProperty<EntitySet>::add(const std::string & s,
                                       Atlas::Message::MapType & ent)
{
    if (!m_data.empty()) {
        get(ent[s]);
    }
}

template<>
void ImmutableProperty<Container>::get(Atlas::Message::Element & e)
{
    // FIXME Not sure if this is best. Why did we bother to virtualise
    // addToMessage() if we have to do this here?
    e = Atlas::Message::ListType();
    Atlas::Message::ListType & contlist = e.asList();
    Container::const_iterator Iend = m_data.end();
    for (Container::const_iterator I = m_data.begin(); I != Iend; ++I) {
        contlist.push_back((*I)->getId());
    }
}

template<>
void ImmutableProperty<Container>::add(const std::string & s,
                                       Atlas::Message::MapType & ent)
{
    m_data.addToMessage(s, ent);
}

template class Property<BBox>;
template class Property<IdList>;

template class ImmutableProperty<EntitySet>;
template class ImmutableProperty<Container>;
