#ifndef DST_H
#define DST_H

#include "TH1D.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TChainElement.h"
#include "TString.h"
#include "TLeaf.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <fstream>

const int NBins = 50;

enum HistogramIndex {
  kShEn,
  kCCEn,
  kNCEn,
  kTrEn
};

struct HISTOGRAMSTRING {
  TString HistogramName;
  TString HistogramTitle;
  TString HistogramXAxisTitle;
  TString HistogramYAxisTitle;
  TString TreeChainHistogramName;
};

class DST {
public:
  DST();
  DST(std::string); // Input: Directory contains the DSTs
  ~DST();

  void OpenDSTs(std::string); // Input: Name of anaTree
  void CalculatePOT();
  void CodeNameParsing(); // Each run has a code name

  // GET
  double GetPOT();
  TH1D* GetHistogram(HistogramIndex);
  std::string GetRunCodeName();

  // SET
  void SetNumberOfEvents(int);
  void SetUnslipstackedBatches(int, int); // Min and max of unslipstacked batches number
  void SetHistograms(HistogramIndex);
  void SetBinningScheme();

private:
  void GetHistogramNameStrings(HistogramIndex);

private:
  std::string DSTFilesLocation;

  std::string TreeName;
  TChain* TreeChain;

  TH1D* hTOTALPOT;
  double POT;

  std::string RunCodeName;

  double BinningScheme[NBins + 1];

  HISTOGRAMSTRING HistogramNameString;
  std::vector<TH1D*> HistogramVector;

  int NumberOfEvents;

  int UnslipstackedMinBatchIndex;
  int UnslipstackedMaxBatchIndex;
};

#endif
