// @(#)root/tmva $Id$
// Author: Omar Zapata
// Mentors: Lorenzo Moneta, Sergei Gleyzer
//NOTE: Based on TMVA::Factory

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Parsing                                                            *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      This is a class to load datasets into every booked method                 *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Simon Pfreundschuh    <s.pfreundschuh@gmail.com> - CERN, Switzerland      *
 *      Peter Speckmayer      <peter.speckmayer@gmx.ch>  - CERN, Switzerland      *
 *      Marc Huwiler          <marc.huwiler@windowslive.com> - CERN, Switzerland  *
 *                                                                                *
 * Copyright (c) 2005-2017:                                                       *
 *      CERN, Switzerland                                                         *
 *                                                                                *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/


/*! \class TMVA::Parsing
\ingroup TMVA

*/

#include "TMVA/Parsing.h"


namespace TMVA {
   namespace DNNParsing {


     TString fetchValue (const std::map<TString, TString>& keyValueMap, TString key)
      {
         key.ToUpper ();
         std::map<TString, TString>::const_iterator it = keyValueMap.find (key);
         if (it == keyValueMap.end()) {
            return TString ("");
         }
         return it->second;
      }     

    

      std::vector<double> fetchValue(const std::map<TString, TString> & keyValueMap,
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
            

      int fetchValue(const std::map<TString,TString>& keyValueMap,
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

      double fetchValue (const std::map<TString,TString>& keyValueMap,
                         TString key, double defaultValue)
      {
         TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
         if (value == "") {
            return defaultValue;
         }
         return value.Atof ();
      }     

      ////////////////////////////////////////////////////////////////////////////////    

      TString fetchValue (const std::map<TString,TString>& keyValueMap,
                          TString key, TString defaultValue)
      {
         TString value (TMVA::DNNParsing::fetchValue (keyValueMap, key));
         if (value == "") {
            return defaultValue;
         }
         return value;
      }     

      ////////////////////////////////////////////////////////////////////////////////    

      bool fetchValue (const std::map<TString,TString>& keyValueMap,
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

}
}
