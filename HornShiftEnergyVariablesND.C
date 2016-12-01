HornShiftEnergyVariablesND() {
	gStyle->SetOptStat(0);
	
	TChain* TotalTimeTreeRun12 = new TChain("timeTree","timeTree");
	//TotalTimeTreeRun12->Add("/minos/data/users/tcarroll/DQM/Run12/CC2014NewCoilCut_February2015_M000200N00060*.root");
	TotalTimeTreeRun12->Add("/minos/app/dphan/HornShiftStudy/Run13VSRun13_OctMay/DataEarlyRun13/*.root");
	//TotalTimeTreeRun12->Add("/minos/data/users/tcarroll/DQM/Run12/CC2014NewCoilCut_February2015_M000200N0006089*.root");
	
	TChain* TotalTimeTreeRun13 = new TChain("timeTree","timeTree");
	//TotalTimeTreeRun13->Add("/minos/data/users/tcarroll/DQM/Run13/CC2014NewCoilCut_February2016_M000200N00063*.root");
	TotalTimeTreeRun13->Add("/minos/app/dphan/HornShiftStudy/Run13VSRun13_OctMay/DataLateRun13/*.root");
	//TotalTimeTreeRun13->Add("/minos/data/users/tcarroll/DQM/Run13/CC2014NewCoilCut_February2016_M000200N000638*.root");
	
	// Calculate the POT for the run
	//------------------------------------------------------------------------------------------------------------------------
	TH1D* hTOTALPOTRUN12;
	TH1D* hTOTALPOTRUN13;
	double POTRun12 = 0;
	double POTRun13 = 0;
	// Run 12
	TObjArray* fileElementsRun12 = TotalTimeTreeRun12->GetListOfFiles();
	TIter next(fileElementsRun12);
	TChainElement* chElRun12 = 0;
	string Run12String;
	int interupt = 1;
	while ( chElRun12 = (TChainElement*)next() ) {
		TFile* file = TFile::Open(chElRun12->GetTitle(), "read");
		//cout << chElRun12->GetTitle() << endl;
		if (interupt == 1) {
			Run12String = chElRun12->GetTitle();
			interupt = 0;
		}
		TH1D* htmpPOT = (TH1D*)file->Get("hTotalPot");
		POTRun12 += htmpPOT->Integral();
		file->Close();
		
	}
	// Run 13
	TObjArray* fileElementsRun13 = TotalTimeTreeRun13->GetListOfFiles();
	TIter next(fileElementsRun13);
	TChainElement* chElRun13 = 0;
	string Run13String;
	interupt = 1;
	while ( chElRun13 = (TChainElement*)next() ) {
		TFile* file = TFile::Open(chElRun13->GetTitle(), "read");
		if (interupt == 1) {
			Run13String = chElRun13->GetTitle();
			interupt = 0;
		}
		TH1D* htmpPOT = (TH1D*)file->Get("hTotalPot");
		POTRun13 += htmpPOT->Integral();
		file->Close();
	}
	// Run string parsing code (not essential for the code, ignore)
	//std::string s = "scott>=tiger>=mushroom";
	std::string delimiter = "_";

	size_t pos = 0;
	std::string token;
	while ((pos = Run12String.find(delimiter)) != std::string::npos) {
		token = Run12String.substr(0, pos);
		//std::cout << token << std::endl;
		Run12String.erase(0, pos + delimiter.length());
	}
	Run12String = token;
	token = Run12String.erase(3, Run12String.length()-5);
	Run12String = token;
	//std::cout << Run12String << std::endl;
	
	pos = 0;
	token;
	while ((pos = Run13String.find(delimiter)) != std::string::npos) {
		token = Run13String.substr(0, pos);
		//std::cout << token << std::endl;
		Run13String.erase(0, pos + delimiter.length());
	}
	Run13String = token;
	token = Run13String.erase(3, Run13String.length()-5);
	Run13String = token;
	//std::cout << Run13String << std::endl;

	cout << "POT " << Run12String << " :" << POTRun12 << endl;
	cout << "POT " << Run13String << " :" << POTRun13 << endl;
	

	// Initiate the needed energy histograms
	//------------------------------------------------------------------------------------------------------------------------
	int shEnNBins = 100;
	double shEnMinBin = 0;
	double shEnMaxBin = 30;
	int CCEnNBins = 100;
	double CCEnMinBin = 0;
	double CCEnMaxBin = 30;
	int NCEnNBins = 100;
	double NCEnMinBin = 0;
	double NCEnMaxBin = 30;
	int trEnNBins = 100;
	double trEnMinBin = 0;
	double trEnMaxBin = 30;
	
	// Run 12
	TString histname;
	histname = "Shower Energy of events in " + Run12String;
	TH1D* shEnRun12 = new TH1D("shEnRun12", histname, shEnNBins, shEnMinBin, shEnMaxBin);
	histname = "CC Energy of events in " + Run12String;
	TH1D* CCEnRun12 = new TH1D("CCEnRun12", histname, CCEnNBins, CCEnMinBin, CCEnMaxBin);
	histname = "NC Energy of events in " + Run12String;
	TH1D* NCEnRun12 = new TH1D("NCEnRun12", histname, NCEnNBins, NCEnMinBin, NCEnMaxBin);
	histname = "Track Energy of events in " + Run12String;
	TH1D* trEnRun12 = new TH1D("trEnRun12", histname, trEnNBins, trEnMinBin, trEnMaxBin);
	
	// Run 13
	histname = "Shower Energy of events in " + Run13String;
	TH1D* shEnRun13 = new TH1D("shEnRun13", histname, shEnNBins, shEnMinBin, shEnMaxBin);
	histname = "CC Energy of events in " + Run13String;
	TH1D* CCEnRun13 = new TH1D("CCEnRun13", histname, CCEnNBins, CCEnMinBin, CCEnMaxBin);
	histname = "NC Energy of events in " + Run13String;
	TH1D* NCEnRun13 = new TH1D("NCEnRun13", histname, NCEnNBins, NCEnMinBin, NCEnMaxBin);
	histname = "Track Energy of events in " + Run13String;
	TH1D* trEnRun13 = new TH1D("trEnRun13", histname, trEnNBins, trEnMinBin, trEnMaxBin);
	
	// Staring the loop over events
	long int NEventsRun12 = TotalTimeTreeRun12->GetEntries();
	long int NEventsRun13 = TotalTimeTreeRun13->GetEntries();
	cout << Run12String << " number of events: " << NEventsRun12 << endl;
	cout << Run13String << " number of events: " << NEventsRun13 << endl;

	
	double tmpshEn = 0.;
	double tmpCCEn = 0.;
	double tmpNCEn = 0.;
	double tmptrEn = 0.;
	bool   tmpSelection = true;
	
	// Run 12
	for (int i = 0; i < NEventsRun12; i++) {
		TotalTimeTreeRun12->GetEntry(i);
		
		tmpSelection = TotalTimeTreeRun12->GetLeaf("selectionevent")->GetValue();
		tmpshEn = TotalTimeTreeRun12->GetLeaf("shwEnkNN")->GetValue();
		tmpCCEn = TotalTimeTreeRun12->GetLeaf("energyCC")->GetValue();
		tmpNCEn = TotalTimeTreeRun12->GetLeaf("energyNC")->GetValue();
		tmptrEn = TotalTimeTreeRun12->GetLeaf("trkEn")->GetValue();
		
		
		if (tmpSelection) {
			//cout << "Run 12 event selected!" << endl;
			shEnRun12->Fill(tmpshEn);
			CCEnRun12->Fill(tmpCCEn);
	         	NCEnRun12->Fill(tmpNCEn);
			trEnRun12->Fill(tmptrEn);
		}
	}
	
	// Run 13
	for (int i = 0; i < NEventsRun13; i++) {
		TotalTimeTreeRun13->GetEntry(i);
		
		tmpSelection = TotalTimeTreeRun13->GetLeaf("selectionevent")->GetValue();
		tmpshEn = TotalTimeTreeRun13->GetLeaf("shwEnkNN")->GetValue();
		tmpCCEn = TotalTimeTreeRun13->GetLeaf("energyCC")->GetValue();
		tmpNCEn = TotalTimeTreeRun13->GetLeaf("energyNC")->GetValue();
		tmptrEn = TotalTimeTreeRun13->GetLeaf("trkEn")->GetValue();
		
		
		if (tmpSelection) {
			//cout << "Run 13 event selected!" << endl;
			shEnRun13->Fill(tmpshEn);
			CCEnRun13->Fill(tmpCCEn);
	         	NCEnRun13->Fill(tmpNCEn);
			trEnRun13->Fill(tmptrEn);
		}
	}
	
	
	
	
	

	// Plotting the rebinned histograms to compare the variables
	//------------------------------------------------------------------------------------------------------------------------
	// Tom's binning scheme
	
	const int bm = 50;
	double mybins[bm +1];

	for(int k=0; k<=40; k++){
		mybins[k] = k*0.25;
	}
	for(int k=41; k<=42; k++){
		mybins[k] = 10+0.5*(k-40);
	}
	for(int k=43; k<=43; k++){
		mybins[k] = 11+(k-42);
	}
	for(int k=44; k<=48; k++){
		mybins[k] = 12+2*(k-43);
	}  
	for(int k=49; k<=50; k++){
		mybins[k] = 22+4*(k-48);
	}
	
	
	// Tom's modified binning scheme
	/*
	const int bm = 30;
	double mybins[bm+1];

	for(int k=0; k<=20; k++){
		mybins[k] = k*0.5;
	}
	for(int k=21; k<=22; k++){
		mybins[k] = 10+0.5*(k-20);
	}
	for(int k=23; k<=23; k++){
		mybins[k] = 11+(k-22);
	}
	for(int k=24; k<=28; k++){
		mybins[k] = 12+2*(k-23);
	}  
	for(int k=29; k<=30; k++){
		mybins[k] = 22+4*(k-28);
	}
	for (int k = 0; k <=30; k++) {
		cout << mybins[k] << "\t";
	}
	cout << endl;
	*/
	// Saving to file
	TString outfilename = "Output" + Run12String + "vs" + Run13String + ".root";
	TFile* OutputFile = new TFile(outfilename, "RECREATE");
	
	//------------------------------------------------------------------------------------------------------------------------
	// Shower Energy 
	shEnRun12->Scale(1/(POTRun12*(1E-18)));
	shEnRun13->Scale(1/(POTRun13*(1E-18)));
	shEnRun12->Sumw2();
	shEnRun13->Sumw2();
	// Rebin
	shEnRun12->Rebin(bm, "shEnRun12Rebinned", mybins);
	histname = "shEn" + Run12String + "Rebinned";
	shEnRun12Rebinned->SetName(histname);
	shEnRun13->Rebin(bm, "shEnRun13Rebinned", mybins);
	histname = "shEn" + Run13String + "Rebinned";
	shEnRun13Rebinned->SetName(histname);	
	// Drawing 
	TCanvas* shEnCanvas = new TCanvas();
	histname = "Shower Energy Comparison (" + Run12String + " - " + Run13String + ")";
	shEnRun12Rebinned->SetTitle(histname);
	shEnRun12Rebinned->GetXaxis()->SetTitle("shEn [GeV]");
	shEnRun12Rebinned->GetYaxis()->SetTitle("Entries");
	shEnRun12Rebinned->SetLineColor(2);
	// Legend
	TLegend* shLeg = new TLegend(0.6, 0.6, 0.85, 0.85);
	TString theLegendRun12 = Run12String;
	shLeg->AddEntry(shEnRun12Rebinned, theLegendRun12, "l");
	TString theLegendRun13 = Run13String;
	shLeg->AddEntry(shEnRun13Rebinned, theLegendRun13, "l");
	shEnRun12Rebinned->Draw();
	shEnRun13Rebinned->Draw("same");
	shLeg->Draw();
	outfilename = "shEn" + Run12String + "vs" + Run13String + ".png";
	shEnCanvas->SaveAs(outfilename);
	shEnRun12Rebinned->Write();
	shEnRun13Rebinned->Write();
	// Shower Energy Ratio
	TH1D* shEnRatio12vs13 = (TH1D*) shEnRun13Rebinned->Clone();
	histname = "shEnRatio" + Run12String + "vs" + Run13String;
	shEnRatio12vs13->SetName(histname);
	shEnRatio12vs13->Divide(shEnRun12Rebinned);
	for(int i = 0; i < shEnRatio12vs13->GetNbinsX(); i++){
		double binerr = shEnRun13Rebinned->GetBinError(i)/shEnRun12Rebinned->GetBinContent(i) -
		(shEnRun13Rebinned->GetBinContent(i))*(shEnRun12Rebinned->GetBinError(i))/((shEnRun12Rebinned->GetBinContent(i))*(shEnRun12Rebinned->GetBinContent(i)));
        	shEnRatio12vs13->SetBinError(i, binerr);
	}
	TCanvas* shEnRatioCanvas = new TCanvas();
	histname = "Shower Energy Ratio (" + Run13String + "/" + Run12String + ")";
	shEnRatio12vs13->SetTitle(histname);
	shEnRatio12vs13->GetXaxis()->SetTitle("shEn [GeV]");
	shEnRatio12vs13->GetYaxis()->SetTitle("");
	TLine *shEnUnityLine = new TLine(shEnMinBin, 1, shEnMaxBin, 1);
	shEnRatio12vs13->Draw();
	shEnUnityLine->Draw("same");
	outfilename = "shEnRatio" + Run12String + "vs" + Run13String + ".png";
	shEnRatio12vs13->SaveAs(outfilename);
	shEnRatio12vs13->Write();
	
	//------------------------------------------------------------------------------------------------------------------------
	// Track Energy 
	trEnRun12->Scale(1/(POTRun12*(1E-18)));
	trEnRun13->Scale(1/(POTRun13*(1E-18)));
	trEnRun12->Sumw2();
	trEnRun13->Sumw2();
	// Rebin the histograms
	trEnRun12->Rebin(bm, "trEnRun12Rebinned", mybins);
	histname = "trEn" + Run12String + "Rebinned";
	trEnRun12Rebinned->SetName(histname);
	trEnRun13->Rebin(bm, "trEnRun13Rebinned", mybins);
	histname = "trEn" + Run13String + "Rebinned";
	trEnRun13Rebinned->SetName(histname);	
	// Drawing 
	TCanvas* trEnCanvas = new TCanvas();
	histname = "Track Energy Comparison (" + Run12String + " - " + Run13String + ")";
	trEnRun12Rebinned->SetTitle(histname);
	trEnRun12Rebinned->GetXaxis()->SetTitle("trEn [GeV]");
	trEnRun12Rebinned->GetYaxis()->SetTitle("Entries");
	trEnRun12Rebinned->SetLineColor(2);
	// Legend
	TLegend* trLeg = new TLegend(0.6, 0.6, 0.85, 0.85);
	trLeg->AddEntry(trEnRun12Rebinned, theLegendRun12, "l");
	trLeg->AddEntry(trEnRun13Rebinned, theLegendRun13, "l");
	trEnRun12Rebinned->Draw();
	trEnRun13Rebinned->Draw("same");
	trLeg->Draw();
	outfilename = "trEn" + Run12String + "vs" + Run13String + ".png";
	trEnCanvas->SaveAs(outfilename);
	trEnRun12Rebinned->Write();
	trEnRun13Rebinned->Write();

	// Track Energy Ratio
	TH1D* trEnRatio12vs13 = (TH1D*) trEnRun13Rebinned->Clone();
	histname = "trEnRatio" + Run12String + "vs" + Run13String;
	trEnRatio12vs13->SetName(histname);
	trEnRatio12vs13->Divide(trEnRun12Rebinned);
	for(int i = 0; i < trEnRatio12vs13->GetNbinsX(); i++){
		double binerr = trEnRun13Rebinned->GetBinError(i)/trEnRun12Rebinned->GetBinContent(i) -
		(trEnRun13Rebinned->GetBinContent(i))*(trEnRun12Rebinned->GetBinError(i))/((trEnRun12Rebinned->GetBinContent(i))*(trEnRun12Rebinned->GetBinContent(i)));
        	trEnRatio12vs13->SetBinError(i, binerr);
	}
	TCanvas* trEnRatioCanvas = new TCanvas();
	histname = "Track Energy Ratio (" + Run13String + "/" + Run12String;
	trEnRatio12vs13->SetTitle(histname);
	trEnRatio12vs13->GetXaxis()->SetTitle("trEn [GeV]");
	trEnRatio12vs13->GetYaxis()->SetTitle("");
	TLine *trEnUnityLine = new TLine(trEnMinBin, 1, trEnMaxBin, 1);
	trEnRatio12vs13->Draw();
	trEnUnityLine->Draw("same");
	outfilename = "trEnRatio" + Run12String + "vs" + Run13String + ".png";
	trEnRatio12vs13->SaveAs(outfilename);
	trEnRatio12vs13->Write();

	//------------------------------------------------------------------------------------------------------------------------
	// CC Energy 
	CCEnRun12->Scale(1/(POTRun12*(1E-18)));
	CCEnRun13->Scale(1/(POTRun13*(1E-18)));
	CCEnRun12->Sumw2();
	CCEnRun13->Sumw2();
	// Rebin the histograms

	CCEnRun12->Rebin(bm, "CCEnRun12Rebinned", mybins);
	histname = "CCEn" + Run12String + "Rebinned";
	CCEnRun12Rebinned->SetName(histname);
	CCEnRun13->Rebin(bm, "CCEnRun13Rebinned", mybins);
	histname = "CCEn" + Run13String + "Rebinned";
	CCEnRun13Rebinned->SetName(histname);	
	// Drawing 
	TCanvas* CCEnCanvas = new TCanvas();
	histname = "CC Energy Comparison (" + Run12String + " - " + Run13String + ")";
	CCEnRun12Rebinned->SetTitle(histname);
	CCEnRun12Rebinned->GetXaxis()->SetTitle("CCEn [GeV]");
	CCEnRun12Rebinned->GetYaxis()->SetTitle("Entries");
	CCEnRun12Rebinned->SetLineColor(2);
	// Legend
	TLegend* CCLeg = new TLegend(0.6, 0.6, 0.85, 0.85);
	CCLeg->AddEntry(CCEnRun12Rebinned, theLegendRun12, "l");
	CCLeg->AddEntry(CCEnRun13Rebinned, theLegendRun13, "l");
	CCEnRun12Rebinned->Draw();
	CCEnRun13Rebinned->Draw("same");
	CCLeg->Draw();
	outfilename = "CCEn" + Run12String + "vs" + Run13String + ".png";
	CCEnCanvas->SaveAs(outfilename);
	CCEnRun12Rebinned->Write();
	CCEnRun13Rebinned->Write();

	// CC Energy Ratio
	TH1D* CCEnRatio12vs13 = (TH1D*) CCEnRun13Rebinned->Clone();
	histname = "CCEnRatio" + Run12String + "vs" + Run13String;
	CCEnRatio12vs13->SetName(histname);
	CCEnRatio12vs13->Divide(CCEnRun12Rebinned);
	for(int i = 0; i < CCEnRatio12vs13->GetNbinsX(); i++){
		double binerr = CCEnRun13Rebinned->GetBinError(i)/CCEnRun12Rebinned->GetBinContent(i) - (CCEnRun13Rebinned->GetBinContent(i))*(CCEnRun12Rebinned->GetBinError(i))/((CCEnRun12Rebinned->GetBinContent(i))*(CCEnRun12Rebinned->GetBinContent(i)));
		CCEnRatio12vs13->SetBinError(i, binerr);
	}
	TCanvas* CCEnRatioCanvas = new TCanvas();
	histname = "CC Energy Ratio (" + Run13String + "/" + Run12String;
	CCEnRatio12vs13->SetTitle(histname);
	CCEnRatio12vs13->GetXaxis()->SetTitle("CCEn [GeV]");
	CCEnRatio12vs13->GetYaxis()->SetTitle("");
	TLine *CCEnUnityLine = new TLine(CCEnMinBin, 1, CCEnMaxBin, 1);
	CCEnRatio12vs13->Draw();
	CCEnUnityLine->Draw("same");
	outfilename = "CCEnRatio" + Run12String + "vs" + Run13String + ".png";
	CCEnRatio12vs13->SaveAs(outfilename);
	CCEnRatio12vs13->Write();
	
	//-------------------------------------------------------------------------------------------------------------------------
	// NC Energy 
	NCEnRun12->Scale(1/(POTRun12*(1E-18)));
	NCEnRun13->Scale(1/(POTRun13*(1E-18)));
	NCEnRun12->Sumw2();
	NCEnRun13->Sumw2();
	// Rebin the histograms

	NCEnRun12->Rebin(bm, "NCEnRun12Rebinned", mybins);
	histname = "NCEn" + Run12String + "Rebinned";
	NCEnRun12Rebinned->SetName(histname);
	NCEnRun13->Rebin(bm, "NCEnRun13Rebinned", mybins);
	histname = "NCEn" + Run13String + "Rebinned";
	NCEnRun13Rebinned->SetName(histname);	
	// Drawing 
	TCanvas* NCEnCanvas = new TCanvas();
	histname = "NC Energy Comparison (" + Run12String + " - " + Run13String + ")";
	NCEnRun12Rebinned->SetTitle(histname);
	NCEnRun12Rebinned->GetXaxis()->SetTitle("NCEn [GeV]");
	NCEnRun12Rebinned->GetYaxis()->SetTitle("Entries");
	NCEnRun12Rebinned->SetLineColor(2);
	// Legend
	TLegend* NCLeg = new TLegend(0.6, 0.6, 0.85, 0.85);
	NCLeg->AddEntry(NCEnRun12Rebinned, theLegendRun12, "l");
	NCLeg->AddEntry(NCEnRun13Rebinned, theLegendRun13, "l");
	NCEnRun12Rebinned->Draw();
	NCEnRun13Rebinned->Draw("same");
	NCLeg->Draw();
	outfilename = "NCEn" + Run12String + "vs" + Run13String + ".png";
	NCEnCanvas->SaveAs(outfilename);
	NCEnRun12Rebinned->Write();
	NCEnRun13Rebinned->Write();

	// NC Energy Ratio
	TH1D* NCEnRatio12vs13 = (TH1D*) NCEnRun13Rebinned->Clone();
	histname = "NCEnRatio" + Run12String + "vs" + Run13String;
	NCEnRatio12vs13->SetName(histname);
	NCEnRatio12vs13->Divide(NCEnRun12Rebinned);
	for(int i = 0; i < NCEnRatio12vs13->GetNbinsX(); i++){
		double binerr = NCEnRun13Rebinned->GetBinError(i)/NCEnRun12Rebinned->GetBinContent(i) - (NCEnRun13Rebinned->GetBinContent(i))*(NCEnRun12Rebinned->GetBinError(i))/((NCEnRun12Rebinned->GetBinContent(i))*(NCEnRun12Rebinned->GetBinContent(i)));
		NCEnRatio12vs13->SetBinError(i, binerr);
	}
	TCanvas* NCEnRatioCanvas = new TCanvas();
	histname = "NC Energy Ratio (" + Run13String + "/" + Run12String;
	NCEnRatio12vs13->SetTitle(histname);
	NCEnRatio12vs13->GetXaxis()->SetTitle("NCEn [GeV]");
	NCEnRatio12vs13->GetYaxis()->SetTitle("");
	TLine *NCEnUnityLine = new TLine(NCEnMinBin, 1, NCEnMaxBin, 1);
	NCEnRatio12vs13->Draw();
	NCEnUnityLine->Draw("same");
	outfilename = "NCEnRatio" + Run12String + "vs" + Run13String + ".png";
	NCEnRatio12vs13->SaveAs(outfilename);
	NCEnRatio12vs13->Write();
	
	// Save file
	OutputFile->Write();
	
	cout << "Job done." << endl;
	return;
}

