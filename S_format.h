#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

// for S type instruction inheriting instuctions class publicly
class S : public INST
{
public:
    bitset<12> imm;  // to store imm value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<5> rs2;   // to store source register 2 value

    // function to fetch rs1,imm,rs2 fields of the S type instruction
    void decode(string stmt, int i)
    {
        // fetching rs2
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
        rs2 = stoi(val);

        // fetching imm
        while (!isdigit(stmt[i]))
        {
            i++;
        }
        string immVal = "";
        while (stmt[i] != ' ' and stmt[i] != '(')
        {
            immVal += stmt[i];
            i++;
        }
        int immdec = stoi(immVal);
        imm = immdec;

        // fetching rs1
        while (stmt[i] != 'x')
        {
            i++;
        }
        i++;
        val = "";
        while (stmt[i] != ' ' and stmt[i] != ')')
        {
            val += stmt[i];
            i++;
        }
        rs1 = stoi(val);

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
            inst[i] = imm[i - 7]; // imm[4:0]
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
            inst[i] = imm[i - 20]; // imm[11:5]
            i++;
        }
        return;
    }

    // sb instruction
    void sb()
    {
        opcode = 35;
        func3 = 0;
    }

    // sw instruction
    void sw()
    {
        opcode = 35;
        func3 = 2;
    }

    // sd instruction
    void sd()
    {
        opcode = 35;
        func3 = 3;
    }

    // sh instruction
    void sh()
    {
        opcode = 35;
        func3 = 1;
    }
};