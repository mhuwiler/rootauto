// @(#)root/tmva $Id$
// Author: Akshay Vashistha

/**********************************************************************************
 * Project: TMVA - a Root-integrated toolkit for multivariate data analysis *
 * Package: TMVA *
 * Class  : MethodDAE *
 * Web    : http://tmva.sourceforge.net *
 *                                                                                *
 * Description: *
 *      A neural network implementation *
 *                                                                                *
 * Authors (alphabetical): *
 *                                                                                *
 *                                                                                *
 * Copyright (c) 2005-2015: *
 *                                                                                *
 *                                                                                *
 * Redistribution and use in source and binary forms, with or without *
 * modification, are permitted according to the terms listed in LICENSE *
 * (http://tmva.sourceforge.net/LICENSE) *
 **********************************************************************************/

/*! \class TMVA::MethodDAE
\ingroup TMVA
 Implementation.
*/
#include "TMVA/MethodDAE.h"

#include "TMVA/ClassifierFactory.h"
#include "TMVA/Configurable.h"
#include "TMVA/IMethod.h"
#include "TMVA/Types.h"

#include "TMVA/DNN/Architectures/Reference.h"
#include "TMVA/DNN/Functions.h"
#include "TMVA/Parsing.h"

#include <iostream>

REGISTER_METHOD(DAE)
ClassImp(TMVA::MethodDAE)

using TMVA::DNN::EActivationFunction;
using TMVA::DNN::ELossFunction;
using TMVA::DNN::EInitialization;
using TMVA::DNN::EOutputFunction;

namespace TMVA {
  //using namespace DNNParsing; 

////////////////////////////////////////////////////////////////////////////////
/// standard constructor
TMVA::MethodDAE::MethodDAE(const TString &jobName, const TString &methodTitle,
                           DataSetInfo &theData, const TString &theOption)
    : MethodBase(jobName, Types::kDAE, methodTitle, theData, theOption),
      fWeightInitialization(), fOutputFunction(), fLayoutString(),
      fErrorStrategy(), fTrainingStrategyString(),
      fWeightInitializationString(), fArchitectureString(), fResume(false),
      fTrainingSettings()

{
  // Nothing to do here
}

////////////////////////////////////////////////////////////////////////////////
/// constructor from a weight file
TMVA::MethodDAE::MethodDAE(DataSetInfo &theData, const TString &theWeightFile)
    : MethodBase(Types::kDAE, theData, theWeightFile), fWeightInitialization(),
      fOutputFunction(), fLayoutString(), fErrorStrategy(),
      fTrainingStrategyString(), fWeightInitializationString(),
      fArchitectureString(), fResume(false), fTrainingSettings() {
  // Nothing to do here
}

////////////////////////////////////////////////////////////////////////////////
/// destructor

TMVA::MethodDAE::~MethodDAE() {
  //delete net; 
}

////////////////////////////////////////////////////////////////////////////////
/// default initializations
void TMVA::MethodDAE::Init() {
  // Nothing to do here
  std::cout << "MethodDAE initialized successfully! " << std::endl; 
}

////////////////////////////////////////////////////////////////////////////////
/// Options to be set in the option string
void TMVA::MethodDAE::DeclareOptions() {                        // Change this 
  DeclareOptionRef(fLayoutString="SOFTSIGN|(N+100)*2,LINEAR",
                                  "Layout",
                                  "Layout of the network.");

   DeclareOptionRef(fErrorStrategy="CROSSENTROPY",
                    "ErrorStrategy",
                    "Loss function: Mean squared error (regression)"
                    " or cross entropy (binary classification).");
   AddPreDefVal(TString("CROSSENTROPY"));
   AddPreDefVal(TString("SUMOFSQUARES"));
   AddPreDefVal(TString("MUTUALEXCLUSIVE"));

   DeclareOptionRef(fWeightInitializationString="XAVIER",
                    "WeightInitialization",
                    "Weight initialization strategy");
   AddPreDefVal(TString("XAVIER"));
   AddPreDefVal(TString("XAVIERUNIFORM"));

   DeclareOptionRef(fArchitectureString = "STANDARD", "Architecture", "Which architecture to perform the training on.");
   AddPreDefVal(TString("STANDARD"));
   AddPreDefVal(TString("CPU"));
   AddPreDefVal(TString("GPU"));
   AddPreDefVal(TString("OPENCL"));

   DeclareOptionRef(
       fTrainingStrategyString = "LearningRate=1e-1,"
                                 "Repetitions=3,"
                                 "ConvergenceSteps=50,"
                                 "BatchSize=30,"
                                 "TestRepetitions=7,"
                                 "Renormalize=L2,"
                                 "Corruption=0.0,"
                                 "DropRepetitions=5|LearningRate=1e-4,"
                                 "Repetitions=3,"
                                 "ConvergenceSteps=50,"
                                 "BatchSize=20,"
                                 "TestRepetitions=7,"
                                 "WeightDecay=0.001,"
                                 "DropRepetitions=5,"
                                 "Multithreading=True",
                                 "TrainingStrategy",
                                 "Defines the training strategies.");
}

void TMVA::MethodDAE::ProcessOptions() {
  if (IgnoreEventsWithNegWeightsInTraining()) {             // Shall we keep this? 
      Log() << kINFO
            << "Will ignore negative events in training!"
            << Endl;
   }

   if (fArchitectureString == "STANDARD") {
      /*Log() << kERROR << "The STANDARD architecture has been deprecated. "
                         "Please use Architecture=CPU or Architecture=CPU."
                         "See the TMVA Users' Guide for instructions if you "
                         "encounter problems."
            << Endl;
      Log() << kFATAL << "The STANDARD architecture has been deprecated. "
                         "Please use Architecture=CPU or Architecture=CPU."
                         "See the TMVA Users' Guide for instructions if you "
                         "encounter problems."
            << Endl;*/
   }

   if (fArchitectureString == "OPENCL") {
      Log() << kERROR << "The OPENCL architecture has not been implemented yet. "
                         "Please use Architecture=CPU or Architecture=CPU for the "
                         "time being. See the TMVA Users' Guide for instructions "
                         "if you encounter problems."
            << Endl;
      Log() << kFATAL << "The OPENCL architecture has not been implemented yet. "
                         "Please use Architecture=CPU or Architecture=CPU for the "
                         "time being. See the TMVA Users' Guide for instructions "
                         "if you encounter problems."
            << Endl;
   }

   if (fArchitectureString == "GPU") {
#ifndef DNNCUDA // Included only if DNNCUDA flag is _not_ set.
      Log() << kERROR << "CUDA backend not enabled. Please make sure "
                         "you have CUDA installed and it was successfully "
                         "detected by CMAKE."
            << Endl;
      Log() << kFATAL << "CUDA backend not enabled. Please make sure "
                         "you have CUDA installed and it was successfully "
                         "detected by CMAKE."
            << Endl;
#endif // DNNCUDA
   }

   if (fArchitectureString == "CPU") {
#ifndef DNNCPU // Included only if DNNCPU flag is _not_ set.
      Log() << kERROR << "Multi-core CPU backend not enabled. Please make sure "
                         "you have a BLAS implementation and it was successfully "
                         "detected by CMake as well that the imt CMake flag is set."
            << Endl;
      Log() << kFATAL << "Multi-core CPU backend not enabled. Please make sure "
                         "you have a BLAS implementation and it was successfully "
                         "detected by CMake as well that the imt CMake flag is set."
            << Endl;
#endif // DNNCPU
   }

   //
   // Set network structure.
   //

   fLayout = TMVA::MethodDAE::ParseLayoutString (fLayoutString);
   size_t inputSize = GetNVariables ();
   size_t outputSize = 1;
   if (fAnalysisType == Types::kRegression && GetNTargets() != 0) {
      outputSize = GetNTargets();
   } else if (fAnalysisType == Types::kMulticlass && DataInfo().GetNClasses() >= 2) {
      outputSize = DataInfo().GetNClasses();
   }

   // Set the input size and the batch size for the net. 

   //net = new TSDAE<fArchitectureString>(batchSize, inputSize, outputSize, fLayout.size(), fLayout)

   #pragma("build net here")
   //
   // Loss function and output.
   //

   /*fOutputFunction = EOutputFunction::kSigmoid;
   if (fAnalysisType == Types::kClassification)
   {
      if (fErrorStrategy == "SUMOFSQUARES") {
         fNet.SetLossFunction(ELossFunction::kMeanSquaredError);
      }
      if (fErrorStrategy == "CROSSENTROPY") {
         fNet.SetLossFunction(ELossFunction::kCrossEntropy);
      }
      fOutputFunction = EOutputFunction::kSigmoid;
   } else if (fAnalysisType == Types::kRegression) {
      if (fErrorStrategy != "SUMOFSQUARES") {
         Log () << kWARNING << "For regression only SUMOFSQUARES is a valid "
                << " neural net error function. Setting error function to "
                << " SUMOFSQUARES now." << Endl;
      }
      fNet.SetLossFunction(ELossFunction::kMeanSquaredError);
      fOutputFunction = EOutputFunction::kIdentity;
   } else if (fAnalysisType == Types::kMulticlass) {
      if (fErrorStrategy == "SUMOFSQUARES") {
         fNet.SetLossFunction(ELossFunction::kMeanSquaredError);
      }
      if (fErrorStrategy == "CROSSENTROPY") {
         fNet.SetLossFunction(ELossFunction::kCrossEntropy);
      }
      if (fErrorStrategy == "MUTUALEXCLUSIVE") {
         fNet.SetLossFunction(ELossFunction::kSoftmaxCrossEntropy);
      }
      fOutputFunction = EOutputFunction::kSoftmax;
   }*/

   //
   // Initialization
   //

   if (fWeightInitializationString == "XAVIER") {
      fWeightInitialization = DNN::EInitialization::kGauss;
   }
   else if (fWeightInitializationString == "XAVIERUNIFORM") {
      fWeightInitialization = DNN::EInitialization::kUniform;
   }
   else {
      fWeightInitialization = DNN::EInitialization::kGauss;
   }

   //
   // Training settings.
   //

   KeyValueVector_t strategyKeyValues = ParseKeyValueString(fTrainingStrategyString,
                                                            TString ("|"),
                                                            TString (","));
   for (auto& block : strategyKeyValues) {
      TTrainingSettings settings;

      settings.convergenceSteps = DNNParsing::fetchValue(block, "ConvergenceSteps", 100);
      settings.batchSize        = DNNParsing::fetchValue(block, "BatchSize", 30);
      settings.testInterval     = DNNParsing::fetchValue(block, "TestRepetitions", 7);
      settings.weightDecay      = DNNParsing::fetchValue(block, "WeightDecay", 0.0);
      settings.learningRate         = DNNParsing::fetchValue(block, "LearningRate", 1e-5);
      //settings.momentum             = DNNParsing::fetchValue(block, "Momentum", 0.3);
      settings.corruption = DNNParsing::fetchValue(block, "Corruption", 0.0);

      //TString regularization = DNNParsing::fetchValue(block, "Regularization", TString ("NONE"));
      /*if (regularization == "L1") {
         settings.regularization = DNN::ERegularization::kL1;
      } else if (regularization == "L2") {
         settings.regularization = DNN::ERegularization::kL2;
      }*/

      TString strMultithreading = DNNParsing::fetchValue(block, "Multithreading",
                                             TString ("True"));
      if (strMultithreading.BeginsWith ("T")) {
         settings.multithreading = true;
      } else {
         settings.multithreading = false;
      }

      fTrainingSettings.push_back(settings);

   }
}

////////////////////////////////////////////////////////////////////////////////
/// parse layout specification string and return a vector, each entry
/// containing the dimension and the activation function of each successive
/// layer
auto TMVA::MethodDAE::ParseLayoutString(TString layoutString)
    -> LayoutVector_t {
  // TO DO
  std::vector<Int_t> layout;

  TString delimiter = ","; 

  int stringLength = layoutString.Length();
  //int delimLength=delim.Length(); 

  //std::cout<<"String: "<<layoutString<<"  "<<stringLength<<std::endl;
  //std::cout<<"Delim: "<<delim<<"  "<<delimLength<<std::endl;  

  std::cout<<layoutString<<"  "<<stringLength<<std::endl; 

  int stop=1;
  TString temp="---";
  while(stop!=-1){
      stop = layoutString.First(delimiter); 

      if(stop!=-1){
          temp = layoutString(0, stop);
          std::cout<<"Substring: "<<temp<<std::endl;
          TSubString subString = layoutString( stop+delimiter.Length(), stringLength );
          layoutString=subString;
          stringLength=layoutString.Length();
      }
      else{
          stringLength=layoutString.Length();
          temp = layoutString(0, stringLength);
          std::cout<<"Substring: "<<temp<<std::endl;
      }                                                               // Works! 

      layout.push_back(std::stoi(std::string(temp)));
  }



  return layout;
}

////////////////////////////////////////////////////////////////////////////////
/// What kind of analysis type can handle the DAE
Bool_t TMVA::MethodDAE::HasAnalysisType(Types::EAnalysisType type,
                                        UInt_t numberClasses,
                                        UInt_t /*numberTargets*/) {
  // TO DO
  return true;
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::Train() {

  std::cout << "MethodDAE is training... " << std::endl;
  for (unsigned int i=0; i<fLayout.size(); i++) {
    std::cout << fLayout[i] << std::endl;  
  }
  if (fArchitectureString == "GPU") {
    TrainGpu();
    return;
  } else if (fArchitectureString == "OpenCL") {
    Log() << kFATAL << "OpenCL backend not yet supported." << Endl;
    return;
  } else if (fArchitectureString == "CPU") {
    TrainCpu();
    return;
  }
  else {
    //TrainStd(); 
    //TrainLayer(); 
  }
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::TrainGpu() {
  // TO DO
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::TrainCpu() {
  // TO DO
}

////////////////////////////////////////////////////////////////////////////////
Double_t TMVA::MethodDAE::GetMvaValue(Double_t * /*errLower*/,
                                      Double_t * /*errUpper*/) {
  // TO DO
  return 0.0;
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::AddWeightsXMLTo(void *parent) const {
  // TO DO
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::ReadWeightsFromXML(void *rootXML) {
  // TO DO
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::ReadWeightsFromStream(std::istream & /*istr*/) {}

////////////////////////////////////////////////////////////////////////////////
const TMVA::Ranking *TMVA::MethodDAE::CreateRanking() {
  // TO DO
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void TMVA::MethodDAE::GetHelpMessage() const {
  // TO DO
}

auto TMVA::MethodDAE::ParseKeyValueString(TString parseString,
                                          TString blockDelim,
                                          TString tokenDelim)
    -> KeyValueVector_t
{
   KeyValueVector_t blockKeyValues;
   const TString keyValueDelim ("=");

   TObjArray* blockStrings = parseString.Tokenize (blockDelim);
   TIter nextBlock (blockStrings);
   TObjString* blockString = (TObjString *) nextBlock();

   for (; blockString != nullptr; blockString = (TObjString *) nextBlock())
   {
      blockKeyValues.push_back (std::map<TString,TString>());
      std::map<TString,TString>& currentBlock = blockKeyValues.back ();

      TObjArray* subStrings = blockString->GetString ().Tokenize (tokenDelim);
      TIter nextToken (subStrings);
      TObjString* token = (TObjString*)nextToken ();

      for (; token != nullptr; token = (TObjString *)nextToken())
      {
         TString strKeyValue (token->GetString ());
         int delimPos = strKeyValue.First (keyValueDelim.Data ());
         if (delimPos <= 0)
             continue;

         TString strKey = TString (strKeyValue (0, delimPos));
         strKey.ToUpper();
         TString strValue = TString (strKeyValue (delimPos+1, strKeyValue.Length ()));

         strKey.Strip (TString::kBoth, ' ');
         strValue.Strip (TString::kBoth, ' ');

         currentBlock.insert (std::make_pair (strKey, strValue));
      }
   }
   return blockKeyValues;
}


} // namespace TMVA
