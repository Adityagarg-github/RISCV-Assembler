#include <bits/stdc++.h>
#include <fstream>
//#include "ins.h"
#define ll long long int
using namespace std;

class R : public INST
{
public:
    bitset<5> rd;    // store destination register value
    bitset<3> func3; // store func3 value
    bitset<5> rs1;   // store source register 1 value
    bitset<5> rs2;   // store source register 2 value
    bitset<7> func7; // store func7 value

    // function to fetch rd,rs1,rs2 fields of the R type instruction
    // rd = registerValueInDecimal;
    void decode(string stmt, int i)
    {
        while (stmt[i] != 'x')
        {
            i++;
        }
        i++;

        // fetching rd
        string val = "";
        while (stmt[i] != ' ' and stmt[i] != ',')
        {
            val += stmt[i];
            i++;
        }
        rd = stoi(val); // string to int conversion

        // fetching rs1
        // rs1 = registerValueInDecimal;
        while (stmt[i] != 'x')
        {
            i++;
        }
        i++;
        val = "";
        while (stmt[i] != ' ' and stmt[i] != ',')
        {
            val += stmt[i];
            i++;
        }
        rs1 = stoi(val);

        // fetching rs2
        while (stmt[i] != 'x')
        {
            i++;
        }
        i++;
        val = "";
        while (i < stmt.size() and stmt[i] != ' ')
        {
            val += stmt[i];
            i++;
        }
        rs2 = stoi(val);

        return;
    }

    // function to make instruction to binary 32 bit format joining all different fields
    // func7 rs2 rs1 func3 rd opcode
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
        while (i < 15)
        {
            inst[i] = func3[i - 12];
            i++;
        }
        while (i < 20)
        {
            inst[i] = rs1[i - 15];
            i++;
        }
        while (i < 25)
        {
            inst[i] = rs2[i - 20];
            i++;
        }
        while (i < 32)
        {
            inst[i] = func7[i - 25];
            i++;
        }
        return;
    }

    // and instruction
    void And()
    {
        opcode = 51;
        func3 = 7;
        func7 = 0;
    }

    // add instruction
    void add()
    {
        opcode = 51;
        func3 = 0;
        func7 = 0;
    }

    // or instruction
    void Or()
    {
        opcode = 51;
        func3 = 6;
        func7 = 0;
    }

    // sll instruction
    void sll()
    {
        opcode = 51;
        func3 = 1;
        func7 = 0;
    }

    // slt instruction
    void slt()
    {
        opcode = 51;
        func3 = 2;
        func7 = 0;
    }

    // srl instruction
    void srl()
    {
        opcode = 51;
        func3 = 5;
        func7 = 0;
    }

    // sra instruction
    void sra()
    {
        opcode = 51;
        func3 = 5;
        func7 = 32;
    }

    // sub instruction
    void sub()
    {
        opcode = 51;
        func3 = 0;
        func7 = 32;
    }

    // xor instruction
    void Xor()
    {
        opcode = 51;
        func3 = 4;
        func7 = 0;
    }

    // DIv instruction
    void Div()
    {
        opcode = 51;
        func3 = 4;
        func7 = 1;
    }

    // mul instruction
    void mul()
    {
        opcode = 51;
        func3 = 0;
        func7 = 1;
    }

    // rem instruction
    void rem()
    {
        opcode = 51;
        func3 = 6;
        func7 = 1;
    }
};