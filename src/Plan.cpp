#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(new Settlement(settlement)),
      selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE),
      facilityOptions(facilityOptions),
      life_quality_score(0),
      economy_score(0),
      environment_score(0) {}

const int Plan::getlifeQualityScore() const
{
    return this->life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return this->economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return this->environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
}

void Plan::step(){
    while(underConstruction.size() < static_cast<int>(this->settlement->getType())+1){
        const FacilityType& newFacility = this->selectionPolicy->selectFacility(facilityOptions);
        underConstruction.push_back(new Facility(newFacility,settlement->getName()));
    }
    for(int i = 0 ; i < underConstruction.size() ; i++ ){
        if(underConstruction[i]->step() == FacilityStatus::OPERATIONAL){
            addFacility(underConstruction[i]);
            underConstruction.erase(underConstruction.begin()+i);
        }
    }
}

void Plan::printStatus(){
    std::cout << "PlanID: " + plan_id << std::endl;
    std::cout << "SettlementName: " + getSettlement()->getName() << std::endl;
    std::cout << "PlanStatus: ";
    if(this->status == PlanStatus::AVALIABLE) std::cout << "Available" << std::endl ;
    if(this->status == PlanStatus::BUSY) std::cout << "Busy" << std::endl ;
    std::cout << "SelectionPolicy: " + getSelectionPolicy()->toString() << std::endl;
    std::cout << "LifeQualityScore: " + getlifeQualityScore() << std::endl;
    std::cout << "EconomyScore: " + getEconomyScore() << std::endl;
    std::cout << "EnvironmentScore: " + getEnvironmentScore() << std::endl;
    for( int i = 0 ; i < facilities.size(); i++ ){
        std::cout <<"FacilityName: " + facilities[i]->getSettlementName() << std::endl;
        std::cout <<"FacilityStatus: OPERATIONAL" << std::endl;
    }
    for( int i = 0 ; i < underConstruction.size(); i++ ){
        std::cout <<"FacilityName: " + underConstruction[i]->getSettlementName() << std::endl;
        std::cout <<"FacilityStatus: UNDER_CONSTRUCTIONS" << std::endl;
    }
}



const vector<Facility *> &Plan::getFacilities() const
{
    return this->facilities;
}

const vector<Facility *> &Plan::getUnderConstruction() const
{
    return this->underConstruction;
}

const Settlement *Plan::getSettlement()
{
    return settlement;
}

SelectionPolicy *Plan::getSelectionPolicy()
{
    return selectionPolicy;
}

void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
    this->life_quality_score += facility->getLifeQualityScore();
    this->economy_score += facility->getEconomyScore();
    this->environment_score += facility->getEnvironmentScore();
}

int Plan::getPlanId()
{
    return plan_id;
}

const string Plan::toString() const
{
    return string();
}
