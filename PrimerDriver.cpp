/************************************************
  Copyright (c) 2012, The University of Tennessee Research Foundation
  All Rights Reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this list of conditions and the following   disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************/

#include <fstream>
#include <iostream>
#include <cstdlib>
#include "Primer.h"
#include "Search.h"

using namespace std;


int PrimerSearch(vector<Primer_set *> &primers, int filec, char ** files, int max_length){
  ifstream ifs;
  string line, tmp, hit, id;
  int f, r, i, j, k;
  Primer_set *p;
  char buff[100];

  for(int i = 0; i < primers.size(); i++){
    p = primers[i];
    for(j = 0; j < p->forward.size(); j++){
      p->forward[j] = toupper(p->forward[j]);
    }
    for(j = 0; j < p->reverse.size(); j++){
      p->reverse[j] = toupper(p->reverse[j]);
    }

    p->Rforward = p->forward;  //Copies
    p->Rreverse = p->reverse;

    p->Rforward = complement(flip(p->Rforward));  //Build reverse-complement of Forward
    p->Rreverse = complement(flip(p->Rreverse));  //Build reverse-complement of Reverse

    //Convert all to bitstrings for bitwise comparison
    p->forward = convert(p->forward);
    p->reverse = convert(p->reverse);
    p->Rforward = convert(p->Rforward);
    p->Rreverse = convert(p->Rreverse);
  }

  for(j = 0; j < filec; j++){   //Files loop
    ifs.open(files[j]);
    if(ifs.fail()){
      perror(files[j]);
    }
    fprintf(stdout, "Searching %50s\r", files[j]);
    fflush(stdout);


    while(getline(ifs, tmp)){  //line loop

      if(tmp.find(">") != string::npos)
      {
        id = tmp.substr(1, tmp.find("/")-2);  //test for info line
        i = 0;
      }else{
        line = convert(tmp);                      //convert sequence to bitstring for comparison

        for(k = 0; k < primers.size(); k++){
          p = primers[k];
          if(p->forward != ""){
            f = Sfind(line, p->forward, 0);
            while(f != string::npos){                   //Forward find Loop
              if(p->Rreverse != ""){
                r = Sfind(line, p->Rreverse, f);
                while(r != string::npos){                //Rreverse find Loop
                  hit = tmp.substr(f, r-f+p->Rreverse.size());   //Pull hit from unconverted sequence
                  fprintf((r-f> max_length)? p->non_ofs: p->ofs,
                    ">%s-%d-FR-%s\n%s\n\n", files[j], 1+i++, id.c_str(),
                    hit.c_str());
                  fflush(p->ofs);
                  r = Sfind(line, p->Rreverse, r+1);
                }
              }
              r = Sfind(line, p->Rforward, f);            
              while(r != string::npos){                //Rreverse find Loop
                hit = tmp.substr(f, r-f+p->Rforward.size());   //Pull hit from unconverted sequence
                fprintf((r-f> max_length)? p->non_ofs: p->ofs,
                  ">%s-%d-FF-%s\n%s\n\n", files[j], 1+i++, id.c_str(),
                  hit.c_str());
                fflush(p->ofs);
                r = Sfind(line, p->Rforward, r+1);
              }
              f = Sfind(line, p->forward, f+1);
            }
          }

          //Search for reverse string
          if(p->reverse != ""){
            f = Sfind(line, p->reverse, 0);
            while(f != string::npos){      //Rforward find Loop
              if(p->Rforward != ""){
                r = Sfind(line, p->Rforward, f);
                while(r != string::npos){              //Reverse find Loop
                  hit = tmp.substr(f, r-f+p->Rforward.size());  //Pull hit from unconverted sequence
                  hit = complement(flip(hit));               //complement and flip it 
                  fprintf((r-f> max_length)? p->non_ofs: p->ofs,
                    ">%s-%d-RF-%s\n%s\n\n", files[j], 1+i++, id.c_str(),
                    hit.c_str());
                  fflush(p->ofs);
                  r = Sfind(line, p->Rforward, r+1);
                }
              }

              r = Sfind(line, p->Rreverse, f);
              while(r != string::npos){                //Rreverse find Loop
                hit = tmp.substr(f, r-f+p->reverse.size());   //Pull hit from unconverted sequence
                hit = complement(flip(hit));
                fprintf((r-f> max_length)? p->non_ofs: p->ofs, 
                  ">%s-%d-RR-%s\n%s\n\n", files[j], 1+i++, id.c_str(),
                  hit.c_str());
                fflush(p->ofs);
                r = Sfind(line, p->Rreverse, r+1);
              }
              f = Sfind(line, p->reverse, f+1);
            }
          }
        }
      }
    }

    ifs.close();
  }
  printf("\n");
  return 0;
}
