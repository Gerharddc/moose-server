#include "handlers.h"

#include <unordered_map>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include "printer.h"
#include "notify.h"

using namespace std;
using namespace rapidjson;
using namespace RequestHandlers;

static int HandleSetTargetTemp(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!data.HasMember("id")) {
        std::cout << "HandleSetTargetTemp-Error: no id" << std::endl;
        return -1;
    }
    
    if (!data.HasMember("temp")) {
        std::cout << "HandleSetTargetTemp-Error: no temp" << std::endl;
        return -1;
    }
    
    Heater* h = Printer::getHeater(data["id"].GetString());
    if (h != nullptr)
        h->setTargetTemp(data["temp"].GetFloat(), setter);
    else {
        std::cout << "HandleSetTargetTemp-Error: unknown heater id" << std::endl;
        return -1;
    }
    
    return 0;
}

static int HandleGetTargetTemp(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!data.HasMember("id")) {
        std::cout << "HandleSetTargetTemp-Error: no id" << std::endl;
        return -1;
    }
    
    Heater* h = Printer::getHeater(data["id"].GetString());
    if (h != nullptr) {
        response.Key("temp");
        response.Double(h->getTargetTemp());
    }
    else {
        std::cout << "HandleSetTargetTemp-Error: unknown heater id" << std::endl;
        return -1;
    }
    
    return 0;
}

static int HandleGetHeaters(const Value &data, Writer<StringBuffer> &response, Client setter) {
    response.Key("heaters");
    response.StartArray();
    Printer::getHeaterList(response);
    response.EndArray();
    
    return 0;
}

static int HandleGetHeater(const Value &data, Writer<StringBuffer> &response, Client setter) {
    Heater* h = Printer::getHeater(data["id"].GetString());
    if (h != nullptr) {
        response.Key("displayName");
        response.String(h->getDisplayName());

        response.Key("isOn");
        response.Bool(h->getHeating());

        response.Key("target");
        response.Double(h->getTargetTemp());

        response.Key("current");
        response.Double(h->getCurrentTemp());
    }
    else {
        std::cout << "HandleSetTargetTemp-Error: unknown heater id" << std::endl;
        return -1;
    }

    return 0;
}

static int HandleSetHeating(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!data.HasMember("id")) {
        std::cout << "HandleSetHeating-Error: no id" << std::endl;
        return -1;
    }

    if (!data.HasMember("heating")) {
        std::cout << "HandleSetHeating-Error: no heating" << std::endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetString());
    if (h != nullptr)
        h->setHeating(data["heating"].GetBool(), setter);
    else {
        std::cout << "HandleSetTargetTemp-Error: unknown heater id" << std::endl;
        return -1;
    }

    return 0;
}

static int HandleGetHeating(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!data.HasMember("id")) {
        std::cout << "HandleGetHeating-Error: no id" << std::endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetString());
    if (h != nullptr) {
        response.Key("heating");
        response.Double(h->getHeating());
    }
    else {
        std::cout << "HandleGetHeating-Error: unknown heater id" << std::endl;
        return -1;
    }

    return 0;
}

typedef int (*RequestHandler)(const Value&, Writer<StringBuffer>&, Client);
unordered_map<string, RequestHandler> handlerMap;

int RequestHandlers::SetupHandlers() {
    handlerMap["SetTargetTemp"] = &HandleSetTargetTemp;
    handlerMap["GetTargetTemp"] = &HandleGetTargetTemp;
    handlerMap["GetHeaters"]    = &HandleGetHeaters;
    handlerMap["SetHeating"]    = &HandleSetHeating;
    handlerMap["GetHeating"]    = &HandleGetHeating;
    handlerMap["GetHeater"]     = &HandleGetHeater;

    return 0;
}

HandleRequestResult RequestHandlers::HandleRequest(const char *request, StringBuffer &response, Client requester)
{
    Document requestJSON;
    HandleRequestResult errorResult;

    Writer<StringBuffer> writer(response);
    writer.StartObject();
    
    if (requestJSON.Parse(request).HasParseError()) {
        errorResult = HandleRequestResult::parseError;
        goto ErrorExit;
    }
    
    // Validate the request
    if (!requestJSON.HasMember("request")) {
        errorResult = HandleRequestResult::noRequest;
        goto ErrorExit;
    }

    writer.Key("request");
    writer.String(requestJSON["request"].GetString());

    if (!requestJSON.HasMember("data")) {
        errorResult = HandleRequestResult::noData;
        goto ErrorExit;
    }
    
    {
        const char* requestName = requestJSON["request"].GetString();
        
        auto han = handlerMap.find(std::string(requestName));
        
        if (han == handlerMap.end()) {
            errorResult = HandleRequestResult::unkownRequest;
            goto ErrorExit;
        }
        
        // Execute the handler if found
        Value &data = requestJSON["data"];
        int res = han->second(data, writer, requester);
        
        if (res != 0) {
            errorResult = HandleRequestResult::handlerError;
            goto ErrorExit;
        }
    }
    
    writer.Key("status");
    writer.String("success");
    writer.EndObject();
    
    return HandleRequestResult::success;
    
    ErrorExit:

    writer.Key("status");
    writer.String("error");
    writer.EndObject();

    return errorResult;
}