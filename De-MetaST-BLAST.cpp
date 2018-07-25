/************************************************
  Copyright (c) 2012, The University of Tennessee Research Foundation
  All Rights Reserved.

  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

  Redistributions of source code must retain the above copyright notice, this list of conditions and the following   disclaimer.

  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ***************************************************/

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <time.h>
#include "Search.h"
#include "Primer.h"

using namespace std;


//Set to 1 for Remote blast hosted by NCBI
//Set to 0 for a Local blast using your database
bool RemoteBlast = 1;

//Set BlastDatabase to the full path to the Database
//if querying a Local blast database
#define BlastDatabase "nr" 

#define BlastxBin "blastx"
#define max_date 15


int main(int argc, char ** argv){
  bool DEFAULT = false;
  char * args[14];
  int rv(0), pid, status;
  int fd2;
  fstream ifs;
  string fn;
  string Forward, Reverse;
  vector <Primer_set *> primers;
  Primer_set *p;
  char buff[100];
  time_t the_time;
  char date[max_date];

  date[0] = '\0';

  the_time = time(NULL);
  if(the_time != -1){
    strftime(date, max_date, "%d%b%y_%H:%M", localtime(&the_time));
  }

  if(argc < 2){
    fprintf(stderr, "USAGE: De-MetaST Databases ...\n");
    exit(1);
  }

  cout << "Input Filename for Primers (press Enter to use Primers.txt)\n";
  getline(cin, fn);
  if(fn == "NONE"){
    p = new Primer_set;
    cout << "Input Forward Primer (5' to 3')" << endl;
    cin >> Forward;
    cout << "Input Reverse Primer (5' to 3')" << endl;
    cin >> Reverse;
    cout << "Forward Primer: " << Forward << endl;
    cout << "Reverse Primer: " << Reverse << endl;
    primers.push_back(p);
    p->forward = Forward;
    p->reverse = Reverse;
    PrimerSearch(primers, argc-1, ++argv);
  }
  else{
    if(fn == ""){
      fn = "Primers.txt";
    }
    printf("Opening: %s\n", fn.c_str());
    ifs.open(fn.c_str());
    if(ifs.fail()){
      perror(fn.c_str());
    }
    while(getline(ifs, Forward)){
      if(Forward != ""){
        p = new Primer_set;
        primers.push_back(p);

        getline(ifs, Reverse);
        p->forward = Forward;
        p->reverse = Reverse;

        cout << "Primer Set: " << primers.size() << endl;
        cout << "Forward Primer: " << p->forward << endl;
        cout << "Reverse Primer: " << p->reverse << endl << endl;

      }
    }
    for(int i = 0; i < primers.size(); i++){
      p = primers[i];
      sprintf(buff, "De-MetaST_hits_%s(%dof%d).fasta",
        date, i+1, (int) primers.size());
      p->ofs = fopen(buff, "w");
      p->blast_hits = buff;
      sprintf(buff, "non-blasted_De-MetaST_hits_%s(%dof%d).fa",
        date, i+1, (int) primers.size());
      p->non_ofs = fopen(buff, "w");
      p->non_blasted = buff;
      sprintf(buff, "BLASTx_results_%s(%dof%d).xml",
        date, i+1, (int) primers.size());
      p->blast_results = buff;
      sprintf(buff, "De-MetaST-BLAST_functions_%s(%dof%d).tsv",
        date, i+1, (int) primers.size());
      p->blast_functions = buff;
    }
    PrimerSearch(primers, argc-1, argv+1);
  }

  for(int i = 0; i < primers.size(); i++){
    p = primers[i];

    //Local BLASTx
    if(!RemoteBlast){
      args[0] = (char *) "blastx";
      args[1] = (char *) "-db";
      args[2] = (char *) BlastDatabase; 
      args[3] = (char *) "-query";
      args[4] = (char *) p->blast_hits.c_str();
      args[5] = (char *) "-max_target_seqs";
      args[6] = (char *) "10";
      args[7] = (char *) "-outfmt";
      args[8] = (char *) "5";
      args[9] = (char *) "-out";
      args[10] = (char *) p->blast_results.c_str();
      args[11] = (char *) "-num_threads";
      args[12] = (char *) "8";
      args[13] = (char *) NULL;
      cout << "Local ";
    }else
      //Remote BLASTx
      if(RemoteBlast){
        args[0] = (char *) "blastx";
        args[1] = (char *) "-db";
        args[2] = (char *) "nr"; 
        args[3] = (char *) "-query";
        args[4] = (char *) p->blast_hits.c_str();
        args[5] = (char *) "-max_target_seqs";
        args[6] = (char *) "10";
        args[7] = (char *) "-outfmt";
        args[8] = (char *) "5";
        args[9] = (char *) "-out";
        args[10] = (char *) p->blast_results.c_str();
        args[11] = (char *) "-remote";
        args[12] = (char *) NULL;
        cout << "Remote ";
      }

    cout << "BLASTx running ..." << endl;
    pid = fork();
    if(pid == 0){
      rv = execvp(BlastxBin, args);
      perror("De-MetaST-BLAST");
      exit(1);
    }
    rv = wait(&status);
    cout << "BLASTx finished" << endl;

    //Build Tab-delimited Table from XML
    pid = fork();
    if(pid == 0){
      fd2 = open(p->blast_functions.c_str(), O_WRONLY | O_TRUNC | O_CREAT,
        0644);
      if(dup2(fd2, 1) != 1){
        perror("De-MetaST-BLAST.cpp: dup2(f2, 1)");
        exit(1);
      }
      close(fd2);		
      rv = execlp("BLASTxml_to_tsv", "BLASTxml_to_tsv",
        p->blast_hits.c_str(), p->blast_results.c_str(), (char *) NULL);
      exit(1);
    }
    rv = wait(&status);
  }
}
