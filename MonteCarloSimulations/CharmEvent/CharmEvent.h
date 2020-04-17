#ifndef Event__h
#define Event__h


#include <cstddef>

#include "TObject.h"
#include "TClonesArray.h"

#include "CharmDecay.h"

class CharmEvent: public TObject
{
public:
   CharmEvent();
   ~CharmEvent(){ clear();}
   void clear();
   void AddCharmMesonInfo(CharmDecay const&);

   unsigned int nDmesons();
   unsigned int nChPart();
   void setNChPart(unsigned int n);

   TClonesArray const* charmMesonsArray()   const;

private:
   unsigned int	mNDmesons;		// number of stored charmed meson decays
   unsigned int mNChPart;
   TClonesArray* mDmesonArray;

   ClassDef(CharmEvent,1)
};

inline TClonesArray const* CharmEvent::charmMesonsArray()   const { return mDmesonArray;}
inline unsigned int CharmEvent::nDmesons() { return mNDmesons;}
inline unsigned int CharmEvent::nChPart() { return mNChPart;}
inline void CharmEvent::setNChPart(unsigned int n) { mNChPart = n;}

#endif
