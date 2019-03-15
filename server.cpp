/**
 * @file server.cpp
 * @brief  Brief description of file.
 *
 */


#include "server.hpp"
#include "msg.hpp"

#define PORT 31984

class Server : public abertcp::Server {
    int test;
public:
    Server() : abertcp::Server(PORT) {
        test=0;
    }
    virtual void handleConnection (){
        msg m;
        for(;;){
            if(read(&m,sizeof(m)))
                break;
            else {
                if(test++==2)
                    sleep(6);
                std::cout << m.buf << std::endl;
                strcpy(m.buf,"Blah");
                send(&m,sizeof(m));
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
    
