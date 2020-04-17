#include "D0AzimuthalCorAnalyzer.h"

#include <string>
#include <iostream>

int main(int argc, char *argv[]){

	if (argc < 3) {
		std::cerr << "Usage: inputFileList OutputFileName" << std::endl;
		return 0;
	}

	char *outFileName = argv[2];
	char *inFileList = argv[1];

	D0AzimuthalCorAnalyzer *mD0AnaMaker = new D0AzimuthalCorAnalyzer();
	mD0AnaMaker->setOutFileName(outFileName);
	mD0AnaMaker->setInFileName(inFileList);
	mD0AnaMaker->init();
	mD0AnaMaker->runAnalysis(100000);
	mD0AnaMaker->saveData();
	return 0;
}
