/**
 * @file server.cpp
 * @brief  Brief description of file.
 *
 */


#include "server.hpp"
#include "msg.hpp"

#define PORT 31984

class Server : public abertcp::Server {
    response res;
public:
    Server() : abertcp::Server(PORT) {}
    
    virtual void handleConnection (){
        request req;
        for(;;){
            if(read(&req,sizeof(req)))
                break;
            else {
                std::cout << "Sending" << std::endl;
                memset(&res.b,0,sizeof(res.b));
                send(&res,sizeof(res));
            }
        }
    }
};


int main(int argc,char *argv[]){
    try {
        Server server;
        server.loop();
        
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
    
