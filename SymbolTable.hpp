#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <cstdint>    // this contains uint16_t
#include <map>        // indexable dictionary
#include <stdexcept>  // throw exception
#include <string>     // process c++ string
using namespace std;
class SymbolTable {
   private:
    /* the symble table storage space */
    map<string, uint16_t> table;
    int VariableCounter;
    int LabelCounter;

   public:
    SymbolTable();
    ~SymbolTable();
    void addLabel(string Label);
    void addVariable(string Var);
    uint16_t lookupSymbol(string Var);
    int incLabelCounter();
};

#endif  // SYMBOLTABLE_HPP
