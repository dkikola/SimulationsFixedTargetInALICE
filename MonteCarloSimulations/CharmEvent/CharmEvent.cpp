#include "CharmEvent.h"

#include <iostream>

CharmEvent::CharmEvent() {
	mDmesonArray = new TClonesArray("CharmDecay",10);
	mNDmesons = 0;
	mNChPart = 0;
}

void CharmEvent::clear() {
	mDmesonArray->Clear("C");
	mNDmesons = 0;
	mNChPart = 0;
}

void CharmEvent::AddCharmMesonInfo(CharmDecay const& t)
{
   new((*mDmesonArray)[mNDmesons++]) CharmDecay(t);
}
