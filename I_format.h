#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

// for I type instruction, inheriting instuctions class publicly
class I : public INST
{
public:
    bitset<5> rd;    // to store destination register value
    bitset<3> func3; // to store func3 value
    bitset<5> rs1;   // to store source register 1 value
    bitset<12> imm;  // to store immediate field value
    int immVal2;
    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,rs1,imm
    void decode2(string stmt, int i)
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

        // fetching rs1
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

        // fetching imm
        while (!isdigit(stmt[i]))
        {
            if (stmt[i] == '-')
            {
                break;
            }
            i++;
        }
        string immVal = "";
        while (i < stmt.size() and stmt[i] != ' ')
        {
            immVal += stmt[i];
            i++;
        }
        int immdec = stoi(immVal);
        immVal2 = immdec;
        if (immdec < 0)
        {
            immdec = (1 << 12) + immdec;
        }
        imm = immdec;
        return;
    }

    // function to fetch rd,rs1,imm fields of the I type instruction of type rd,imm(rs1)
    void decode3(string stmt, int i)
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
            if (stmt[i] == '-')
            {
                break;
            }
            i++;
        }
        string immVal = "";
        while (stmt[i] != ' ' and stmt[i] != '(')
        {
            immVal += stmt[i];
            i++;
        }
        int immdec = stoi(immVal);
        immVal2 = immdec;
        if (immdec < 0)
        {
            immdec = (1 << 12) + immdec;
        }
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
    // imm rs1 funct3 rd opcode
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
        while (i < 32)
        {
            inst[i] = imm[i - 20];
            i++;
        }
        return;
    }

    // andi instruction
    void andi()
    {
        opcode = 19;
        func3 = 7;
    }

    // ori instruction
    void ori()
    {
        opcode = 19;
        func3 = 6;
    }

    // addi instruction
    void addi()
    {
        opcode = 19;
        func3 = 0;
    }

    // lh instruction
    void lh()
    {
        opcode = 3;
        func3 = 1;
    }

    // lw instruction
    void lw()
    {
        opcode = 3;
        func3 = 2;
    }

// lb instruction
    void lb()
    {
        opcode = 3;
        func3 = 0;
    }

    // ld instruction
    void ld()
    {
        opcode = 3;
        func3 = 3;
    }

    // jalr instruction y
    void jalr()
    {
        opcode = 103;
        func3 = 0;
    }
};
