#ifndef HEATER_H
#define HEATER_H

#include <string>
#include "notify.h"

class Heater
{
private:
    float targetTemp_ = 100.0f;
    uint8_t id_;
    std::string idString;
    bool heating_ = false;
    std::string displayName_;
    
public:
    void setTargetTemp(float temp, Client setter);
    float getTargetTemp() {
        return targetTemp_;
    }
    
    void setHeating(bool heating, Client setter);
    bool getHeating() {
        return heating_;
    }
    
    const char* SystemName = "Heater";
    
    Heater(uint8_t id, std::string displayName) :
        id_(id), displayName_(displayName), idString(std::to_string(id)) {}

    void setDisplayName(std::string displayName, Client setter);
    std::string getDisplayName() {
        return displayName_;
    };

    float getCurrentTemp();
};

#endif // HEATER_H
