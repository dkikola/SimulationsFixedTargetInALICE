/*
 * CharmDecay.h
 *
 *  Created on: Apr 15, 2020
 *      Author: dkikola
 */

#ifndef CHARMDECAY_H_
#define CHARMDECAY_H_

#include "TVector3.h"
#include "TObject.h"
#include "TClonesArray.h"


class Part : public TObject{
public:
	Part();
	Part(short id, float pT, float eta, float phi, float yLab);
	~Part();

private:
	short id;	// particle PDG ID
	float pT;	// transverse momentum
	float eta;	// pseudorapidity
	float phi;	// azimuthal angle
	float yLab;	// rapidity (in the laboratory frame)

   ClassDef(Part,1)
};


class CharmDecay : public TObject{
public:
	CharmDecay();
	CharmDecay(const CharmDecay &);
	~CharmDecay();
	unsigned int getNDaughters();
	Part getDaughter(unsigned int);
	void addDaughter(Part p);
	void setDecayVrt(double x, double y, double z);
	void clear();

	const TVector3& getDecayVrt() const {
		return decayVrt;
	}

	float getEta() const {	return eta;	}
	void setEta(float eta) { this->eta = eta; }
	short getId() const { return id; }
	void setId(short id) { this->id = id; }
	short getIdParent() const { return idParent; }
	void setIdParent(short id) { this->idParent = id; }
    short getOrigin() const { return origin; }
    void setOrigin(short o) { this->origin=o; }
    bool getIsFD() const { return isFD; }
    void setIsFD(bool FD) { this->isFD = FD; }
	float getPhi() const {	return phi; }
	void setPhi(float phi) { this->phi = phi;}
	float getPt() const { return pT;	}
	void setPt(float t) { pT = t; }
	float getYLab() const { return yLab;	}
	void setYLab(float lab) { yLab = lab; }

private:
	short id;	// particle PDG ID
	short idParent; // PDG ID of the parent
    short origin; // quark/gluon origin
    bool isFD;  // if D meson from B decay
	float pT;	// transverse momentum
	float eta;	// pseudorapidity
	float phi;	// azimuthal angle
	float yLab;	// rapidity (in the laboratory frame)
	unsigned int nDaughters;
	TVector3 decayVrt;	// decay point of the charmed meson
	std::vector<Part> listOdDaughters;	// for now, I consider only: D0->Kpi, D+->K-pi+pi+ and D->mu

   ClassDef(CharmDecay,1)
};



#endif /* CHARMDECAY_H_ */
