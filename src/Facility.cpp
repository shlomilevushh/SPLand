#include "Facility.h"

FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : 
name(name) , category(category) , price(price) , lifeQuality_score(lifeQuality_score) , economy_score(economy_score) , environment_score(environment_score) {}

int FacilityType::getCost() const{
    return this->price;
}

int FacilityType::getLifeQualityScore() const{
    return this->lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const{
    return this->environment_score;
}

int FacilityType::getEconomyScore() const{
    return this->economy_score;
}

FacilityCategory FacilityType::getCategory() const{
    return FacilityCategory();
}

Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) :
FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score) ,
settlementName(settlementName) , status(status) , timeLeft(timeLeft) {}

Facility::Facility(FacilityType &type, const string &settlementName)
FacilityType(type.getName(),other.getCategory(),other.getCost(),other.getLifeQualityScore(),other.getEconomyScore(),other.getEnvironmentScore()) , settlementName(settlementName)  {}
FacilityType.
const string &Facility::getSettlementName() const
{
    return this->settlementName;
}

const int Facility::getTimeLeft() const
{
    return this->timeLeft;
}

FacilityStatus Facility::step()
{
    return FacilityStatus();
}

void Facility::setStatus(FacilityStatus status)
{
    this->status = status;
}

const FacilityStatus &Facility::getStatus() const
{
    return this->status;
}

const string Facility::toString() const
{
    return string();
}
