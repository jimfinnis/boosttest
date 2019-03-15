/**
 * @file server.cpp
 * @brief  Brief description of file.
 *
 */


#include <iostream>
#include <boost/asio.hpp>
#include "msg.hpp"

using boost::asio::ip::tcp;

#define PORT 31984

int snark=0;
void handleConnection(tcp::socket& socket){
    msg m;
    boost::system::error_code error;
    
    for(;;){
        boost::asio::read(socket,boost::asio::buffer(&m,sizeof(m)),
                          boost::asio::transfer_all(),error);
        if(error){
            // an error occurred
            if(error == boost::asio::error::eof){
                // this is fine; the client closed the connection.
                std::cerr << "Client closed connection." << std::endl;
            } else {
                // or something else.
                std::cerr << "error in read " << error.message() << std::endl;
            }
            // either case we just stop.
            return;
        } else {
            std::cerr << "Read a message: " << m.buf << std::endl;
            // all is well, send something 
            msg sm;
            sprintf(sm.buf,"Read: %s",m.buf);
            if(snark++==2)
                sleep(6);
            socket.send(boost::asio::buffer(&m,sizeof(m)));
        }
    }
}

int main(int argc,char *argv[]){
    try {
        boost::asio::io_service io;
        tcp::endpoint endpoint(tcp::v4(),PORT);
        tcp::acceptor acceptor(io,endpoint);
    
        for(;;){
            std::cerr << "Waiting... " << std::endl;
            tcp::socket socket(io);
            acceptor.accept(socket);
            handleConnection(socket);
            
        }
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
    
