#include "printer.h"

#include <map>
#include <iostream>
#include "notify.h"

static std::map<std::string, Heater> Heaters;

void Printer::InitPrinter()
{
    Heaters.emplace("Ext1", Heater("Ext1"));
    Heaters.emplace("Ext2", Heater("Ext2"));
}

Heater* Printer::getHeater(std::string id)
{
    auto itr = Heaters.find(id);
    
    if (itr != Heaters.end())
        return &(itr->second);
    else
        return nullptr;
}

void Printer::getHeaterList(rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
    for (auto pair : Heaters) {
        writer.String(pair.first);
    }
}

