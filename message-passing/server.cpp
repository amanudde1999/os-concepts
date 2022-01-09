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
#include <fstream> 
#include <iostream>
#include <iomanip>
#include <string>
#include <boost/asio/ip/host_name.hpp>
#include <boost/algorithm/string.hpp>
#include <poll.h>
#include <chrono>
#include <vector>
#include <unordered_map>
#include "helper.h"

// Global variables declared
int listenfd, connfd;
int done = 0;
double timestamp, start_time;
char msg[256];
pid_t pid;
socklen_t clnt_len;
struct sockaddr_in serv_addr, clnt_addr;
std::fstream output_file;
std::string host_name;
std::unordered_map<std::string, int> umap;
std::vector<pollfd> clnt_conns;

void socket_creation(){
    if ((listenfd=socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error opening socket");
        exit(-1);
    }
}

void bind_to_addr(int port_num){
    serv_addr.sin_family = AF_INET;
    
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port_num);

    if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Error binding");
        exit(-1);
    }
    else{
        host_name = boost::asio::ip::host_name();
        pid = getpid();
        // Only create the file if server start-up is successful
        std::string file_name = host_name+"."+std::to_string(pid);
        output_file.open(file_name, std::ios::out);
        output_file << std::fixed << std::setprecision(2);
        output_file << "Using port "<< port_num << std::endl;
    }
}


void listen_to_connections(){
    // Set a backlog of 10 conenctions (according to forums)
    if (listen(listenfd, 10) < 0){
        perror("Error listening");
        exit(-1);
    }

}

void accept_connections(){
    clnt_len = sizeof(clnt_addr);
    if((connfd = accept(listenfd,(struct sockaddr *) &clnt_addr, &clnt_len)) < 0){
        perror("Error accepting");
        exit(-1);
    }
    else{
        pollfd clnt;
        clnt.fd = connfd;
        clnt.events = POLLIN;
        clnt_conns.push_back(clnt);
    }
}

void summary(){
    output_file << "SUMMARY" << std::endl;
    double res = done/(timestamp-start_time);
    for (auto& x: umap)
        output_file <<" "<< x.second << " transactions from " << x.first << std::endl;
    output_file << std::setprecision(1) << done/(timestamp-start_time) << " transactions/sec  (" 
                << std::setprecision(2) << done << "/" << timestamp-start_time << ")" << std::endl;
}

int main(int argc, char *argv[]){
    start_time = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    start_time /= 1000;
    int port_num = atoi(argv[1]);
    std::vector<std::string> clnt_response; 
    socket_creation();
    bind_to_addr(port_num);
    listen_to_connections();

    pollfd server_struct;
    server_struct.fd = listenfd;
    server_struct.events = POLLIN;
    clnt_conns.push_back(server_struct);
    bool timeout = 1;

    while(timeout){
        // Set to 30000 for 30 seconds timeout
        // Pass in memory location of first element in array which is a pollfd struct
        int poll_events = poll(&clnt_conns[0], clnt_conns.size(), 30000);
        if (poll_events == 0){
            // If no events have occured for 30 seconds then server closes
            timeout = 0;
        }
        else{
            for (int i = 0; i < clnt_conns.size(); i++){
                // If there is an event from our server (client wants to connect)
                if (clnt_conns[i].revents == POLLIN && i == 0){
                    accept_connections();
                }
                // Event is a message being sent to server
                else if (clnt_conns[i].revents == POLLIN && i != 0){
                    connfd = clnt_conns[i].fd;
                    // Only if the previous function calls work does any of the following occur
                    if(read(clnt_conns[i].fd, &msg, 255) < 0){
                        perror("Error reading");
                        exit(-1);
                    }
                    else{
                        boost::split(clnt_response, std::string(msg), boost::is_any_of(" "));
                        if (clnt_response.size() != 2){
                            // Remove clients that are already done communicating with the server.
                            close(clnt_conns[i].fd);
                            clnt_conns.erase(clnt_conns.begin()+i);
                        }
                        else{
                            auto it = umap.find(clnt_response[1]);
                            // If this connection has not been found before, add it to the hashmap
                            if (it == umap.end()){
                                // Set the # of transactions as 1 since this is the first transaction we recieve
                                umap.insert(std::make_pair<std::string,int>(clnt_response[1].c_str(),1));
                            }
                            // This connection is in the hashmap, increment operations sent from this connection
                            else{
                                umap[clnt_response[1]]++;
                            }
                            done++;
                            timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();
                            // Convert timestamp to seconds
                            timestamp /= 1000;
                            output_file << timestamp <<": # "<< done << " (T    "+clnt_response[0].substr(1)+") from "<< clnt_response[1] << std::endl;
                            Trans(stoi(clnt_response[0].substr(1)));
                            timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::system_clock::now().time_since_epoch()).count();
                            timestamp /= 1000;
                            output_file << timestamp << ": # " << done << " (Done) from "+clnt_response[1] << std::endl;
                            // Send back Done ID to the client
                            const char *p = std::to_string(done).c_str();
                            strcpy(msg, p);
                            if(write(clnt_conns[i].fd, &msg, 255) < 0){
                                perror("Error writing");
                                exit(-1);
                            }
                        }
                    }
                }
            }
        }
    }
    summary();
    output_file.close();
    return 0;
}
