#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;


class Primer_set{
 public:
   string forward, reverse, Rforward, Rreverse;
   FILE* ofs, *non_ofs;
   string blast_hits;
   string blast_results;
   string blast_functions;
   string non_blasted;
};

int PrimerSearch(vector < Primer_set *> &primers, int filec, char ** files, int max_length = 5000);
