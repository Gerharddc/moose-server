#ifndef PRINTER
#define PRINTER

#include "heater.h"
#include "rapidjson/writer.h"

namespace Printer
{
    extern void InitPrinter();
    
    extern Heater* getHeater(uint8_t id);
    extern void getHeaterList(rapidjson::Writer<rapidjson::StringBuffer> &writer);
};

#endif
