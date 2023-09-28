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
        boost::asio::ip::udp::socket socket(io_context);

        // Adresse et port du serveur
        boost::asio::ip::udp::endpoint server_endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 4242);

        // Ouvrir le socket UDP
        socket.open(boost::asio::ip::udp::v4());

        // Envoi de la structure
        socket.send_to(boost::asio::buffer(&packetData, sizeof(struct packetData_s)), server_endpoint);

        std::cout << "Structure envoyée avec succès au serveur." << std::endl;

        // Fermer le socket
        socket.close();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
