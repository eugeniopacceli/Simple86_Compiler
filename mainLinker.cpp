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
    string outputName = "exec.sa";
    string inputName = "";
    ifstream* input;
    ofstream* output;
    Linker* comp;

    if(argc < 2){
        cerr << MainMessages::noSource;
        exit(EXIT_FAILURE);
    }
    
    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i],"-v") == 0){
            verboseEnabled = true;
        } else if(strcmp(argv[i],"-o") == 0){
            outputName = string(argv[i+1]);
            i++;
        } else{
            inputName = string(argv[i]);
        }
    }

    if(inputName.empty()){
        cerr << MainMessages::badInput;
        exit(EXIT_FAILURE);
    }

    input = new ifstream(inputName.c_str());
    output = new ofstream(outputName.c_str(),ios::binary);

    // Are the files ok?
    if(input->is_open() && output->is_open()){
        comp = new Linker(input, output, verboseEnabled);
        comp->link();
    }else{
        cerr << MainMessages::badIO;
        exit(EXIT_FAILURE);
    }

    delete comp;
    delete input;
    delete output;
    return EXIT_SUCCESS;
}