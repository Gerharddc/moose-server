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
    if (!(data.HasMember("id") && data["id"].IsInt())) {
        cout << "HandleSetTargetTemp-Error: no id" << endl;
        return -1;
    }
    
    if (!(data.HasMember("temp") && data["temp"].IsFloat())) {
        cout << "HandleSetTargetTemp-Error: no temp" << endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetInt());
    if (h != nullptr)
        h->setTargetTemp(data["temp"].GetFloat(), setter);
    else {
        cout << "HandleSetTargetTemp-Error: unknown heater id" << endl;
        return -1;
    }
    
    return 0;
}

static int HandleGetTargetTemp(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!(data.HasMember("id") && data["id"].IsInt())) {
        std::cout << "HandleSetTargetTemp-Error: no id" << endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetInt());
    if (h != nullptr) {
        response.Key("temp");
        response.Double(h->getTargetTemp());
    }
    else {
        cout << "HandleSetTargetTemp-Error: unknown heater id" << endl;
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
    if (!(data.HasMember("id") && data["id"].IsInt())) {
        cout << "HandleGetHeater-Error: no id" << endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetInt());
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
        cout << "HandleGetHeater-Error: unknown heater id" << endl;
        return -1;
    }

    return 0;
}

static int HandleSetHeating(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!(data.HasMember("id") && data["id"].IsInt())) {
        cout << "HandleSetHeating-Error: no id" << endl;
        return -1;
    }

    if (!(data.HasMember("heating") && data["heating"].IsBool())) {
        cout << "HandleSetHeating-Error: no heating" << endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetInt());
    if (h != nullptr)
        h->setHeating(data["heating"].GetBool(), setter);
    else {
        cout << "HandleSetTargetTemp-Error: unknown heater id" << endl;
        return -1;
    }

    return 0;
}

static int HandleGetHeating(const Value &data, Writer<StringBuffer> &response, Client setter) {
    if (!(data.HasMember("id") && data["id"].IsInt())) {
        cout << "HandleGetHeating-Error: no id" << endl;
        return -1;
    }

    Heater* h = Printer::getHeater(data["id"].GetInt());
    if (h != nullptr) {
        response.Key("heating");
        response.Bool(h->getHeating());
    }
    else {
        cout << "HandleGetHeating-Error: unknown heater id" << endl;
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
    Document doc;
    HandleRequestResult requestResult = HandleRequestResult::success;

    Writer<StringBuffer> writer(response);
    writer.StartObject();
    
    if (doc.Parse(request).HasParseError()) {
        requestResult = HandleRequestResult::parseError;
        goto Exit;
    }
    
    // Validate the request
    if (!(doc.HasMember("request") && doc["request"].IsString())) {
        requestResult = HandleRequestResult::noRequest;
        goto Exit;
    }

    // Return the request key if provided
    if (doc.HasMember("id") && doc["id"].IsString()) {
        writer.Key("id");
        writer.String(doc["id"].GetString());
    }

    writer.Key("request");
    writer.String(doc["request"].GetString());

    if (!doc.HasMember("data")) {
        requestResult = HandleRequestResult::noData;
        goto Exit;
    }
    
    {
        const char* requestName = doc["request"].GetString();
        
        auto han = handlerMap.find(std::string(requestName));
        
        if (han == handlerMap.end()) {
            requestResult = HandleRequestResult::unkownRequest;
            goto Exit;
        }
        
        // Execute the handler if found
        Value &data = doc["data"];
        int res = han->second(data, writer, requester);
        
        if (res != 0) {
            requestResult = HandleRequestResult::handlerError;
            goto Exit;
        }
    }
    
    Exit:

    writer.Key("status");
    writer.String((requestResult == HandleRequestResult::success) ? "success" : "error");

    writer.EndObject();

    return requestResult;
}
