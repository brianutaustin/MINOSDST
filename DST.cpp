#include "DST.h"

DST::DST() {
}

DST::DST(std : string dstLocation) {
  DSTFilesLocation = dstLocation;
}

DST::~DST() {
}

void DST::OpenDSTs(std::string treeName) {
  TreeName = treeName;
  TreeChain = new TChain(TreeName.c_str(), TreeName.c_str());
  std::string DSTlist = DSTFilesLocation + "/*.root";
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
  cout << "POT " << RunCodeName << " :" << POT << "." << endl;

  return
}

void DST::SetHistograms() {
  TString histogramName;
  TString histogramTitle;

  histogramTitle = "Shower Energy - " + RunCodeName;
  histogramName = "shEn" + RunCodeName;
  shEn = new TH1D(histogramName, histogramTitle, NBins, EnergyRangeMinimum, EnergyRangeMinimum);

  histogramTitle = "Track Energy - " + RunCodeName;
  histogramName = "trEn" + RunCodeName;
  trEn = new TH1D(histogramName, histogramTitle, NBins, EnergyRangeMinimum, EnergyRangeMinimum);

  histogramTitle = "Charge Current Energy - " + RunCodeName;
  histogramName = "ccEn" + RunCodeName;
  ccEn = new TH1D(histogramName, histogramTitle, NBins, EnergyRangeMinimum, EnergyRangeMinimum);

  histogramTitle = "Neutral Current Energy - " + RunCodeName;
  histogramName = "ncEn" + RunCodeName;
  ncEn = new TH1D(histogramName, histogramTitle, NBins, EnergyRangeMinimum, EnergyRangeMinimum);

  for (int i = 0; i < NumberOfEvents; i++) {
    TreeChain->GetEntry(i);
    if (TreeChain->GetLeaf("selectionevent")->GetValue()) {
      shEn->Fill(TreeChain->GetLeaf("shwEnkNN")->GetValue());
      CCEn->Fill(TreeChain->GetLeaf("energyCC")->GetValue());
      NCEn->Fill(TreeChain->GetLeaf("energyNC")->GetValue());
      trEn->Fill(TreeChain->GetLeaf("trkEn")->GetValue());
    }
  }

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

TH1D* DST::GetHistogram(HistogramIndex histIndex) {
  if (histIndex == kShEn) {
    return shEn;
  } else if (histIndex == kTrEn) {
    return trEn;
  } else if (histIndex == kCCEn) {
    return ccEn;
  } else if (histIndex == kNCEn) {
    return ncEn;
  }
}
