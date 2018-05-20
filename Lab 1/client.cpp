/**
 * Lab 1 client
 * Open a new connection with the server
 * Receive a puzzle
 * Solve puzzle
 * Send result
 * Repeat
 */

 #include <zmq.hpp>
 #include <string>
 #include <iostream>
 #include <cstring>
 #include <stdio.h>
 #include <big.h>
 #include "zzn.h"
 #include <time.h>
 #include <sstream>
 #include "miracl.h"
 #include <bitset>
 #ifndef _WIN32
   #include <unistd.h>
 #else
   #include <windows.h>

  #define sleep(n)    Sleep(n)
 #endif

Miracl precision = 100;

std::string StrToBin(std::string m){
  std::string m_bin;
  m_bin = "";
  for(int i = 0; i < m.size(); i++){
    std::bitset<8> b(m.c_str()[i]);
    m_bin += b.to_string();
  }

  return m_bin;
}

std::string BinToStr(std::string m_bin){
  std::string m = "";
  char c;

  std::stringstream sstream(m_bin);

  while(sstream.good()){
    std::bitset<8> b;
    sstream >> b;
    c = char(b.to_ulong());

    m += c;
  }

  return m;
}

std::string IncGuess(std::string x, int k){
  std::string x_bin = StrToBin(x);

  for(int i = 0; i < k; i++){
    if(x_bin[i] == '0'){
      x_bin[i] = '1';
      break;
    }
    else{
      x_bin[i] = '0';
    }
  }

  return BinToStr(x_bin);
}

std::string solvePuzzle(std::string p){
  std::string p_bin, x, y, pTime, x_guess;
  std::ostringstream ss;
  sha sh;
  int i, k = 0;
  char hash[20];
  Big h;

  shs_init(&sh);

  i = p.find(";");
  x = p.substr(0, i);
  p.erase(0, i + 1);

  i = p.find(";");
  y = p.substr(0, i);
  p.erase(0, i + 1);

  i = p.find(";");
  pTime = p.substr(0, i);
  p.erase(0, i + 1);

  p_bin = StrToBin(x);

  while(p_bin[k] == '0'){
    k++;
  }

  x_guess = x;

  while(ss.str() != y){
    for(int j = 0; j < x.length() - 1; j++){
      shs_process(&sh, x_guess[j]);
    }

    shs_hash(&sh, hash);
    h = from_binary(20, hash);
    ss.str("");
    ss << h;
    if(ss.str() != y)
      x_guess = IncGuess(x_guess, k);
 }

  // send result
  ss.str("");
  ss << x_guess << ";" << pTime << ";" << "1";
  return ss.str();
}

int main(){
  zmq::context_t context (1);
  zmq::socket_t socket (context, ZMQ_REQ);
  std::string p;

  printf("Client starting. Connecting to server\n");
  socket.connect ("tcp://localhost:5555");

  //Send initial request
  printf("Sending initial request\n");
  zmq::message_t request (5);
  memcpy(request.data (), "hello", 5);
  socket.send(request);

  //Receive puzzle
  zmq::message_t reply;
  socket.recv(&reply);
  printf("Received puzzle\n");

  //Solve puzzle
  p = solvePuzzle(string(static_cast<char*>(reply.data()), reply.size()));

  printf("Sending response\n");
  zmq::message_t puzzleResponse (p.length());
  memcpy(puzzleResponse.data (), p.c_str(), p.length());
  socket.send(puzzleResponse);

  socket.recv(&reply);
  if(string(static_cast<char*>(reply.data()), reply.size()) == "ok"){
    printf("We're in\n");
  }

  return 0;
 }
