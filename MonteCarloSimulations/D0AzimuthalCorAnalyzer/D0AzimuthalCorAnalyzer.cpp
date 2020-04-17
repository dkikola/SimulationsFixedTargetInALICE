/*
 * D0AzimuthalCorAnalyzer.cpp
 *
 *  Created on: Mar 25, 2020
 *      Author: dkikola
 */
#include <iostream>
#include <stdio.h>
#include <fstream>

#include "D0AzimuthalCorAnalyzer.h"
#include "../CharmEvent/CharmDecay.h"

#include "TMath.h"

#define D0PDG_ID 421
#define D0barPDG_ID -421
#define DPLUS_PDG_ID 411
#define DMINUS_PDG_ID -411



D0AzimuthalCorAnalyzer::D0AzimuthalCorAnalyzer():mD0File(nullptr),mD0Tree(nullptr),	mDmeson(nullptr) {
	mD0Event = new CharmEvent();
	inputData = new TChain("DmesonsEventALICE_FT");

	hMult = new TH1F("hMult", "Event mult.;Mult; dN/dn_{ch}",1000,-0.5, 999.5);
	hDecayVz = new TH1F("hDecayVz", "Decay Vertex Z; V_{Z} [cm]; dN/dVz",100,-10., 10);
	hND0Pairs = new TH1F("hND0Pairs", "No. of D0 pairs in an event; # of D0 pairs; dN/dn(n_{D0 pairs})",100,0,100);

	int Nbins = 20;

	/* Histograms to store correlation in the azimuthal angle */
	hdNdDeltaPhiSigSig = new TH1F("hdNdDeltaPhiSigSig", "dN/d#Delta #phi Signal candidates;#Delta #phi;1/N dN/d#Delta #phi", Nbins,-TMath::PiOver2(), 3 * TMath::PiOver2());
	hdNdDeltaPhiSigSig->Sumw2();
	hdNdDeltaPhiBgBg  = (TH1F*)hdNdDeltaPhiSigSig->Clone("hdNdDeltaPhiBgBg");
	hdNdDeltaPhiBgBg->SetTitle("dN/d#Delta #phi BG-BG candidates");

	Nbins = 50;
	hdNdPhiSig = new TH1F("hdNdPhiSig", "dN/dphi Signal candidates;#phi;1/N dN/d#phi", Nbins,-TMath::Pi(),TMath::Pi());
	hdNdPhiBg = (TH1F*)hdNdPhiSig->Clone("hdNdPhiBg");
}

void D0AzimuthalCorAnalyzer::init() {

	//read data
	char* filename = new char[1000];
	//.. loop over file list
	std::ifstream fileStream(inFileListName.Data(),std::ifstream::in);
	while (fileStream >> filename) {
		std::cout << filename << " added" << std::endl;
		inputData->Add(filename);
	}

	inputData->SetBranchAddress("CharmEvent",&mD0Event);

}

void D0AzimuthalCorAnalyzer::runAnalysis(unsigned int NeventsMax) {

	unsigned int nEntries = inputData->GetEntries();
	if (NeventsMax > nEntries) NeventsMax = nEntries;

	for (unsigned int i = 0; i < NeventsMax; i++) {
		inputData->GetEvent(i);
		hND0Pairs->Fill(mD0Event->nDmesons());
		hMult->Fill(mD0Event->nChPart());

		// first, clear lists of Signal candidates
		idxD0Signal.clear();
		idxD0barSignal.clear();

		// loop over D0 candidates, fill  invariant mass histogram
		for (unsigned int i = 0; i < mD0Event->nDmesons(); ++i) {
			mDmeson = (CharmDecay*) mD0Event->charmMesonsArray()->At(i);

			hDecayVz->Fill(mDmeson->getDecayVrt().z());

			if (!passPairCuts()) continue;

			if ((mDmeson->getId() == D0PDG_ID) || (mDmeson->getId() == DPLUS_PDG_ID)) {
				idxD0Signal.push_back(i);
				hdNdPhiSig->Fill(mDmeson->getPhi());
			}

			if ((mDmeson->getId() == D0barPDG_ID) || (mDmeson->getId() == DMINUS_PDG_ID)) {
				idxD0barSignal.push_back(i);
				hdNdPhiSig->Fill(mDmeson->getPhi());
			}
		}

		//float pt = mDmeson->getPt(); - in the future, calculate correlations vs D-meson pT
		calcAzimuthalCorr();
	}
}

D0AzimuthalCorAnalyzer::~D0AzimuthalCorAnalyzer() {
	delete mD0Event;
	delete hMult;
	delete hDecayVz;
	delete hND0Pairs;
	delete hdNdDeltaPhiSigSig;
	delete hdNdDeltaPhiBgBg;
	delete hdNdPhiSig;
	delete hdNdPhiBg;
	delete mD0File;
	delete mD0Tree;
}

void D0AzimuthalCorAnalyzer::saveData(){
	TFile* fOut = new TFile(outFileName,"RECREATE");
	fOut->cd();
	hMult->Write();
	hDecayVz->Write();
	hND0Pairs->Write();
	hdNdDeltaPhiSigSig->Write();
	hdNdDeltaPhiBgBg->Write();
	hdNdPhiSig->Write();
	hdNdPhiBg->Write();
	fOut->Close();
}

void D0AzimuthalCorAnalyzer::setOutFileName(TString in) {
	outFileName = in;
}

void D0AzimuthalCorAnalyzer::setInFileName(TString in) {
	inFileListName = in;
}

void D0AzimuthalCorAnalyzer::calcAzimuthalCorr() {

	float dphi = 0.0;
	CharmDecay *firstCand = nullptr; // first candidate
	CharmDecay *secondCand = nullptr; // first candidate

	/*
	 * Todo:
	 * 1. Add efficiency correction
	 * 3. Include background in the study, which will increase statistical uncertainty
	 *
	 */

	// signal candidates pairs
	for (unsigned int i = 0; i < idxD0Signal.size(); ++i) {
		firstCand =  (CharmDecay*)mD0Event->charmMesonsArray()->At(idxD0Signal[i]);

		for (unsigned int j = 0; j < idxD0barSignal.size(); ++j) {
			secondCand =  (CharmDecay*)mD0Event->charmMesonsArray()->At(idxD0barSignal[j]);

			dphi = firstCand->getPhi() - secondCand->getPhi();
			hdNdDeltaPhiSigSig->Fill(foldDeltaPhi(dphi));
		}
	}

	// background - background pairs
	// Todo: implement background in the simulation, if need
}

float D0AzimuthalCorAnalyzer::foldDeltaPhi(float dPhi) {

	/* folding delta phi into -pi/2, 3/2pi range */
	if (dPhi >  1.5 * TMath::Pi()) { return dPhi -= TMath::TwoPi(); }
	if (dPhi < -0.5 * TMath::Pi()) { return dPhi += TMath::TwoPi(); }

	return dPhi;
}

bool D0AzimuthalCorAnalyzer::passPairCuts() {

	//Todo: apply cuts on the decay daughters, to mimic experimental studies
	return true;
}



