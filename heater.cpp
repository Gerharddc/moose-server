#include "heater.h"

#include "notify.h"

void Heater::setTargetTemp(float temp, Client setter)
{
    if (targetTemp_ != temp)
    {
        // TODO: send command to printer
    
        Notify(SystemName, id_, "TargetTemp", setter);
    
        targetTemp_ = temp;
    }
}

void Heater::setHeating(bool heating, Client setter)
{
    if (heating_ != heating)
    {
        // TODO: send command to printer
    
        Notify(SystemName, id_, "Heating", setter);
    
        heating_ = heating;
    }
}

void Heater::setDisplayName(std::string displayName, Client setter) {
    if (displayName != displayName_)
    {
        Notify(SystemName, id_, "DisplayName", setter);

        displayName_ = displayName;
    }
}

float Heater::getCurrentTemp() {
    // TODO: read from the printer

    return 25.0f;
}
