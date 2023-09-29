/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <Nitwork.hpp>

struct packetData_s {
        struct header_s header;
        struct action_s action;
        struct msgInit_s msgInit;
} __attribute__((packed));

int main()
{
    struct header_s header = {
        .magick1 = 0x42,
        .ids_received = 0,
        .last_id_received = 0,
        .id = 0,
        .nb_action = 1,
        .magick2 = 0x42,
    };
    struct action_s action = {
        .magick = INIT,
    };
    struct msgInit_s msgInit = {
        .magick = INIT,
    };
    struct packetData_s packetData = {
        .header = header,
        .action = action,
        .msgInit = msgInit,
    };
    const int port = 4242; // Remplacez par le port du serveur

    try {
        // Initialisation de Boost.Asio
        boost::asio::io_context io_context;

        // Adresse et port du serveur
        boost::asio::ip::udp::resolver resolver(io_context);
        boost::asio::ip::udp::endpoint client_endpoint = *resolver.resolve(boost::asio::ip::udp::v4(), "127.0.0.1", std::to_string(port)).begin();

        // Ouvrir le socket UDP
        boost::asio::ip::udp::socket socket(io_context);
        socket.open(boost::asio::ip::udp::v4());
        std::thread t([&io_context](){ io_context.run(); });

        if (!socket.is_open()) {
            std::cerr << "Error: socket not open" << std::endl;
            return 84;
        }

        // Envoi de la structure
        socket.send_to(boost::asio::buffer(&packetData, sizeof(struct packetData_s)), client_endpoint);

        std::cout << "Structure envoyée avec succès au serveur. au port " << port << std::endl;

        // Fermer le socket
        socket.close();
        t.join();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
