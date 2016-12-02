#include "DST.h"

DST::DST() {
}

DST::DST(std::string dstLocation) {
  DSTFilesLocation = dstLocation;
}

DST::~DST() {
}

void DST::OpenDSTs(std::string treeName) {
  TreeName = treeName;
  TreeChain = new TChain(TreeName.c_str(), TreeName.c_str());
  std::string dummyDSTlist = DSTFilesLocation + "/*.root";
  TreeChain->Add(dummyDSTlist.c_str());

  return;
}

void DST::CalculatePOT() {
  POT = 0;

  TObjArray * fileElements = TreeChain->GetListOfFiles();
  TIter next(fileElements);
  TChainElement * chainElement = 0;
  bool InteruptToGetOneFileNameExample = true;
  while (chainElement = (TChainElement *) next()) {
    TFile * file = TFile::Open(chainElement->GetTitle(), "read");
    if (InteruptToGetOneFileNameExample) {
      RunCodeName = chainElement->GetTitle();
      InteruptToGetOneFileNameExample = false;
    }
    TH1D * htmpPOT = (TH1D *) file->Get("hTotalPot");
    POT += htmpPOT->Integral();
    file->Close();
  }

  return;
}

double DST::GetPOT() {
  return POT;
}

std::string DST::GetRunCodeName() {
  return RunCodeName;
}

void DST::CodeNameParsing() {
  std::string delimiter = "_";

  size_t pos = 0;
  std::string token;

  while ((pos = RunCodeName.find(delimiter)) != std::string::npos) {
    token = RunCodeName.substr(0, pos);
    RunCodeName.erase(0, pos + delimiter.length());
  }
  RunCodeName = token;
  token = RunCodeName.erase(3, RunCodeName.length() - 5);
  RunCodeName = token;
  std::cout << "POT " << RunCodeName << " :" << POT << "." << std::endl;

  return;
}

void DST::GetHistogramNameStrings(HistogramIndex histIndex) {
  TString variableName;

  switch (histIndex) {
    case kShEn: {
      variableName = "shwEnkNN";
      break;
    }
    case kTrEn: {
      variableName = "trkEn";
      break;
    }
    case kCCEn: {
      variableName = "energyCC";
      break;
    }
    case kNCEn: {
      variableName = "energyNC";
      break;
    }
  }

  HistogramNameString.HistogramName = variableName + RunCodeName;
  HistogramNameString.HistogramTitle = variableName + RunCodeName;
  HistogramNameString.HistogramXAxisTitle = variableName + "[GeV]";
  HistogramNameString.HistogramYAxisTitle = "Entries";
  HistogramNameString.TreeChainHistogramName = variableName;

  return;
}

void DST::SetUnslipstackedBatches(int min, int max) {
  UnslipstackedMinBatchIndex = min;
  UnslipstackedMaxBatchIndex = max;

  return;
}

void DST::SetHistograms(HistogramIndex histIndex) {

  GetHistogramNameStrings(histIndex);
  TH1D * dummyHistogram = new TH1D(HistogramNameString.HistogramName, HistogramNameString.HistogramTitle, NBins, BinningScheme);
  dummyHistogram->GetXaxis()->SetTitle(HistogramNameString.HistogramXAxisTitle);
  dummyHistogram->GetXaxis()->SetTitle(HistogramNameString.HistogramYAxisTitle);
  dummyHistogram->SetLineColor(histIndex + 1);

  for (int i = 0; i < NumberOfEvents; i++) {
    TreeChain->GetEntry(i);
    if (TreeChain->GetLeaf("selectionevent")->GetValue()) {
      if ((TreeChain->GetLeaf("whichBatch")->GetValue() >= UnslipstackedMinBatchIndex) && (TreeChain->GetLeaf("whichBatch")->GetValue() <= UnslipstackedMaxBatchIndex)) {
        dummyHistogram->Fill(TreeChain->GetLeaf(HistogramNameString.TreeChainHistogramName)->GetValue());
      }
    }
  }

  CalculatePOT();
  dummyHistogram->Scale(1 / (POT * (1E-18)));
  dummyHistogram->Sumw2();
  HistogramVector.push_back(dummyHistogram);

  return;
}

void DST::SetNumberOfEvents(int flag) {
  if (flag == -1) {
    NumberOfEvents = TreeChain->GetEntries();
  } else {
    NumberOfEvents = flag;
  }

  return;
}

void DST::SetBinningScheme() {
  for (int k = 0; k <= 40; k++) {
    BinningScheme[k] = k * 0.25;
  }
  for (int k = 41; k <= 42; k++) {
    BinningScheme[k] = 10 + 0.5 * (k - 40);
  }
  for (int k = 43; k <= 43; k++) {
    BinningScheme[k] = 11 + (k - 42);
  }
  for (int k = 44; k <= 48; k++) {
    BinningScheme[k] = 12 + 2 * (k - 43);
  }
  for (int k = 49; k <= 50; k++) {
    BinningScheme[k] = 22 + 4 * (k - 48);
  }

  return;
}

TH1D * DST::GetHistogram(HistogramIndex histIndex) {
  return HistogramVector.at(histIndex);
}
