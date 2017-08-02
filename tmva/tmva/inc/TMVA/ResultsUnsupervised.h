// @(#)root/tmva $Id$
// Author: Andreas Hoecker, Peter Speckmayer, Joerg Stelzer, Helge Voss

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis       *
 * Package: TMVA                                                                  *
 * Class  : ResultsUnsupervised                                                   *
 * Web    : http://tmva.sourceforge.net                                           *
 *                                                                                *
 * Description:                                                                   *
 *      Base-class for result-vectors                                             *
 *                                                                                *
 * Authors (alphabetical):                                                        *
 *      Marc Huwiler       CERN Summer Student     <marc.huwiler@windowslive.com> *
 *                                                                                *
 * Copyright (c) 2017:                                                            *
 *      CERN, Switzerland                                                         *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without             *
 * modification, are permitted according to the terms listed in LICENSE           *
 * (http://tmva.sourceforge.net/LICENSE)                                          *
 **********************************************************************************/

#ifndef ROOT_TMVA_ResultsUnsupervised
#define ROOT_TMVA_ResultsUnsupervised 


#include <vector>

#include "TMVA/Results.h"

namespace TMVA {

   class MsgLogger;

   class ResultsUnsupervised :  public Results {

   public:

      ResultsUnsupervised( const DataSetInfo* dsi, TString resultsName );
      ~ResultsUnsupervised();

      // setters
      void     SetValue( Float_t value, Int_t ievt );

      void     Resize( Int_t entries )   { fMvaValues.resize( entries ); }
      using TObject::Clear;
      virtual void     Clear(Option_t *)                   { fMvaValues.clear(); fMvaValuesTypes.clear(); }

      // getters
      Long64_t GetSize()                  const { return fMvaValues.size(); }
      virtual const std::vector< Float_t >&  operator [] ( Int_t ievt ) const { fRet[0] = fMvaValues[ievt]; return  fRet; }
      std::vector<Float_t>* GetValueVectorDecoded()    { return &fMvaValuesDecoded; }
      std::vector<Bool_t>*  GetValueVectorEncoded()    { return &fMvaValuesEncoded; }
      
      Types::EAnalysisType  GetAnalysisType() { return Types::kUnsupervised; }


   private:

      std::vector< Float_t >  fMvaValuesDecoded;   // mva values (Results)
      std::vector< Bool_t>    fMvaValuesEncoded;// mva values type(sig/bkg) (Results)
      mutable std::vector< Float_t >  fRet;         // return val
      mutable MsgLogger*      fLogger;      //! message logger
      MsgLogger& Log() const { return *fLogger; }
   protected:
       
       ClassDef(Results,2);
      
   };
}

#endif
