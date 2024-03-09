#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

// for U type instruction inheriting instuctions class publicly
class U : public INST
{
public:
    bitset<5> rd;   // to store destination register value
    bitset<20> imm; // to store immediate field value

    // function to fetch rd,imm fields of the I type instruction of type rd,rs1,imm
    void decode(string stmt, int i)
    {

        // fetching rd
        while (stmt[i] != 'x')
        {
            i++;
        }
        i++;
        string val = "";
        while (stmt[i] != ' ' and stmt[i] != ',')
        {
            val += stmt[i];
            i++;
        }
        rd = stoi(val);

        // fetching imm
        while (!isdigit(stmt[i]))
        {
            i++;
        }
        string immVal = "";
        while (i < stmt.size() and stmt[i] != ' ')
        {
            immVal += stmt[i];
            i++;
        }
        imm = stoi(immVal);

        return;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    void binary()
    {
        int i = 0;
        while (i < 7)
        {
            inst[i] = opcode[i];
            i++;
        }
        while (i < 12)
        {
            inst[i] = rd[i - 7];
            i++;
        }
        while (i < 32)
        {
            inst[i] = imm[i - 12];
            i++;
        }
        return;
    }

    // lui instruction
    void lui()
    {
        opcode = 55;
    }

    // auipc instruction
    void auipc()
    {
        opcode = 23;
    }
};