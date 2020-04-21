// playing with class inheritance
// we want a system class to inherit variable things
// g++ -std=c++11 -o test baseVars.cpp -I/usr/local/lib -Wl,-rpath,/usr/local/lib -lcjson

#ifndef __MAPVARS_H
#define __MAPVARS_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include <unordered_map>
#include "cjson/cJSON.h"

using namespace std;

class Var {
  // TODO use template
public:
  Var(const char *vnam, float *fptr) {
    fvar = fptr;
    ivar = NULL;
    svar = NULL;
    vname = vnam;
  }
  
  Var(const char * vnam, int *iptr) {
    fvar = NULL;
    ivar = iptr;
    svar = NULL;
    vname = vnam;
  }
  
  Var(const char *vnam, const char **sptr) {
    fvar = NULL;
    ivar = NULL;
    svar = sptr;
    vname = vnam;
  }

  ~Var() {};
  char *getVal(char *tmp, int len) {
    if (fvar) ( snprintf(tmp, len, "%f" , *fvar));
    else if (ivar) ( snprintf(tmp, len, "%d" , *ivar));
    else ( snprintf(tmp, len, "%s" , *svar));
    
    return tmp;			      
  };
  cJSON *getCj() {
    cJSON * cj;
    if (fvar) ( cj = cJSON_CreateNumber(*fvar));
    else if (ivar) ( cj= cJSON_CreateNumber(*ivar));
    else (cj= cJSON_CreateString(*svar));
    
    return cj;			      
  };
  
  void setcJ(cJSON *cJ) {
    if (fvar) (*fvar = cJ->valuedouble);
    else if (ivar) ( *ivar = cJ->valueint);
    else {
      if (*svar) free ((void *)*svar);
      *svar = strdup(cJ->valuestring);
    }
  };
  
public:
  float *fvar;
  int *ivar;
  const char **svar;
  const char *vname;
};


int createcJMap(unordered_map <string , cJSON *> &mmap, cJSON * cj)
{
  if(!cj->child ) return -1;
  cj = cj->child;
  while (cj) {
    mmap.insert(make_pair(cj->string,cj));
    cj = cj->next;
  }
  return 0;
}




cJSON *findcJName(unordered_map <string , cJSON *> &mmap, const char *iname)
{
  cJSON *cJ = NULL;
  unordered_map <string , cJSON *> ::const_iterator it = mmap.find(iname);
  if ( it != mmap.end() ) {
    cJ = it->second;
  }
  return cJ;
}
// Vars class
// contains a list of vars and references
//This hides behind any system object but provides a string reference
//for each pointer.
// This allows an "instant"  conversion to and from cJSON

class mapVars {
public:
  unordered_map <string , Var *> myVars;
  void addVar(const char *vname, float *fptr, float fval) {
    myVars.insert(make_pair(vname, new Var(vname, fptr)));
    *fptr = fval;
  };
  
  void addVar(const char *vname, int *iptr, int ival) {
    myVars.insert(make_pair(vname, new Var(vname, iptr)));
    *iptr = ival;
  };

  void addVar(const char *vname, const char **sptr, const char *sval) {
    myVars.insert(make_pair(vname, new Var(vname, sptr)));
    *sptr = strdup(sval);
  };

  void showVars() {
    char tmp[64];
     cout << "mymap contains:\n";
     for ( auto it = myVars.begin(); it != myVars.end(); ++it ) {
       cout << " " << it->first << ":" << it->second->getVal(tmp,64) <<endl;
     }
     cout << endl;

  };
  cJSON *getCj() {
    cJSON *cj = cJSON_CreateObject();
    for ( auto it = myVars.begin(); it != myVars.end(); ++it ) {
      cJSON_AddItemToObject(cj,(const char *)it->first.c_str(),it->second->getCj());
     }
    return cj;
  };
  void parseCj(cJSON *cj) {
    // first turn the cJson Object into a map
    unordered_map <string , cJSON *> mycJ;
    createcJMap(mycJ, cj);
    for ( auto it = myVars.begin(); it != myVars.end(); ++it ) {
      cJSON *cJi  = findcJName(mycJ,(const char *)it->first.c_str());
      if(cJi)
         it->second->setcJ(cJi);
    }
    // then navigate through all the items in mYvars and pull them from the cJSON
  };
};
#endif
