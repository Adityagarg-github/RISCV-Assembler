#include<bits/stdc++.h>
using namespace std;
#define int long long
#include <fstream>

map <int, int> pc_ins;
vector<int> pc, ins;
map<int, int> pc_tar;
map<int, pair<string, string>> always_taken_btb, always_not_taken_btb, oneb_btb, twob_btb;
map<int, string> two_bit_state;
int hit[4];
int miss[4];

// function to convert address to decimal from hex
int hex2dec(string hexVal) 
{ 
    int len = hexVal.size(); 
    int base = 1;   
    int dec_val = 0;    
    for (int i = len - 1; i >= 0; i--) { 
        if (hexVal[i] >= '0' && hexVal[i] <= '9') { 
            dec_val += ((int)(hexVal[i]) - 48) * base;
            base = base * 16; 
        } 
        else if (hexVal[i] >= 'a' && hexVal[i] <= 'f') { 
            dec_val += (int)((hexVal[i]) - (int)('a')+10) * base; 
            base = base * 16; 
        } 
        else if (hexVal[i] >= 'A' && hexVal[i] <= 'F') { 
            dec_val += (int)((hexVal[i]) - (int)('A')+10) * base; 
            base = base * 16; 
        } 
    } 
    return dec_val; 
}

// function to take input form the trace file
void read_pc(){
    int n=0;
    ifstream file("input.txt");
    if(!file){
        cout<<"Error in openong file!";
        exit(0);
        return;
    }
    int p, inst;
    // string to store the current line from the file
    string line;
    while(getline(file, line)){
        if(line.size()==1){
            continue;
        }
        n++;
        string s1, s2;
        int i=0;
        // iterate till we reach the pc
        while(line[i]!='x'){
            i++;
            if(i>=line.size()){
                continue;
            }
        }
        i++;
        // storing addresss in s1
        for(int j=0;j<8;j++){
            s1+=line[i];
            i++;
        }
        // iterating till we reach the instruction type
        while(line[i]!='x'){
            i++;
        }
        i++;
        // storing the instruction in s2
        for(int j=0;j<8;j++){
            s2+=line[i];
            i++;
        }
        // p value becomes pc address in decimal
        p=hex2dec(s1);
        // inst value becomes the instruction in decimal
        inst=hex2dec(s2);
        // pc vector stores the all the program counter addresses 
        pc.push_back(p);
        // ins stores all instructions
        ins.push_back(inst);
        // stores pc and instruction as a pair
        pc_ins.insert({p, inst});
    }
    file.close();
}

// function to check if the instructionis a branch instruction or not
int checkBranch(int inst){
    int opcode=inst%128;
    // we compare by opcode and return 1 if the instruction is a branch instruction
    if(opcode==99){
        return 1;
    }
    if(opcode==103 || opcode==111){
        return -1;
    }
    return 0;
}

// function to generate target address of each pc
int target_add(int currpc, int nextpc, int inst){
    int opcode=inst%128;
    // if instruction is an unconditional branch instruction, simply return next pc
    if(opcode==103 || opcode==111){
        return nextpc;
    }
    // if instruction is a conditional branch instruction,  we extract immediate field from the instruction
    // and add it to the curr pc to get the target address
    if(opcode==99){
        bitset <32> b=inst;
        bitset <11> imm;
        int sign;
        sign=b[31];
        imm[10]=b[7];
        for(int i=1;i<=6;i++){
            imm[10-i]=b[31-i];
        }
        for(int i=7;i<=10;i++){
            imm[10-i]=b[18-i];
        }
        int num=imm.to_ulong();
        int f=0;
        if(sign==1){
            f=-1;
        }
        num=num+(1ll<<11)*f;
        num*=2;
        return currpc+num;
    }
    return nextpc;
}

// function to execute always taken and always not taken branch predictors
void static_bp(){
    for(int i=0;i<pc.size()-1;i++){
        if(checkBranch(ins[i])){
            // the map of pair of strings maps the actual value to the predicted value
            // stores always taken predictions
            always_taken_btb[pc[i]].first.push_back('T');
            // stores always not taken predictions
            always_not_taken_btb[pc[i]].first.push_back('N');
            // if branch is taken it is a hit for always taken and miss for always not taken
            if(pc[i+1]!=pc[i]+4){
                always_taken_btb[pc[i]].second.push_back('T');
                always_not_taken_btb[pc[i]].second.push_back('T');
                hit[0]++;
                miss[1]++;
            }
            // if branch is not taken it is a hit for always not taken and miss for always taken
            else{
                always_taken_btb[pc[i]].second.push_back('N');
                always_not_taken_btb[pc[i]].second.push_back('N');
                miss[0]++;
                hit[1]++;
            }
        }
    }
}

// function to execute one bit branch predictions
void one_bit(){
    for(int i=0;i<pc.size()-1;i++){
        if(checkBranch(ins[i])){
            // if branch instruction exists compare the 
            // actual vlaue with current prediction and update the prediction
            if(pc[i+1]!=pc[i]+4){
                if(oneb_btb[pc[i]].first.back()=='T'){
                    hit[2]++;
                }
                else{
                    miss[2]++;
                }
                oneb_btb[pc[i]].second.push_back('T');
                oneb_btb[pc[i]].first.push_back('T');
            }
            else{
                if(oneb_btb[pc[i]].first.back()=='N'){
                    hit[2]++;
                }
                else{
                    miss[2]++;
                }
                oneb_btb[pc[i]].second.push_back('N');
                oneb_btb[pc[i]].first.push_back('N');
            }
        }
    }
}

// function to execute two bit branch predictor
void two_bit(){
    int size=pc_tar.size();
    for(auto it:pc_tar){
        two_bit_state[it.first]="SNT";
    }
    for(int i=0;i<pc.size()-1;i++){
        if(checkBranch(ins[i])){
            string state=two_bit_state[pc[i]];
            if(pc[i+1]!=pc[i]+4){
                twob_btb[pc[i]].second.push_back('T');
                if(state=="ST" || state=="WT"){
                    hit[3]++;
                    if(state=="ST"){
                        twob_btb[pc[i]].first.push_back('T');
                        two_bit_state[pc[i]]=("ST");
                    }
                    else{
                        twob_btb[pc[i]].first.push_back('T');
                        two_bit_state[pc[i]]=("ST");
                    }
                }
                else{
                    miss[3]++;
                    if(state=="SNT"){
                        twob_btb[pc[i]].first.push_back('N');
                        two_bit_state[pc[i]]=("WNT");
                    }
                    else{
                        twob_btb[pc[i]].first.push_back('T');
                        two_bit_state[pc[i]]=("WT");
                    }
                }
            }
            else{
                twob_btb[pc[i]].second.push_back('N');
                if(state=="ST" || state=="WT"){
                    miss[3]++;
                    if(state=="ST"){
                        twob_btb[pc[i]].first.push_back('T');
                        two_bit_state[pc[i]]=("WT");
                    }
                    else{
                        twob_btb[pc[i]].first.push_back('N');
                        two_bit_state[pc[i]]=("WNT");
                    }
                }
                else{
                    hit[3]++;
                    if(state=="SNT"){
                        twob_btb[pc[i]].first.push_back('N');
                        two_bit_state[pc[i]]=("SNT");
                    }
                    else{
                        twob_btb[pc[i]].first.push_back('N');
                        two_bit_state[pc[i]]=("SNT");
                    }
                }
            }
        }
    }
}

int32_t main(){
    // reading the trace
    read_pc();
    // initialising the hit and miss of each predictor by 0
    for(int i=0;i<4;i++){ 
        hit[i]=0;
        miss[i]=0;
    }
    char c;
    cout<<"Do you want to print the \"Branch Table Buffer\" (y/n): ";
    cin>>c;
    int count=0;
    // initialising all the maps for the predictors
    for(int i=0;i<pc.size()-1;i++){
        if(checkBranch(ins[i])!=0){
            count++;
                pc_tar.insert({pc[i], target_add(pc[i], pc[i+1], ins[i])});
                always_taken_btb.insert({pc[i],{"",""}});
                always_not_taken_btb.insert({pc[i],{"",""}});
                oneb_btb.insert({pc[i],{"N",""}});
                twob_btb.insert({pc[i],{"N",""}});
        }
    }
    
    static_bp();
    one_bit();
    two_bit();
    
    set <int> PC;
    for(int i=0 ; i<pc.size() ; i++){
        if(checkBranch(ins[i]))
            PC.insert(pc[i]);
    }
    if(c=='Y'||c=='y'){
        cout << "\t\t\t\tAlways Taken\n";
        cout << "Accuracy of \"Always Taken\" branch predictor : " << ((double)hit[0]*100)/count << "%\n";
        cout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
        for(auto it : PC){
            cout << it << "\t" << pc_tar[it] << "\t\t" << always_taken_btb[it].second << "\t\t\t" << always_taken_btb[it].first << "\n"; 
        }

        cout << "\n\t\t\t\tAlways Not Taken\n";
        cout << "Accuracy of \"Always Not Taken\" branch predictor : " << ((double)hit[1]*100)/count << "%\n";
        cout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
        for(auto it : PC){
            cout << it << "\t" << pc_tar[it] << "\t\t" << always_not_taken_btb[it].second << "\t\t\t" << always_not_taken_btb[it].first << "\n"; 
        }

        cout << "\n\t\t\t\tOne bit predictor\n";
        cout << "Accuracy of \"One bit\" branch predictor : " << ((double)hit[2]*100)/count << "%\n";
        cout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
        for(auto it : PC){
            cout << it << "\t" << pc_tar[it] << "\t\t" << oneb_btb[it].second << "\t\t\t" << oneb_btb[it].first << "\n"; 
        }

        cout << "\n\t\t\t\tTwo bit predictor\n";
        cout << "Accuracy of \"Two bit\" branch predictor : " << ((double)hit[3]*100)/count << "%\n";
        cout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
        for(auto it : PC){
            cout << it << "\t" << pc_tar[it] << "\t\t" << twob_btb[it].second << "\t\t\t" << twob_btb[it].first << "\n"; 
        }
    }
    
    //file to write branch table buffer and accuracy
    ofstream fout("output.txt");
    
    fout << "\t\t\t\tAlways Taken\n";
    fout << "Accuracy of \"Always Taken\" branch predictor : " << ((double)hit[0]*100)/count << "%\n";
    fout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
    for(auto it : PC){
        fout << it << "\t" << pc_tar[it] << "\t\t" << always_taken_btb[it].second << "\t\t\t" << always_taken_btb[it].first << "\n"; 
    }

    fout << "\n\t\t\t\tAlways Not Taken\n";
    fout << "Accuracy of \"Always Not Taken\" branch predictor : " << ((double)hit[1]*100)/count << "%\n";
    fout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
    for(auto it : PC){
        fout << it << "\t" << pc_tar[it] << "\t\t" << always_not_taken_btb[it].second << "\t\t\t" << always_not_taken_btb[it].first << "\n"; 
    }

    fout << "\n\t\t\t\tOne bit predictor\n";
    fout << "Accuracy of \"One bit\" branch predictor : " << ((double)hit[2]*100)/count << "%\n";
    fout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
    for(auto it : PC){
        fout << it << "\t" << pc_tar[it] << "\t\t" << oneb_btb[it].second << "\t\t\t" << oneb_btb[it].first << "\n"; 
    }

    fout << "\n\t\t\t\tTwo bit predictor\n";
    fout << "Accuracy of \"Two bit\" branch predictor : " << ((double)hit[3]*100)/count << "%\n";
    fout << "PC\t\tTarget Address\t\tActual History\t\tPrediction History\n"; 
    for(auto it : PC){
        fout << it << "\t" << pc_tar[it] << "\t\t" << twob_btb[it].second << "\t\t\t" << twob_btb[it].first << "\n"; 
    }

    cout<<"\n";
    cout<<"Total number of instructions: "<<pc.size()<<"\n";
    cout<<"Total number of branch instructions: "<<count<<"\n";   
    cout<<"\n"; 
    cout << "Accuracy of \"Always Taken\" branch predictor : " << ((double)hit[0]*100)/count << "%\n";
    cout << "Accuracy of \"Always Not Taken\" branch predictor : " << ((double)hit[1]*100)/count << "%\n";
    cout << "Accuracy of \"One bit\" branch predictor : " << ((double)hit[2]*100)/count << "%\n";
    cout << "Accuracy of \"Two bit\" branch predictor : " << ((double)hit[3]*100)/count << "%\n"; 
    cout<<"\n";

    fout<<"\n";
    fout<<"Total number of instructions: "<<pc.size()<<"\n";
    fout<<"Total number of branch instructions: "<<count<<"\n";   
    fout<<"\n"; 
    fout << "Accuracy of \"Always Taken\" branch predictor : " << ((double)hit[0]*100)/count << "%\n";
    fout << "Accuracy of \"Always Not Taken\" branch predictor : " << ((double)hit[1]*100)/count << "%\n";
    fout << "Accuracy of \"One bit\" branch predictor : " << ((double)hit[2]*100)/count << "%\n";
    fout << "Accuracy of \"Two bit\" branch predictor : " << ((double)hit[3]*100)/count << "%\n"; 
    fout<<"\n";
    fout.close();
    return 0;
}