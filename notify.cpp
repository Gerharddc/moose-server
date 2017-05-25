#include "notify.h"

#include <list>
#include "rapidjson/writer.h"

using namespace std;
using namespace uWS;
using namespace rapidjson;

static list<WebSocket<SERVER>*> clients;

void Notify(std::string SystemName, std::string ID, std::string property, Client setter)
{
    // TODO: implement
    
    StringBuffer sb;
    Writer<StringBuffer> writer(sb);
    
    writer.StartObject();
    
    writer.Key("status");
    writer.String("notify");
    
    writer.Key("systemname");
    writer.String(SystemName);
    
    writer.Key("id");
    writer.String(ID);
    
    writer.Key("property");
    writer.String(property);
    
    writer.EndObject();

    // Notify all clients of the change except the one that requested it
    for (Client client : clients)
    {
        if (client != setter)
            client->send(sb.GetString(), sb.GetLength(), OpCode::TEXT);
    }
}

void AddClient(WebSocket<SERVER>* ws)
{
    clients.push_back(ws);
}

void RemoveClient(WebSocket<SERVER>* ws)
{
    clients.remove(ws);
}
