#include <iostream>
#include <vector>
#include <string>
#include <big.h>

struct key{
  string k;
  int i;
}

struct keypair{
  key pk;
  key sk
};

int kappa = 128;
int k = 32;
int t = 1024;

keypair PQ_KeyGen(int i, int d){
  int st = 1;
  vector<string> s;
  vector<string> v;
  string h, R;
  keypair kp;

  big z = rand(kappa);
  int len = d * t;

  for(int j = 0; j < len; j++){
    h = hash(z||j);
    s.push(h);
    v.push(hash(h));
  }

  R = Form(v);

  kp.pk.k = R;
  kp.sk.k = z;

  kp.pk.i = d;
  kp.sk.i = st;
}

vector<string> PQ_Sig(string m, key sk){
  if(sk.i > (bits(sk.z) / t)){
    exit();
  }

  string h = hash(m);

  vector<string> subh;
  vector<string> sigma;
  vector<string> s;
  vector<string> v;
  vector<int> i;

  int st = sk.i;

  int len = log2(t);

  for(int j = 0; j < h.length(); j += len){
    subh.push(h.substr(j, len));
  }

  for(int j = 0; j < k; j++){
    i.push((st - 1) * t + subh.at(j));
  }

  for(int j = 0; j < k; j++){
    h = hash(z||i.at(j));
    s.push(h);
    v.push(hash(h));
  }
}

int PQ_Ver(string m, vector<string> sigma, key pk){

}

int main(){
  int kappa = 128;
  int k = 32;
  int t = 1024;

}
