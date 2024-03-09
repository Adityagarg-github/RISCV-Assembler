#include <bits/stdc++.h>
#include <fstream>
#define ll long long int
using namespace std;

// class for UJ type INST inheriting instuctions class publicly
class UJ : public INST
{
public:
    bitset<5> rd;   // to store destination register value
    bitset<20> imm; // to store imm value
    int imm_dec;

    // function to fetch rd,imm fields of the UJ type instruction
    void decode(string stmt, int i, unordered_map<string, int> m, int currPC)
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
        // rd = registerValueInDecimal;

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
        while (i < 12)
        {
            inst[i] = rd[i - 7];
            i++;
        }
        while (i < 20)
        {
            inst[i] = imm[i - 1]; // imm[19:12]
            i++;
        }
        inst[i] = imm[10]; // imm[11]
        i++;
        while (i < 31)
        {
            inst[i] = imm[i - 21]; // imm[10:1]
            i++;
        }
        inst[i] = imm[19]; // imm[20]

        return;
    }

    // jal instruction
    void jal()
    {
        opcode = 111;
    }
};