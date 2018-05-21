#include <iostream>
#include <vector>
#include <string>

using namespace std;

class MerkleTree{
  vector<string> v;
  string root;
public:
  MerkleTree(){

  }

  string Form(vector<string> v){
    int i = 0;
    int j;
    sha sh;
    Big h;
    char hash[20];
    char hashStr[200];


    while(v.size() > 1){
      if(v.size() % 2){
        v.push(v.at(v.side() - 1));
      }

      for(i = 0; i < v.size(); i++){
        shs_init(&sh);

        for(j = 0; j < v.at(i).size(); j++){
          shs_process(sh, v.at(i)[j]);
        }
        for(j = 0; j < v.at(i+1).size(); j++){
          shs_process(sh, v.at(i+1)[j]);
        }

        shs_hash(sh, hash);

        h.from_binary(20, hash);
        cotstr(h, hashStr);

        v.pop(i+1);
        v.at(i) = hashStr;
      }
    }
    return v.at(0);
  }

  vector<string> Path(string v){
    

  }

  int Verify(string R, vector<string> p, string v){
    int i, j;
    sha sh;
    Big h;
    char hash[20];
    char hashStr[200];

    while(p.size() > 1){
      for(i = 0; i < p.size(); i += 2){
        shs_init(&sh);

        for(j = 0; j < p.at(i).size(); j++){
          shs_process(sh, p.at(i)[j]);
        }
        for(j = 0; j < p.at(i+1).size(); j++){
          shs_process(sh, p.at(i+1)[j]);
        }

        shs_hash(sh, hash);

        h.from_binary(20, hash);
        cotstr(h, hashStr);

        p.pop(i+1);
        p.at(i) = hashStr;
      }
    }

    return p.at(0) == v;
  }

private:

}
