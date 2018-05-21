#include <iostream>
#include <vector>
#include <string>
#include <big.h>

struct key{
  string k;
  vector<string> v;
}

struct keypair{
  key pk;
  key sk
};

keypair KeyGen(int l, string k, int t){
  keypair kp;
  string s;
  string v;
  for(int i = 0; i < t; i++){
    string s = 'random string';
    v = func(s);

    kp.pk.at(i) = v;
    kp.sk.at(i) = s;
  }

  kp.pk.k = k;
  kp.sk.k = k;

  return kp;
}

vector<string> Sign(string m, key sk){
  string h = hash(m);
  vector<string> subh;
  vector<string> sigma;

  int len = log2(t);

  for(int i = 0; i < h.length(); i += len){
    subh.push(h.substr(i, len));
  }

  return sigma;
}


string Verify(string m, vector<string> sigma, key pk){
  string h = hash(m);
  vector<string> subh;
  vector<string> sigma;

  int len = log2(t);

  for(int i = 0; i < h.length(); i += len){
    subh.push(h.substr(i, len));
  }

  stuff;

  return "accept";

}
