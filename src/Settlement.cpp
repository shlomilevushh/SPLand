#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type): name(name), type(type) {}

const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return type;
}

const string Settlement::toString() const
{
    std::string ret = "Settlement Name: " + name + ", Type: ";
    if (type == SettlementType::VILLAGE) ret = ret + "Village";
    if (type == SettlementType::METROPOLIS) ret = ret + "Metropolis";
    if (type == SettlementType::CITY) ret = ret + "City";
    return ret;
}
