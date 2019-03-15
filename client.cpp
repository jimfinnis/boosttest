/**
 * @file client.cpp
 * @brief  Brief description of file.
 *
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include "msg.hpp"

using boost::asio::ip::tcp;

#define PORT 31984
#define SPORT "31984"

template <typename SyncReadStream, typename MutableBufferSequence>
void readTimeout(SyncReadStream& s, 
                 const MutableBufferSequence& buffers, 
                 const boost::asio::deadline_timer::duration_type& expiry_time)
{
    boost::optional<boost::system::error_code> timer_result;
    boost::optional<boost::system::error_code> read_result;
    
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
        else if (timer_result)
            s.cancel();
    }

    if (*read_result)
        throw boost::system::system_error(*read_result);
}

int main(int argc,char *argv[]){
    try {
        boost::asio::io_service io;
        tcp::resolver resolver(io);
        tcp::socket socket(io);
        auto endpoint_iterator = resolver.resolve(tcp::resolver::query("localhost",SPORT));
        boost::asio::connect(socket,endpoint_iterator);
        
        for(int i=0;i<10;i++){
            // send data
            msg m;
            strcpy(m.buf,"Hello world");
            socket.send(boost::asio::buffer(&m,sizeof(m)));
        
            // await response
            boost::system::error_code error;
            readTimeout(socket,boost::asio::buffer(&m,sizeof(m)),
                        boost::posix_time::seconds(4));
            std::cout << "Read a response: " << m.buf << std::endl;
        }
    } catch (std::exception &e){
        std::cerr << e.what() << std::endl;
    }
}
