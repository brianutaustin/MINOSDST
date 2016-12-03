#include "DST.h"
#include "Analysis.h"

#include <iostream>

int main() {
  DST * DST_Run12 = new DST("/minos/app/dphan/HornShiftStudy/WithBatch/MINOSDST/Run12/");
  DST_Run12->OpenDSTs("timeTree");
  DST_Run12->SetBinningScheme();
  DST_Run12->SetNumberOfEvents(-1);
  DST_Run12->SetUnslipstackedBatches(4, 5);
  DST_Run12->CalculatePOT();
  DST_Run12->CodeNameParsing();
  DST_Run12->SetHistograms(kShEn);
  DST_Run12->SetHistograms(kCCEn);
  DST_Run12->SetHistograms(kNCEn);
  DST_Run12->SetHistograms(kTrEn);

  DST * DST_Run13_1 = new DST("/minos/app/dphan/HornShiftStudy/WithBatch/MINOSDST/Run13_Oct15/");
  DST_Run13_1->OpenDSTs("timeTree");
  DST_Run13_1->SetBinningScheme();
  DST_Run13_1->SetNumberOfEvents(-1);
  DST_Run13_1->SetUnslipstackedBatches(4, 5);
  DST_Run13_1->CalculatePOT();
  DST_Run13_1->CodeNameParsing();
  DST_Run13_1->SetHistograms(kShEn);
  DST_Run13_1->SetHistograms(kCCEn);
  DST_Run13_1->SetHistograms(kNCEn);
  DST_Run13_1->SetHistograms(kTrEn);


  DST * DST_Run13_2 = new DST("/minos/app/dphan/HornShiftStudy/WithBatch/MINOSDST/Run13_Dec15/");
  DST_Run13_2->OpenDSTs("timeTree");
  DST_Run13_2->SetBinningScheme();
  DST_Run13_2->SetNumberOfEvents(-1);
  DST_Run13_2->SetUnslipstackedBatches(4, 5);
  DST_Run13_2->CalculatePOT();
  DST_Run13_2->CodeNameParsing();
  DST_Run13_2->SetHistograms(kShEn);
  DST_Run13_2->SetHistograms(kCCEn);
  DST_Run13_2->SetHistograms(kNCEn);
  DST_Run13_2->SetHistograms(kTrEn);

  DST * DST_Run13_3 = new DST("/minos/app/dphan/HornShiftStudy/WithBatch/MINOSDST/Run13_Mar16/");
  DST_Run13_3->OpenDSTs("timeTree");
  DST_Run13_3->SetBinningScheme();
  DST_Run13_3->SetNumberOfEvents(-1);
  DST_Run13_3->SetUnslipstackedBatches(4, 5);
  DST_Run13_3->SetHistograms(kShEn);
  DST_Run13_3->SetHistograms(kCCEn);
  DST_Run13_3->SetHistograms(kNCEn);
  DST_Run13_3->SetHistograms(kTrEn);

  DST * DST_Run13_4 = new DST("/minos/app/dphan/HornShiftStudy/WithBatch/MINOSDST/Run13_May16/");
  DST_Run13_4->OpenDSTs("timeTree");
  DST_Run13_4->SetBinningScheme();
  DST_Run13_4->SetNumberOfEvents(-1);
  DST_Run13_4->SetUnslipstackedBatches(4, 5);
  DST_Run13_4->SetHistograms(kShEn);
  DST_Run13_4->SetHistograms(kCCEn);
  DST_Run13_4->SetHistograms(kNCEn);
  DST_Run13_4->SetHistograms(kTrEn);

  Analysis * shENAna = new Analysis(kShEn);
  shENAna->SetImageFileFormat("eps");
  shENAna->ImportBaseDST(DST_Run12);
  shENAna->ImportCompareDSTs(DST_Run13_1);
  shENAna->ImportCompareDSTs(DST_Run13_2);
  shENAna->ImportCompareDSTs(DST_Run13_3);
  shENAna->ImportCompareDSTs(DST_Run13_4);
  shENAna->GetPlotString();
  shENAna->DrawWithLogScale(true);
  shENAna->DrawHistogram();
  shENAna->DrawRatioHistogram();


  Analysis * ccENAna = new Analysis(kCCEn);
  ccENAna->SetImageFileFormat("eps");
  ccENAna->ImportBaseDST(DST_Run12);
  ccENAna->ImportCompareDSTs(DST_Run13_1);
  ccENAna->ImportCompareDSTs(DST_Run13_2);
  ccENAna->ImportCompareDSTs(DST_Run13_3);
  ccENAna->ImportCompareDSTs(DST_Run13_4);
  ccENAna->GetPlotString();
  ccENAna->DrawWithLogScale(true);
  ccENAna->DrawHistogram();
  ccENAna->DrawRatioHistogram();

  Analysis * ncENAna = new Analysis(kNCEn);
  ncENAna->SetImageFileFormat("eps");
  ncENAna->ImportBaseDST(DST_Run12);
  ncENAna->ImportCompareDSTs(DST_Run13_1);
  ncENAna->ImportCompareDSTs(DST_Run13_2);
  ncENAna->ImportCompareDSTs(DST_Run13_3);
  ncENAna->ImportCompareDSTs(DST_Run13_4);
  ncENAna->GetPlotString();
  ncENAna->DrawWithLogScale(true);
  ncENAna->DrawHistogram();
  ncENAna->DrawRatioHistogram();

  Analysis * trENAna = new Analysis(kTrEn);
  trENAna->SetImageFileFormat("eps");
  trENAna->ImportBaseDST(DST_Run12);
  trENAna->ImportCompareDSTs(DST_Run13_1);
  trENAna->ImportCompareDSTs(DST_Run13_2);
  trENAna->ImportCompareDSTs(DST_Run13_3);
  trENAna->ImportCompareDSTs(DST_Run13_4);
  trENAna->GetPlotString();
  trENAna->DrawWithLogScale(true);
  trENAna->DrawHistogram();
  trENAna->DrawRatioHistogram();

  return 0;
}
