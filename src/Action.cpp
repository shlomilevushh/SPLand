#include "Action.h"
#include "iostream"

//BaseAction Class:
BaseAction::BaseAction(){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

void BaseAction::complete(){
    this->status = ActionStatus::COMPLETED;
}


void BaseAction::error(string errorMsg){
    this->status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " + errorMsg;
}

const string &BaseAction::getErrorMsg() const{
    return errorMsg;
}


//SimulateStep Class:
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation){
    for( int i = 0 ; i < simulation.getPlans().size() ; i++ ){

    }
    
}

const string SimulateStep::toString() const
{
    return string();
}

SimulateStep *SimulateStep::clone() const
{
    return nullptr;
}

//AddPlan Class:

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) :
settlementName(settlementName) , selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation){
    SelectionPolicy* selectedPolicy = nullptr;
    if(simulation.isSettlementExists(settlementName)) this->error("Cannot create this plan");
    else{
        if(selectionPolicy == "nve")  selectedPolicy =  new NaiveSelection();
        else if(selectionPolicy == "bal")  selectedPolicy =  new BalancedSelection(sett.);
        else if(selectionPolicy == "eco")  selectedPolicy =  new EconomySelection();
        else if(selectionPolicy == "env")  selectedPolicy =  new SustainabilitySelection();
        else this->error("Cannot create this plan");
    }
    if(selectedPolicy != nullptr){
        simulation.addPlan(simulation.getSettlement(settlementName),selectedPolicy);
        this->complete();
    }
}
const string AddPlan::toString() const{
    return string();
}

AddPlan *AddPlan::clone() const{
    return new AddPlan(this->settlementName,this->selectionPolicy);
}

//AddSetelment Class: 

AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation){
    if(simulation.getSettlement(settlementName)) this->error("Settlement already exists");
    else {
        simulation.addSettlement(new Settlement(settlementName,settlementType));
        this->complete();
    }
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(this->settlementName,this->settlementType);
}

const string AddSettlement::toString() const
{
    return string();
}

//AddFacility Class:

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore):
facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation){
    if(simulation.isFacilityExists(facilityName)) this->error("Facility already exists");
    else {
        FacilityType newFacility(facilityName,facilityCategory,price,lifeQualityScore,economyScore,environmentScore);
        simulation.addFacility(newFacility);
        this->complete();
    }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(this->facilityName,this->facilityCategory,this->price,this->lifeQualityScore,this->economyScore,this->environmentScore);
}

const string AddFacility::toString() const
{
    return string();
}

//PrintPlanStatus Class:

PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation){
    if(!simulation.isPlanExists(this->planId)) this->error("Plan doesn't exist");
    else{
        Plan toPrintPlan = simulation.getPlan(planId);
        std::cout << "PlanID: " + planId << std::endl;
        std::cout << "SettlementName: " + toPrintPlan.getSettlement()->getName() << std::endl;
        std::cout << "PlanStatus: ";
        toPrintPlan.printStatus();
        std::cout << "\n";
        std::cout << "SelectionPolicy: " + toPrintPlan.getSelectionPolicy()->toString() << std::endl;
        std::cout << "LifeQualityScore: " + toPrintPlan.getlifeQualityScore() << std::endl;
        std::cout << "EconomyScore: " + toPrintPlan.getEconomyScore() << std::endl;
        std::cout << "EnvironmentScore: " + toPrintPlan.getEnvironmentScore() << std::endl;
        for( int i = 0 ; i < toPrintPlan.getFacilities().size(); i++ ){
            std::cout <<"FacilityName: " + toPrintPlan.getFacilities()[i]->getSettlementName() << std::endl;
            std::cout <<"FacilityStatus: " + toPrintPlan.getFacilities()[i].getStatusString() << std::endl;
        }
        this->complete();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(this->planId);
}

const string PrintPlanStatus::toString() const
{
    return string();
}

//ChangePlanPolicy Class:

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy):
planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation){
    if(!simulation.isPlanExists(planId)) this->error("Cannot change selection policy");
    else{
        Plan toChangePlan = simulation.getPlan(planId);
        if(toChangePlan.getSelectionPolicy()->toString() == newPolicy)this->error("Cannot change selection policy");
        else{
            std::cout << "PlanID: " + planId << std::endl;
            std::cout << "previousPolicy: " + toChangePlan.getSelectionPolicy()->toString() << std::endl;
            std::cout << "newPolicy: " + newPolicy << std::endl;
            SelectionPolicy* selectedPolicy = nullptr;
            if(newPolicy == "nve")  selectedPolicy =  new NaiveSelection();
            else if(newPolicy == "bal")  selectedPolicy =  new BalancedSelection(toChangePlan.getlifeQualityScore(),toChangePlan.getEconomyScore(),toChangePlan.getEnvironmentScore());
            else if(newPolicy == "eco")  selectedPolicy =  new EconomySelection();
            else if(newPolicy == "env")  selectedPolicy =  new SustainabilitySelection();
            else this->error("Cannot change selection policy");
            if(selectedPolicy != nullptr){
                toChangePlan.setSelectionPolicy(selectedPolicy);
                this->complete();
            }
        }
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const{
    return new ChangePlanPolicy(planId,newPolicy);
}

const string ChangePlanPolicy::toString() const{
    return string();
}

//PrintActionsLog Class:

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation){
    for( int i = 0 ; i < simulation.getActionsLog().size() ; i++ ){
        std::cout << simulation.getActionsLog()[i].    }
}

PrintActionsLog *PrintActionsLog::clone() const{
    return new PrintActionsLog();
}

const string PrintActionsLog::toString() const{
    return string();
}
