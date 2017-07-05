// AUTOGENERATED file, created by the tool generate_stub.py, don't edit!
// If you want to add your own functionality, instead edit the stubRuleset_custom.h file.

#include "server/Ruleset.h"
#include "stubRuleset_custom.h"

#ifndef STUB_SERVER_RULESET_H
#define STUB_SERVER_RULESET_H


#ifndef STUB_Ruleset_Ruleset
//#define STUB_Ruleset_Ruleset
   Ruleset::Ruleset(EntityBuilder * eb)
    : m_taskHandler(nullptr),m_entityHandler(nullptr),m_opHandler(nullptr),m_propertyHandler(nullptr),m_archetypeHandler(nullptr)
  {
    
  }
#endif //STUB_Ruleset_Ruleset

#ifndef STUB_Ruleset_Ruleset_DTOR
//#define STUB_Ruleset_Ruleset_DTOR
   Ruleset::~Ruleset()
  {
    
  }
#endif //STUB_Ruleset_Ruleset_DTOR

#ifndef STUB_Ruleset_installItem
//#define STUB_Ruleset_installItem
  void Ruleset::installItem(const std::string & class_name, const Atlas::Objects::Root & class_desc)
  {
    
  }
#endif //STUB_Ruleset_installItem

#ifndef STUB_Ruleset_installRuleInner
//#define STUB_Ruleset_installRuleInner
  int Ruleset::installRuleInner(const std::string & class_name, const Atlas::Objects::Root & class_desc, std::string & dependent, std::string & reason)
  {
    return 0;
  }
#endif //STUB_Ruleset_installRuleInner

#ifndef STUB_Ruleset_getRulesFromFiles
//#define STUB_Ruleset_getRulesFromFiles
  void Ruleset::getRulesFromFiles(const std::string &, std::map<std::string, Atlas::Objects::Root> &)
  {
    
  }
#endif //STUB_Ruleset_getRulesFromFiles

#ifndef STUB_Ruleset_loadRules
//#define STUB_Ruleset_loadRules
  void Ruleset::loadRules(const std::string &)
  {
    
  }
#endif //STUB_Ruleset_loadRules

#ifndef STUB_Ruleset_waitForRule
//#define STUB_Ruleset_waitForRule
  void Ruleset::waitForRule(const std::string & class_name, const Atlas::Objects::Root & class_desc, const std::string & dependent, const std::string & reason)
  {
    
  }
#endif //STUB_Ruleset_waitForRule

#ifndef STUB_Ruleset_init
//#define STUB_Ruleset_init
   void Ruleset::init(const std::string &)
  {
    
  }
#endif //STUB_Ruleset_init

#ifndef STUB_Ruleset_installRule
//#define STUB_Ruleset_installRule
  int Ruleset::installRule(const std::string & class_name, const std::string & section, const Atlas::Objects::Root & class_desc)
  {
    return 0;
  }
#endif //STUB_Ruleset_installRule

#ifndef STUB_Ruleset_modifyRule
//#define STUB_Ruleset_modifyRule
  int Ruleset::modifyRule(const std::string & class_name, const Atlas::Objects::Root & class_desc)
  {
    return 0;
  }
#endif //STUB_Ruleset_modifyRule


#endif