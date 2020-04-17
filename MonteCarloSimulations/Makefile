CXX = g++
CXXFLAGS = -g -Wall `pythia8-config --cxxflags` `root-config --cflags` 
LFLAGS =  `pythia8-config --libs` `root-config --ldflags --glibs` 

SRC_DATA_GEN = $(wildcard DataGeneration/*.cpp) \
		 $(wildcard CharmEvent/*.cpp) 

SRC_CORR_ANA = $(wildcard D0AzimuthalCorAnalyzer/*.cpp) \
		 $(wildcard CharmEvent/*.cpp) 

HEADERS = $(wildcard CharmEvent/*.h)

OBJS_DATA_GEN = $(SRC_DATA_GEN:.cpp=.o)

OBJS_CORR_ANA = $(SRC_CORR_ANA:.cpp=.o)


all: runPythiaDmesonSimALICEFT runDmesonCorrAnalysisALICEFT

MyDict.cxx: $(HEADERS) Linkdef.h
	rootcint -f $@ $^ 

runPythiaDmesonSimALICEFT: MyDict.cxx $(OBJS_DATA_GEN)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)

runDmesonCorrAnalysisALICEFT: MyDict.cxx $(OBJS_CORR_ANA)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LFLAGS)


clean:
	rm -f *.o DataGeneration/*.o CharmEvent/*o D0AzimuthalCorAnalyzer/*o runPythiaDmesonSimALICEFT runDmesonCorrAnalysisALICEFT

.PHONY: all clean
