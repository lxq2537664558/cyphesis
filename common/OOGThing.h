// This file may be redistributed and modified only under the terms of
// the GNU General Public License (See COPYING for details).
// Copyright (C) 2000,2001 Alistair Riddoch

#ifndef SERVER_OOG_THING_H
#define SERVER_OOG_THING_H

#include "BaseEntity.h"

/// \brief This is the base class from which all OOG entity classes inherit.
///
/// This class only overrides OtherOperation() ensuring that operations on
/// unknown types flag an error.
class OOGThing : public BaseEntity {
  protected:
    explicit OOGThing(const std::string & id);
  public:
    virtual ~OOGThing();
    virtual OpVector OtherOperation(const RootOperation & op);
};

#endif // SERVER_OOG_THING_H
