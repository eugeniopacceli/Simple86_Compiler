/* Simple86_Linker main
 *
 * Entry point for a program that implements the Simple86 machine.
 * Specification of that machine is defined in "TP1 - Software Básico.pdf"
 *
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include "Linker.h"

using namespace std;

// Output messages in case of error.
class MainMessages{
    public:
        const static string noSource;
        const static string badInput;
        const static string badIO;
};

const string MainMessages::noSource = "Args must contain at least the address of the source code to be compiled.";
const string MainMessages::badInput = "The arguments are not in the expected format.";
const string MainMessages::badIO = "Could not open or create files. An error has occurred while performing required IO operations.";

/* ------------------------------------------------------------------------
* int main(int argc, char* argv[])
* Accepts arguments in different orders, initializes the compiler based on
* those.
* ------------------------------------------------------------------------ */
int main (int argc, char *argv[]){
    bool verboseEnabled = false;
    ofstream* output;
    Linker* comp;
    vector<string> inputFiles;

    if(argc < 3){
        cerr << MainMessages::noSource;
        exit(EXIT_FAILURE);
    }
    
    output = new ofstream(argv[1],ios::binary);

    for(int i = 2; i < argc; i++){
        if(strcmp(argv[i],"-v") == 0){
            verboseEnabled = true;
        }else{
            inputFiles.push_back(string(argv[i]));
        }
    }

    // Are the files ok?
    if(output->is_open()){
        comp = new Linker(inputFiles, output, verboseEnabled);
        comp->link();
    }else{
        cerr << MainMessages::badIO;
        exit(EXIT_FAILURE);
    }

    delete comp;
    delete output;
    return EXIT_SUCCESS;
}