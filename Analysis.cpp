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

void Analysis::DrawHistogram() {
  gStyle->SetOptStat(0);
  TLegend * Legend = new TLegend(0.75, 0.75, 0.95, 0.95);

  Canvas = new TCanvas();
  if (&BaseRun == NULL) {
    std::cout << "Base DST has to be input." << std::endl;
  } else {
    BaseRun.TheHistogram->SetTitle(PlotString.HistogramTitle);
    BaseRun.TheHistogram->GetXaxis()->SetTitle(PlotString.HistogramXAxisTitle);
    BaseRun.TheHistogram->GetYaxis()->SetTitle(PlotString.HistogramYAxisTitle);
    BaseRun.TheHistogram->SetLineColor(kBlack);
    BaseRun.TheHistogram->SetLineWidth(3);
    //BaseRun.TheHistogram->GetYaxis()->SetRangeUser(0, 1);
    BaseRun.TheHistogram->Draw();
    Legend->AddEntry(BaseRun.TheHistogram, BaseRun.TheCodeName.c_str(), "l");
  }

  if (CompareRuns.size() == 0) {
    std::cout << "Compare DST has to be input." << std::endl;
  } else {
    for (int i = 0; i < CompareRuns.size(); i++) {
      CompareRuns.at(i).TheHistogram->SetLineColor(i + 2);
      CompareRuns.at(i).TheHistogram->Draw("same");
      Legend->AddEntry(CompareRuns.at(i).TheHistogram, CompareRuns.at(i).TheCodeName.c_str(), "l");
    }
  }

  Legend->Draw();
  if (LogScaleFlag) {
    Canvas->SetLogy();
  }
  Canvas->SaveAs(PlotString.HistogramPlotFileName);
  return;
}

void Analysis::DrawRatioHistogram() {
  gStyle->SetOptStat(0);

  Canvas = new TCanvas();
  TLegend * Legend = new TLegend(0.75, 0.75, 0.95, 0.95);

	for (int i = 0; i < CompareRuns.size(); i++) {
    CompareRuns.at(i).TheRatioHistogram->SetTitle(PlotString.RatioHistogramTitle);
    CompareRuns.at(i).TheRatioHistogram->Divide(BaseRun.TheHistogram);
    for(int jbin = 0; jbin < CompareRuns.at(i).TheRatioHistogram->GetNbinsX(); jbin++){
  		double binerr = CompareRuns.at(i).TheHistogram->GetBinError(jbin)/BaseRun.TheHistogram->GetBinContent(jbin) - (CompareRuns.at(i).TheHistogram->GetBinContent(jbin))*(BaseRun.TheHistogram->GetBinError(jbin))/((BaseRun.TheHistogram->GetBinContent(jbin))*(BaseRun.TheHistogram->GetBinContent(jbin)));
  		CompareRuns.at(i).TheRatioHistogram->SetBinError(jbin, binerr);
  	}
    CompareRuns.at(i).TheRatioHistogram->GetXaxis()->SetTitle(PlotString.HistogramXAxisTitle);
    CompareRuns.at(i).TheRatioHistogram->GetYaxis()->SetTitle("");
    CompareRuns.at(i).TheRatioHistogram->SetLineColor(i + 2);
    Legend->AddEntry(CompareRuns.at(i).TheRatioHistogram, PlotString.RatioHistogramLegend.at(i), "l");
  }

  CompareRuns.at(0).TheRatioHistogram->Draw();
  CompareRuns.at(0).TheRatioHistogram->GetYaxis()->SetRangeUser(0.7, 1.3);
  for (int i = 1; i < CompareRuns.size(); i++) {
    CompareRuns.at(i).TheRatioHistogram->Draw("same");
  }
	TLine * UnityLine = new TLine(0, 1, 30, 1);
  UnityLine->SetLineColor(kBlack);
  UnityLine->SetLineWidth(2);
  UnityLine->SetLineStyle(4);
	UnityLine->Draw("same");
  Legend->Draw();

	Canvas->SaveAs(PlotString.RatioHistogramPlotFileName);
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
    PlotString.RatioHistogramLegend.push_back(CompareRuns.at(i).TheCodeName + "/" + BaseRun.TheCodeName);
  }
  PlotString.HistogramPlotFileName = "spectrum_" + VariableName + "." + ImageFormat;
  PlotString.RatioHistogramPlotFileName = "ratio_" + VariableName + "." + ImageFormat;
  PlotString.HistogramXAxisTitle = VariableName + "[GeV]";
  PlotString.HistogramYAxisTitle = "Entries";

  return;
}
