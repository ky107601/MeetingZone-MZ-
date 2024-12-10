#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <thread>
#include <string>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

void read_messages(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        while (true) {
            boost::beast::flat_buffer buffer;
            ws->read(buffer);
            std::cout << "Message from server: " << boost::beast::buffers_to_string(buffer.data()) << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Read error: " << e.what() << std::endl;
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("localhost", "8080");

        auto ws = std::make_shared<websocket::stream<tcp::socket>>(io_context);
        boost::asio::connect(ws->next_layer(), endpoints);
        ws->handshake("localhost", "/");

        std::cout << "Connected to the server!" << std::endl;

        std::thread(read_messages, ws).detach();

        std::string message;
        while (true) {
            std::getline(std::cin, message);
            if (message == "exit") break;

            ws->write(boost::asio::buffer(message));
        }

        ws->close(websocket::close_code::normal);
    } catch (const std::exception& e) {
        std::cerr << "Client error: " << e.what() << std::endl;
    }

    return 0;
}
