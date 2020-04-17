/*
 * CharmDecay.cpp
 *
 *  Created on: Apr 15, 2020
 *      Author: dkikola
 */

#include "CharmDecay.h"
#include <iostream>

CharmDecay::CharmDecay(): id(0), pT(0), eta(0), phi(0), yLab(0), nDaughters(0)
{
}



CharmDecay::CharmDecay(const CharmDecay &d){

	id = d.id;
	idParent = d.idParent;
	pT = d.pT;
	eta = d.eta;
	phi = d.phi;
	yLab = d.yLab;
	decayVrt.SetXYZ(d.decayVrt.x(),d.decayVrt.y(),d.decayVrt.z());
	nDaughters = d.nDaughters;

	for(Part t : d.listOdDaughters){
		listOdDaughters.push_back(Part(t));
	}
}

CharmDecay::~CharmDecay() {
}

unsigned int CharmDecay::getNDaughters(){
	//return listOdDaughters.size();
	return nDaughters;
}

Part CharmDecay::getDaughter(unsigned int idx){
	return listOdDaughters.at(idx);	// will through an exception if idx out of bounds
}

void CharmDecay::addDaughter(Part const t){
	listOdDaughters.push_back(Part(t));
	nDaughters++;
}

void CharmDecay::setDecayVrt(double x, double y, double z){
	decayVrt.SetXYZ(x,y,z);
}

Part::Part(short id, float pT, float eta, float phi, float yLab){
	this->id = id;
	this->pT = pT;
	this->eta = eta;
	this->phi = phi;
	this->yLab = yLab;
}

Part::Part(){
	this->id = 0;
	this->pT = 0;
	this->eta = 0;
	this->phi = 0;
	this->yLab = 0;
}

Part::~Part(){
}

