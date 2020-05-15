/*
 * SimCharmEvents.h
 *
 *  Created on: Nov 8, 2019
 *      Author: dkikola
 */

#include <TString.h>
#include <TTree.h>
#include <TH1D.h>
#include <TFile.h>
#include <TObjArray.h>
#include <Pythia8/Pythia.h>
#include <Pythia8/Event.h>
#include "../CharmEvent/CharmEvent.h"

#include <vector>

#ifndef SIMCHARMEVENTS_H_
#define SIMCHARMEVENTS_H_

using namespace Pythia8;

class SimCharmEvents {
public:
	SimCharmEvents();
	virtual ~SimCharmEvents();

	void init();
	void saveData();
	void run(int N = 1e4);

	float getEnergy() const {
		return energy;
	}

	void setEnergy(float energy) {
		this->energy = energy;
	}

    bool getUseExternalPDF() const {
        return useExternalPDF;
    }

    void setUseExternalPDF(bool ispdf){
        this->useExternalPDF = ispdf;
    }

    bool getUseExternalConfFile() const {
        return useExternalConfFile;
    }

    void setUseExternalConfFile(bool isfile){
        this->useExternalConfFile = isfile;
    }

	float getMaxHadronEta() const {
		return maxHadronEta;
	}

	void setMaxHadronEta(float maxHadronEta) {
		this->maxHadronEta = maxHadronEta;
	}

	float getMinHadronEta() const {
		return minHadronEta;
	}

	void setMinHadronEta(float minHadronEta) {
		this->minHadronEta = minHadronEta;
	}

	float getMinCharmY() const {
		return minCharmY;
	}

	void setMinCharmY(float minCharmY) {
		this->minCharmY = minCharmY;
	}

	float getMaxCharmY() const {
		return maxCharmY;
	}

	void setMaxCharmY(float maxCharmY) {
		this->maxCharmY = maxCharmY;
	}


	float getMinPtHadron() const {
		return minPtHadron;
	}

	void setMinPtHadron(float minPtHadron) {
		this->minPtHadron = minPtHadron;
	}

	const TString& getOutFileName() const {
		return outFileName;
	}

	void setOutFileName(const TString& outFileName) {
		this->outFileName = outFileName;
	}

    const TString& getPythiaConfigFile() const {
        return pythiaConfigFile;
    }

    void setPythiaConfigFile(const TString& pythiaFileName) {
        this->pythiaConfigFile = pythiaFileName;
    }

	float getMaxMuonY() const {
		return maxMuonY;
	}

	void setMaxMuonY(float maxMuonY) {
		this->maxMuonY = maxMuonY;
	}

	float getMinMuonY() const {
		return minMuonY;
	}

	void setMinMuonY(float minMuonY) {
		this->minMuonY = minMuonY;
	}

private:
	Pythia pythia;

	TString outFileName;
	TFile* fOut;
    TString pythiaConfigFile;

	int nEvents;
	float energy;
    bool  useExternalPDF;
    bool  useExternalConfFile;
	float minPtHadron;
	float minCharmY; // lower acceptance limit in rapidity;
	float maxCharmY; // upper acceptance limit in rapidity;
	float minMuonY; // lower acceptance limit in rapidity;
	float maxMuonY; // upper acceptance limit in rapidity;
	float minHadronEta; // lower acceptance limit in pseudorapidity;
	float maxHadronEta; // upper acceptance limit in pseudorapidity;
	std::set<int> listOfHfMesons;	// list of PDG IDs of heavy-flavor mesons

	TTree* mCharmTree;
    CharmEvent	*mDmesonEvent;

	TH1D* hEventMult;
	TH1D* hDmesonEventMult;
	TH1D* dNdpT_Dmeson;
	TH1D* dNdpT_hadron;
	TH1D* dNdy_Dmeson;
	TH1D* dNdy_hadron;
    TH1D *hSigma;
    TH1D *hStat;
	bool passHadronCut(Particle& p);
	bool isDmesonDecay(Particle& p, CharmDecay& c);
	bool isHfMuon(Particle& p, CharmDecay& c);
	bool isDmeson(int id);
	void initHfHadronIds();
    short getOrigin(Particle& p);
    bool isFeedDown(Particle& p);

};

#endif /* SIMCHARMEVENTS_H_ */
