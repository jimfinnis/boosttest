/*!
 * \file server.hpp
 * \brief 
 * \author Jim Finnis
 *
 * Copyright (C) 2019 Aberystwyth University
 * This program comes with ABSOLUTELY NO WARRANTY;
 * GPLv3 for details see LICENSE file
 */

#ifndef __SERVER_HPP
#define __SERVER_HPP

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace abertcp {

class Server {
private:
    boost::asio::io_service io_;
    tcp::socket socket_;
    int port_;
          
protected:
    // read a message and show a debug message.
    boost::system::error_code read(void *p,size_t s){
        boost::system::error_code error;
        boost::asio::read(socket_,boost::asio::buffer(p,s),
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
        } else {
            std::cerr << "Read a message" << std::endl;
        }
        return error;
    }
    
    // send a message back.
    void send(void *p,size_t s){
        socket_.send(boost::asio::buffer(p,s));
    }
    
    // we override this to provide functionality
    virtual void handleConnection() = 0;
    
public:
    
    Server(int port) : port_(port), socket_(io_) {
    }
    
    void loop(){
        tcp::endpoint endpoint(tcp::v4(),port_);
        tcp::acceptor acceptor(io_,endpoint);
        
        for(;;){ // outer loop waits for connections
            std::cerr << "Waiting... " << std::endl;
            acceptor.accept(socket_);
            // inner loop handles connections and exits when we're
            // done (usually when read returns an error).
            handleConnection();
            socket_.close(); // so we can accept again.
        }
    }
};

}
    


#endif // __SERVER_HPP

