#include <zmq.hpp>
#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;
struct node{
  char c;
  int count;
  std::string ID;
  node* left;
  node* right;
};

struct node* newNode(char newC, int c){
  struct node* node = (struct node*) malloc(sizeof(struct node));

  node->c = newC;
  node->count = c;
  node->left = NULL;
  node->right = NULL;

  return node;
}

struct node* newNode(char newC, int count, struct node* l, struct node* r){
  struct node* node = (struct node*) malloc(sizeof(struct node));

  node->c = newC;
  node->count = count;
  node->left = l;
  node->right = r;

  return node;
}

string encodeTree(struct node *n, string route){
  if(n->left == NULL){
    if(n->right == NULL){
      return route + n->c;
    }
    else{
      return encodeTree(n->right, route + n->c + route);
    }
  }
  else{
    return encodeTree(n->left, "0" + route) + encodeTree(n->right, "1" + route);
  }
}

string encodeMessage(string m, string t){
  string e;
  e = "";
  int pos, j;

  for(int i = 0; i < m.length(); i++){
      // cout << m[i] << endl;
      pos = t.find(m[i]);

      j = pos - 1;
      // cout << t[j];
      while((t[j] == '1' || t[j] == '0')){
        j--;
        if(j < 0)
          break;
      }
      // cout << endl;
      // cout << pos << " " << j << " " << t.substr(j + 1, pos - (j + 1)) << endl;
      e += t.substr(j + 1, pos - (j + 1));
  }

  return e;
}

bool compNodes(struct node* a, struct node* b){
  return a->count < b->count;
}

struct node* buildTree(vector<node*> t){

  while(t.size() > 1){
    struct node* a = newNode(t.at(0)->c, t.at(0)->count, t.at(0)->left, t.at(0)->right);
    struct node* b = newNode(t.at(1)->c, t.at(1)->count, t.at(1)->left, t.at(1)->right);

    t.erase(t.begin());
    t.erase(t.begin());

    struct node* n = (struct node*) malloc(sizeof(struct node));
    n->count = a->count + b->count;
    if(a->count < b->count){
      n->left = b;
      n->right = a;
    }
    else{
      n->right = b;
      n->left = a;
    }

    t.push_back(n);
    // cout << n->count << " " << n->left->c << endl;
    sort(t.begin(), t.end(), compNodes);
  }

  return t.at(0);
}

string compress(string m){
  vector<int> count(256);
  vector<node*> nodes;
  struct node* n;
  int i;

  for(i = 0; i < m.length(); i++){
    count[(int)m[i]]++;
  }

  for(i = 0; i < 256; i++){
    if(count.at(i) > 0){
      n = newNode((char) i, count.at(i));
      nodes.push_back(n);
    }
  }

  sort(nodes.begin(), nodes.end(), compNodes);
  struct node *tree = buildTree(nodes);

  string t = encodeTree(tree, "");
  return t + ";" + encodeMessage(m, t);
}

struct node* decodeBranch(string b, struct node* n){
  // cout << b.substr(0, b.length()) << endl;
  if(n == NULL){
    n = newNode(' ', 0);
  }
  if(b[0] == '1'){
    n->right = decodeBranch(b.substr(1, b.length()), n->right);
  }
  else if(b[0] == '0'){
    n->left = decodeBranch(b.substr(1, b.length()), n->left);
  }
  else{
    n->c = b[0];
  }

  return n;
}

struct node* decodeTree(string t){
  struct node *root = newNode(' ', 0);
  struct node *it;

  it = root;
  for(int i = 0; i < t.length(); i++){
    if(t[i] == '0'){
      if(!it->left){
        it->left = newNode(' ', 0);
      }
      it = it->left;
    }
    else if(t[i] == '1'){
      if(!it->right){
        it->right = newNode(' ', 0);
      }
      it = it->right;
    }
    else{
      // cout << t[i]<< endl;
      it->c = t[i];
      it = root;
    }
  }

  return root;
}

string decompress(string em){
  string t, e, m;
  int i = 0;

  while(em[i] != ';'){
    i++;
  }

  t = em.substr(0, i);
  e = em.substr(i + 1, m.length() - (i + 1));
  // cout << e << endl;
  struct node *tNode = decodeTree(t);
  struct node *it = tNode;

  m = "";
  for(i = 0; i < e.length(); i++){
    cout << e[i];
    if(e[i] == '1'){
      if(it->right)
        it = it->right;
      else{
        cout << it->c << endl;
        m += it->c;
        it = tNode->right;
      }
    }
    else{
      if(it->left)
        it = it->left;
      else{
        cout << it->c << endl;
        m += it->c;
        it = tNode->left;
      }
    }
  }

  return m;
}

string dispersal(string V, int l, int t){

}

string recover(string V, int l, int t){

}

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);
    string m, n;
    /*std::cout << "Connecting to hello world server…" << std::endl;
    socket.connect ("tcp://localhost:5555");

    //  Do 10 requests, waiting each time for a response
    for (int request_nbr = 0; request_nbr != 10; request_nbr++) {
        zmq::message_t request (5);
        memcpy (request.data (), "Hello", 5);
        std::cout << "Sending Hello " << request_nbr << "…" << std::endl;
        socket.send (request);

        //  Get the reply.
        zmq::message_t reply;
        socket.recv (&reply);
        std::cout << "Received World " << request_nbr << std::endl;
    }*/
    m = compress("Hello World");
    cout << m << endl;
    n = decompress(m);
    cout << n << endl;
    return 0;
}
