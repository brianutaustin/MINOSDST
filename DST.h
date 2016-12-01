#ifndef DST_h
#define DST_h

#include "TH1D.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TIter.h"
#include "TChainElement.h"
#include "TString.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <fstream>

enum HistogramIndex {
  kShEn,
  kCCEn,
  kNCEn,
  kTrEn
};

class DST {
public:
  DST();
  DST(std::string); // Input: Directory contains the DSTs
  ~DST();

  void OpenDSTs(std::string); // Input: Name of anaTree

  // GET
  double GetPOT();
  TH1D* GetHistogram(HistogramIndex);

  // SET
  void SetNumberOfEvents(int);
  void SetHistograms();
  void SetBinningScheme();

private:
  void CalculatePOT();
  void CodeNameParsing(); // Each run has a code name

private:
  std::string DSTFilesLocation;

  std::string TreeName;
  TChain* TreeChain;

  TH1D* hTOTALPOT;
  double POT;

  std::string RunCodeName;

  const int NBins = 50;
  double BinningScheme[NBins + 1];

  TH1D* shEn;
  TH1D* trEn;
  TH1D* CCEn;
  TH1D* NCEn;

  int NumberOfEvents;
}

#endif
