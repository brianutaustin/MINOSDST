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

void DST::CalculateBatchPOT() {
  BatchPOT = 0;
  int CurrentSnarlIndex = -1;
  int CurrentBatchIndex = -1;
  bool NewBatchFlag = false;
  for (int i = 0; i < NumberOfEvents; i++) {
    TreeChain->GetEntry(i);
    if (TreeChain->GetLeaf("snarl")->GetValue() != CurrentSnarlIndex) {
      CurrentSnarlIndex = TreeChain->GetLeaf("snarl")->GetValue();
      CurrentBatchIndex = -1;
    }
    if ((TreeChain->GetLeaf("whichBatch")->GetValue() >= UnslipstackedMinBatchIndex) && (TreeChain->GetLeaf("whichBatch")->GetValue() <= UnslipstackedMaxBatchIndex)) {
      if (TreeChain->GetLeaf("whichBatch")->GetValue() != CurrentBatchIndex) {
        NewBatchFlag = true;
        CurrentBatchIndex = TreeChain->GetLeaf("whichBatch")->GetValue();
      }
      if (NewBatchFlag) {
        // std::cout << "Event: " << i << std::endl;
        BatchPOT += TreeChain->GetLeaf("batchPot")->GetValue();
        NewBatchFlag = false;
      }
    }
  }

  return;
}

void DST::CalculateAllBatchPOT() {
  AllBatchPOT = 0;
  int CurrentSnarlIndex = -1;
  int CurrentBatchIndex = -1;
  bool NewBatchFlag = false;
  for (int i = 0; i < NumberOfEvents; i++) {
    TreeChain->GetEntry(i);
    if (TreeChain->GetLeaf("snarl")->GetValue() != CurrentSnarlIndex) {
      CurrentSnarlIndex = TreeChain->GetLeaf("snarl")->GetValue();
      CurrentBatchIndex = -1;
    }
    if ((TreeChain->GetLeaf("whichBatch")->GetValue() >= 0) && (TreeChain->GetLeaf("whichBatch")->GetValue() <= 6)) {
      if (TreeChain->GetLeaf("whichBatch")->GetValue() != CurrentBatchIndex) {
        NewBatchFlag = true;
        CurrentBatchIndex = TreeChain->GetLeaf("whichBatch")->GetValue();
      }
      if (NewBatchFlag) {
        // std::cout << "Event: " << i << std::endl;
        AllBatchPOT += TreeChain->GetLeaf("batchPot")->GetValue();
        NewBatchFlag = false;
      }
    }
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

  std::cout << "POT       " << RunCodeName << " :" << POT << "." << std::endl;
  std::cout << "Batch POT " << RunCodeName << " :" << BatchPOT << "." << std::endl;

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
    case kInEl: {
      variableName = "inElasticity";
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
  TH1D * dummyHistogram;
  if (histIndex != kInEl) {
    dummyHistogram = new TH1D(HistogramNameString.HistogramName, HistogramNameString.HistogramTitle, NBins, BinningScheme);
  } else if (histIndex == kInEl) {
    dummyHistogram = new TH1D(HistogramNameString.HistogramName, HistogramNameString.HistogramTitle, 10, 0, 1);
  }

  dummyHistogram->GetXaxis()->SetTitle(HistogramNameString.HistogramXAxisTitle);
  dummyHistogram->GetXaxis()->SetTitle(HistogramNameString.HistogramYAxisTitle);
  dummyHistogram->SetLineColor(histIndex + 1);

  for (int i = 0; i < NumberOfEvents; i++) {
    TreeChain->GetEntry(i);
    if (TreeChain->GetLeaf("selectionevent")->GetValue()) {
      if ((TreeChain->GetLeaf("whichBatch")->GetValue() >= UnslipstackedMinBatchIndex) && (TreeChain->GetLeaf("whichBatch")->GetValue() <= UnslipstackedMaxBatchIndex)) {
        if (histIndex != kInEl) {
          dummyHistogram->Fill(TreeChain->GetLeaf(HistogramNameString.TreeChainHistogramName)->GetValue());
        } else if (histIndex == kInEl) {
          double Enn = TreeChain->GetLeaf("energyCC")->GetValue();
          double Enl = TreeChain->GetLeaf("trkEn")->GetValue();
          dummyHistogram->Fill((Enn - Enl) / Enn);
        }
      }
    }
  }

  dummyHistogram->Sumw2();
  // Change Normalization Here
  //dummyHistogram->Scale(1 / dummyHistogram->Integral());
  std::cout << "POT ratio: " << BatchPOT / AllBatchPOT << std::endl;
  dummyHistogram->Scale(1 / (POT * (BatchPOT / AllBatchPOT)));


  for (int i = 1; i <= dummyHistogram->GetXaxis()->GetNbins(); i++) {
    dummyHistogram->SetBinContent(i, dummyHistogram->GetBinContent(i) / dummyHistogram->GetBinWidth(i));
  }
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
/* Tom's binning
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
 */

/* Dung's binning
 */
  for (int k = 0; k <= 10; k++) {
    BinningScheme[k] = k;
  }
  for (int k = 11; k <= 15; k++) {
    BinningScheme[k] = 10 + 2 * (k - 10);
  }
  for (int k = 16; k <= 17; k++) {
    BinningScheme[k] = 20 + 5 * (k - 15);
  }

  return;
}

TH1D * DST::GetHistogram(HistogramIndex histIndex) {
  return HistogramVector.at(histIndex);
}
