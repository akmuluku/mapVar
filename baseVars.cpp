// playing with class inheritance
// we want a system class to inherit variable things

#include "mapVars.h"

// Derived class
class SomeSystem: public mapVars {
public:
  SomeSystem() {
    model = NULL;
   };
public:
  const char *model;
  float fval; 
  int ival;
  
};

int main() {
  SomeSystem mySystem;
  // This creates the mapVar object map 
  mySystem.addVar("model",&mySystem.model, "some model");
  mySystem.addVar("fval",&mySystem.fval, 4.56);
  mySystem.addVar("ival",&mySystem.ival, 2345);

  mySystem.showVars();
  cJSON * cj = mySystem.getCj();
  char * foo = cJSON_Print(cj);
  cout << " this is the cJson"<<endl;
  cout << foo << endl;
  mySystem.ival = 1234;
  cout << " changed ival " << endl;
  mySystem.showVars();
  
  //next to parse a cJSON object
  mySystem.parseCj(cj);
  cout << " after parseCj " << endl;

  mySystem.showVars();
  
  return 0;
}
