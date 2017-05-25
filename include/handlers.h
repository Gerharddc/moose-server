#ifndef HANDLERS
#define HANDLERS

#include "rapidjson/stringbuffer.h"
#include "notify.h"

namespace RequestHandlers {
  extern int SetupHandlers();

  enum HandleRequestResult {
    success = 0,
    unkownRequest = -1,
    handlerError = -2,
    noRequest = -3,
    noData = -4,
    parseError = -5
  };

  extern HandleRequestResult HandleRequest(const char *request, rapidjson::StringBuffer &response, Client requester);
}

#endif // HANDLERS
