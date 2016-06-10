#ifndef SIMULA_COMPILER
#define SIMULA_COMPILER 1

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>

using namespace std;

#define LOW_MASK  0000000011111111
#define HIGH_MASK 1111111100000000
#define MEMORY_LIMIT 1000

// Compiler module for Simple86
class Compiler{

    public:
        Compiler(ifstream& input, ofstream& output, bool verboseEnabled){
            input.close();
            output.close();
        }

        int compile(){
            string str;
            queue<string> terms;
        	while(getline(input, str)){
        		istringstream iss(str);
        		string buffer;
        		while(iss >> buffer){
        			cout << buffer << "\n";
        			terms.push(buffer);
        		}
        		
        		if(terms.front()[0] == '_'){ //label
        			//put label in the table
        		}
        		else{ //opcode
        			if(terms.front() == "MOV"){
        				//binarycode << "1 em binario em 16bits?"
        				//binarycode << (char)0 << (char)1 ?
        				//update memPos
        				terms.pop();
        				if(terms.front()[0] > 64 && terms.front()[0] < 104){ //register
        					if(terms.front()=="AX")
        
        					else if(terms.front()=="AL")
        
        					else if(terms.front()=="AH")
        
        					else if(terms.front()=="BX")
        
        					else if(terms.front()=="BL")
        
        					else if(terms.front()=="BH")
        
        					else if(terms.front()=="CX")
        
        					else if(terms.front()=="CH")
        
        					else if(terms.front()=="CL")
        
        				}else{ //memory
        					//turn the address into binary?
        				}
        
        				terms.pop();
        				if(terms.front()[0] > 64 && terms.front()[0] < 104){ //register
        					if(terms.front()=="AX")
        
        					else if(terms.front()=="AL")
        
        					else if(terms.front()=="AH")
        
        					else if(terms.front()=="BX")
        
        					else if(terms.front()=="BL")
        
        					else if(terms.front()=="BH")
        
        					else if(terms.front()=="CX")
        
        					else if(terms.front()=="CH")
        
        					else if(terms.front()=="CL")
        
        				}
        				else if(terms.front().length == 5){ //immediate length=5?
        					
        				}
        				else{//memory length!=5?
        
        				}
        			}
        			}else if(terms.front() == "ADD"){
        
        			}//repeat and adjust for each instruction
        		}
        
        	}
            
            return 1;
        }

        ostream& write_word(ostream& out, int16_t value){
            for (uint size = sizeof(int16_t); size > 0; size--)
                out.put(static_cast<char>(value & 0xFF));
                value >>= 8;
            return out;
        }

        istream& read_word(istream& in, int16_t value){
            uint size = 0;
            for (value = 0; size < sizeof(int16_t); size++)
                value |= in.get() << (8 * size);
            return in;
        }
};

#endif
