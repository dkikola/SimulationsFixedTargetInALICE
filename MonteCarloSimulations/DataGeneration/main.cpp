/*
 * main.cpp
 *
 *  Created on: Nov 8, 2019
 *      Author: dkikola
 */
#include "SimCharmEvents.h"

int main(int argc, char *argv[]){

	if (argc < 3) {
		std::cerr << "Usage: nEvents OutputFileName" << std::endl;
		return 0;
	}

	char*  outFileName =  argv[2];
	int nEvents = atoi(argv[1]);

	SimCharmEvents simulations;
	simulations.setOutFileName(outFileName);

	// LHCb settings
	simulations.setEnergy(8000.0);
	simulations.setMinPtHadron(0.15);
	simulations.setMinHadronEta(1.3);
	simulations.setMaxHadronEta(2.5);
	simulations.setMinCharmY(1.3);
	simulations.setMaxCharmY(2.5);

	// Muon rapidity cuts set arbitrary, just for a test.
	// We need to re-evaluate them
	simulations.setMinMuonY(3.);
	simulations.setMaxMuonY(8.);


	simulations.init();
	simulations.run(nEvents);
	simulations.saveData();
	return 0;
}


