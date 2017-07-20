#ifndef PARSING
#define PARSING

#include "TString.h"
#include "TObject.h"
#include <vector>
#include <map>


namespace TMVA 
{
  class DNNParsing {  
  

  public:   

  static TString fetchValue (const std::map<TString, TString>& keyValueMap, TString key);  

  ////////////////////////////////////////////////////////////////////////////////  

  template <typename T>
  static T fetchValue(const std::map<TString,TString>& keyValueMap,
                TString key,
                T defaultValue);  

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  static std::vector<double> fetchValue(const std::map<TString, TString> & keyValueMap,
                                 TString key,
                                 std::vector<double> defaultValue);   

    

  template <>
  static int fetchValue(const std::map<TString,TString>& keyValueMap,
                 TString key,
                 int defaultValue);   

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  static double fetchValue (const std::map<TString,TString>& keyValueMap,
                     TString key, double defaultValue); 

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  static TString fetchValue (const std::map<TString,TString>& keyValueMap,
                      TString key, TString defaultValue);   

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  static bool fetchValue (const std::map<TString,TString>& keyValueMap,
                   TString key, bool defaultValue);   

  ClassDef(DNNParsing,0);

  }; 

}


#endif
