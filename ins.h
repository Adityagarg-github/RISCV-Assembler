#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

class INST
{
public:
    bitset<32> inst;  // binary instruction code
    string hexinst;   // hexadecimal instruction code
    bitset<7> opcode; // opcode of the instruction

    // function to convert 32 - bit binary to hexadecimal
    void Hex()
    {
        hexinst = "0x"; // hexadecimal initialization

        for (int i = 31; i >= 0;)
        {
            // converting 4 bits at a time to hex
            int ans = 8 * inst[i] + 4 * inst[i - 1] + 2 * inst[i - 2] + inst[i - 3];
            i -= 4;
            if (ans < 10)
            {
                hexinst += ans + '0';
            }
            else
            {
                hexinst += ans - 10 + 'A';
            }
        }
    }
};

// class for id in instruction and labels
class id
{
public:
    string name; // id name
    int pc = 0;  
};