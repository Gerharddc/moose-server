#include <iostream>
#include <uWS/uWS.h>

#include "handlers.h"
#include "printer.h"
#include "notify.h"

int main (int argc, char *argv[]) {
  uWS::Hub h;

  RequestHandlers::SetupHandlers();
  std::cout << "Handlers ready" << std::endl;
  
  Printer::InitPrinter();
  std::cout << "Printer ready" << std::endl;

  std::cout << "Waiting for requests" << std::endl;

  h.onMessage([](uWS::WebSocket<uWS::SERVER> *ws, char *message, size_t length, uWS::OpCode opCode) {
    rapidjson::StringBuffer response;
    
    char body[length + 1];
    strncpy(body, message, length);
    body[length] = '\0';
    int res = RequestHandlers::HandleRequest(body, response, ws);
    
    // TODO: deal with handler errors better
    ws->send(response.GetString(), response.GetLength(), opCode);
    
    if (res != 0)
        std::cout << "Request error: " << std::to_string(res) << std::endl;
  });
  
  h.onConnection([](uWS::WebSocket<uWS::SERVER> *ws, uWS::HttpRequest hr) {
      std::cout << "Connection added" << std::endl;
      AddClient(ws);

      // TODO: assign client id and use to not notify the client that made the request
  });
  
  h.onDisconnection([](uWS::WebSocket<uWS::SERVER> *ws, int code, char *message, size_t length) {
      std::cout << "Connection removed" << std::endl;
      RemoveClient(ws);
  });

  h.listen(8080);
  h.run();

  return 0;
}
