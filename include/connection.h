#ifndef CONNECTION
#define CONNECTION

namespace Connection
{
    extern int CreateConnection(const char* port);
    
    extern int HeatToTemp(int heaterID, float temp);
    extern int StopHeater(int heaterID);
}

#endif
