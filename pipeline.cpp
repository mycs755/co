#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

struct labels{
    string label;
    int32_t line_number_of_label;
};

struct memory_elements{
    string name;
    int start_address;
};

struct registers_of_each_instruction{
    int r1;
    int r2;
    int r3;
    int typeo;
};

class simulator{
    public:
    string registers[32]={"ze","at","v0","v1","a0","a1","a2","a3","t0","t1","t2","t3","t4","t5","t6",
	"t7","s0","s1","s2","s3","s4","s5","s6","s7","t8","t9","k0","k1","gp","sp","s8","ra"};
    int value_of_registers[32];
    vector<string> program;
    int number_of_instructions;
    string present_instruction;
    int memory_element_values[1000];
    int total_stalls;
    //void read_instruction(int line_number);
    int registers_in_present_instruction[4];
    string operations_allowed[17]={"add","sub","mul","and","or","nor","slt","addi","andi","ori","slti","lw",
	"sw","beq","bne","j","la"};
    int pc ;
    vector<int>clockcycles_with_stalls;   //for cck stalls
    bool data_forwarding_enabled;
    int position_of_element_in_memory;
    vector<struct labels>labels_in_program;
    vector<struct memory_elements>memory_used_in_program;
    vector<struct registers_of_each_instruction>each_instruction_info;
    vector<int>all_execute_state_values;
    void RemoveSpaces(string &str);
    void display();
    void run();
    int clockcycle;
    int stalls;
    int main_found;
    string instruction_fetch(int n);
    int * instruction_drf(string s);
    int is_there_datahazard(int n);
    int instruction_execute(int n);
    int instruction_memory(int m,int n);
    void instruction_writeback(int m,int n);
   // void execute_present_operation(int n);  //******************
    void valid_register(string s,int n);
    simulator(ifstream& file,bool need);

};

void simulator::run(){
    //value_of_registers[32]={0};
    for(int i=0;i<32;i++){
        value_of_registers[i]=0;
    }
    clockcycle=1; /////////////////////////////////////////////////////////////
    memory_element_values[1000]={0};
    position_of_element_in_memory=0;
    
   // value_of_registers[2]=10;
   main_found=-1;
    int data_found=-1;
    int text_found=-1;
    for(int i=0;i<number_of_instructions;i++){
        string s=program[i];
        RemoveSpaces(s);
        if(s==".data"){
            data_found=i;
            break;
        }
    }
    if(data_found==-1){
        cout<<"error as .data is not found";
    }
    for(int j=data_found+1;j<number_of_instructions;j++){
        string s=program[j];
        RemoveSpaces(s);
        if(s==".text"){
            text_found=j;
            break;
        }
    }
    if(text_found==-1){
        cout<<"error as .text is not found";
    }

     for(int j=data_found+1;j<text_found;j++){ 
        int x=-1;
        string s = program[j];
    
        RemoveSpaces(s);
        
         x = s.find(":");
        
         string list_name;
         memory_elements m;
        
         if(x!=-1 && x!=0){
            list_name=s.substr(0,x);
            
            m.name=list_name;
            m.start_address=position_of_element_in_memory;
            memory_used_in_program.push_back(m);
            string s1=s.substr(x+6);
            stringstream commas(s1);
            string intermediate1;
            while(getline(commas,intermediate1,',')){
          
            memory_element_values[position_of_element_in_memory++]=stoi(intermediate1);
            }
        }
     }   
   // int main_found=3;
    for(int k=text_found+1;k<number_of_instructions;k++){
        int x=-1;
        string s = program[k];
        RemoveSpaces(s);
        if(s=="main:"){
            main_found=k;
        }
        cout<<main_found<<"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM,,,,,,,,,,,,,,,,,,"<<endl;
         x = s.find(":");
        string label_name;
        labels l;
        if(x!=-1 && x!=0){
            label_name=s.substr(0,x);
            l.label=label_name;
            l.line_number_of_label=k;
            labels_in_program.push_back(l);
        }
        
    }
    pc=text_found+2;//extra

   // cout<<pc<<"////////////////////////////////////////////////////"<<endl;
    cout<<"number of instructions"<<number_of_instructions<<endl; 
    while(pc<number_of_instructions){
       
        cout<<pc;
        string s2=program[pc];
       
        RemoveSpaces(s2);
        if(s2=="stop"){
            pc=number_of_instructions+1;
        }
        if(s2==""){
            pc++;
            continue;
        }
        int is_label=-1;
        is_label=program[pc].find(":");
        if(is_label!=-1){
            pc++;
            continue;
        }
        cout<<":::::::::::::::::::::::::::::::::::::::::::"<<endl;
        string ifs = instruction_fetch(pc);
        cout<<";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;"<<endl;
        int* idrf_regs;
        idrf_regs = instruction_drf(ifs);
        cout<<"kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk"<<endl;
        int type_of_operation = idrf_regs[3];
       // bool is_there_datahazard(pc-main_found);

        int exe_val = instruction_execute(type_of_operation);
        int mem_val = instruction_memory(exe_val,type_of_operation);
        instruction_writeback(mem_val,type_of_operation);

        clockcycle++;
       // read_instruction(pc);///////////////////////edit here
       // break;
    }
}

int simulator::is_there_datahazard(int n){
    cout<<"hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh"<<n<<endl;
    if(n==1){
        cout<<"hhhhhhhhhhhhhhh111111111111111111111"<<endl;
        return -100;
    }
   else if(n==2){
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;


        if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
             cout<<"hhhhhhhhhhhhhhh22222222222222222"<<endl;
            return 1;
        }
        else{
             cout<<"hhhhhhhhhhhhhhh22222222222222222"<<endl;
            return -100;
        }
    }
    else{
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        int reg_dest_prev_prev = each_instruction_info[each_instruction_info.size()-3].r1;
         if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev || reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
            cout<<"hhhhhhhhhhhhhhh3333333333"<<endl;
            return 2;
        }
        else{
             cout<<"hhhhhhhhhhhhhhh3333333333"<<endl;
            return -100;
        }
    }
    
}

void simulator::RemoveSpaces(string &str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    //return str; 
}

simulator::simulator(ifstream& file,bool need){
   
    number_of_instructions=0;
    pc=0;
    data_forwarding_enabled=need;
    total_stalls=0;
    file.open("pipeline.s");
    if(!file.is_open()){
        cout<<"unable to open file";
    }
    else{
        cout<<"opened file";
        while(file.good()){
            number_of_instructions++;
            string line;
            getline(file,line);
        cout<<line<<endl;
        program.push_back(line);
        }
    }
   
    file.close();  
}

string simulator::instruction_fetch(int n){
    string s = program[n];
    cout<<"ififififififififififififififififififi"<<endl;
    RemoveSpaces(s);
    pc=pc+1;
    cout<<s<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"<<endl;
    return s;
}

int * simulator::instruction_drf(string s){
    RemoveSpaces(s);
    cout<<s<<"[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[["<<endl;
   // present_instruction=s;
    registers_of_each_instruction r;  ////////////////
    int count=0;
    int size_of_operation;
    cout<<s<<"jjjjjjjjjjjjjjjjjjjjjjjjj"<<endl;
    for(int i=0;i<4;i++){
        if(s[i]==' '|| s[i]=='\t')
		{
			break;
		}
        count++;
    }
    string operation =s.substr(0,count);
    cout<<operation<<"lllllllllllllllllllllllllllllll"<<endl;
    string registers_in_instruction;
    if(count<s.size()-1)
        registers_in_instruction=s.substr(count+1);
    RemoveSpaces(registers_in_instruction);  
    cout<<registers_in_instruction<<"\\\\\\\\\\\\\\\\\\\\\\"<<endl;  
    int position_in_array=-1;
    for(int i=0;i<17;i++){
        if(operation==operations_allowed[i]){
            position_in_array=i;
            break;
        }
    }
    
    if(position_in_array==-1){
        cout<<"invalid-operation";
        registers_in_present_instruction[4]={-2};
        r.r1=-2;
        r.r2=-2;
        r.r3=-2;
        r.typeo=-2;
        //return registers_in_present_instruction;
    }

    else if(position_in_array<7){
        registers_in_present_instruction[4]={-1};
        valid_register(registers_in_instruction,6); //*********
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       // return registers_in_present_instruction;
       // execute_present_operation(position_in_array);  //************
    }
    else if(position_in_array<11){
        cout<<"eeeeeeeeeeeeeeeeeeeeellllllllllsssssssssssssssss>>endl;";
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,10);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
        //return registers_in_present_instruction;
        //execute_present_operation(position_in_array);
    }
    else if(position_in_array<13){
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,12);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       // return registers_in_present_instruction;
       // execute_present_operation(position_in_array);
        //******************************  execute
    }
    else if(position_in_array<15){
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,14);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       // return registers_in_present_instruction;
       // execute_present_operation(position_in_array);
    }
    else if(position_in_array==15){
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,15);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       // return registers_in_present_instruction;
       // execute_present_operation(position_in_array);
    }
    else if(position_in_array==16){
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,16);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       // return registers_in_present_instruction;
       // execute_present_operation(position_in_array);
    }
    return registers_in_present_instruction;  
}

int simulator::instruction_memory(int m,int n){
    if(n>=0 && n<=10){
        return m;
    }
    else if(n==11){
        return memory_element_values[m];
    }
    else{
        return memory_element_values[m];
    }         //////////change change
}

void simulator::instruction_writeback(int m,int n){  
        value_of_registers[registers_in_present_instruction[0]]=m;
}

int simulator::instruction_execute(int n){
    cout<<n<<"executeexecute"<<endl;

     int execute_stage_value=-111;
    
        if(is_there_datahazard(pc-main_found-1)==1){
        clockcycles_with_stalls.push_back(clockcycle+1);
         clockcycles_with_stalls.push_back(clockcycle+2);
         clockcycles_with_stalls.push_back(clockcycle+3);
        clockcycle=clockcycle+3;
         total_stalls=total_stalls+3;
           
        }
        else if(is_there_datahazard(pc-main_found-1)==2){
         clockcycles_with_stalls.push_back(clockcycle+1);
         clockcycles_with_stalls.push_back(clockcycle+2);
        clockcycle=clockcycle+2;
         total_stalls=total_stalls+2;
        }
        else if(is_there_datahazard(pc-main_found-1)==-100){
            clockcycle=clockcycle+0;
        }

     switch(n){

        

        case 0:
       
		//return value_of_registers[registers_in_present_instruction[1]]+value_of_registers[registers_in_present_instruction[2]]; 
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 1:
        
		//return value_of_registers[registers_in_present_instruction[1]]-value_of_registers[registers_in_present_instruction[2]]; 
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]-value_of_registers[registers_in_present_instruction[2]]; 
        
        break;
        case 2:
       
		//return value_of_registers[registers_in_present_instruction[1]]*value_of_registers[registers_in_present_instruction[2]]; 
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]*value_of_registers[registers_in_present_instruction[2]]; 
       break;
        case 3:
        
		//return value_of_registers[registers_in_present_instruction[1]] & value_of_registers[registers_in_present_instruction[2]]; 
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]&value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 4:
        
		return value_of_registers[registers_in_present_instruction[1]]|value_of_registers[registers_in_present_instruction[2]]; 
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]|value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 5:

        break;  /////////////////////////////////////////////////////////////////////////////////////////////
        case 6:
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]<value_of_registers[registers_in_present_instruction[2]];
        break;
        case 7:
           
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
          //  return value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2];
          //  pc++;
           execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]; 
            break;
        case 8:
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
            //return value_of_registers[registers_in_present_instruction[1]] & registers_in_present_instruction[2];
           // pc++;
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]-registers_in_present_instruction[2]; 
            break;
        break;
        case 9:
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[2]<<endl;
           // return value_of_registers[registers_in_present_instruction[1]] | registers_in_present_instruction[2];
           // pc++;
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]|registers_in_present_instruction[2]; 
            break;
        break;
        case 10:

        break;
        case 11:
           
             cout<<registers_in_present_instruction[0]<<endl;
              cout<<registers_in_present_instruction[1]<<endl;
             cout<<registers_in_present_instruction[2]<<endl;
           //  return value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4;
           //  cout<<memory_element_values[value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4]<<"mmmmmmmmmmmmmmmm"<<endl;
           // pc++;
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4; 
        break;
        case 12:
            
             cout<<registers_in_present_instruction[0]<<endl;
              cout<<registers_in_present_instruction[1]<<endl;
             cout<<registers_in_present_instruction[2]<<endl;
             return value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4;
             //memory_element_values[value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4]=value_of_registers[registers_in_present_instruction[0]];
            return 222;
          //  pc++; 
        break;
        case 13:
            if(value_of_registers[registers_in_present_instruction[0]]==value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2]+1;
            }
            else{
                pc++;
            }
        break;
        case 14:
            if(value_of_registers[registers_in_present_instruction[0]]!=value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2]+1;
            }
            else{
                pc++;
            }
        break;
        case 15:
            pc = registers_in_present_instruction[0];
        break;
        case 16:
       
            cout<<registers_in_present_instruction[0]<<endl;
            cout<<registers_in_present_instruction[1]<<endl;
            value_of_registers[registers_in_present_instruction[0]]=registers_in_present_instruction[1];
            cout<<value_of_registers[registers_in_present_instruction[0]]<<endl;
            pc++;
        break;
        case 17:

        break;
    }
    return execute_stage_value;
}

void simulator::valid_register(string s,int n){
    vector <string> tokens;
    stringstream commas(s);
    string intermediate;
    while(getline(commas,intermediate,',')){
        if(intermediate[0]=='$')
        tokens.push_back(intermediate.substr(1));
        else
        tokens.push_back(intermediate);
    }
    int x=tokens.size();
    cout<<x<<endl;
    
    for(int i=0;i<tokens.size();i++){
        cout<<tokens[i]<<endl;
    }
    int y=0;
    if(n==6 && x==3){
            for(int i=0;i<x;i++){
                for(int j=0;j<32;j++){
                    if(tokens[i]==registers[j]){
                        registers_in_present_instruction[i]=j;
                        y++;
                    }
                }
            }
            if(y!=x){
                cout<<"all registers given are not valid";
            }  
    }
    else if(n==10 && x==3){
       
        for(int i=0;i<2;i++){
            for(int j=0;j<32;j++){
                if(tokens[i]==registers[j]){
                    
                    registers_in_present_instruction[i]=j;
                   // cout<<registers_in_present_instruction[i]<<"jjjjjjjjjjjjjjjjjjj"<<endl;
                    y++;
                }
            }
        }
        cout<<tokens[2]<<endl;
        cout<<stoi(tokens[2])<<endl;
        registers_in_present_instruction[2]=stoi(tokens[2]);
        y++;
       
        if(y!=3){
            cout<<"all registers given are not valid";
        }
    }
    
    else if(n==12 && x==2){
         string s1=tokens[1];
        int offset=-1;
        int x=s1.find("(");
        char c=s1[0];
        int z=s1.find(")");
        string s2="";
        
        if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4'|| c=='5' || c=='6' || c=='7' || c=='8' || c=='9'){
           offset=stoi(s1.substr(0,x));
           //cout<<offset<<">>>>>>>>>>>>>>>>>>"<<endl;
          s2=s1.substr(x+2,z-x-2);
         
        }
        else{
           
            cout<<s1<<endl;
            cout<<x<<endl;
            cout<<z<<endl;
            s2=s1.substr(x+2,z-x-2);
            
            for(int i=0;i<32;i++){
                if(s2==registers[i]){
                    offset=value_of_registers[i];
                   // cout<<offset<<"??????????????????????"<<endl;
                    break;
                }
            }
        } 
       // string s2 = s1.substr(x+2,z-x-2);
        for(int i=0;i<32;i++){
            if(registers[i]==tokens[0]){
                registers_in_present_instruction[0]=i;
                y++;
            }
        }
        for(int i=0;i<32;i++){
            if(registers[i]==s2){
                registers_in_present_instruction[1]=i;
                y++;
            }
        }
        if(offset==-1){
            cout<<"wrong offset value";
        }
        registers_in_present_instruction[2]=offset;
        cout<<registers_in_present_instruction[0]<<endl;
        cout<<registers_in_present_instruction[1]<<endl;
        cout<<registers_in_present_instruction[2]<<endl;
      
        if(y!=2){
        cout<<"wrong"<<endl;
        }
   
    }
    else if(n==14 && x==3){
        for(int i=0;i<2;i++){
            for(int j=0;j<32;j++){
                if(tokens[i]==registers[j]){
                    registers_in_present_instruction[i]=j;
                    y++;
                }
            }
        }
        if(y!=2){
            cout<<"error in number of registers,beq,bne";
        }
        int found=0;
        for(int k=0;k<labels_in_program.size();k++){
            if(tokens[2]==labels_in_program[k].label){
                registers_in_present_instruction[2]=labels_in_program[k].line_number_of_label;
                found=1;
            }
        }
        if(found==0){
            cout<<"wrong label name written";
        }

    }
    else if(n==15){
        string s=tokens[0];
        int found=0;
        for(int i=0;i<labels_in_program.size();i++){
            if(s==labels_in_program[i].label){
                registers_in_present_instruction[0]=labels_in_program[i].line_number_of_label;
                found=1;
            }
        }
        if(found==0){
            cout<<"wrong label name is written";
        }
    }
    else if(n==16){
        for(int j=0;j<32;j++){
            if(tokens[0]==registers[j]){
                    registers_in_present_instruction[0]=j;
                    y++;
             }
         }
        for(int j=0;j<memory_used_in_program.size();j++){
            if(tokens[1]==memory_used_in_program[j].name){
                registers_in_present_instruction[1]=memory_used_in_program[j].start_address;
            }
        }

        registers_in_present_instruction[2]==-1000;

        if(y!=1){
            cout<<"error in number of registers,beq,bne";
        }
    }
    else{
            cout<<"wrong number of instructions"<<" in valid_register";
        }
}

void simulator::display(){
     for(int i=0;i<32;i++){
         cout<<registers[i]<<"  "<<value_of_registers[i]<<endl;
     }
     cout<<"stalls in program"<<total_stalls;

     
}

int main(){
    ifstream file;
    bool need;
    cout<<"need data forwarding true/false";
    cin>>need;
     simulator s(file,need);
     s.run();
     s.display();
    return 0;
}