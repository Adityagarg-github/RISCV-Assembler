#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

class SB : public INST
{
public:
    bitset<12> imm;  // to store imm value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<5> rs2;   // to store source register 2 value
    int imm_dec;

    // function to fetch rs1,imm,rs2 fields of the SB type instruction
    // uses unordered map for finding id
    void decode(string stmt, int i, unordered_map<string, int> m, int currPC)
    {

        // fetching rs1
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
        rs1 = stoi(val);
        // rs1 = registerValueInDecimal;

        // fetching rs2
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
        rs2 = stoi(val);
        // rs2= registerValueInDecimal;

        // fetching imm
        while (stmt[i] != ',')
        {
            i++;
        }
        i++;
        while (stmt[i] == ' ')
            i++;
        string Label = "";
        while (i < stmt.size() and stmt[i] != ' ')
        {
            Label += stmt[i];
            i++;
        }
        auto it = m.find(Label);
        if (it != m.end())
        {
            int labelPc = it->second;
            imm = (labelPc - currPC) / 2;
            imm_dec = labelPc - currPC;
        }
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
        inst[i] = imm[10]; // imm[11]
        i++;
        while (i < 12)
        {
            inst[i] = imm[i - 8]; // imm[4:1]
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
        while (i < 31)
        {
            inst[i] = imm[i - 21]; // imm[10:5]
            i++;
        }
        inst[i] = imm[11]; // imm[12]
        return;
    }

    // beq instruction
    void beq()
    {
        opcode = 99;
        func3 = 0;
    }

    // bne instruction
    void bne()
    {
        opcode = 99;
        func3 = 1;
    }

    // bge instruction
    void bge()
    {
        opcode = 99;
        func3 = 5;
    }

    // blt instruction
    void blt()
    {
        opcode = 99;
        func3 = 4;
    }
};