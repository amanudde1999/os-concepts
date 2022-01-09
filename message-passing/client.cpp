/**
    Assignment 3: Client/Server
	Name: Amro Amanuddein
	StudentID: 1572498
	Course: CMPUT 379 Fall 2021
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <iomanip>
#include <boost/asio/ip/host_name.hpp>
#include <string>
#include <fstream>
#include <sys/time.h>
#include <chrono>
#include "helper.h"

int connfd;
int sent = 0;
struct sockaddr_in serv_addr;
struct timeval current_time;
pid_t pid;
std::fstream output_file;
std::string host_name;
char msg[256];

// This function will connect to the server based on the entered ip address and port number
void connect_to_server(int port_num, std::string ip_addr){
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port_num);

    if (connect(connfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Error connecting");
        exit(-1);
    }
    else{
        // File creation and set decimal places to 2 for epoch times
        host_name = boost::asio::ip::host_name();
        pid = getpid();
        std::string file_name = host_name+"."+std::to_string(pid);
        // Only create client log file if client connection is successful
        output_file.open(file_name, std::ios::out);
        output_file << std::fixed << std::setprecision(2);
        output_file << "Using port " << port_num << std::endl;
        output_file << "Using server address "<< ip_addr << std::endl;
        output_file << "Host " << host_name << "." << pid << std::endl;
    }
}  



int main(int argc, char *argv[]){
    std::string input, ip_addr, host_pid;
    int port_num = atoi(argv[1]);
    ip_addr = argv[2];


    connect_to_server(port_num, ip_addr);
    while(scanf("%s",msg) != EOF){
        // T operations get sent to server and client has to wait for server response before performing next operation
        if (msg[0] == 'T'){
            // Finding EPOCH time
            double timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
            timestamp /= 1000;
            output_file << timestamp << ": Send (T   " << msg+1 << ")"<< std::endl;
            host_pid = " "+host_name+"."+std::to_string(pid);
            strcat(msg,host_pid.c_str());
            if(write(connfd, &msg, 255) < 0){
                perror("Error writing");
                exit(-1);
            }
            else{
                sent++;
            }
            if(read(connfd, &msg, 255) < 0){
                perror("Error reading");
                exit(-1);
            }
            else{
                double timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                timestamp /= 1000;
                output_file << timestamp <<": Recv (D   "+std::string(msg)+")"<< std::endl;
            }
        }  
        // S operations get executed on client side
        if (msg[0] == 'S'){
            Sleep(atoi((msg+1)));
            output_file << "Sleep "+std::string(msg+1)+" units" << std::endl;
        }
    }
    output_file << "Sent " << sent << " transactions" << std::endl;
    output_file.close();
    // Close connection when client is finished with it's operations (via CTRL-D)
    close(connfd);
    return 0;

}
