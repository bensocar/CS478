/**
 * Open socket to tcp://*:5555
 * Accept connections
 * Send puzzles
 */

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include "zzn.h"
#include <big.h>
#include <time.h>
#include <sstream>
#include <bitset>
#include "miracl.h"
#ifndef _WIN32
  #include <unistd.h>
#else
  #include <windows.h>

 #define sleep(n)    Sleep(n)
#endif

Miracl precision = 100;

std::string makePuzzle(sha* psh, std::string secret, int pNum, int k){
  std::ostringstream ss;
  std::string puzzleStr;
  std::string x, y;
  Big h;
  time_t pTime;
  time(&pTime);
  char hash[20];
  char c;

  ss << pTime << secret << "hello" << pNum;

  puzzleStr =  ss.str();

  for(int i = 0; i < puzzleStr.length(); i++){
    shs_process(psh, puzzleStr[i]);
  }
  shs_hash(psh, hash);

  h = from_binary(20, hash);

  ss.str("");
  ss << h;
  x = ss.str();

  for(int i = 0; i < x.length(); i++){
    shs_process(psh, x[i]);
  }

  shs_hash(psh, hash);
  h = from_binary(20, hash);

  ss.str("");
  ss << h;
  y = ss.str();

  std::string x_bin;
  for(int i = 0; i < x.size(); ++i){
    std::bitset<8> b(x.c_str()[i]);
    x_bin += b.to_string();
  }

  for(int i = 0; i < k; i++){
    x_bin[i] = '0';
  }
  std::string x_puzzle = "";
  std::stringstream sstream(x_bin);
  while(sstream.good()){
    std::bitset<8> b;
    sstream >> b;
    c = char(b.to_ulong());
    x_puzzle += c;
  }

  std::cout << x << "\n" << std::endl;
  ss.str("");
  ss << x_puzzle << ";" << y << ";" << pTime << ";" << pNum;

  return ss.str();
}

bool checkSol(std::string s, sha* psh, std::string secret){
  int i;
  std::ostringstream puzzleBase;
  std::string x, pTime, pNum, puzzleStr;
  char hash[20];
  Big h;
  printf("Got potential solution\n");
  i = s.find(";");
  x = s.substr(0, i);
  s.erase(0, i + 1);

  i = s.find(";");
  pTime = s.substr(0, i);
  s.erase(0, i + 1);

  pNum = s;

  puzzleBase << pTime << secret << "hello" << pNum;

  puzzleStr = puzzleBase.str();
  for(int i = 0; i < puzzleStr.length(); i++){
    shs_process(psh, puzzleStr[i]);
  }
  shs_hash(psh, hash);
  h = from_binary(20, hash);
  puzzleBase.str("");
  puzzleBase << h;

  return (puzzleBase.str() == x.substr(0, puzzleBase.str().length()));
}

int main(){
  int pNum = 1;
  int k = 3;
  time_t seed;
  sha sh;
  std::string puzzleStr;
  std::ostringstream puzzleInput;
  std::string zmqReq;
  std::string secret = "81725cc80a71d6be7f63ab851d69d874";

  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REP);
  socket.bind ("tcp://*:5555");

  shs_init(&sh);
  printf("Server up\n");
  while(true){
    zmq::message_t request;

    socket.recv(&request);
    zmqReq = std::string(static_cast<char*>(request.data()), request.size());

    if(zmqReq == "hello"){
      printf("Received request\n");
      puzzleStr = makePuzzle(&sh, secret, pNum, k);

      printf("Sending puzzle\n");

      zmq::message_t reply(puzzleStr.length());
      memcpy(reply.data (), puzzleStr.c_str(), puzzleStr.length());
      socket.send (reply);
    }
    else if(checkSol(zmqReq, &sh, secret)){
      printf("Received correct puzzle solution\n");
      zmq::message_t reply(2);
      memcpy(reply.data (), "ok", 2);
      socket.send(reply);
    }
    else{
      printf("Received incorrect puzzle solution\n");
      zmq::message_t reply(2);
      memcpy(reply.data (), "no", 2);
      socket.send(reply);
    }
  }
  return 0;
}
