/*!
 * \file client.hpp
 * \brief 
 * \author Jim Finnis
 *
 * Copyright (C) 2019 Aberystwyth University
 * This program comes with ABSOLUTELY NO WARRANTY;
 * GPLv3 for details see LICENSE file
 */

#ifndef __CLIENT_HPP
#define __CLIENT_HPP

#include <iostream>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

using boost::asio::ip::tcp;

namespace abertcp {

// handy function for a blocking read with timeout
template <typename SyncReadStream, typename MutableBufferSequence>
boost::system::error_code readTimeout(SyncReadStream& s, 
                 const MutableBufferSequence& buffers, 
                 const boost::asio::deadline_timer::duration_type& expiry_time)
{
    boost::optional<boost::system::error_code> timer_result;
    boost::optional<boost::system::error_code> read_result;
    boost::system::error_code error;
    
    // start the timer and start a wait on it
    boost::asio::deadline_timer timer(s.get_io_service());
    timer.expires_from_now(expiry_time);
    timer.async_wait([&timer_result] (const boost::system::error_code& error)
                 { timer_result.reset(error); });
    
    // start an async read
    boost::asio::async_read(s, buffers,
                            [&read_result](
                                           const boost::system::error_code& error, size_t)
                        { read_result.reset(error); });
    
    // run the IO loop for at most one handler. If
    // read_result gets set, the read succeeded. Otherwise if the timer gets
    // set, we timed out.
    s.get_io_service().reset();
    while (s.get_io_service().run_one())
    { 
        if (read_result)
            timer.cancel();
        else if (timer_result){
            s.cancel();
            error = boost::system::errc::make_error_code(boost::system::errc::timed_out);
            return error;
        }
    }

    if (*read_result)
        return *read_result;
    else
        return error;
}


class Client {
private:
    boost::asio::io_service io_;
    tcp::socket socket_;
    
public:
    
    Client(const char *hostname,int port) : socket_(io_){
        tcp::resolver resolver(io_);
        auto endpoint_iterator = resolver.resolve
              (tcp::resolver::query(hostname,std::to_string(port)));
        boost::asio::connect(socket_,endpoint_iterator);
    }
    
    // read a message and show a debug message.
    boost::system::error_code read(void *p,size_t s,float timeout){
        return readTimeout(socket_,boost::asio::buffer(p,s),
                    boost::posix_time::seconds(timeout));
    }
    
    // send a message.
    void send(void *p,size_t s){
        socket_.send(boost::asio::buffer(p,s));
    }
};

}
    
    
        


#endif // __CLIENT_HPP

