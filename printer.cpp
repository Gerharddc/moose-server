#include "printer.h"

#include <iostream>

static std::vector<Heater> Heaters;

void Printer::InitPrinter()
{
    Heaters.emplace_back(0, "Ext 1");
    Heaters.emplace_back(1, "Heatbed");
}

Heater* Printer::getHeater(uint8_t id)
{
    if (id >= Heaters.size())
        return nullptr;

    return &Heaters[id];
}

void Printer::getHeaterList(rapidjson::Writer<rapidjson::StringBuffer> &writer)
{
    for (int i = 0; i < Heaters.size(); i++)
        writer.Int(i);
}

