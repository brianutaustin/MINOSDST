#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "TStyle.h" // Need to call gStyle
#include "TH1D.h"
#include "TChain.h"
#include "TTree.h"
#include "TFile.h"
#include "TROOT.h"
#include "TObjArray.h"
#include "TChainElement.h"
#include "TString.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TLegend.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
#include <fstream>

#include "DST.h"

struct RUNDATA {
  DST* TheDST;
  TH1D* TheHistogram;
  TH1D* TheRatioHistogram;
  std::string TheCodeName;
};

struct PLOTSTRING {
  TString HistogramTitle;
  TString RatioHistogramTitle;
  std::vector<TString> RatioHistogramLegend;
  TString HistogramPlotFileName;
  TString RatioHistogramPlotFileName;
  TString HistogramXAxisTitle;
  TString HistogramYAxisTitle;
};

class Analysis {
public:
  Analysis();
  Analysis(HistogramIndex histIndex);
  ~Analysis();

  void ImportBaseDST(DST*);
  void ImportCompareDSTs(DST*);
  void SetImageFileFormat(std::string);
  void DrawWithLogScale(bool);
  void Draw();

private:
  void GetPlotString();

private:
  HistogramIndex AnalysisIndex;

  RUNDATA BaseRun;
  std::vector<RUNDATA> CompareRuns;

  bool LogScaleFlag = true;

  TCanvas* Canvas;
  TCanvas* RatioCanvas;
  PLOTSTRING PlotString;

  std::string ImageFormat;
};

#endif
