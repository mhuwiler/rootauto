// @(#)root/tmva $Id$
// Author: Simon Pfreundschuh

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : Parsing                                                               *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      NeuralNetwork                                                             *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 * 		Marc Huwiler 		  <marc.huwiler@windowslive.com> - CERN, Switzerland  *
 *      Peter Speckmayer      <peter.speckmayer@gmx.at>  - CERN, Switzerland      *
 *      Simon Pfreundschuh    <s.pfreundschuh@gmail.com> - CERN, Switzerland	  *
 *                                                                                *
 * Copyright (c) 2005-2017:                                                       *
 *      CERN, Switzerland                                                         *
 *      U. of Victoria, Canada                                                    *
 *      MPI-K Heidelberg, Germany                                                 *
 *      U. of Bonn, Germany                                                       *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef PARSING
#define PARSING

#include "TString.h"
#include "TObject.h"
#include <vector>
#include <map>

#include "TString.h"

#include "TMVA/Tools.h"







namespace TMVA 
{
  namespace DNNParsing {  
  

 
  TString fetchValue (const std::map<TString, TString>& keyValueMap, TString key); 
     
  std::vector<double> fetchValue(const std::map<TString, TString> & keyValueMap, TString key, std::vector<double> defaultValue);             

  int fetchValue(const std::map<TString,TString>& keyValueMap, TString key, int defaultValue); 

  double fetchValue (const std::map<TString,TString>& keyValueMap, TString key, double defaultValue); 
 
  TString fetchValue (const std::map<TString,TString>& keyValueMap, TString key, TString defaultValue); 
      
  bool fetchValue (const std::map<TString,TString>& keyValueMap, TString key, bool defaultValue); 
      

  
  }

}


#endif
