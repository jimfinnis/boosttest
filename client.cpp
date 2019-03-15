/**
 * @file client.cpp
 * @brief  Brief description of file.
 *
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include "client.hpp"
#include "msg.hpp"

using boost::asio::ip::tcp;

#define PORT 31984

class Client {
    abertcp::Client client;
    request req;
    response res;
public:
    Client() : client("cranberry",PORT) {}
    
    bool get(){
        req.n = 0;
        client.send(&req,sizeof(req));
        boost::system::error_code error;
        error = client.read(&res,sizeof(res),5);
        if(error){
            std::cerr << "Error: " << error.message() << std::endl;
            return false;
        }
        std::cout << "Read a response." << std::endl;
        return true;
    }
};
        
    

int main(int argc,char *argv[]){
    try {
        Client client;
        
        for(int i=0;i<1000;i++){
            if(!client.get())break;
        }
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
