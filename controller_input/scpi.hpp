#pragma once
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <algorithm>

// Add this pragma for automatic linking in Visual Studio:
#pragma comment(lib, "ws2_32.lib")


enum RXTX_STATE {
    TX_FAILED,
    TX_SUCCESS,
    RX_FAILED,
    RX_SUCCESS,
    RXTX_FAILED,
    RXTX_SUCCESS
};


class scpi {
private:
    std::string delimiter = "\r\n";
    const char* host_address;
    int port_number;

    WSADATA Winsockdata;
    int iWsaStartup;
    int iWsaCleanup;

    SOCKET TCPClientSocket;
    int iCloseSocket;
    struct sockaddr_in TCPServerAdd;
    int iConnect;

    int iSend;

    int iRecv;
    char RecvBuffer[512];
    int iRecvBuffer;


    //Step-1 WSAStartup Fun------------------------------------
public:
    scpi(const char* host, const int& port = 5000):RecvBuffer(),iCloseSocket(),iRecv(),iRecvBuffer(),iSend(),iWsaCleanup(),iConnect() {
        host_address = host;
        port_number = port;


        iWsaStartup = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
        if (iWsaStartup != 0)
        {
            std::cout << "WSAStartup Failed" << std::endl;
        }
        std::cout << "WSAStartup Success" << std::endl;

        //Step -2 Socket Creation------------------------------------

        TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (TCPClientSocket == INVALID_SOCKET)
        {
            std::cout << "TCP Client Socket Creation failed" << WSAGetLastError() << std::endl;
        }
        std::cout << "TCP client socket creation success";
        // STEP -3 Fill the structure-------------------------------

        TCPServerAdd.sin_family = AF_INET;
        if (inet_pton(AF_INET, host_address, &TCPServerAdd.sin_addr) != 1) {
            std::cout << "Invalid IPv4 address: " << host_address << std::endl;
            return;
        }
        TCPServerAdd.sin_port = htons(port_number);

        // STEP-4 Connect fun---------------------------------------------

        iConnect = connect(TCPClientSocket, (SOCKADDR*)&TCPServerAdd, sizeof(TCPServerAdd));
        if (iConnect == SOCKET_ERROR)
        {
            std::cout << "Connection failed & Error No ->" << WSAGetLastError() << std::endl;
        }
        std::cout << "Connection success" << std::endl;
    }

    ~scpi() {
        //STEP - 7 Close Socket

        iCloseSocket = closesocket(TCPClientSocket);
        if (iCloseSocket == SOCKET_ERROR)
        {
            std::cout << "Closing Failed & Error No->" << WSAGetLastError() << std::endl;
        }
        std::cout << "Closing Socket success" << std::endl;
    }
    // STEP-6 Send Data to the server

    bool tx_txt(const std::string& message) {
        std::string full_message = message + delimiter;  // Add \r\n like Python

        int bytes_sent = send(TCPClientSocket, full_message.c_str(), (int)full_message.length(), 0);

        if (bytes_sent == SOCKET_ERROR) {
            //std::cout << "Sending Failed & Error No->" << WSAGetLastError() << std::endl;
            return false;
        }

        //std::cout << "Data sending success" << std::endl;
        return true;
    }

    // Corrected rx_txt - reads until delimiter like Python
    std::string rx_txt(int chunksize = 4096) {
        std::string message = "";

        while (true) {
            int bytes_received = recv(TCPClientSocket, RecvBuffer, (chunksize < (int)sizeof(RecvBuffer) - 1 ? chunksize : (int)sizeof(RecvBuffer) - 1), 0);

            if (bytes_received == SOCKET_ERROR) {
                std::cout << "Receiving Failed & Error No->" << WSAGetLastError() << std::endl;
                return "";
            }

            if (bytes_received == 0) {
                std::cout << "Connection closed by server" << std::endl;
                return "";
            }

            // Null terminate and add to message
            RecvBuffer[bytes_received] = '\0';
            message += std::string(RecvBuffer);

            // Check if we have the delimiter "\r\n"
            if (message.length() >= 2 &&
                message.substr(message.length() - 2) == delimiter) {
                return message.substr(0, message.length() - 2); // Remove delimiter
            }
        }
    }

    // Corrected txrx_txt - matches Python behavior
    std::string txrx_txt(const std::string& message) {
        if (!tx_txt(message)) {
            return "";  // Send failed
        }
        return rx_txt();  // Return received response
    }
};


