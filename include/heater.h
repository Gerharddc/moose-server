#ifndef HEATER_H
#define HEATER_H

#include <string>
#include "notify.h"

class Heater
{
private:
    float targetTemp_ = 100.0f;
    std::string id_;
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
    
    Heater(std::string id) :
        id_(id) {}

    void setDisplayName(std::string displayName, Client setter);
    std::string getDisplayName() {
        return displayName_;
    };

    float getCurrentTemp();
};

#endif // HEATER_H
