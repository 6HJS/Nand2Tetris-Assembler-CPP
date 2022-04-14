#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>

#include "Parser.hpp"
#include "SymbolTable.hpp"

using namespace std;

SymbolTable MyST;
Parser MyParser;

int main(int argc, char const *argv[]) {
    string fnameIn;
    string fnameOut;
    ifstream fIn;
    ofstream fOut;

    queue<string> temp1;
    queue<string> temp2;

    /* handling parameters*/
    if (argc < 2) {
        throw runtime_error(
            "main(): plseas specify input file MyAssembler *.asm [*.hack]");
    } else {
        fnameIn = string(argv[1]);
        // check the input file is a .asm?
        if (fnameIn.find(".asm") == string::npos)
            throw runtime_error("main(): input fime must be .asm");
        fIn.open(fnameIn);
        // check the specified file is able to open?
        if (!fIn.is_open()) {
            throw runtime_error("main(): unable to open file " + fnameIn);
        }

        if (argc == 2) {
            string::size_type idx = fnameIn.find_last_of('.');  // find the base name
            fnameOut = fnameIn.substr(0, idx) + ".hack";        // change to .hack
        } else if (argc == 3) {
            fnameOut = string(argv[2]);
        } else {
            throw runtime_error("main(): too many parameters");
        }
        fOut.open(fnameOut);
        if (!fOut.is_open()) {
            throw runtime_error("main(): unable to open file " + fnameOut);
        }

        cout << "output to " << fnameOut << endl;

        /* Pre pass: read source file clean-up the code */
        cout << "Pre-pass" << endl;
        string line;
        /* read each line from the .asm file */
        while (getline(fIn, line)) {
            cout << line << endl;
            /* remove comments */
            string::size_type idx = line.find("//");  // find start of "//"
            string lineRmComm = line.substr(0, idx);
            if (lineRmComm.size() == 0) continue;  // skip empty line
            /* remove spaces */
            string::iterator str_iter = remove(lineRmComm.begin(), lineRmComm.end(), ' ');
            lineRmComm.erase(str_iter, lineRmComm.end());
            temp1.push(lineRmComm);  // push to queue
        }

        /* First pass read lables build symbol table*/
        cout << "First-pass" << endl;
        while (!temp1.empty()) {
            line = temp1.front();
            temp1.pop();
            cout << line << endl;
            /* Is this line a L-instruction (Label) */
            if (line.find('(') != string::npos && line.find(')') != string::npos) {
                string::size_type idx_L = line.find_last_of('(');
                string::size_type idx_R = line.find_first_of(')');
                line = line.substr(idx_L + 1, idx_R - 1);
                MyST.addLabel(line);
                continue;
            } else {
                MyST.incLabelCounter();
            }
            temp2.push(line);
        }

        /* Second pass Translate instructions */
        cout << "Second-pass" << endl;
        while (!temp2.empty()) {
            line = temp2.front();
            temp2.pop();
            cout << line << endl;
            string macCode = MyParser.parseInst(line);
            cout << macCode << endl;
            fOut << macCode << endl;
        }
        cout << "second pass finished" << endl;
        fIn.close();
        fOut.close();
        cout << "file closed" << endl;
    }
    return 0;
}
