/************************************************
Copyright (c) 2012, The University of Tennessee Research Foundation
All Rights Reserved.
 
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
Redistributions of source code must retain the above copyright notice, this list of conditions and the following   disclaimer.
 
Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************/

#include <string>
#include <iostream>
#include <assert.h>
#include "Primer.h"

using namespace std;


const string comparray=  "TVGHZZCDZZMZKNZZZYSAZBWNRZ";
const string array = "BOCNZZELZZMZDPZZZFGIZHJPKZ";
string complement(string s)  //converts s to its complement
{
  for(int i = 0; i < s.size(); i++){
    s[i] = comp2((char) s[i]);
  }
  return s;
}


char comp2(char c)
{
  return comparray[c-'A'];
}

string flip(string s)  //reverses the order of the string
{
  string tmp;

  tmp = "";
  for(int i = 0; i < s.size(); i++){
    tmp = s[i] + tmp;
  }
  return tmp;
}


string convert(string s)
{
  for(int i = 0; i < s.size(); i++){
    s[i] = (array[s[i]-'A'] - 'A');
  }
  return s;
}


int Sfind(string s1,string s2,int in){  //normal string find with comp function as bitwise And instead of exact match
  int i, j;

  for(i = in; i < s1.size(); i++){  //run through search string
    for(j = 0; j < s2.size(); j++){ //run through query string
      if( (i+j) < s1.size() && ((char)(s1[i+j] & s2[j]) == (char)s1[i+j])) continue;
      else break;
    }
    if(j == s2.size()) return i;
  }
  return -1;
}
