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

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}

void Plan::step()
{
}

void Plan::printStatus()
{
    if(this->status == PlanStatus::AVALIABLE) std::cout << "Available" ;
    if(this->status == PlanStatus::BUSY) std::cout << "Busy" ;
}

const vector<Facility *> &Plan::getFacilities() const
{
    return this->facilities;
}

void Plan::addFacility(Facility *facility)
{
    facilities.push_back(facility);
}

const string Plan::toString() const
{
    return string();
}
