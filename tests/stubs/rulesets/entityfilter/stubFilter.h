// AUTOGENERATED file, created by the tool generate_stub.py, don't edit!
// If you want to add your own functionality, instead edit the stubFilter_custom.h file.

#include "rulesets/entityfilter/Filter.h"
#include "stubFilter_custom.h"

#ifndef STUB_RULESETS_ENTITYFILTER_FILTER_H
#define STUB_RULESETS_ENTITYFILTER_FILTER_H

namespace EntityFilter {

#ifndef STUB_Filter_Filter
//#define STUB_Filter_Filter
   Filter::Filter(const std::string &what, ProviderFactory* factory)
  {
    
  }
#endif //STUB_Filter_Filter

#ifndef STUB_Filter_Filter_DTOR
//#define STUB_Filter_Filter_DTOR
   Filter::~Filter()
  {
    
  }
#endif //STUB_Filter_Filter_DTOR

#ifndef STUB_Filter_match
//#define STUB_Filter_match
  bool Filter::match(LocatedEntity& entity)
  {
    return false;
  }
#endif //STUB_Filter_match

#ifndef STUB_Filter_match
//#define STUB_Filter_match
  bool Filter::match(const QueryContext& context)
  {
    return false;
  }
#endif //STUB_Filter_match


}  // namespace EntityFilter

#endif