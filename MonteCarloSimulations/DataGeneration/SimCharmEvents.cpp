/*
 * SimCharmEvents.cpp
 *
 *  Created on: Nov 8, 2019
 *      Author: dkikola
 */

#include "SimCharmEvents.h"

#include <Pythia8/Event.h>
#include <TNtuple.h>
#include <TH1D.h>
#include <TFile.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TRandom3.h>


#include <stdlib.h>

SimCharmEvents::SimCharmEvents() {

	hEventMult  = new TH1D("hEventMult","charged track multiplicity; N_{ch};dN/N_{ch}", 1000, -0.5, 999.5);
	hDmesonEventMult = (TH1D*)hEventMult->Clone("hDmesonEventMult");

	dNdpT_hadron = new TH1D("dNdpT_hadron","p_{T} spectrum; p_{T} [GeV/c];dN/dp_{T}",100 ,0.,20.);
	dNdpT_Dmeson = (TH1D*)dNdpT_hadron->Clone("dNdpT_Dmeson");

	dNdy_Dmeson = new TH1D("dNdy_Dmeson","y_{Lab} spectrum; y;dN/dy",100 ,-5.,15.);
	dNdy_hadron = (TH1D*)dNdy_Dmeson->Clone("dNdy_hadron");


	minPtHadron = 0.2;// GeV/c
    minCharmY = -1.0;
	maxCharmY = 1.0; // lower acceptance in rapidity;
	minHadronEta = -1.0; // lower acceptance edge in pseudorapidity;
	maxHadronEta = 1.0;

	energy = 8000.0;	// Beam energy, in GeV
}

SimCharmEvents::~SimCharmEvents() {

	//delete mCharmTree;
	delete hEventMult;
	delete dNdpT_Dmeson;
	delete dNdpT_hadron;
	delete dNdy_Dmeson;
	delete dNdy_hadron;
	delete mDmesonEvent;

}

void SimCharmEvents::init() {

	/* Initialize list of HF mesons */
	initHfHadronIds();

	// set fixed-target collision mode
	/*
	 * From http://home.thep.lu.se/~torbjorn/pythia82html/BeamParameters.html:
	 * option 2 : the beams are back-to-back, but with different energies,
	 * see Beams:eA and Beams:eB below.
	 * This option could also be used for fixed-target configurations.
	 */

	pythia.readString("Beams:frameType = 2");

	TString strBeamCollEenergy;
	strBeamCollEenergy.Form("Beams:eA = %f",energy);
	pythia.readString(strBeamCollEenergy.Data());

	pythia.readString("Beams:eB = 0.");

	pythia.readString("Random:setSeed = on");
	pythia.readString("Random:seed = 0");

	pythia.readString("HardQCD:all = on");
	pythia.readString("Charmonium:all = on");

	/* following settings based on https://www.star.bnl.gov/protected/heavy/ullrich/pythia8/star_hf_tune_v1.1.cmnd" */
	/* Scales (Ramona's suggestions),
		This sets the scale to settings typically for hard probes:
		mu_F = mu_R = 2*mT
	 */
	pythia.readString("SigmaProcess:renormScale2 = 3");
	pythia.readString("SigmaProcess:factorScale2 = 3");
	pythia.readString("SigmaProcess:renormMultFac = 2");   // ! 2mT
	pythia.readString("SigmaProcess:factorMultFac = 2");   // ! 2mT

	pythia.init();

	// a tree to store information
    mDmesonEvent = new CharmEvent();
	fOut = new TFile(outFileName,"RECREATE");

    mCharmTree = new TTree("DmesonsEventALICE_FT", "Tree with PYTHIA events with D-meson decays");
    mCharmTree->SetAutoSave(1000000); // autosave every 1 Mbytes
    mCharmTree->Branch("CharmEvent","CharmEvent",mDmesonEvent);
}

void SimCharmEvents::saveData() {

	/* write basic simulation info for a cross-check in a future */

	TString simInfo;

	simInfo.Form("p beam E =  %1.1f GeV, pT_hadron > %1.2f GeV/c, %1.2f < eta_hadron < %1.2f, %1.2f < y_D0 < %1.2f",energy,minPtHadron,minHadronEta,
			maxHadronEta, minCharmY,maxCharmY);

	TObjString info(simInfo);

	fOut->WriteObject(&info,"SimulationInformation");

	mCharmTree->Write();
	hEventMult->Write();
	dNdpT_Dmeson->Write();
	dNdpT_hadron->Write();
	dNdy_Dmeson->Write();
	dNdy_hadron->Write();
	fOut->Close();

}

void SimCharmEvents::run(int N) {

	Particle p;

	for (int iEvent = 0; iEvent < N; ++iEvent) {
		if (!pythia.next())
			continue;

		mDmesonEvent->clear();
		CharmDecay charmedMesonDecay;

		bool isThereDmeson = false;
		int nCharged = 0;

		for (int i = 0; i < pythia.event.size(); ++i){
			p = pythia.event[i];

			/* for all charged hadrons: fill basic histograms */
		    if (p.isFinal() && p.isCharged()){
		    	dNdy_hadron->Fill(p.y());
		    	dNdpT_hadron->Fill(p.pT());
		    	++nCharged;
		    }

		    /*
		     *  The approach:
		     *  a) find D0 and D+/- mesons
		     *  b) check, if it decays as required
		     *  c) save data to an D-meson event
		     */

			if(isDmeson(p.id())) {

				dNdpT_Dmeson->Fill(p.pT());
				dNdy_Dmeson->Fill(p.y());

				/* apply rapidity cuts for D mesons */
				if(p.y()<minCharmY)continue;
				if(p.y()>maxCharmY)continue;


				if(isDmesonDecay(p,charmedMesonDecay)==true){
					mDmesonEvent->AddCharmMesonInfo(charmedMesonDecay);
					isThereDmeson = true;
				}
			} else {

				/* look for muons from HF-hadron decays */
				if(p.y()<minMuonY)continue;
				if(p.y()>maxMuonY)continue;

				if(isHfMuon(p,charmedMesonDecay)==true){
					mDmesonEvent->AddCharmMesonInfo(charmedMesonDecay);
					isThereDmeson = true;
				}
			}
		}

	    mDmesonEvent->setNChPart(nCharged);
		hEventMult->Fill(nCharged);
		if(isThereDmeson){
			hDmesonEventMult->Fill(nCharged);
			mCharmTree->Fill();
		}

		// End of event loop. Statistics. Histogram. Done.
	}
	pythia.stat();


}

bool SimCharmEvents::passHadronCut(Particle& p) {

	if(p.isFinal() && p.pT()> minPtHadron
			&& p.eta() > minHadronEta
			&& p.eta() < maxHadronEta
			&& p.charge() > 0)
		return true;
	else
		return false;
}

bool SimCharmEvents::isDmesonDecay(Particle& p, CharmDecay& c){

	unsigned pID = p.idAbs();

	Particle k;
	Particle pi1;
	Particle d;

	/*
	 * In the code below, no daughter kinematic cuts were applied
	 * We could add 'passHadronCut' for each daughter to save disk space.
	 *
	 * */

	if(pID == 421){
		// store D^0-> Kpi decay
		//is a two-body decay?
		if(p.daughterList().size()!=2) return false;

		int dauthter1Id = pythia.event[p.daughterList()[0]].idAbs();
		int dauthter2Id = pythia.event[p.daughterList()[1]].idAbs();

		// with daughter kinematic cuts:
		/*
		if(!passHadronCut(pythia.event[p.daughterList()[0]])) return false;
		if(!passHadronCut(pythia.event[p.daughterList()[1]])) return false;
		*/

		if((dauthter1Id == 321 && dauthter2Id == 211)
			|| (dauthter1Id == 211 && dauthter2Id == 321)){

			// fill information about D-meson and its decay products
			c.setId(p.id());
			c.setDecayVrt(p.xDec(),p.yDec(), p.zDec());	// decay point, in [mm]
			c.setIdParent( pythia.event[p.mother1()].id()); // PDG Id of mother
			c.setEta(p.eta());
			c.setPhi(p.phi());
			c.setPt(p.pT());
			c.setYLab(p.y());

			/* here I ordered daughters, K is first, but we could skip it*/
			if(dauthter1Id == 321){
				k = pythia.event[p.daughterList()[0]];	// get the first daughter
				pi1 = pythia.event[p.daughterList()[1]]; // get the second daughter
			} else {
				pi1 = pythia.event[p.daughterList()[0]];
				k = pythia.event[p.daughterList()[1]];
			}

			Part kaon(k.id(),k.pT(),k.eta(),k.phi(),k.y());
			Part pion(pi1.id(),pi1.pT(),pi1.eta(),pi1.phi(),pi1.y());

			c.addDaughter(kaon);
			c.addDaughter(pion);

			return true;
		}
	} else if(pID == 411){
		// store D^{+/-}-> Kpipi decay
		//is a three-body decay?
		if(p.daughterList().size()!=3) return false;

		int nPion = 0;
		int nKaon = 0;

		int pdgId;

		for(unsigned int i=0;i<p.daughterList().size();++i){

			// with daughter kinematic cuts:
			/*
			if(!passHadronCut(pythia.event[p.daughterList()[i]])) return false;
			*/

			pdgId = pythia.event[p.daughterList()[i]].idAbs();

			if(pdgId == 321)nKaon++;
			if(pdgId == 211)nPion++;
		}

		// is the correct decay?
		if(nPion == 2 && nKaon == 1){

			c.setId(p.id());
			c.setDecayVrt(p.xDec(),p.yDec(), p.zDec());	// decay point, in [mm]
			c.setIdParent( pythia.event[p.mother1()].id()); // PDG Id of mother
			c.setEta(p.eta());
			c.setPhi(p.phi());
			c.setPt(p.pT());
			c.setYLab(p.y());

			for(int i=0;i<3;++i){
					d = pythia.event[p.daughterList()[i]];
					Part part(d.id(),d.pT(),d.eta(),d.phi(),d.y());
					c.addDaughter(part);
				}
			return true;
			}
		}

	return false;
}

bool SimCharmEvents::isHfMuon(Particle &mu, CharmDecay &c) {

	unsigned pID = mu.idAbs();

	// skip if it's not muon
	if (pID != 13) return false;

	/* check if the muon is from the D or B hadron decay
	 * based on  pythia.event[mu.mother1()].id()
	 */

	unsigned parentID = pythia.event[mu.mother1()].idAbs();

	std::set<int>::iterator it;
	it = listOfHfMesons.find(parentID);

	if(it != listOfHfMesons.end()){	// if parent PDG id on the list, then save information

		Particle parent = pythia.event[mu.mother1()];

		// fill information about Heavy Flavor meson and its decay product
		c.setDecayVrt(parent.xDec(), parent.yDec(), parent.zDec());	// parent decay point, in [mm]
		c.setId(parent.id()); // PDG Id of the parent HF meson
		c.setIdParent(pythia.event[parent.mother1()].id()); // PDG Id of the grandparent
		c.setEta(parent.eta());
		c.setPhi(parent.phi());
		c.setPt(parent.pT());
		c.setYLab(parent.y());

		Part muonHF(mu.id(), mu.pT(), mu.eta(), mu.phi(), mu.y());
		c.addDaughter(muonHF);

		return true;

	}

	return false;
}

bool SimCharmEvents::isDmeson(int id){

	unsigned pID = TMath::Abs(id);

	/* 411: D+/-; 421:  D^0 */
	if((pID == 411) || (pID == 421)) return true;
	else return false;
}

void SimCharmEvents::initHfHadronIds(){

	const int Ncharm = 18;
	const int Nbottom = 24;

	int cMesonPGD_ID[Ncharm] = { 411, 421, 10411, 10421, 413, 423, 10413,
			10423, 20413, 20423, 415, 425, 431, 10431, 433, 10433, 20433, 435 };
	int bMesonPGD_ID[Nbottom] = { 511, 521, 10511, 10521, 513, 523,
			10513, 10523, 20513, 20523, 515, 525, 531, 10531, 533, 10533, 20533,
			535, 541, 10541, 543, 10543, 20543, 545 };

	for (int i = 0; i < Ncharm; i++)
		listOfHfMesons.insert(cMesonPGD_ID[i]);

	for (int i = 0; i < Nbottom; i++)
		listOfHfMesons.insert(bMesonPGD_ID[i]);

}

