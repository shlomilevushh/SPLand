#include "Plan.h"
#include <iostream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId),
      settlement(&settlement),
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


void Plan::step()
{
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
}

int Plan::getPlanId()
{
    return plan_id;
}

const string Plan::toString() const
{
    return string();
}
