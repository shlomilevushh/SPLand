#include "Action.h"
#include "iostream"

extern Simulation* backup;

//BaseAction Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
BaseAction::BaseAction(): status(ActionStatus::COMPLETED),errorMsg(""){}

ActionStatus BaseAction::getStatus() const{
    return status;
}

string BaseAction::getStatusString() const{
    string s;
    if(getStatus()==ActionStatus::COMPLETED) s = "COMPLETED"; else s = "ERROR";
    return s;
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
//-----------------------------------------------------------------------------------------------------------------------------------------------
//SimulateStep Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation){
    for( int i = 0 ; i < numOfSteps ; i++ ) simulation.step();
}

const string SimulateStep::toString() const{
    return "step " + std::to_string(numOfSteps) + " " + getStatusString();
}

SimulateStep *SimulateStep::clone() const{
    return new SimulateStep(numOfSteps);
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//AddPlan Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy) :
settlementName(settlementName) , selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation){
    SelectionPolicy* selectedPolicy = nullptr;
    if(!simulation.isSettlementExists(settlementName)) this->error("Cannot create this plan");
    else{
        if(selectionPolicy == "nve")  selectedPolicy =  new NaiveSelection();
        else if(selectionPolicy == "bal")  selectedPolicy =  new BalancedSelection(0,0,0);
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
    return "plan " + settlementName + " " + selectionPolicy + " " + getStatusString();
}

AddPlan *AddPlan::clone() const{
    AddPlan* theClone = new AddPlan(this->settlementName,this->selectionPolicy);
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//AddSetelment Class: 
//-----------------------------------------------------------------------------------------------------------------------------------------------
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType):
settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation){
    if(simulation.getSettlement(settlementName)) this->error("Settlement already exists");
    else {
        simulation.addSettlement(new Settlement(settlementName,settlementType));
        this->complete();
    }
}

AddSettlement *AddSettlement::clone() const{
    AddSettlement* theClone =new AddSettlement(this->settlementName,this->settlementType);
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string AddSettlement::toString() const{
    return "settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + " " + getStatusString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//AddFacility Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
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

AddFacility *AddFacility::clone() const{
    AddFacility* theClone = new AddFacility(this->facilityName,this->facilityCategory,this->price,this->lifeQualityScore,this->economyScore,this->environmentScore);
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}


const string AddFacility::toString() const{
    return "facility " + facilityName + " " +std::to_string(static_cast<int>(facilityCategory)) + " " + std::to_string(price) + " " + 
    std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + getStatusString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//PrintPlanStatus Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation){
    if(!simulation.isPlanExists(this->planId)) this->error("Plan doesn't exist");
    else{
        Plan toPrintPlan = simulation.getPlan(planId);
        toPrintPlan.printStatus();
        this->complete();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const{
    PrintPlanStatus* theClone = new PrintPlanStatus(this->planId);
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string PrintPlanStatus::toString() const{
    return "planStatus " + std::to_string(planId) + " " +getStatusString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//ChangePlanPolicy Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
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
            else if(newPolicy == "bal"){
                int underConstructionEco = 0;
                int underConstructionLife = 0;
                int underConstructionEnv = 0;
                vector<Facility*> underConstruction = toChangePlan.getUnderConstruction();
                for(int i=0; i< underConstruction.size(); i++){
                    underConstructionEco = underConstruction[i]->getEconomyScore();
                    underConstructionLife = underConstruction[i]->getLifeQualityScore();
                    underConstructionEnv = underConstruction[i]->getEnvironmentScore();
                }  
                selectedPolicy =  new BalancedSelection(toChangePlan.getlifeQualityScore() + underConstructionLife
                ,toChangePlan.getEconomyScore() + underConstructionEco,toChangePlan.getEnvironmentScore()+ underConstructionEnv);
            }
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
    ChangePlanPolicy* theClone = new ChangePlanPolicy(planId,newPolicy);
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string ChangePlanPolicy::toString() const{
    return "changePolicy " + std::to_string(planId) + " " + newPolicy + " " + getStatusString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//PrintActionsLog Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(Simulation &simulation){
    for( int i = 0 ; i < simulation.getActionsLog().size() ; i++ ){
        std::cout << simulation.getActionsLog()[i]->toString() << std::endl;
    }
}

PrintActionsLog *PrintActionsLog::clone() const{
    PrintActionsLog* theClone = new PrintActionsLog();
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string PrintActionsLog::toString() const{
    return "log COMPLETED";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//Close Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
Close::Close(){}

void Close::act(Simulation &simulation){
    for(int i=0 ; i < simulation.getPlans().size() ; i++ ){
        Plan tempPlan = simulation.getPlans()[i];
        std::cout << "PlanID: " + tempPlan.getPlanId() << std::endl;
        std::cout << "SettlementName: " + tempPlan.getSettlement()->getName() << std::endl;
        std::cout << "LifeQuality_Score: " + tempPlan.getlifeQualityScore() << std::endl;
        std::cout << "Economy_Score: " + tempPlan.getEconomyScore() << std::endl;
        std::cout << "Enviroment_Score: " + tempPlan.getEnvironmentScore() << std::endl;
    }
    simulation.close();
    this->complete();
}

Close *Close::clone() const{
    Close* theClone = new Close();
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string Close::toString() const{
    return "close COMPLETED";
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//BackupSimulation Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
BackupSimulation::BackupSimulation(){}

void BackupSimulation::act(Simulation &simulation){
    if(backup == nullptr) backup = new Simulation(simulation);
    else *backup = simulation;
}

BackupSimulation *BackupSimulation::clone() const{
    BackupSimulation* theClone = new BackupSimulation();
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string BackupSimulation::toString() const{
    return "backup COMPLETED" ;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
//RestoreSimulation Class:
//-----------------------------------------------------------------------------------------------------------------------------------------------
RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation::act(Simulation &simulation){
    if(backup == nullptr) this->error("No backup available ");
    else{
        simulation = *backup;
        this->complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const{
    RestoreSimulation* theClone = new RestoreSimulation();
    if(getStatus() == ActionStatus::ERROR) theClone->error(getErrorMsg());
    return theClone;
}

const string RestoreSimulation::toString() const{
    return "restore " + getStatusString();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------