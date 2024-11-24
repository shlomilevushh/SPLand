#include "Simulation.h"
#include "Auxiliary.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Simulation::Simulation(const string &configFilePath): planCounter(0), isRunning(true)
{
    ifstream file("GFG.txt");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            std::vector<string> parsedLine = Auxiliary::parseArguments(line);
            if(parsedLine[0] == "settlement"){
                string name = parsedLine[1];
                SettlementType Stype;
                if(parsedLine[2]== "0") Stype = SettlementType::VILLAGE;
                if(parsedLine[2]== "1") Stype = SettlementType::CITY;
                if(parsedLine[2]== "2") Stype = SettlementType::METROPOLIS;
                Settlement* newSettlement = new Settlement(name, Stype);
                settlements.push_back(newSettlement);
            }
            if(parsedLine[0] == "facility"){
                string name = parsedLine[1];
                FacilityCategory Ftype;
                if(parsedLine[2] == "0") Ftype = FacilityCategory::LIFE_QUALITY;
                if(parsedLine[2] == "1") Ftype = FacilityCategory::ECONOMY;
                if(parsedLine[2] == "2") Ftype = FacilityCategory::ENVIRONMENT;
                int price = stoi(parsedLine[3]);
                int lifeq = stoi(parsedLine[4]);
                int eco = stoi(parsedLine[5]);
                int env = stoi(parsedLine[6]);
                FacilityType newFacility(name, Ftype, price, lifeq, eco, env);
                facilitiesOptions.push_back(newFacility);
            }
            if(parsedLine[0] == "plan"){
                Plan newPlan();
                string settlementName = parsedLine[1];
                Settlement tmpSettlement = *(getSettlement(settlementName));
                if(parsedLine[2] == "env") {
                    SustainabilitySelection* newSusSelection = new SustainabilitySelection();
                    Plan newPlan(planCounter, tmpSettlement, newSusSelection, facilitiesOptions);
                    plans.push_back(newPlan);
                }
                if(parsedLine[2] == "nve") {
                    NaiveSelection* newNaiveSelection = new NaiveSelection();
                    Plan newPlan(planCounter, tmpSettlement, newNaiveSelection, facilitiesOptions);
                    plans.push_back(newPlan);
                }
                if(parsedLine[2] == "eco") {
                    EconomySelection* newEcoSelection = new EconomySelection();
                    Plan newPlan(planCounter, tmpSettlement, newEcoSelection, facilitiesOptions);
                    plans.push_back(newPlan);
                }
                if(parsedLine[2] == "bal") {
                    BalancedSelection* newBalSelection = new BalancedSelection(0,0,0);
                    Plan newPlan(planCounter, tmpSettlement, newBalSelection, facilitiesOptions);
                    plans.push_back(newPlan);
                }
                planCounter++;
            }
        }
        file.close();
    }
    else {
        cerr << "Unable to open file!" << endl;
    }
}
void Simulation::start()
{
    while(isRunning){
        
    }

}
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy)
{
    Plan newPlan(planCounter, *settlement, selectionPolicy, facilitiesOptions);
    plans.push_back(newPlan);
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    if(isSettlementExists(settlement->getName())) return false;
    else settlements.push_back(settlement);
    return true;
    
}

bool Simulation::addFacility(FacilityType facility)
{
    for(int i=0; i< facilitiesOptions.size(); i++){
        if(facilitiesOptions[i].getName() == facility.getName()){
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for(int i=0; i< settlements.size(); i++){
        if(settlements[i]->getName() == settlementName){
            return true;
        }
    }
    return false;
}

vector<Plan>& Simulation::getPlans()
{
    vector<Plan>& plansRef = plans;
    return plansRef;
}

bool Simulation::isFacilityExists(const string &facilityName)
{
    for(int i=0; i< facilitiesOptions.size();i++){
        if(facilitiesOptions[i].getName()== facilityName){
            return true;
        }
    }
    return false;
}

bool Simulation::isPlanExists(const int planID)
{
    for(int i=0; i< plans.size();i++){
        if(plans[i].getPlanId() == planID){
            return true;
        }
    }
    return false;
}
