#include "Parsing.h"

ClassImp(TMVA::DNNParsing);

TString TMVA::DNNParsing::fetchValue (const std::map<TString, TString>& keyValueMap, TString key)
{
   key.ToUpper ();
   std::map<TString, TString>::const_iterator it = keyValueMap.find (key);
   if (it == keyValueMap.end()) {
      return TString ("");
   }
   return it->second;
}

template <>
std::vector<double> TMVA::DNNParsing::fetchValue(const std::map<TString, TString> & keyValueMap,
                               TString key,
                               std::vector<double> defaultValue)
{
   TString parseString (TMVA::DNNParsing::fetchValue (keyValueMap, key));
   if (parseString == "") {
      return defaultValue;
   }
   parseString.ToUpper ();
   std::vector<double> values;

   const TString tokenDelim ("+");
   TObjArray* tokenStrings = parseString.Tokenize (tokenDelim);
   TIter nextToken (tokenStrings);
   TObjString* tokenString = (TObjString*)nextToken ();
   for (; tokenString != NULL; tokenString = (TObjString*)nextToken ()) {
      std::stringstream sstr;
      double currentValue;
      sstr << tokenString->GetString ().Data ();
      sstr >> currentValue;
      values.push_back (currentValue);
   }
   return values;
}
 

template <>
int TMVA::DNNParsing::fetchValue(const std::map<TString,TString>& keyValueMap,
               TString key,
               int defaultValue)
{
   TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
   if (value == "") {
      return defaultValue;
   }
   return value.Atoi ();
}

////////////////////////////////////////////////////////////////////////////////

template <>
double TMVA::DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                   TString key, double defaultValue)
{
   TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
   if (value == "") {
      return defaultValue;
   }
   return value.Atof ();
}

////////////////////////////////////////////////////////////////////////////////

template <>
TString TMVA::DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                    TString key, TString defaultValue)
{
   TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
   if (value == "") {
      return defaultValue;
   }
   return value;
}

////////////////////////////////////////////////////////////////////////////////

template <>
bool TMVA::DNNParsing::fetchValue (const std::map<TString,TString>& keyValueMap,
                 TString key, bool defaultValue)
{
   TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
   if (value == "") {
      return defaultValue;
   }
   value.ToUpper ();
   if (value == "TRUE" || value == "T" || value == "1") {
      return true;
   }
   return false;
}