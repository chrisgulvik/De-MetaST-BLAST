/***********************************************************
Copyright (c) 2012, The University of Tennessee Research Foundation
All Rights Reserved.
 
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*************************************************************/

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cstdlib>

using namespace std;


class Hit{
  public:
    string eval, accn, defn;
};

class Query{
  public:
    string name, seq;
    vector <Hit *> hits;
};

int main(int argc, char **argv){
  ifstream ifs;
  ofstream ofs;
  string line, fn;
  string qrydefln, itmsg, hitnum, hitdefn, hiteval, hitaccn, hitscore, hitlen;
  map <string, Query*> q;
  map <string, Query*>::iterator qit;
  Query *query;
  Hit *hit;
  int state(1);

  fn = argv[1];
  ifs.open(argv[1]);
  if(ifs.fail()){
    perror(argv[1]);
    exit (1);
  }

  while(getline(ifs, line)){
    query = new Query;
    query->name = line.substr(1);
    getline(ifs, line);
    query->seq = line;
    q[query->name] = query;
    getline(ifs, line);
  }

  ifs.close();

  itmsg = "<Iteration_message>No hits found</Iteration_message>";
  qrydefln = "<Iteration_query-def>";
  hitdefn = "<Hit_def>";
  hiteval = "<Hsp_evalue>";
  hitaccn = "<Hit_accession>";
  fn = argv[2];

  ifs.open(argv[2]);
  if (ifs.fail()){
    perror(argv[2]);
    exit(1);
  }

  while(getline(ifs, line)){
    if(line.find(qrydefln) != string::npos){
      fn = line.substr(line.find(">")+1,
      	line.size()-qrydefln.size()-2 - (int)line.find(">"));
      qit = q.find(fn);
      query = qit->second;
    }else if(line.find(hitdefn) !=string::npos){
      hit = new Hit;
      hit->defn = line.substr(line.find(">")+1,
      	line.size()-hitdefn.size()-2 - (int)line.find(">"));
    }else if(line.find(hiteval) != string::npos){
      hit->eval = line.substr(line.find(">")+1,
      	line.size()-hiteval.size()-2 - (int)line.find(">"));
      query->hits.push_back(hit);
    }else if(line.find(hitaccn) != string::npos){
      hit->accn = line.substr(line.find(">")+1,
      	line.size()-hitaccn.size()-2 - (int)line.find(">"));
    }
  }

  for(qit = q.begin(); qit != q.end(); qit++){
    query = qit->second;
    for(int i = 0; i < query->hits.size(); i++){
      printf("%d\t%s\t%s\t%s\t%s\t%s\n", i+1,
        query->hits[i]->eval.c_str(),
        query->hits[i]->accn.c_str(),
        query->hits[i]->defn.c_str(),
        query->seq.c_str(),
        query->name.c_str());
    }
  }
ifs.close();
ofs.close();
q.clear();

return 0;
}
