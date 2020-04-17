/*
 * D0AzimuthalCorAnalyzer.h
 *
 *  Created on: Mar 25, 2020
 *      Author: dkikola
 */

#ifndef D0AZIMUTHALCORANALYZER_H_
#define D0AZIMUTHALCORANALYZER_H_

#include "TH1F.h"
#include "TH2.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TString.h"
#include "TChain.h"
#include <vector>

#include "../CharmEvent/CharmEvent.h"
#include "../CharmEvent/CharmDecay.h"


class D0AzimuthalCorAnalyzer {
public:
	D0AzimuthalCorAnalyzer();
	~D0AzimuthalCorAnalyzer();
	void setOutFileName(TString in);
	void setInFileName(TString in);
	void saveData();
	void runAnalysis (unsigned int);
	void calcAzimuthalCorr();
	void init();

	bool passPairCuts();

private:
	TChain* inputData;
	TFile* mD0File;
	TTree* mD0Tree;
	CharmDecay* mDmeson;
	CharmEvent *mD0Event;

	TH1F* hMult;
	TH1F* hDecayVz;
	TH1F* hND0Pairs;

	TH1F* hdNdDeltaPhiSigSig;
	TH1F* hdNdDeltaPhiBgBg;

	TH1F* hdNdPhiSig;
	TH1F* hdNdPhiBg;

	TString inFileListName;
	TString outFileName;

	std::vector<unsigned int> idxD0Signal;
	std::vector<unsigned int> idxD0barSignal;

	float foldDeltaPhi(float dphi);
};


#endif /* D0AZIMUTHALCORANALYZER_H_ */
