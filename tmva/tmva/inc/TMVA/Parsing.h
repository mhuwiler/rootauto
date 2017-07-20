#ifndef PARSING
#define PARSING

#include "TString.h"
#include "TObject.h"
#include <vector>
#include <map>


namespace TMVA 
{
  namespace DNNParsing {  
  

 
  TString fetchValue (const std::map<TString, TString>& keyValueMap, TString key);  

  ////////////////////////////////////////////////////////////////////////////////  

  template <typename T>
  T fetchValue(const std::map<TString,TString>& keyValueMap,
                TString key,
                T defaultValue);  

  ////////////////////////////////////////////////////////////////////////////////  




  template <>
  std::vector<double> DNNParsing::fetchValue(const std::map<TString, TString> & keyValueMap,
                                 TString key,
                                 std::vector<double> defaultValue);   

    

  template <>
  int DNNParsing::fetchValue(const std::map<TString,TString>& keyValueMap,
                 TString key,
                 int defaultValue);   

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  double DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                     TString key, double defaultValue); 

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  TString DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                      TString key, TString defaultValue);   

  ////////////////////////////////////////////////////////////////////////////////  

  template <>
  bool DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                   TString key, bool defaultValue);   
  
  }

}


#endif
