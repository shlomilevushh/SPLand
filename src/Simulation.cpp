#include "Simulation.h"
#include "Auxiliary.h"
#include "Action.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

Simulation::Simulation(const string &configFilePath): planCounter(0), isRunning(true)
{
    ifstream file(configFilePath);
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
                FacilityCategory Ftype = static_cast<FacilityCategory>(stoi(parsedLine[2]));
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

Simulation::Simulation(const Simulation &other): planCounter(other.planCounter), isRunning(other.isRunning)
{
        for(int i=0; i< other.plans.size(); i++){
            this->plans.push_back(other.plans[i]);
        }
        for(int i=0; i< other.facilitiesOptions.size(); i++){
            this->facilitiesOptions.push_back(other.facilitiesOptions[i]);
        }
        for(int i=0; i< other.settlements.size(); i++){
            this->settlements.push_back(new Settlement(*other.settlements[i]));
        }
        for(int i=0; i< other.actionsLog.size(); i++){
            this->actionsLog.push_back(other.actionsLog[i]->clone());
        }
}

Simulation &Simulation::operator=(const Simulation &other)
{
    if(&other != this){
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        this->plans.clear();
        for(int i=0; i < other.plans.size(); i++){
            this->plans.push_back(other.plans[i]);
        }
        this->facilitiesOptions.clear();
        for(int i=0; i< other.facilitiesOptions.size(); i++){
            this->facilitiesOptions.push_back(other.facilitiesOptions[i]);
        }

        for(int i=0; i< settlements.size(); i++){
            delete settlements[i];
        }
        this->settlements.clear();
        for(int i=0; i< other.settlements.size(); i++){
            this->settlements.push_back(new Settlement(*other.settlements[i]));
        }

        for(int i=0; i< actionsLog.size(); i++){
            delete actionsLog[i];
        }
        actionsLog.clear();
        for(int i=0; i< other.actionsLog.size(); i++){
            this->actionsLog.push_back(other.actionsLog[i]->clone());
        }
    }
    return *this;
}

Simulation::~Simulation()
{
    for(int i=0; i<actionsLog.size();i++){
        delete actionsLog[i];
    }
    actionsLog.clear();

    for(int i=0; i<settlements.size();i++){
        delete settlements[i];
    }
    settlements.clear();
    plans.clear();
    facilitiesOptions.clear();
}

void Simulation::start()
{
    string str;
    vector<string> parsed_str;
    while(isRunning){
        getline(cin, str);
        parsed_str = Auxiliary::parseArguments(str);
        if(parsed_str[0] == "step"){
            BaseAction* newStep = new SimulateStep(stoi(parsed_str[1]));
            newStep->act(*this);
            actionsLog.push_back(newStep);
        }
        if(parsed_str[0] == "plan"){
            string name = parsed_str[1];
            string selectionPol = parsed_str[2];
            BaseAction* newPlan = new AddPlan(name, selectionPol);
            newPlan->act(*this);
            actionsLog.push_back(newPlan);
        }
        if(parsed_str[0] == "settlement"){
            string name = parsed_str[1];
            SettlementType Stype;
            if(parsed_str[2]== "0") Stype = SettlementType::VILLAGE;
            if(parsed_str[2]== "1") Stype = SettlementType::CITY;
            if(parsed_str[2]== "2") Stype = SettlementType::METROPOLIS;
            BaseAction* newSettlement = new AddSettlement(name, Stype);
            newSettlement->act(*this);
            actionsLog.push_back(newSettlement);
        }
        if(parsed_str[0] == "facility"){
            string name = parsed_str[1];
            FacilityCategory Ftype;
            if(parsed_str[2] == "0") Ftype = FacilityCategory::LIFE_QUALITY;
            if(parsed_str[2] == "1") Ftype = FacilityCategory::ECONOMY;
            if(parsed_str[2] == "2") Ftype = FacilityCategory::ENVIRONMENT;
            int price = stoi(parsed_str[3]);
            int lifeq = stoi(parsed_str[4]);
            int eco = stoi(parsed_str[5]);
            int env = stoi(parsed_str[6]);
            BaseAction* newFacility = new AddFacility(name, Ftype,price,lifeq,eco,env);
            newFacility->act(*this);
            actionsLog.push_back(newFacility);
        }
        if(parsed_str[0] == "planStatus"){
            int plan_id = stoi(parsed_str[1]);
            BaseAction* printPlanStatus = new PrintPlanStatus(plan_id);
            printPlanStatus->act(*this);
            actionsLog.push_back(printPlanStatus);
        }
        if(parsed_str[0] == "changePolicy"){
            int plan_id = stoi(parsed_str[1]);
            string selection_policy = parsed_str[2];
            BaseAction* changePlanPolicy = new ChangePlanPolicy(plan_id, selection_policy);
            changePlanPolicy->act(*this);
            actionsLog.push_back(changePlanPolicy);
        }
        if(parsed_str[0] == "log"){
            BaseAction* printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
            actionsLog.push_back(printActionsLog);
        }
        if(parsed_str[0] == "close"){
            BaseAction* close = new Close();
            close->act(*this);
            actionsLog.push_back(close);

        }
        if(parsed_str[0] == "backup"){
            BaseAction* backup = new BackupSimulation();
            backup->act(*this);
            actionsLog.push_back(backup);
        }
        if(parsed_str[0] == "restore"){
            BaseAction* restore = new RestoreSimulation();
            restore->act(*this);
            actionsLog.push_back(restore);
        }


    }

}
void Simulation::addPlan(const Settlement *settlement, SelectionPolicy *selectionPolicy)
{
    Plan newPlan(planCounter, *settlement, selectionPolicy, facilitiesOptions);
    planCounter++;
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

Settlement *Simulation::getSettlement(const string &settlementName)
{
    for(int i=0; i< settlements.size(); i++){
        if(settlements[i]->getName() == settlementName){
            return settlements[i];
        }
    }
}

Plan &Simulation::getPlan(const int planID)
{
    for(int i=0; i< plans.size(); i++){
        if(plans[i].getPlanId() == planID){
            return plans[i];
        }
    }
}

vector<Plan>& Simulation::getPlans()
{
    vector<Plan>& plansRef = plans;
    return plansRef;
}

vector<BaseAction *> Simulation::getActionsLog()
{
    return actionsLog;
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

void Simulation::step()
{
    for(int i =0; i< plans.size(); i++){
        plans[i].step();
    }
}

void Simulation::close()
{
    isRunning = false;
}
