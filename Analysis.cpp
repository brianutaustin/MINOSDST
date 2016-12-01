#include "Analysis.h"

Analysis::Analysis() {
}

Analysis::Analysis(HistogramIndex histIndex) {
  AnalysisIndex = histIndex;
}

Analysis::~Analysis() {
}

void Analysis::ImportBaseDST(DST * baseDST) {
  BaseRun.TheDST = baseDST;
  BaseRun.TheHistogram = (TH1D *) baseDST->GetHistogram(AnalysisIndex)->Clone();
  BaseRun.TheCodeName = baseDST->GetRunCodeName();

  return;
}

void Analysis::ImportCompareDSTs(DST * compareDST) {
  RUNDATA dummyRunDataStruct;

  dummyRunDataStruct.TheDST = compareDST;
  dummyRunDataStruct.TheHistogram = (TH1D *) compareDST->GetHistogram(AnalysisIndex)->Clone();
  dummyRunDataStruct.TheRatioHistogram = (TH1D *) compareDST->GetHistogram(AnalysisIndex)->Clone();
  dummyRunDataStruct.TheCodeName = compareDST->GetRunCodeName();
  CompareRuns.push_back(dummyRunDataStruct);

  return;
}

void Analysis::DrawWithLogScale(bool drawWithLogScale) {
  LogScaleFlag = drawWithLogScale;

  return;
}

void Analysis::Draw() {
  gStyle->SetOptStat(0);
  GetPlotString();

  Canvas = new TCanvas();
  BaseRun.TheHistogram->SetTitle(PlotString.HistogramTitle);
  BaseRun.TheHistogram->GetXaxis()->SetTitle(PlotString.HistogramXAxisTitle);
  BaseRun.TheHistogram->GetYaxis()->SetTitle(PlotString.HistogramYAxisTitle);
  BaseRun.TheHistogram->SetLineColor(kBlack);
  BaseRun.TheHistogram->SetLineWidth(3);

  TLegend * Legend = new TLegend(0.6, 0.6, 0.85, 0.85);
  for (int i = 0; i < CompareRuns.size(); i++) {
    Legend->AddEntry(CompareRuns.at(i).TheHistogram, CompareRuns.at(i).TheCodeName.c_str(), "l");
  }
  BaseRun.TheHistogram->Draw();
  for (int i = 0; i < CompareRuns.size(); i++) {
    CompareRuns.at(i).TheHistogram->Draw("same");
  }
  Legend->Draw();
  Canvas->SaveAs(PlotString.HistogramPlotFileName);

  return;
}

void Analysis::SetImageFileFormat(std::string theFormatExtension) {
  ImageFormat = theFormatExtension;

  return;
}

void Analysis::GetPlotString() {
  std::string VariableName;

  switch (AnalysisIndex) {
    case kShEn: {
      VariableName = "shwEnkNN";
      break;
    }
    case kTrEn: {
      VariableName = "trkEn";
      break;
    }
    case kCCEn: {
      VariableName = "energyCC";
      break;
    }
    case kNCEn: {
      VariableName = "energyNC";
      break;
    }
  }

  PlotString.HistogramTitle = "Comparison of " + VariableName;
  PlotString.RatioHistogramTitle = "Ratio Comparison of " + VariableName;
  for (int i = 0; i < CompareRuns.size(); i++) {
    PlotString.RatioHistogramLegend.at(i) = CompareRuns.at(i).TheCodeName + "/" + BaseRun.TheCodeName;
  }
  PlotString.HistogramPlotFileName = "spectrum_" + VariableName + "." + ImageFormat;
  PlotString.RatioHistogramPlotFileName = "ratio_" + VariableName + "." + ImageFormat;
  PlotString.HistogramXAxisTitle = VariableName + "[GeV]";
  PlotString.HistogramYAxisTitle = "Entries";

  return;
}
