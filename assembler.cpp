#include <bits/stdc++.h>
#include <fstream>
#include "ins.h"
#include "R_format.h"
#include "I_format.h"
#include "S_format.h"
#include "SB_format.h"
#include "U_format.h"
#include "UJ_format.h"
#define ll long long int
using namespace std;

map<ll, string> instruction;
map<ll, bitset<8>> memory;
ll start = 1ll << 28, f;
vector<int> reg(32, 0);

// functions for individual instruction
//function toconvert 8 bits to hexadecimal
string hex2(bitset<8> &b)
{
    string s = "0x";
    int i = 7;
    while (i >= 0)
    {
        int ans = 8 * b[i] + 4 * b[i - 1] + 2 * b[i - 2] + b[i - 3];
        i -= 4;
        if (ans < 10)
        {
            s += ans + '0';
        }
        else
        {
            s += ans - 10 + 'A';
        }
    }
    return s;
}

//function to convert 32 bits intohexadecimal
string hex(bitset<32> &b)
{
    string s = "0x";
    int i = 31;
    while (i >= 0)
    {
        int ans = 8 * b[i] + 4 * b[i - 1] + 2 * b[i - 2] + b[i - 3];
        i -= 4;
        if (ans < 10)
        {
            s += ans + '0';
        }
        else
        {
            s += ans - 10 + 'A';
        }
    }
    return s;
}

// class to make machine code for whole program
class makeMC
{
public:
    bitset<32> pc = 0;            // to store pc
    string hexpc;                 // to store pc in hex
    unordered_map<string, int> m; // map that stores all labels along with their gotopc

    // function to increment pc by 4
    void incPC(bitset<32> &pc)
    {
        for (int i = 2; i < 32; i++)
        {
            if (pc[i] == 0)
            {
                pc[i] = 1;
                break;
            }
            else
            {
                pc[i] = 0;
            }
        }
        return;
    }

    // function to convert 32 bit binary to hexadecimal
    void bthPC()
    {
        // if pc is 0 initialising with 0x
        if (pc == 0)
        {
            hexpc = "0x0";
            return;
        }
        hexpc = "0x"; // initialising with 0x
        bool flag = 0;
        // converting 4 bits at a time to hex
        for (int i = 31; i >= 0;)
        {
            int ans = 0;
            for (int j = 8; j >= 1; j /= 2)
            {
                ans += pc[i] * j;
                i--;
            }
            if (ans != 0)
                flag = 1;
            if (flag)
            {
                if (ans < 10)
                {
                    hexpc += ans + '0';
                }
                else
                {
                    hexpc += ans - 10 + 'A';
                }
            }
        }
        return;
    }

    // to check if the id is correctly made in input.asm
    bool checkLabel(string stmt)
    {
        int i = stmt.size();
        while (stmt[i] != ':')
        {
            i--;
            if (i < 0)
                return false;
        }
        return true;
    }

    // to store id name and its corresponding gotopc in labelmap
    void makeLabel(string stmt)
    {
        int i = 0;
        string lname = "";
        while (stmt[i] != ' ' && stmt[i] != ':')
        {
            lname += stmt[i];
            i++;
        }
        while (stmt[i] != ':')
        {
            i++;
        }
        for (int j = i + 1; j < stmt.size(); j++)
        {
            if (stmt[j] != ' ')
                return;
        }
        id *newlab = new id();
        newlab->pc = this->pc.to_ulong();
        newlab->name = lname;
        m[lname] = newlab->pc;
        return;
    }

    // function to check if the instruction is right or not and which instruction is it
    string check(string stmt)
    {
        int i = 0;
        while (stmt[i] == ' ')
            i++;
        // fetching instruction name in oper
        string oper = "";
        while (stmt[i] != ' ')
        {
            oper += stmt[i];
            i++;
        }

        if (oper == "add")
        {
            R *Add = new R(); // making a new object of class add
            Add->add();
            Add->decode(stmt, i); // fetching fields
            if (f == 1)
                reg[Add->rd.to_ulong()] = reg[Add->rs1.to_ulong()] + reg[Add->rs2.to_ulong()];
            Add->binary();       // converting to 32 bit code
            Add->Hex();          // converting to hex
            return Add->hexinst; // returning mc in hex
        }

        else if (oper == "and")
        {
            R *ANd = new R();
            ANd->And();
            ANd->decode(stmt, i); 
            if (f == 1)
                reg[ANd->rd.to_ulong()] = reg[ANd->rs1.to_ulong()] & reg[ANd->rs2.to_ulong()];
            ANd->binary();       
            ANd->Hex();         
            return ANd->hexinst; 
        }

        else if (oper == "or")
        {
            R *OR = new R(); 
            OR->Or();
            OR->decode(stmt, i);
            if (f == 1)
                reg[OR->rd.to_ulong()] = reg[OR->rs1.to_ulong()] | reg[OR->rs2.to_ulong()];
            OR->binary();       
            OR->Hex();         
            return OR->hexinst; 
        }

        else if (oper == "sll")
        {
            R *Sll = new R();
            Sll->sll();
            Sll->decode(stmt, i);
            if (f == 1)
                reg[Sll->rd.to_ulong()] = reg[Sll->rs1.to_ulong()] << reg[Sll->rs2.to_ulong()];
            Sll->binary();       
            Sll->Hex();          
            return Sll->hexinst; 
        }

        else if (oper == "slt")
        {
            R *Slt = new R(); 
            Slt->slt();
            Slt->decode(stmt, i); 
            if (f == 1)
                reg[Slt->rs1.to_ulong()] < reg[Slt->rs2.to_ulong()] ? reg[Slt->rd.to_ulong()] = 1 : reg[Slt->rd.to_ulong()] = 0;
            Slt->binary();      
            Slt->Hex();          
            return Slt->hexinst; 
        }

        else if (oper == "sra")
        {
            R *Sra = new R(); 
            Sra->sra();
            Sra->decode(stmt, i); 
            if (f == 1)
                reg[Sra->rd.to_ulong()] = reg[Sra->rs1.to_ulong()] >> reg[Sra->rs2.to_ulong()];
            Sra->binary();      
            Sra->Hex();          
            return Sra->hexinst; 
        }

        else if (oper == "srl")
        {
            R *Srl = new R(); 
            Srl->srl();
            Srl->decode(stmt, i); 
            if (f == 1)
                reg[Srl->rd.to_ulong()] = reg[Srl->rs1.to_ulong()] >> reg[Srl->rs2.to_ulong()];
            Srl->binary();      
            Srl->Hex();          
            return Srl->hexinst; 
        }

        else if (oper == "sub")
        {
            R *Sub = new R(); 
            Sub->sub();
            Sub->decode(stmt, i); 
            if (f == 1)
                reg[Sub->rd.to_ulong()] = reg[Sub->rs1.to_ulong()] - reg[Sub->rs2.to_ulong()];
            Sub->binary();      
            Sub->Hex();        
            return Sub->hexinst; 
        }

        else if (oper == "xor")
        {
            R *XOr = new R(); 
            XOr->Xor();
            XOr->decode(stmt, i); 
            if (f == 1)
                reg[XOr->rd.to_ulong()] = reg[XOr->rs1.to_ulong()] ^ reg[XOr->rs2.to_ulong()];
            XOr->binary();       
            XOr->Hex();         
            return XOr->hexinst; 
        }

        else if (oper == "mul")
        {
            R *Mul = new R(); // making an object of class mul
            Mul->mul();
            Mul->decode(stmt, i); 
            if (f == 1)
                reg[Mul->rd.to_ulong()] = reg[Mul->rs1.to_ulong()] * reg[Mul->rs2.to_ulong()];
            Mul->binary();      
            Mul->Hex();          
            return Mul->hexinst; 
        }

        else if (oper == "div")
        {
            R *DIv = new R(); // making an object of class Div
            DIv->Div();
            DIv->decode(stmt, i); 
            if (f == 1)
                reg[DIv->rd.to_ulong()] = reg[DIv->rs1.to_ulong()] / reg[DIv->rs2.to_ulong()];
            DIv->binary();       
            DIv->Hex();          
            return DIv->hexinst; 
        }

        else if (oper == "rem")
        {
            R *Rem = new R(); // making an object of class rem
            Rem->rem();
            Rem->decode(stmt, i); 
            if (f == 1)
                reg[Rem->rd.to_ulong()] = reg[Rem->rs1.to_ulong()] % reg[Rem->rs2.to_ulong()];
            Rem->binary();
            Rem->Hex();          
            return Rem->hexinst;
        }

        else if (oper == "addi")
        {
            I *Addi = new I(); // making an object of class addi
            Addi->addi();
            Addi->decode2(stmt, i); 
            if (f == 1)
                reg[Addi->rd.to_ulong()] = reg[Addi->rs1.to_ulong()] + Addi->immVal2;
            Addi->binary();      
            Addi->Hex();         
            return Addi->hexinst; 
        }

        else if (oper == "andi")
        {
            I *Andi = new I(); // making an object of class andi
            Andi->andi();
            Andi->decode2(stmt, i); 
            if (f == 1)
                reg[Andi->rd.to_ulong()] = reg[Andi->rs1.to_ulong()] & Andi->immVal2;
            Andi->binary();
            Andi->Hex();          
            return Andi->hexinst; 
        }

        else if (oper == "ori")
        {
            I *Ori = new I(); // making an object of class ori
            Ori->ori();
            Ori->decode2(stmt, i); 
            if (f == 1)
                reg[Ori->rd.to_ulong()] = reg[Ori->rs1.to_ulong()] | Ori->immVal2;
            Ori->binary();       
            Ori->Hex();         
            return Ori->hexinst; 
        }

        else if (oper == "lb")
        {
            I *Lb = new I(); // making an object of class lb
            Lb->lb();
            Lb->decode3(stmt, i); 
            if (f == 1)
            {
                int mem = reg[Lb->rs1.to_ulong()];
                mem += Lb->immVal2;
                int temp = 0;
                for (int i = 0; i < 1; i++)
                {
                    temp = temp << 8;
                    if (memory.find(mem) == memory.end())
                    {
                        memory[mem] = 0;
                    }
                    temp += memory[mem].to_ulong();
                    mem -= 1;
                }
                reg[Lb->rd.to_ulong()] = temp;
            }
            Lb->binary();      
            Lb->Hex();         
            return Lb->hexinst; 
        }

        else if (oper == "ld")
        {
            I *Ld = new I(); // making an object of class ld
            Ld->ld();
            Ld->decode3(stmt, i); 
            if (f == 1)
            {
                int mem = reg[Ld->rs1.to_ulong()] + 7;
                mem += Ld->immVal2;
                int temp = 0;
                for (int i = 0; i < 8; i++)
                {
                    temp = temp << 8;
                    if (memory.find(mem) == memory.end())
                    {
                        memory[mem] = 0;
                    }
                    temp += memory[mem].to_ulong();
                    mem -= 1;
                }
                reg[Ld->rd.to_ulong()] = temp;
            }
            Ld->binary();      
            Ld->Hex();          
            return Ld->hexinst; 
        }

        else if (oper == "lh")
        {
            I *Lh = new I(); // making an object of class lh
            Lh->lh();
            Lh->decode3(stmt, i); 
            if (f == 1)
            {
                int mem = reg[Lh->rs1.to_ulong()] + 1;
                mem += Lh->immVal2;
                int temp = 0;
                for (int i = 0; i < 2; i++)
                {
                    temp = temp << 8;
                    if (memory.find(mem) == memory.end())
                    {
                        memory[mem] = 0;
                    }
                    temp += memory[mem].to_ulong();
                    mem -= 1;
                }
                reg[Lh->rd.to_ulong()] = temp;
            }
            Lh->binary();     
            Lh->Hex();          
            return Lh->hexinst;
        }

        else if (oper == "lw")
        {
            I *Lw = new I(); // making an object of class lw
            Lw->lw();
            Lw->decode3(stmt, i); 
            if (f == 1)
            {
                int mem = reg[Lw->rs1.to_ulong()] + 3;
                mem += Lw->immVal2;
                int temp = 0;
                for (int i = 0; i < 4; i++)
                {
                    temp = temp << 8;
                    if (memory.find(mem) == memory.end())
                    {
                        memory[mem] = 0;
                    }
                    temp += memory[mem].to_ulong();
                    mem -= 1;
                }
                reg[Lw->rd.to_ulong()] = temp;
            }
            Lw->binary();      
            Lw->Hex();          
            return Lw->hexinst; 
        }

        else if (oper == "jalr")
        {
            I *Jalr = new I(); // making an object of class jalr
            Jalr->jalr();
            Jalr->decode3(stmt, i); 
            if (f == 1)
            {
                reg[Jalr->rd.to_ulong()] = this->pc.to_ulong() + 4;
                this->pc = (reg[Jalr->rs1.to_ulong()] + Jalr->immVal2 - 4);
                reg[0] = 0;
            }
            Jalr->binary();       
            Jalr->Hex();         
            return Jalr->hexinst;
        }

        else if (oper == "sb")
        {
            S *Sb = new S();
            Sb->sb();            // making an object of class sb
            Sb->decode(stmt, i); 
            if (f == 1)
            {
                int temp = reg[Sb->rs2.to_ulong()];
                int mem = reg[Sb->rs1.to_ulong()] + Sb->imm.to_ulong();
                bitset<8> b = temp;
                memory[mem] = b;
            }
            Sb->binary();       
            Sb->Hex();          
            return Sb->hexinst; 
        }

        else if (oper == "sw")
        {
            S *Sw = new S();
            Sw->sw();            // making an object of class sw
            Sw->decode(stmt, i); 
            if (f == 1)
            {
                int temp = reg[Sw->rs2.to_ulong()];
                int mem = reg[Sw->rs1.to_ulong()] + Sw->imm.to_ulong();
                for (int i = 0; i < 4; i++)
                {
                    bitset<8> b = temp;
                    temp = temp >> 8;
                    memory[mem] = b;
                    mem += 1;
                }
            }
            Sw->binary();       
            Sw->Hex();          
            return Sw->hexinst; 
        }

        else if (oper == "sd")
        {
            S *Sd = new S();
            Sd->sd();            // making an object of class sd
            Sd->decode(stmt, i);
            if (f == 0)
            {
                int temp = reg[Sd->rs2.to_ulong()];
                int mem = reg[Sd->rs1.to_ulong()] + Sd->imm.to_ulong();
                for (int i = 0; i < 8; i++)
                {
                    bitset<8> b = temp;
                    temp = temp >> 8;
                    memory[mem] = b;
                    mem += 4;
                }
            }
            Sd->binary();       
            Sd->Hex();          
            return Sd->hexinst; 
        }

        else if (oper == "sh")
        {
            S *Sh = new S();
            Sh->sh();            // making an object of class sh
            Sh->decode(stmt, i);
            if (f == 0)
            {
                int temp = reg[Sh->rs2.to_ulong()];
                int mem = reg[Sh->rs1.to_ulong()] + Sh->imm.to_ulong();
                for (int i = 0; i < 2; i++)
                {
                    bitset<8> b = temp;
                    temp = temp >> 8;
                    memory[mem] = b;
                    mem += 4;
                }
            }
            Sh->binary();    
            Sh->Hex();        
            return Sh->hexinst;
        }

        else if (oper == "beq")
        {
            SB *Beq = new SB();
            Beq->beq();                                   // making an object of class beq
            Beq->decode(stmt, i, m, this->pc.to_ulong()); 
            if (f == 1)
            {
                if (reg[Beq->rs1.to_ulong()] == reg[Beq->rs2.to_ulong()])
                {
                    this->pc = (this->pc.to_ulong() + Beq->imm_dec - 4);
                }
            }
            Beq->binary();      
            Beq->Hex();       
            return Beq->hexinst; 
        }

        else if (oper == "bne")
        {
            SB *Bne = new SB();
            Bne->bne();                                   // Creating a new object of type bne
            Bne->decode(stmt, i, m, this->pc.to_ulong());
            if (f == 1)
            {
                if (reg[Bne->rs1.to_ulong()] != reg[Bne->rs2.to_ulong()])
                {
                    this->pc = (this->pc.to_ulong() + Bne->imm_dec - 4);
                }
            }
            Bne->binary();     
            Bne->Hex();       
            return Bne->hexinst; 
        }

        else if (oper == "bge")
        {
            SB *Bge = new SB();
            Bge->bge();                                   // Creating a new object of type bge
            Bge->decode(stmt, i, m, this->pc.to_ulong()); 
            if (f == 1)
            {
                if (reg[Bge->rs1.to_ulong()] >= reg[Bge->rs2.to_ulong()])
                {
                    this->pc = (this->pc.to_ulong() + Bge->imm_dec - 4);
                }
            }
            Bge->binary();     
            Bge->Hex();          
            return Bge->hexinst; 
        }

        else if (oper == "blt")
        {
            SB *Blt = new SB();
            Blt->blt();                                   
            Blt->decode(stmt, i, m, this->pc.to_ulong()); 
            if (f == 1)
            {
                if (reg[Blt->rs1.to_ulong()] < reg[Blt->rs2.to_ulong()])
                {
                    this->pc = (this->pc.to_ulong() + Blt->imm_dec - 4);
                }
            }
            Blt->binary();     
            Blt->Hex();     
            return Blt->hexinst;
        }

        else if (oper == "auipc")
        {
            U *Auipc = new U(); 
            Auipc->auipc();
            Auipc->decode(stmt, i); 
            if (f == 1)
                reg[Auipc->rd.to_ulong()] = (Auipc->imm.to_ulong() << 12) + this->pc.to_ulong();
            Auipc->binary();  
            Auipc->Hex();         
            return Auipc->hexinst; 
        }

        else if (oper == "lui")
        {
            U *Lui = new U();
            Lui->lui();
            Lui->decode(stmt, i); 
            if (f == 1)
                reg[Lui->rd.to_ulong()] = Lui->imm.to_ulong() << 12;
            Lui->binary();       
            Lui->Hex();        
            return Lui->hexinst; 
        }

        else if (oper == "jal")
        {
            UJ *Jal = new UJ();
            Jal->jal();                                   // Creating a new object of type jal
            Jal->decode(stmt, i, m, this->pc.to_ulong()); // Fetching instruction details
            if (f == 1)
            {
                reg[Jal->rd.to_ulong()] = this->pc.to_ulong() + 4;
                // cout<<Jal->imm_dec<<"\n";
                this->pc = (this->pc.to_ulong() + Jal->imm_dec - 4);
                // cout<<this->pc.to_ulong();
            }
            Jal->binary();       // Converting instruction to 32-bit code
            Jal->Hex();          // Converting binary code to hexadecimal
            return Jal->hexinst; // Returning the instruction in hexadecimal format
        }

        else
            return "error"; // return error if instruction doesnot match to any name
    }

    // function to read from .asm file and write to .mc file
    void updatemc()
    {
        ifstream fin;  // to read a file
        ofstream fout; // to write to a file
        string stmt;
        if (f == 0)
        {
            fin.open("input.asm");
            //provision for assembly directives
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (stmt != ".data")
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (stmt == ".text")
                {
                    break;
                }
                int l = stmt.size();
                int i = 0;
                string id = "";
                while (stmt[i] != ':')
                {
                    id += stmt[i];
                    i++;
                }
                i++;
                string dir = "";
                while (stmt[i] != ' ')
                {
                    dir += stmt[i];
                    i++;
                }
                i++;
                if (dir == ".asciiz")
                {
                    i++;
                }
                string s = "";
                vector<int> v;
                while (i < l)
                {
                    if (dir != ".asciiz")
                    {
                        if (stmt[i] == ' ')
                        {
                            int temp = stoi(s);
                            v.push_back(temp);
                            s = "";
                        }
                        else
                        {
                            s += stmt[i];
                        }
                        if (i == l - 1)
                        {
                            if (s == "")
                            {
                                i++;
                                continue;
                            }
                            ll temp = stoi(s);
                            v.push_back(temp);
                            s = "";
                        }
                        i++;
                    }
                    else
                    {
                        if (stmt[i] == '"')
                        {
                            v.push_back(0); // terminate string
                            i = l;
                            continue;
                        }
                        v.push_back(int(stmt[i]));
                        i++;
                    }
                }
                for (auto it : v)
                {
                    if (dir == ".word")
                    {
                        ll temp = it;
                        for (int i = 0; i < 4; i++)
                        {
                            bitset<8> b = temp;
                            temp = temp >> 8;
                            memory[start] = b;
                            start += 1;
                        }
                    }
                    if (dir == ".dword")
                    {
                        ll temp = it;
                        for (int i = 0; i < 8; i++)
                        {
                            bitset<8> b = temp;
                            temp = temp >> 8;
                            memory[start] = b;
                            start += 1;
                        }
                    }
                    if (dir == ".half")
                    {
                        ll temp = it;
                        for (int i = 0; i < 2; i++)
                        {
                            bitset<8> b = temp;
                            temp = temp >> 8;
                            memory[start] = b;
                            start += 1;
                        }
                    }
                    if (dir == ".byte")
                    {
                        ll temp = it;
                        for (int i = 0; i < 1; i++)
                        {
                            bitset<8> b = temp;
                            temp = temp >> 8;
                            memory[start] = b;
                            start += 1;
                        }
                    }
                    if (dir == ".asciiz")
                    {
                        ll temp = it;
                        for (int i = 0; i < 1; i++)
                        {
                            bitset<8> b = temp;
                            temp = temp >> 8;
                            memory[start] = b;
                            start += 1;
                        }
                    }
                }
            }
            fin.close();
            this->pc = 0;
            fin.open("input.asm"); // open input.asm to read
            // checking all labels only and storing them
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (stmt != ".text")
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (checkLabel(stmt))
                {
                    makeLabel(stmt);
                }
                else
                    incPC(this->pc);
            }
            fin.close(); // close input.asm
        }

        this->pc = 0;

        fin.open("input.asm"); // open input.asm to read
        if (f == 0)
            fout.open("output.mc"); // open output.mc to write
        // checking INST only (not labels)
        fout << "Text Segment: \n";
        if (f == 0)
        {
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (stmt != ".text")
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
            while (getline(fin, stmt))
            {
                if (stmt.size() == 0)
                {
                    continue;
                }
                if (!checkLabel(stmt))
                {
                    instruction[this->pc.to_ulong()] = stmt;
                    bthPC();
                    fout << hexpc << " " << check(stmt) << endl; // writing to output.mc
                    incPC(this->pc);
                }
            }
            fin.close(); // close input.asm
            if (f == 0)
                fout.close(); // close output.mc
        }
        if (f == 1)
        {
            int temp = 0;
            while (temp <= instruction.rbegin()->first)
            {
                cout << instruction[temp] << "       ";
                check(instruction[temp]);
                incPC(this->pc);
                temp = this->pc.to_ulong();
                for (int i = 0; i < 32; i++)
                {
                    cout << reg[i] << " ";
                }
                cout << "\n";
            }
        }
    }
};

int main()
{
    reg[2] = 2147483612; // declaring reg[2]
    reg[3] = 268435456;  // declaring reg[3]
    reg[10] = 1;         // reg[10] is 1
    reg[11] = 2147483612;
    makeMC *newMC = new makeMC(); // making a new object for makeMC class
    f = 0;
    newMC->updatemc(); // makes output.mc taking inputs from input.asm
    f = 1;
    newMC->updatemc();
    for (int i = 0; i < 32; i++)
    {
        cout << reg[i] << " ";
    }
    cout << "\n";
    ofstream fout;
    fout.open("output.mc", std::ios::app); // open file in append mode
    fout << "\n### End of Text Segment ###\n";
    fout << "\nData Segment:\n";
    for (auto it = memory.begin(); it != memory.end(); it++)
    {
        bitset<32> b1 = (it->first);
        bitset<8> b2 = (it->second.to_ulong());
        fout << hex(b1) << " " << hex2(b2) << endl;
        cout << hex(b1) << " " << hex2(b2) << endl;
        // fout<<it->first<<" "<<(it->second.to_ulong())<<endl;
        // cout<<it->first<<" "<<(it->second.to_ulong());
    }
    fout << "\n### End of Data Segment ###\n";
    //fout << "\nValues stored in Registers:\n";
    for (int i = 0; i < 32; i++)
    {
        //fout << "x" << i << dec << " ";
        bitset<32> b1 = reg[i];
        //fout << hex(b1) << "\n";
    }
    fout.close();
    return 0;
}