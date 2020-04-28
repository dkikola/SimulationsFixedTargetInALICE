/*
 * main.cpp
 *
 *  Created on: Nov 8, 2019
 *      Author: dkikola
 */
#include "SimCharmEvents.h"

int main(int argc, char *argv[]){

    bool useConfFile = false; 

    if(argc==3) useConfFile = false;
    else useConfFile = atoi(argv[3]);

    if (!useConfFile && argc < 3) {
        std::cerr << "Usage: nEvents OutputFileName" << std::endl;
        return 0;
    }
    else if(useConfFile && argc < 5) {
        std::cerr << "Usage: nEvents OutputFileName (bool)usePythiaConfig PythiaConfFile" << std::endl;
        return 0;
    }


	char*  outFileName =  argv[2];
	int nEvents = atoi(argv[1]);

	SimCharmEvents simulations;
	simulations.setOutFileName(outFileName);

    simulations.setUseExternalConfFile(useConfFile);
    if(useConfFile) {
        simulations.setPythiaConfigFile(argv[4]);
    }
    else {
        simulations.setEnergy(8000.0);
        simulations.setUseExternalPDF(true);                           // if to use external LHAPDF package
    }

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


