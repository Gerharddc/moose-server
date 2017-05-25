#ifndef NOTIFY
#define NOTIFY

#include <string>
#include <uWS/uWS.h>

typedef uWS::WebSocket<uWS::SERVER> *Client;

extern void Notify(std::string SystemName, std::string ID, std::string property, Client setter);

extern void AddClient(Client ws);
extern void RemoveClient(Client ws);

#endif
