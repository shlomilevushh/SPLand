#include "SelectionPolicy.h"


NaiveSelection::NaiveSelection()
{
    this->lastSelectedIndex = -1;
}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex++;
    if(lastSelectedIndex == facilitiesOptions.size()) lastSelectedIndex == 0;
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return string();
}

NaiveSelection *NaiveSelection::clone() const
{
    NaiveSelection* ret = new NaiveSelection();
    ret->lastSelectedIndex = this->lastSelectedIndex;
    return ret;
}



BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore): LifeQualityScore(LifeQualityScore),
 EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int distance = 0;
    int retIndex = 0;
    int tempEconScore = 0;
    int tempEnvScore = 0;
    int tempLifeScore = 0;
    int max = 0;
    int min = 0;
    for(int i = 0; i < facilitiesOptions.size(); i++){
        tempEconScore = this->EconomyScore + facilitiesOptions[i].getEconomyScore();
        tempEnvScore = this->EnvironmentScore + facilitiesOptions[i].getEnvironmentScore();
        tempLifeScore = this->LifeQualityScore + facilitiesOptions[i].getLifeQualityScore();
        max = std::max(std::max(tempEconScore, tempEnvScore), tempLifeScore);
        min = std::min(std::min(tempEconScore, tempEnvScore), tempLifeScore);
        if((max - min) < distance){
            retIndex = i;
            distance = max - min;
        }
    }
    return facilitiesOptions[retIndex];
}

const string BalancedSelection::toString() const
{
    return string();
}

BalancedSelection *BalancedSelection::clone() const
{
    BalancedSelection* ret = new BalancedSelection(this->EconomyScore, this->EnvironmentScore, this->LifeQualityScore);
    return ret;
}



EconomySelection::EconomySelection()
{
    lastSelectedIndex = 0;
}
const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int index = lastSelectedIndex;
    while(index <= facilitiesOptions.size()){
        if(index == facilitiesOptions.size()) index = 0;
        if(facilitiesOptions[index].getCategory()== FacilityCategory::ECONOMY){
            lastSelectedIndex = index;
        }
        else index++;
    }
}

const string EconomySelection::toString() const
{
    return string();
}

EconomySelection *EconomySelection::clone() const
{
    EconomySelection* ret = new EconomySelection();
    ret->lastSelectedIndex = this->lastSelectedIndex;
    return ret;
}



SustainabilitySelection::SustainabilitySelection()
{
    lastSelectedIndex = 0;
}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int index = lastSelectedIndex;
    while(index <= facilitiesOptions.size()){
        if(index == facilitiesOptions.size()) index = 0;
        if(facilitiesOptions[index].getCategory()== FacilityCategory::ENVIRONMENT){
            lastSelectedIndex = index;
        }
        else index++;
    }
}

const string SustainabilitySelection::toString() const
{
    return string();
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    SustainabilitySelection* ret = new SustainabilitySelection();
    ret->lastSelectedIndex = this->lastSelectedIndex;
    return ret;
}
