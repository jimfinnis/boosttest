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

int main(int argc,char *argv[]){
    try {
        abertcp::Client client(PORT);
        
        for(int i=0;i<10;i++){
            // send data
            msg m;
            strcpy(m.buf,"Hello world");
            client.send(&m,sizeof(m));
        
            // await response
            boost::system::error_code error;
            error = client.read(&m,sizeof(m),5);
            if(error){
                std::cerr << "Error: " << error.message() << std::endl;
                break;
            }
            std::cout << "Read a response: " << m.buf << std::endl;
        }
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
