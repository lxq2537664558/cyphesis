// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2001 Alistair Riddoch

#include <common/debug.h>

#include "Lobby.h"

#include "Connection_methods.h"
#include "ServerRouting.h"

#include <Atlas/Objects/Operation/Appearance.h>
#include <Atlas/Objects/Operation/Disappearance.h>

static const bool debug_flag = false;

Lobby::Lobby(const std::string & id, ServerRouting & s) : OOGThing(id), m_server(s)
{
}

void Lobby::addObject(Account * ac)
{
    debug(std::cout << "Lobby::addObject(" << ac->getId() << ")"
                    << std::endl << std::flush;);

    Appearance a;
    Element::ListType & args = a.getArgs();
    args.push_back(Element::MapType());
    Element::MapType & us = args.back().asMap();
    us["id"] = ac->getId();
    us["loc"] = "lobby";
    a.setFrom(ac->getId());
    a.setTo("lobby");
    a.setSerialno(m_server.newSerialNo());
    operation(a);

    m_accounts[ac->getId()] = ac;
}

void Lobby::delObject(Account * a)
{
    debug(std::cout << "Lobby::delObject(" << a->getId() << ")"
                    << std::endl << std::flush;);
                    
    Disappearance d;
    Element::ListType & args = d.getArgs();
    args.push_back(Element::MapType());
    Element::MapType & us = args.back().asMap();
    us["id"] = a->getId();
    us["loc"] = "lobby";
    d.setFrom(a->getId());
    d.setTo("lobby");
    d.setSerialno(m_server.newSerialNo());
    operation(d);

    m_accounts.erase(a->getId());
}


OpVector Lobby::operation(const RootOperation & op)
{
    debug(std::cout << "Lobby::operation(" << op.getParents().front().asString()
                                           << std::endl << std::flush; );
    const std::string & to = op.getTo();
    if (to.empty() || to == "lobby") {
        AccountDict::const_iterator I = m_accounts.begin();
        RootOperation newop(op);
        for (; I != m_accounts.end(); ++I) {
            Connection * c = I->second->m_connection;
            if (c != NULL) {
                newop.setTo(I->first);
                debug(std::cout << "Lobby sending " << newop.getParents().front().asString() << " operation to " << I->first << std::endl << std::flush; );
                c->send(newop);
            }
        }
    } else {
        AccountDict::const_iterator I = m_accounts.find(to);
        if (I == m_accounts.end()) {
            return error(op, "Target account not logged in");
        } else {
            Connection * c = I->second->m_connection;
            if (c != NULL) {
                c->send(op);
            }
        }
    }
    return OpVector();
}

void Lobby::addToObject(Element::MapType & omap) const
{
    omap["objtype"] = "object";
    omap["id"] = "lobby";
    omap["name"] = "lobby";
    Element::ListType plist(1, "room");
    omap["parents"] = plist;
    Element::ListType player_list;
    AccountDict::const_iterator I = m_accounts.begin();
    for(; I != m_accounts.end(); I++) {
        player_list.push_back(I->first);
    }
    omap["people"] = player_list;
    omap["rooms"] = Element::ListType();
}
