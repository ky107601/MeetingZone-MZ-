#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/websocket.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>

using tcp = boost::asio::ip::tcp;
namespace websocket = boost::beast::websocket;

std::vector<std::shared_ptr<websocket::stream<tcp::socket>>> clients;
std::mutex client_mutex;

void broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(client_mutex);
    for (auto& client : clients) {
        try {
            client->write(boost::asio::buffer(message));
        } catch (const std::exception& e) {
            std::cerr << "Broadcast error: " << e.what() << std::endl;
        }
    }
}

void handle_client(std::shared_ptr<websocket::stream<tcp::socket>> ws) {
    try {
        ws->accept();
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            clients.push_back(ws);
        }

        std::string message;
        while (true) {
            boost::beast::flat_buffer buffer;
            ws->read(buffer);
            message = boost::beast::buffers_to_string(buffer.data());

            std::cout << "Received: " << message << std::endl;
            broadcast_message(message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Client disconnected: " << e.what() << std::endl;
    }

    // Remove the client from the list
    std::lock_guard<std::mutex> lock(client_mutex);
    clients.erase(std::remove(clients.begin(), clients.end(), ws), clients.end());
}

int main() {
    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "Server running on port 8080..." << std::endl;

        while (true) {
            auto socket = std::make_shared<tcp::socket>(io_context);
            acceptor.accept(*socket);

            std::cout << "Client connected!" << std::endl;

            auto ws = std::make_shared<websocket::stream<tcp::socket>>(std::move(*socket));
            std::thread(handle_client, ws).detach();
        }
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }

    return 0;
}
