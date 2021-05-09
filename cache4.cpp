#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include<bits/stdc++.h>
#include<math.h>
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

struct type_of_offset{
    int position;
    int type;
};

class Block{
    public:
    int *data;
    string tag;
    bool dirty;
    int lru;
    Block();
};
Block::Block(){
    tag = "NULL";
    dirty = false;
    lru = 0;
}
class Set{
    public:
    Block *blocks_level;
    void setnum_of_blocks_in_set_level(int x);
    int getnum_of_blocks_in_set_level();
    int level_set_index_size;
    int level_set_offset_size;
    int req_block_no_in_set_in_level;
    
   int find_block_in_set_level(string s3,Set ss,string addr);
    private:
    int num_of_blocks_in_set_level;
};
int Set::find_block_in_set_level(string s3,Set ss,string addr){
    for(int i=0;i<ss.getnum_of_blocks_in_set_level();i++){
      //  cout<<ss.blocks_level[i].tag<<endl;
        if(s3 == ss.blocks_level[i].tag){
         //  cout<<"yyyyyyyyyyyyyyyyyeeeeeeeeeeeeeeessssssssssssssss"<<endl;
            req_block_no_in_set_in_level = i;
            return i;
        }
    }
   return -1;
}
int Set::getnum_of_blocks_in_set_level(){
    return num_of_blocks_in_set_level;
}
void Set::setnum_of_blocks_in_set_level(int x){
    num_of_blocks_in_set_level = x;
}

class Cache{
    public:
    Set *sets_level;
    int level_index_size;
    int level_offset_size;
    void setnum_of_sets_in_cache_level(int x);
    int getnum_of_sets_in_cache_level();
    int access_level(Cache c,string addr);
    int req_set_index_level(Cache c,string addr);
    req_data_position_in_block(Cache c,string addr);
   // write_back_to_main_memory(Cache c);
    string level_index_str;
    string level_offset_str;
    private:
    int num_of_sets_in_cache_level;
};
Cache design_level_cache(int cache_size,int block_size,int assosciativity){
    Cache level_cache;
    int a = cache_size/(block_size*assosciativity);
    level_cache.setnum_of_sets_in_cache_level(a);
    level_cache.level_index_size=a;
    level_cache.level_offset_size=block_size;
    level_cache.sets_level = new Set[level_cache.getnum_of_sets_in_cache_level()];
    for(int i=0;i<level_cache.getnum_of_sets_in_cache_level();i++){
        level_cache.sets_level[i].setnum_of_blocks_in_set_level(assosciativity);
        level_cache.sets_level[i].blocks_level = new Block[assosciativity];
        for(int j=0;j<assosciativity;j++){
            level_cache.sets_level[i].blocks_level[j].data = new int[level_cache.level_offset_size];
            for(int k=0;k<level_cache.level_offset_size;k++){
                level_cache.sets_level[i].blocks_level[j].data[k]=-1000000;
            }
        }
    }
    //////// call the acess  
  //////////////////////////  level1_cache.access(addr);
  return level_cache;
}
int Cache::req_set_index_level(Cache c,string addr){
    ///////////////////////////////// to get set index
    c.level_index_str = addr.substr(13-(int)log2(c.level_offset_size)-(int)log2(c.level_index_size),(int)log2(c.level_index_size));
  // cout<<c.level_index_str<<"ooooooooofffffffssssssseeeeeeetttttttsssssssstttttrrrrrrr"<<endl;
    int set_index_required = stoi(c.level_index_str,nullptr,2);
   // cout<<set_index_required<<endl;
    return set_index_required;
}
int Cache::req_data_position_in_block(Cache c,string addr){
    c.level_offset_str = addr.substr(13-(int)log2(c.level_offset_size),(int)log2(c.level_offset_size));
    //cout<<c.level_offset_str<<"ooooooooofffffffssssssseeeeeeetttttttsssssssstttttrrrrrrr"<<endl;
    int yy = stoi(c.level_offset_str,nullptr,2);
    return yy;
}
int Cache::access_level(Cache c,string addr){
    int req_set = req_set_index_level(c,addr);
    string level_block_tag = addr.substr(0,13-((int)log2(c.level_offset_size)+(int)log2(c.level_index_size)));
    int req_block = sets_level[req_set].find_block_in_set_level(level_block_tag,sets_level[req_set],addr);
    if(req_block!=-1){
    c.level_offset_str = addr.substr(13-(int)log2(c.level_offset_size),(int)log2(c.level_offset_size));
    int data_index_in_block_req = stoi(c.level_offset_str,nullptr,2);
   return c.sets_level[req_set].blocks_level[req_block].data[data_index_in_block_req];
    }
    return INT_MAX;
}
int Cache::getnum_of_sets_in_cache_level(){
    return num_of_sets_in_cache_level;
}
void Cache::setnum_of_sets_in_cache_level(int x){
    num_of_sets_in_cache_level = x;
}

void display_cache_level(Cache c){
    cout<<c.getnum_of_sets_in_cache_level()<<endl;
   cout<<c.level_index_size<<endl;
    cout<<c.level_offset_size<<endl;
    for(int i=0;i<c.level_index_size;i++){
        for(int j=0;j<c.sets_level[i].getnum_of_blocks_in_set_level();j++){
            cout<<"tag"<<c.sets_level[i].blocks_level[j].tag<<endl;
            cout<<"lru"<<c.sets_level[i].blocks_level[j].lru<<endl;
            for(int k=0;k<c.level_offset_size;k++){
                cout<<"data"<<c.sets_level[i].blocks_level[j].data[k]<<endl;
            }
            cout<<"-----------------"<<endl;
        }
    }
}

string bin(long n)
  {
    long i;
    string s;
    s=s+"0";
    for (i = 1 << 30; i > 0; i = i / 2)
    {
      if((n & i) != 0)
      {
        s=s+"1";
      }
      else
      {
        s=s+"0";
      }
    }
    return s;
  }

void write_back_to_main_memory(Cache c,int *memo){
    for(int i=0;i<c.level_index_size;i++){
        for(int j=0;j<c.sets_level[i].getnum_of_blocks_in_set_level();j++){
             if(c.sets_level[i].blocks_level[j].dirty==1){
                for(int k=0;k<c.level_offset_size;k++){
                    int yy = (int)log2(c.level_offset_size);
                    string ss1 = bin(j).substr(32-(yy+1),yy+1);
                    int xx = 13-((int)log2(c.level_offset_size))-(c.sets_level[i].blocks_level[j].tag.length());
                    string ss2 = bin(i).substr(32-(xx+1),xx+1);
                    string to_be_replaced_address = c.sets_level[i].blocks_level[j].tag+ss2+ss1;
                    int qq = stoi(to_be_replaced_address,nullptr,2);
                    memo[qq] = c.sets_level[i].blocks_level[j].data[k];
                }
            cout<<"-----------------"<<endl;
             }
        }
    }
}

//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

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
    vector<struct type_of_offset>lwsw_offset_info;
    vector<string> instructions_with_stalls;
    int number_of_labels_found;
    int number_of_instructions_executed;
    void RemoveSpaces(string &str);
    void display();
    void run();
    int clockcycle;
    int stalls;
    int main_found;
    int lru1;
    int lru2;
    int level1_cache_latency;
    int level2_cache_latency;
    int main_mem_latency;
    int hit;
    int lwsw_ins;
    ////////////////////////////////
    Cache sim_level1;
    Cache sim_level2;
    int block_size1;
    int block_size2;
    string instruction_fetch(int n);
    int * instruction_drf(string s);
    int is_there_datahazard(int n);
    int instruction_execute(int n);
    int instruction_memory(int m,int n);
    void instruction_writeback(int m,int n);
    void valid_register(string s,int n);
    simulator(ifstream& file,int need,Cache level1,Cache level2,int b1,int b2,int lca1,int lca2,int mma);

};

void simulator::run(){
    for(int i=0;i<32;i++){  // initializing all reister values to 0
        value_of_registers[i]=0;
    }
    clockcycle=1; /////////////////////////////////////////////////////////////
    memory_element_values[1000]={0};
    position_of_element_in_memory=0;
    hit = 0;
    lwsw_ins = 0;
    lru1=0;
    lru2=0;
   main_found=-1;
    int data_found=-1;
    int text_found=-1;
    for(int i=0;i<number_of_instructions;i++){ // to find .data
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
    for(int j=data_found+1;j<number_of_instructions;j++){ // to find .text
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
        // storing the word data in vector memory used i program with its name,start address,
         if(x!=-1 && x!=0){
            list_name=s.substr(0,x);
            
            m.name=list_name;
            m.start_address=position_of_element_in_memory;
            memory_used_in_program.push_back(m);
            string s1=s.substr(x+6);
            stringstream commas(s1);
            string intermediate1;
            while(getline(commas,intermediate1,',')){ // storing the values in memoryelement values array
          
            memory_element_values[position_of_element_in_memory++]=stoi(intermediate1);
            }
        }
     }   
    for(int k=text_found+1;k<number_of_instructions;k++){ // reading the instructions
        int x=-1;
        string s = program[k];
        RemoveSpaces(s);
        if(s=="main:"){ // finding main
            main_found=k;
        }
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
    cout<<"number of instructions"<<number_of_instructions<<endl; 
    while(pc<number_of_instructions){ // instructions
       // cout<<pc;
        string s2=program[pc];
        RemoveSpaces(s2);  // to rempve sapces in instructions
        if(s2=="stop"){ // if stop is encountered terminate the program
            pc=number_of_instructions+1;
        }
        if(s2==""){
            pc++;
            continue;
        }
        int is_label=-1;
        is_label=program[pc].find(":"); // to check whether it is of label type
        if(is_label!=-1){
            number_of_labels_found++; // increment number of labels found
            pc++;
            continue;
        }
        string ifs = instruction_fetch(pc); // calling instruction fetch
       number_of_instructions_executed++;
        int* idrf_regs;
        idrf_regs = instruction_drf(ifs);// calling decode register fetch
        int type_of_operation = idrf_regs[3];
        int exe_val = instruction_execute(type_of_operation); // execute function
        int mem_val = instruction_memory(exe_val,type_of_operation); // memory function
        instruction_writeback(mem_val,type_of_operation);  // writebac function
        clockcycle++; // incrementing clock cycle
        cout<<"###########################################################################"<<endl;
        for(int i=0;i<32;i++){
         cout<<registers[i]<<"  "<<value_of_registers[i]<<endl;
        }
        cout<<"##############################################################################"<<endl;
    }
    // at the end if there is any changes left in cache write back them to main memory
    write_back_to_main_memory(sim_level1,memory_element_values); 
    write_back_to_main_memory(sim_level2,memory_element_values);
}

int simulator::is_there_datahazard(int n){ // for finding hazards
    if(data_forwarding_enabled==false){
        int x1=-2;
    if(n==1){ // if it is first instruction
       x1=0;
    }
   else if(n==2){ // second instruction check with the previous
        int reg_pres_type = each_instruction_info[each_instruction_info.size()-1].typeo;
        int reg_prev_type = each_instruction_info[each_instruction_info.size()-2].typeo;
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = -1;
       if(reg_pres_type == 13 || reg_pres_type == 14){
           reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r1;
       }
        else{
          reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        }
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        if(reg_pres_type==11){
            
            if(lwsw_offset_info[lwsw_offset_info.size()-1].type==1){
                
                if(reg_src1_pres==reg_dest_prev){
                if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
                   x1=3;
                }
                else
                 x1=0;
                }
                else 
                x1=0;
            }
            else{
            
            if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
            if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
            x1=3;
            }
            else{
            x1=0;
            }
            }
            else
            x1=0;
            
            } //a1;
        }
        else if(reg_pres_type==13 || reg_pres_type==14){
            
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev){
               if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){ 
                x1=3+2;//add 2
            }
            else{
                x1=0+2;
            }
            }
            else 
            x1=0+2;
            
        }   
        else{
          
        if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
            if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
            x1=3;
        }
        else{
            x1=0;
        }
        }
        
        else x1=0;
        }
    }
    
    else if(n==3){
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        int reg_dest_prev_prev = each_instruction_info[each_instruction_info.size()-3].r1;
        int reg_pres_type = each_instruction_info[each_instruction_info.size()-1].typeo;
        int reg_prev_type = each_instruction_info[each_instruction_info.size()-2].typeo;
        int reg_prev_prev_type = each_instruction_info[each_instruction_info.size()-3].typeo;
        int reg_src1_prev = each_instruction_info[each_instruction_info.size()-2].r2;
        int reg_src2_prev = each_instruction_info[each_instruction_info.size()-2].r3;
        int reg_src1_prev_prev = each_instruction_info[each_instruction_info.size()-3].r2;
        int reg_src2_prev_prev = each_instruction_info[each_instruction_info.size()-3].r3;

        if(reg_pres_type==11){
            if(lwsw_offset_info[lwsw_offset_info.size()-1].type==1){
                if(reg_src1_pres==reg_dest_prev){
                  if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
                   x1=3;
                  }
                  else x1=0;
                }
                else if(reg_src1_pres==reg_dest_prev_prev){
                     if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){
                    x1=2;
                     }
                     else x1=0;
                }
                else
                 x1=0;
            }
            else{
            if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
            if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){    
            x1=3;
            }
            else x1=0;
            }
            else if(reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
            if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){    
            x1=2;
            }
            else x1=0;
            }
            else{
            x1=0;
            }
        }
        }
        else if(reg_pres_type==13 || reg_pres_type==14){
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev ){
                x1=3+2;
            }
            else if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev_prev ){
                x1=2+2;
            }
            else{
                x1=0+2;
            }
        }
        else{
         if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
             if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
            x1=3;
             }
             else x1=0;

        }
        else if(reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
            if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){
              if(reg_src2_pres==reg_dest_prev_prev){
                  if(reg_src2_pres==reg_src1_prev || reg_src2_pres==reg_src2_prev){
                      x1=0;
                  }
                  else{
                      x1=2;
                  }
              }
              else{
                  if(reg_src1_pres==reg_src1_prev || reg_src1_pres==reg_src2_prev){
                      x1=0;
                  }
                  else{
                      x1=2;
                  }
              }
            }
            else
             x1=0;
        }
        else{
           x1=0;
        }
        }
    }
    else{
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        int reg_dest_prev_prev = each_instruction_info[each_instruction_info.size()-3].r1;
        int reg_dest_prev_prev_prev = each_instruction_info[each_instruction_info.size()-4].r1;
        int reg_pres_type = each_instruction_info[each_instruction_info.size()-1].typeo;
        int reg_prev_type = each_instruction_info[each_instruction_info.size()-2].typeo;
        int reg_prev_prev_type = each_instruction_info[each_instruction_info.size()-3].typeo;
        int reg_prev_prev_prev_type = each_instruction_info[each_instruction_info.size()-4].typeo;
        int reg_src1_prev = each_instruction_info[each_instruction_info.size()-2].r2;
        int reg_src2_prev = each_instruction_info[each_instruction_info.size()-2].r3;
        int reg_src1_prev_prev = each_instruction_info[each_instruction_info.size()-3].r2;
        int reg_src2_prev_prev = each_instruction_info[each_instruction_info.size()-3].r3;
        if(reg_pres_type==11){
            if(lwsw_offset_info[lwsw_offset_info.size()-1].type==1){
                if(reg_src1_pres==reg_dest_prev){
                if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){
                   x1=3;
                }
                else x1=0;
                }
                else if(reg_src1_pres==reg_dest_prev_prev){
                    if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){
                    x1=2;
                    }
                    else x1=0;
                }
                else if(reg_src1_pres==reg_dest_prev_prev_prev){
                    if(reg_prev_prev_prev_type!=13 && reg_prev_prev_prev_type!=14 && reg_prev_prev_prev_type!=15){
                    x1=1;
                   }
                   else x1=0;
                }
                else
                 x1=0;
            }
            else{
            if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
              if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){  
            x1=3;
              }
              else x1=0;
            }
            else if(reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
             if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){   
            x1=2;
            }
            else x1=0;
            }
            else if(reg_src2_pres == reg_dest_prev_prev_prev || reg_src1_pres == reg_dest_prev_prev_prev){
                if(reg_prev_prev_prev_type!=13 && reg_prev_prev_prev_type!=14 && reg_prev_prev_prev_type!=15){
            x1=1;
            }
            else x1=0;
            }
            else{
            x1=0;
            }
        }
        }
        else if(reg_pres_type==13 || reg_pres_type==14){
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev ){
                x1=3+2; 
            }
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev_prev ){
                x1=2+2;  
            }
            else if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev_prev_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev_prev_prev){
                x1=1+2;  
            }
            else{
                x1=0+2;   
            }
        }
        else{
         if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
             if(reg_prev_type!=13 && reg_prev_type!=14 && reg_prev_type!=15){ 
            
            x1=3;
             }
             else x1=0;
        }
        else if(reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
            if(reg_prev_prev_type!=13 && reg_prev_prev_type!=14 && reg_prev_prev_type!=15){
              if(reg_src2_pres==reg_dest_prev_prev){
                  if(reg_src2_pres==reg_src1_prev || reg_src2_pres==reg_src2_prev){
                      x1=0;
                  }
                  else{
                      x1=2;
                  }
              }
              else{
                  if(reg_src1_pres==reg_src1_prev || reg_src1_pres==reg_src2_prev){
                      x1=0;
                  }
                  else{
                      x1=2;
                  }
              }
            }
            else
             x1=0;
        }
        else if(reg_src2_pres == reg_dest_prev_prev_prev || reg_src1_pres == reg_dest_prev_prev_prev){
             if(reg_prev_prev_prev_type!=13 && reg_prev_prev_prev_type!=14 && reg_prev_prev_prev_type!=15){
                if(reg_src2_pres==reg_dest_prev_prev_prev){
                  if(reg_src2_pres==reg_src1_prev || reg_src2_pres==reg_src2_prev || reg_src2_pres==reg_src2_prev_prev || reg_src2_pres == reg_src1_prev_prev){
                      x1=0;
                  }
                  else{
                      x1=1;
                  }
              }
              else{
                  if(reg_src1_pres==reg_src1_prev || reg_src1_pres==reg_src2_prev || reg_src2_pres==reg_src2_prev_prev || reg_src2_pres == reg_src1_prev_prev){
                      x1=0;
                  }
                  else{
                      x1=1;
                  }
              }
             }
             else x1=0;
        }
        else{
           x1=0;
        }
        }

    }
      return x1;
    }
    else{            ////////////////////////////////forwarding
        int x2=-2;
    if(n==1){
        x2=0;
    } 
    else if(n==2){
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        int reg_pres_type = each_instruction_info[each_instruction_info.size()-1].typeo;
        int reg_prev_type = each_instruction_info[each_instruction_info.size()-2].typeo;
        if(reg_pres_type==13 || reg_pres_type==14){
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev ){
                if(each_instruction_info[each_instruction_info.size()-2].typeo==11||each_instruction_info[each_instruction_info.size()-2].typeo==12){
                    x2=1+2;
                }
                else{
                    x2=0+2;
                }
            }
            else{
                x2=0+2;
            }
        }
        else{
            if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
           if(reg_prev_type==11){
             
             x2=1;
           }
           else if(reg_prev_type==13 || reg_prev_type==14){
             
              x2=0;
           }
           else{
              
              x2=0;
           }
        }
        }
    }
    else{
        int reg_src1_pres = each_instruction_info[each_instruction_info.size()-1].r2;
        //cout<<"present"<<reg_src1_pres<<endl;
        int reg_src2_pres = each_instruction_info[each_instruction_info.size()-1].r3;
        //cout<<"present"<<reg_src2_pres<<endl;
        int reg_dest_prev = each_instruction_info[each_instruction_info.size()-2].r1;
        //cout<<"present"<<reg_dest_prev<<endl;
        int reg_dest_prev_prev = each_instruction_info[each_instruction_info.size()-3].r1;
        //cout<<"present"<<reg_dest_prev_prev<<endl;
        int reg_pres_type = each_instruction_info[each_instruction_info.size()-1].typeo;
        int reg_prev_type = each_instruction_info[each_instruction_info.size()-2].typeo;
        if(reg_pres_type==15){
            x2=2;
        }
        else if(reg_pres_type==13 || reg_pres_type==14){
            if(each_instruction_info[each_instruction_info.size()-1].r1==reg_dest_prev ||each_instruction_info[each_instruction_info.size()-1].r2==reg_dest_prev ){
                if(each_instruction_info[each_instruction_info.size()-2].typeo==11||each_instruction_info[each_instruction_info.size()-2].typeo==12){
                    x2=1+2;
                }
                else{
                    x2=0+2;
                }
            }
            else{
                x2=0+2;
            }
        }
        else if(reg_src2_pres == reg_dest_prev || reg_src1_pres == reg_dest_prev){
           if(reg_prev_type==11){    
             x2=1;
           }
           else if(reg_prev_type==13 || reg_prev_type==14){ 
              x2=0;
           }
           else{  
              x2=0;
           }
        }
        else if(reg_src2_pres == reg_dest_prev_prev || reg_src1_pres == reg_dest_prev_prev){
           // cout<<"hhhhhhhhhhh333333333222222222"<<endl;
            
            x2=0;
          
        }
        else{
           x2=0;
        }
    }   
        return x2;
    }
}

void simulator::RemoveSpaces(string &str)  
{ 
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); 
    //return str; 
}

simulator::simulator(ifstream& file,int need,Cache level1,Cache level2,int b1,int b2,int lca1,int lca2,int mma){  // start of program
   
    number_of_instructions=0;
    pc=0;
    sim_level1 = level1;
    sim_level2 = level2;
    block_size1 = b1;
    block_size2 = b2;
    level1_cache_latency = lca1;
    level2_cache_latency = lca2;
    main_mem_latency = mma;
    number_of_labels_found=0;
    number_of_instructions_executed=0;
    if(need==1){
    data_forwarding_enabled=true;
    }
    else{
    data_forwarding_enabled=false;
    }
    total_stalls=0;
    file.open("cachefor.s"); // opening the file
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
        program.push_back(line); // keeping all the instructions in vector
        }
    }
   
    file.close();  
}

string simulator::instruction_fetch(int n){
    string s = program[n];
   // cout<<"ififififififififififififififififififi"<<endl;
    RemoveSpaces(s); //removing spaces
    pc=pc+1; // incrementing pc
   // cout<<s<<"JJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJJ"<<endl;
    return s;
}

int * simulator::instruction_drf(string s){
    registers_of_each_instruction r;  ////////////////
    int count=0;
    int size_of_operation;
    string s1=program[pc-1];/////
    for(int i=0;i<4;i++){ //as it have max4 to get size of operation
        if(s1[i]==' '|| s1[i]=='\t')
		{
			break;
		}
        count++;
    }
    string operation =s1.substr(0,count); // to get the operation
    string registers_in_instruction;
    if(count<s.size()-1)
        registers_in_instruction=s.substr(count+1); // registers
    RemoveSpaces(registers_in_instruction);  
    //cout<<registers_in_instruction<<"\\\\\\\\\\\\\\\\\\\\\\"<<endl;  
    int position_in_array=-1;
    for(int i=0;i<17;i++){ // checking with the position
        if(operation==operations_allowed[i]){
            position_in_array=i;
            break;
        }
    }
    
    if(position_in_array==-1){  // it is an invalid operation
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
        valid_register(registers_in_instruction,6); // validating 
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       
    }
    else if(position_in_array<11){
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,10);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
        
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
      
    }
    else if(position_in_array==15){
        // cout<<"11111111111111111111113333333333333333333333333111111111111111111111111111144444444444444444444444"<<endl;
        cout<<registers_in_instruction<<endl;
       // cout<<"11111111111111111111113333333333333333333333333111111111111111111111111111144444444444444444444444"<<endl;
        registers_in_present_instruction[4]={0};
        valid_register(registers_in_instruction,15);
        registers_in_present_instruction[3]=position_in_array;
        r.r1=registers_in_present_instruction[0];
        r.r2=registers_in_present_instruction[1];
        r.r3=registers_in_present_instruction[2];
        r.typeo=registers_in_present_instruction[3];
        each_instruction_info.push_back(r);
       
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
      
    }
    return registers_in_present_instruction;  
}

string to_bit_string(int x){  // to get the bits in form of string
    return bitset<13>(x).to_string(); // as we have 4kb we used 13
}

void fun(int *a,int l,int k,vector<int>&b){ // function to get the required data from main memory to keep in cache
    int f = l%k;
    for(int j=0;j<=l%k;j++){
        b.push_back(a[l-f]);
        f--;
    }
    int k1=1;
    for(int j=l%k+1;j<=(k-1);j++){
        b.push_back(a[l+k1]);
        k1++;
    }
   // cout<<"-----------------"<<endl;
   /* for(int i=0;i<b.size();i++){
        cout<<b[i]<<endl;
    }*/
}
void change_memory(Cache c,int a1,int a2,int k,int l,int *a){ // to change the main memory data values
    for(int i=0;i<k;i++){
     //  cout<<"xxxxxxxxxxx"<<endl;
        cout<<c.sets_level[a1].blocks_level[a2].data[i]<<endl;
        a[l+i]=c.sets_level[a1].blocks_level[a2].data[i];
    }
} 

int simulator::instruction_memory(int m,int n){ // memory function
   // cout<<"wwwwwwwwwwwwwwwwwwhhhhhhhhhhhhhhhhhhhhhaaaaaaaaaaaaaaaaaaaattttttttttttttttt"<<endl;
    int re = -1;
    if(n>=0 && n<=10){
       // return m;
       re = m;
    }
    else if(n==11){ // lw type instruction
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"lw "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        lwsw_ins++;
        string address = to_bit_string(m); // converting the address to a bit string format
        int found_in_level2 = INT_MAX;
      //  cout<<"start checking"<<endl;
        int found_in_level1 = sim_level1.access_level(sim_level1,address);  // to check whether we have that address in level1 cache
       // cout<<"ccccccccccccccccccccccccccccccccccccccccccccccccccccccc"<<endl;
        cout<<found_in_level1<<endl;
       // cout<<"cccccccccccccccccccccccccccccccccccccccccccccccccccccccccc"<<endl;
        if(found_in_level1==INT_MAX){
            found_in_level2 = sim_level2.access_level(sim_level2,address); // checking whether we have that address in level2 cache
        }
        int x1 = sim_level1.req_set_index_level(sim_level1,address); // to get req set index if foud in level1
       // cout<<x1<<endl;
        int x2 = sim_level1.getnum_of_sets_in_cache_level();  // total no of sets in level1
       // cout<<x2<<endl;
        int x3 = sim_level1.sets_level[x1].getnum_of_blocks_in_set_level(); // no of blocks in set in level1
       // cout<<x3<<endl;
        int x4=-1;
        int y1 = sim_level2.req_set_index_level(sim_level2,address);  // to get req set index if found in level2
       // cout<<y1<<endl;
        int y2 = sim_level2.getnum_of_sets_in_cache_level();  // total no of sets in level2
      //  cout<<y2<<endl;
        int y3 = sim_level2.sets_level[y1].getnum_of_blocks_in_set_level();  // to get no of blocks in set in level2
       // cout<<y3<<endl;
        int y4=-1;
        for(int j=0;j<x3;j++){  // checking whether there is empty block satisfying our needs in level1
        if(sim_level1.sets_level[x1].blocks_level[j].tag == "NULL"){
            x4 = j;
            break;
        }
    }
    for(int j=0;j<y3;j++){  // same here with level2
        if(sim_level2.sets_level[y1].blocks_level[j].tag == "NULL"){
            y4 = j;
            break;
        }
    }
    cout<<x4<<endl;
        if(found_in_level1 == INT_MAX && found_in_level2!=INT_MAX){ // miss in level1 an hit in level2
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"found in level 2 cache "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
            lru2++;
            hit++;
          //  sim_level2.sets_level[y1].blocks_level[]
            int y5 = sim_level2.sets_level[y1].req_block_no_in_set_in_level; // as it is hit in level2 increase its lru
          //  cout<<y5<<"nnnnnnnnnnnneeeeeeeeeeeeeeeddddddddddddddddddddddddddddddddddd"<<endl;
            int y6 = sim_level2.req_data_position_in_block(sim_level2,address);
           // cout<<y6<<"innnnnnnndddddeeeeeexxxxxxxx"<<endl;
            re = sim_level2.sets_level[y1].blocks_level[y5].data[y6];  // getting the required data from level 2 cache
           // cout<<re<<"           "<<"***************"<<endl;
            sim_level2.sets_level[y1].blocks_level[y5].lru = lru2;
        if(x4 != -1){  // if there is empty block we can place the block there
        /////////////////////////// add the block into the null place
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"ther is an empty place to keep in level1 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        // here we keep the values we get from the main memory into cache
        for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[x4].data[j] = data_values_in_block[j];
          //  cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ11111111111"<<endl;
            cout<<data_values_in_block[j]<<endl;
        }
        lru1++;
        sim_level1.sets_level[x1].blocks_level[x4].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
        sim_level1.sets_level[x1].blocks_level[x4].lru = lru1;
        }
        else{ // if there is no place we have to apply lru in level1 to replace the block
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"found in level 2 cache "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
            int b1=-1;
            int min1=100000;
            for(int k=0;k<x3;k++){ // to get the block with least lru in level1
                if(sim_level1.sets_level[x1].blocks_level[k].lru<min1){
                min1 = sim_level1.sets_level[x1].blocks_level[k].lru;
                b1=k;
                }
            }
             int yy = (int)log2(block_size1);
             string ss1 = bin(block_size1).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size1))-(sim_level1.sets_level[x1].blocks_level[b1].tag.length());
             string ss2 = bin(x1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level1.sets_level[x1].blocks_level[b1].tag+ss2+ss1;  // this is the address of the block that is to be replaced according to lru policy
            // we search this address in cache level2 and update the value
             int set_index_to_search = sim_level2.req_set_index_level(sim_level2,to_be_replaced_address);
             int xx2 = (int)log2(block_size2);
             int xx3 = (int)log2(sim_level2.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level2.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level2.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size2;f1++){
                         sim_level2.sets_level[set_index_to_search].blocks_level[f].data[f1] = sim_level1.sets_level[x1].blocks_level[b1].data[f1];
                         
                     }
                     sim_level2.sets_level[set_index_to_search].blocks_level[f].lru = sim_level1.sets_level[x1].blocks_level[b1].lru;
                     sim_level2.sets_level[set_index_to_search].blocks_level[f].dirty = sim_level1.sets_level[x1].blocks_level[b1].dirty;
                 }
             }
             /////////////////////////////////////////////////////////////////////
             /////////////////////////////////////////////////////////////////////

             // keeping the block in the place 
            vector<int> data_values_in_block;
            fun(memory_element_values,m,block_size1,data_values_in_block);
            for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[b1].data[j] = data_values_in_block[j];
            }
            lru1++;
            sim_level1.sets_level[x1].blocks_level[b1].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
            sim_level1.sets_level[x1].blocks_level[b1].lru = lru1; 
            sim_level1.sets_level[x1].blocks_level[b1].dirty=0;
        }
        clockcycle = clockcycle+(level1_cache_latency+level2_cache_latency)-1;
        }

       else if(found_in_level2==INT_MAX && found_in_level1==INT_MAX){   //miss in both level1 and level2
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"no place in level1 and level2 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
        if(x4 != -1){ 
        /////////////////////////// add the block into the null place
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"ther is place in level1"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        for(int j=0;j<block_size1;j++){ // keeping data values
            sim_level1.sets_level[x1].blocks_level[x4].data[j] = data_values_in_block[j];
        }
        lru1++; 
        sim_level1.sets_level[x1].blocks_level[x4].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2)); // updating tag
        sim_level1.sets_level[x1].blocks_level[x4].lru = lru1; // updating the lru
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(y4!=-1){ // there is empty place in level2 cache
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"ther is an empty place to keep in level2 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size2,data_values_in_block2);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[y4].data[j] = data_values_in_block2[j];
        }
        lru2++;
        sim_level2.sets_level[y1].blocks_level[y4].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[y4].lru = lru2;
        }
        else{  // there is no empty place in level2 
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"no empty place in level2 we have to replace "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        int b2=-1;
        int min2=0;
        for(int k=0;k<y3;k++){
            if(sim_level2.sets_level[y1].blocks_level[k].lru<min2){
                min2 = sim_level2.sets_level[y1].blocks_level[k].lru;
                b2=k;
            }
        }
        // here this if condition should be there
        if(sim_level2.sets_level[y1].blocks_level[b2].dirty==1){ // if the block to be replaced in level1 have dirty bit 1
                // write back to memory
                int yo = m-(m%block_size2);
                change_memory(sim_level2,y1,b2,block_size2,yo,memory_element_values);  // write it back to the main memory
         }

         /////////////////////////////////////////////////////////////////////

             int yy = (int)log2(block_size2);
             string ss1 = bin(block_size2).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size2))-(sim_level2.sets_level[y1].blocks_level[b2].tag.length());
             string ss2 = bin(y1).substr(32-(xx+1),xx+1);

             string to_be_replaced_address = sim_level2.sets_level[y1].blocks_level[b2].tag+ss2+ss1;
             // here we check the above address in level1 also and free it
             int set_index_to_search = sim_level1.req_set_index_level(sim_level1,to_be_replaced_address);
             int xx2 = (int)log2(block_size1);
             int xx3 = (int)log2(sim_level1.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level1.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level1.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size1;f1++){
                         sim_level1.sets_level[set_index_to_search].blocks_level[f].data[f1] = -1000000;
                         
                     }
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].lru = 0;
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].tag="NULL";
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].dirty=0;
                     break;
                 }
             }
             ////////////////////////////////////////////////////////////////////////////////////////////
             ///////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////////////////////////////////////////////////////////////////////
             //////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////////////////////////////////////////////////////////////
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size1,data_values_in_block2); // we replace the block
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[b2].data[j] = data_values_in_block2[j];
        }
        lru2++;
        sim_level2.sets_level[y1].blocks_level[b2].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[b2].lru = lru2;
        sim_level2.sets_level[y1].blocks_level[b2].dirty=0;
        }
    }
    else{ // there is no enough place in level1
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is no enough place in level1 we have to replace"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"99999999999999999999999999999999999999999999999"<<endl;
        int b1=-1;
        int min1=100000;
        for(int k=0;k<x3;k++){
            if(sim_level1.sets_level[x1].blocks_level[k].lru<min1){
                min1 = sim_level1.sets_level[x1].blocks_level[k].lru;
                b1=k;
            }
        }
             int yy = (int)log2(block_size1);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size1).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size1))-(sim_level1.sets_level[x1].blocks_level[b1].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(x1).substr(32-(xx+1),xx+1);

             string to_be_replaced_address = sim_level1.sets_level[x1].blocks_level[b1].tag+ss2+ss1;

             int set_index_to_search = sim_level2.req_set_index_level(sim_level2,to_be_replaced_address);
             int xx2 = (int)log2(block_size2);
             int xx3 = (int)log2(sim_level2.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level2.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level2.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size2;f1++){
                         sim_level2.sets_level[set_index_to_search].blocks_level[f].data[f1] = sim_level1.sets_level[x1].blocks_level[b1].data[f1];
                         
                     }
                     sim_level2.sets_level[set_index_to_search].blocks_level[f].lru = sim_level1.sets_level[x1].blocks_level[b1].lru;
                 }
             }

        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[b1].data[j] = data_values_in_block[j];
           // cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<<endl;
            //cout<<data_values_in_block[j]<<endl;
        }
        lru1++;
        sim_level1.sets_level[x1].blocks_level[b1].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
        sim_level1.sets_level[x1].blocks_level[b1].lru = lru1;

        if(y4!=-1){
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"ther is place in level2  "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;

        vector<int> data_values_in_block1;
        fun(memory_element_values,m,block_size2,data_values_in_block1);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[y4].data[j] = data_values_in_block1[j];
            //cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<<endl;
           // cout<<data_values_in_block1[j]<<endl;
        }
        lru2++;
        sim_level2.sets_level[y1].blocks_level[y4].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[y4].lru = lru2;
        }
        else{
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is no enough place in level2 we need to replace"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        int b2=-1;
        int min2=0;
        for(int k=0;k<y3;k++){
            if(sim_level2.sets_level[y1].blocks_level[k].lru<min1){
                min2 = sim_level2.sets_level[y1].blocks_level[k].lru;
                b2=k;
            }
        }
        if(sim_level2.sets_level[y1].blocks_level[b2].dirty==1){ // if the block to be replaced in level1 have dirty bit 1
                // write back to memory
                 //cout<<"bbbbbbbbbboooooooooooooooommmmmmmmmmmmm2222222222222222222222"<<endl;
                int yo = m-(m%block_size2);
                change_memory(sim_level2,y1,b2,block_size2,yo,memory_element_values);  // write it back to the main memory
        }
             int yy = (int)log2(block_size2);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size2).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size2))-(sim_level2.sets_level[y1].blocks_level[b2].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(y1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level2.sets_level[y1].blocks_level[b2].tag+ss2+ss1;

             int set_index_to_search = sim_level1.req_set_index_level(sim_level1,to_be_replaced_address);
             int xx2 = (int)log2(block_size1);
             int xx3 = (int)log2(sim_level1.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level1.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level1.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size1;f1++){
                         sim_level1.sets_level[set_index_to_search].blocks_level[f].data[f1] = -1000000;
                         
                     }
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].lru = 0;
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].tag="NULL";
                     break;
                 }
             }
             //////////////////////////////////////////////////////////////////////////////////////////////////
             ////////////////////////////////////////////////////////////////////////////////////////
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size1,data_values_in_block2);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[b2].data[j] = data_values_in_block2[j];
            //cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<<endl;
           // cout<<data_values_in_block2[j]<<endl;
        }
        lru2++;
        sim_level2.sets_level[y1].blocks_level[b2].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[b2].lru = lru2;
        }

      }
      clockcycle = clockcycle+(level1_cache_latency+level2_cache_latency+main_mem_latency)-1;
      re = memory_element_values[m];
    }
        else{
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"found in level1 cache itself"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        hit++;
            //cout<<"===================================================================="<<endl;
             int x5 = sim_level1.sets_level[x1].req_block_no_in_set_in_level; // as it is hit in level2 increase its lru
            //cout<<x5<<"nnnnnnnnnnnneeeeeeeeeeeeeeeddddddddddddddddddddddddddddddddddd"<<endl;
           int x6 = sim_level1.req_data_position_in_block(sim_level1,address);
          // cout<<x6<<"innnnnnnndddddeeeeeexxxxxxxx"<<endl;
            lru1++;
            sim_level1.sets_level[x1].blocks_level[x5].lru = lru1;
            re = sim_level1.sets_level[x1].blocks_level[x5].data[x6];
            clockcycle = clockcycle+level1_cache_latency-1;
        }
      // re = memory_element_values[m];
      cout<<"rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr"<<endl;
      cout<<re<<endl;
      cout<<"eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee"<<endl;
    }

    else if(n==12){ // instruction of type sw
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"sw case"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        lwsw_ins++;
        // this case is also similar to lw but we aslso change the data we keep in cache 
       // cout<<"ssssssssssssssssssssssssssssssssssssssssssssssssssswwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"<<endl;
         string address = to_bit_string(m); // converting the address to a bit string format
        int found_in_level2 = INT_MAX;
        int found_in_level1 = sim_level1.access_level(sim_level1,address);  // to check whether we have that address in level1 cache
            found_in_level2 = sim_level2.access_level(sim_level2,address); // checking whether we have that address in level2 cache
        int x1 = sim_level1.req_set_index_level(sim_level1,address); // to get req set index if foud in level1
        int x2 = sim_level1.getnum_of_sets_in_cache_level();  // total no of sets in level1
        int x3 = sim_level1.sets_level[x1].getnum_of_blocks_in_set_level(); // no of blocks in set in level1
        int x4=-1;
        int x6 = sim_level1.req_data_position_in_block(sim_level1,address);
        int y1 = sim_level2.req_set_index_level(sim_level2,address);  // to get req set index if found in level2
        int y2 = sim_level2.getnum_of_sets_in_cache_level();  // total no of sets in level2
        int y3 = sim_level2.sets_level[y1].getnum_of_blocks_in_set_level();  // to get no of blocks in set in level2
        int y4=-1;
        int y6 = sim_level2.req_data_position_in_block(sim_level2,address);
        for(int j=0;j<x3;j++){  // checking whether there is empty block satisfying our needs in level1
            if(sim_level1.sets_level[x1].blocks_level[j].tag == "NULL"){
                x4 = j;
                break;
            }
        }
        for(int j=0;j<y3;j++){  // same here with level2
            if(sim_level2.sets_level[y1].blocks_level[j].tag == "NULL"){
                y4 = j;
                break;
            }
        }
        if(found_in_level2==INT_MAX && found_in_level1==INT_MAX){   //miss in both level1 and level2
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"didnt found in lvel1 and level2"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"|||||||||||||||||||||||||||||||||||||||||||||||||||||||"<<endl;
        if(x4 != -1){ 
            cout<<"-----------------------------------------------------------"<<endl;
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is enough place in level1 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        /////////////////////////// add the block into the null place
        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[x4].data[j] = data_values_in_block[j];
        }
        sim_level1.sets_level[x1].blocks_level[x4].data[x6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level1.sets_level[x1].blocks_level[x4].dirty=1;
        lru1++;
        sim_level1.sets_level[x1].blocks_level[x4].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
        sim_level1.sets_level[x1].blocks_level[x4].lru = lru1;
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(y4!=-1){
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is enough place in level2"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size2,data_values_in_block2);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[y4].data[j] = data_values_in_block2[j];
        }
       // cout<<y6<<"                  "<<"}}}}}}}}}}}}}}}}}"<<endl;
        sim_level2.sets_level[y1].blocks_level[y4].data[y6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level2.sets_level[y1].blocks_level[y4].dirty=1;
        lru2++;
        sim_level2.sets_level[y1].blocks_level[y4].tag = address.substr(0,32-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[y4].lru = lru2;
        }
        else{
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"no enough place in level2 we need to replace "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        int b2=-1;
        int min2=0;
        for(int k=0;k<y3;k++){
            if(sim_level2.sets_level[y1].blocks_level[k].lru<min2){
                min2 = sim_level2.sets_level[y1].blocks_level[k].lru;
                b2=k;
            }
        }
        if(sim_level2.sets_level[y1].blocks_level[b2].dirty==1){ // if the block to be replaced in level2 have dirty bit 1
                // write back to memory
                int yo = m-(m%block_size2);
                change_memory(sim_level2,y1,b2,block_size2,yo,memory_element_values);  // write it back to the main memory
         }

            int yy = (int)log2(block_size2);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size2).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size2))-(sim_level2.sets_level[y1].blocks_level[b2].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(y1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level2.sets_level[y1].blocks_level[b2].tag+ss2+ss1;

             int set_index_to_search = sim_level1.req_set_index_level(sim_level1,to_be_replaced_address);
             int xx2 = (int)log2(block_size1);
             int xx3 = (int)log2(sim_level1.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level1.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level1.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size1;f1++){
                         sim_level1.sets_level[set_index_to_search].blocks_level[f].data[f1] = -1000000;
                         
                     }
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].lru = 0;
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].tag="NULL";
                     break;
                 }
             }
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size1,data_values_in_block2);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[b2].data[j] = data_values_in_block2[j];
        }
        sim_level2.sets_level[y1].blocks_level[b2].data[y6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level2.sets_level[y1].blocks_level[b2].dirty=1;
        lru2++;
        sim_level2.sets_level[y1].blocks_level[b2].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[b2].lru = lru2;
        }
    }
    else{
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"no place in level1 we need to replace "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        cout<<"++++++++++++++++++++++++++++++++++++++++++++"<<endl;
        int b1=-1;
        int min1=100000;
        for(int k=0;k<x3;k++){
            if(sim_level1.sets_level[x1].blocks_level[k].lru<min1){
                min1 = sim_level1.sets_level[x1].blocks_level[k].lru;
                b1=k;
            }
        }
        cout<<b1<<endl;
        int yy = (int)log2(block_size1);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size1).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size1))-(sim_level1.sets_level[x1].blocks_level[b1].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(x1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level1.sets_level[x1].blocks_level[b1].tag+ss2+ss1;

             int set_index_to_search = sim_level2.req_set_index_level(sim_level2,to_be_replaced_address);
             int xx2 = (int)log2(block_size2);
             int xx3 = (int)log2(sim_level2.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level2.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level2.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size2;f1++){
                         sim_level2.sets_level[set_index_to_search].blocks_level[f].data[f1] = sim_level1.sets_level[x1].blocks_level[b1].data[f1];
                         
                     }
                     sim_level2.sets_level[set_index_to_search].blocks_level[f].lru = sim_level1.sets_level[x1].blocks_level[b1].lru;
                 }
             }
       // cout<<"111111111122222222222222233333333333333333333"<<endl;
        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        for(int j=0;j<block_size1;j++){
        }
        sim_level1.sets_level[x1].blocks_level[b1].data[x6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level1.sets_level[x1].blocks_level[b1].dirty=1;
        lru1++;
        sim_level1.sets_level[x1].blocks_level[b1].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
        sim_level1.sets_level[x1].blocks_level[b1].lru = lru1;

        if(y4!=-1){
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is place in level2 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block1;
        fun(memory_element_values,m,block_size2,data_values_in_block1);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[y4].data[j] = data_values_in_block1[j];
        }
        sim_level2.sets_level[y1].blocks_level[y4].data[y6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level2.sets_level[y1].blocks_level[y4].dirty=1;
        lru2++;
        sim_level2.sets_level[y1].blocks_level[y4].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[y4].lru = lru2;
        }
        else{
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"there is no enough place in level2 we need to replace"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        int b2=-1;
        int min2=0;
        for(int k=0;k<y3;k++){
            if(sim_level2.sets_level[y1].blocks_level[k].lru<min1){
                min2 = sim_level2.sets_level[y1].blocks_level[k].lru;
                b2=k;
            }
        }
        if(sim_level2.sets_level[y1].blocks_level[b2].dirty==1){ // if the block to be replaced in level1 have dirty bit 1
                // write back to memory
                int yo = m-(m%block_size2);
                change_memory(sim_level2,y1,b2,block_size2,yo,memory_element_values);  // write it back to the main memory
        }

         int yy = (int)log2(block_size2);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size2).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size2))-(sim_level2.sets_level[y1].blocks_level[b2].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(y1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level2.sets_level[y1].blocks_level[b2].tag+ss2+ss1;

             int set_index_to_search = sim_level1.req_set_index_level(sim_level1,to_be_replaced_address);
             int xx2 = (int)log2(block_size1);
             int xx3 = (int)log2(sim_level1.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level1.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level1.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size1;f1++){
                         sim_level1.sets_level[set_index_to_search].blocks_level[f].data[f1] = -1000000;
                         
                     }
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].lru = 0;
                     sim_level1.sets_level[set_index_to_search].blocks_level[f].tag="NULL";
                     break;
                 }
             }
        vector<int> data_values_in_block2;
        fun(memory_element_values,m,block_size1,data_values_in_block2);
        for(int j=0;j<block_size2;j++){
            sim_level2.sets_level[y1].blocks_level[b2].data[j] = data_values_in_block2[j];
           // cout<<"QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ"<<endl;
           // cout<<data_values_in_block2[j]<<endl;
        }
        sim_level2.sets_level[y1].blocks_level[b2].data[y6]=value_of_registers[registers_in_present_instruction[0]];
        sim_level2.sets_level[y1].blocks_level[b2].dirty=1;
        lru2++;
        sim_level2.sets_level[y1].blocks_level[b2].tag = address.substr(0,13-(int)log2(block_size2)-(int)log2(y2));
        sim_level2.sets_level[y1].blocks_level[b2].lru = lru2;
        }

      }
      clockcycle= clockcycle+(level2_cache_latency+level2_cache_latency+main_mem_latency)-1;
      re = memory_element_values[m];
    }



    else if(found_in_level1 == INT_MAX && found_in_level2!=INT_MAX){ // miss in level1 an hit in level2
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"found in level 2 cache not in level1 cache"<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
            lru2++;
            hit++;
          //  sim_level2.sets_level[y1].blocks_level[]
            int y5 = sim_level2.sets_level[y1].req_block_no_in_set_in_level; // as it is hit in level2 increase its lru
            int y6 = sim_level2.req_data_position_in_block(sim_level2,address);
            sim_level2.sets_level[y1].blocks_level[y5].lru = lru2;
            sim_level2.sets_level[y1].blocks_level[y5].data[y6] = value_of_registers[registers_in_present_instruction[0]];
        if(x4 != -1){  // if there is empty block we can place the block there
        /////////////////////////// add the block into the null place
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"enough place in level1 "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        vector<int> data_values_in_block;
        fun(memory_element_values,m,block_size1,data_values_in_block);
        for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[x4].data[j] = data_values_in_block[j];
        }
        if(sim_level1.sets_level[x1].blocks_level[x4].data[x6]!=value_of_registers[registers_in_present_instruction[0]]){
         sim_level1.sets_level[x1].blocks_level[x4].dirty=1;
        }
        sim_level1.sets_level[x1].blocks_level[x4].data[x6]=value_of_registers[registers_in_present_instruction[0]];
        lru1++;
        sim_level1.sets_level[x1].blocks_level[x4].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
        sim_level1.sets_level[x1].blocks_level[x4].lru = lru1;
        }
        else{ // if there is no place we have to apply lru in level1 to replace the block
        cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"not enough place in level1 we need to replace "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
            int b1=-1;
            int min1=100000;
            for(int k=0;k<x3;k++){ // to get the block with least lru in level1
                if(sim_level1.sets_level[x1].blocks_level[k].lru<min1){
                min1 = sim_level1.sets_level[x1].blocks_level[k].lru;
                b1=k;
                }
            }
             int yy = (int)log2(block_size1);
             //string ss1 = bitset<yy>(block_size1).to_string();
             string ss1 = bin(block_size1).substr(32-(yy+1),yy+1);
             int xx = 13-((int)log2(block_size1))-(sim_level1.sets_level[x1].blocks_level[b1].tag.length());
             //string ss2 = bitset<((int)log2(xx))>(x1);
             string ss2 = bin(x1).substr(32-(xx+1),xx+1);
             string to_be_replaced_address = sim_level1.sets_level[x1].blocks_level[b1].tag+ss2+ss1;

             int set_index_to_search = sim_level2.req_set_index_level(sim_level2,to_be_replaced_address);
             int xx2 = (int)log2(block_size2);
             int xx3 = (int)log2(sim_level2.getnum_of_sets_in_cache_level());
             string tag_to_be_checked = to_be_replaced_address.substr(0,13-(xx2+xx3));
             for(int f=0;f<sim_level2.sets_level[set_index_to_search].getnum_of_blocks_in_set_level();f++){
                 if(tag_to_be_checked == sim_level2.sets_level[set_index_to_search].blocks_level[f].tag){
                     for(int f1=0;f1<block_size2;f1++){
                         sim_level2.sets_level[set_index_to_search].blocks_level[f].data[f1] = sim_level1.sets_level[x1].blocks_level[b1].data[f1];
                         
                     }
                     sim_level2.sets_level[set_index_to_search].blocks_level[f].lru = sim_level1.sets_level[x1].blocks_level[b1].lru;
                 }
             }
             // keeping the block in that place
            vector<int> data_values_in_block;
            fun(memory_element_values,m,block_size1,data_values_in_block);
            for(int j=0;j<block_size1;j++){
            sim_level1.sets_level[x1].blocks_level[b1].data[j] = data_values_in_block[j];
            }
            if(sim_level1.sets_level[x1].blocks_level[b1].data[x6]!=value_of_registers[registers_in_present_instruction[0]]){
            sim_level1.sets_level[x1].blocks_level[b1].dirty=1;
            }
            sim_level1.sets_level[x1].blocks_level[b1].data[x6]=value_of_registers[registers_in_present_instruction[0]];
            lru1++;
            sim_level1.sets_level[x1].blocks_level[b1].tag = address.substr(0,13-(int)log2(block_size1)-(int)log2(x2));
            sim_level1.sets_level[x1].blocks_level[b1].lru = lru1; 
           // sim_level1.sets_level[x1].blocks_level[b1].dirty=0;
        }
        clockcycle = clockcycle+(level1_cache_latency+level2_cache_latency)-1;
        } 





     else{
         cout<<"----------------------------------------------------------------------------------"<<endl;
        cout<<"found in level 1 cache itself "<<endl;
        cout<<"--------------------------------------------------------------------"<<endl;
        hit++;
         int x5 = sim_level1.sets_level[x1].req_block_no_in_set_in_level; // as it is hit in level2 increase its lru
        cout<<x5<<"nnnnnnnnnnnneeeeeeeeeeeeeeeddddddddddddddddddddddddddddddddddd"<<endl;
        sim_level1.sets_level[x1].blocks_level[x5].data[x6] = value_of_registers[registers_in_present_instruction[0]];
        lru1++;
        sim_level1.sets_level[x1].blocks_level[x5].lru = lru1;
         sim_level1.sets_level[x1].blocks_level[x5].dirty=1;
         clockcycle = clockcycle + (level1_cache_latency-1);
     }   
       cout<<m<<endl;
       re = m;
    }
    else if(n==12){
       re = m;
    }
    else if(n==13 || n==14){
        re = m;
    }
    else{
        re = m;
    }         //////////change change
    return re;
}

void simulator::instruction_writeback(int m,int n){   // write back stage
// generally we update the values of destination register
        if(n==12){ 
            memory_element_values[m]=value_of_registers[registers_in_present_instruction[0]];
        }
        else if(n==13 || n==14 || n==15){

        }
        else
        value_of_registers[registers_in_present_instruction[0]]=m;
}

int simulator::instruction_execute(int n){  // execute function
      int execute_stage_value=-111;
        if(data_forwarding_enabled==false){
        int q2 = is_there_datahazard(pc-main_found-1-number_of_labels_found); // check whether there are hazards
        if(q2!=0){ // if there is hazard
              instructions_with_stalls.push_back(program[pc-1]);
          }
          clockcycle=clockcycle+q2;
          total_stalls=total_stalls+q2;
        }
        else{
          int q1 = is_there_datahazard(pc-main_found-1-number_of_labels_found);
          if(q1!=0){
              instructions_with_stalls.push_back(program[pc-1]);
          }
          clockcycle=clockcycle+q1;
          total_stalls=total_stalls+q1;
        }

     switch(n){
        case 0: // add
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 1:  //sub
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]-value_of_registers[registers_in_present_instruction[2]]; 
        
        break;
        case 2: //mul
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]*value_of_registers[registers_in_present_instruction[2]]; 
       break;
        case 3: // div
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]&value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 4:  //or
         execute_stage_value=value_of_registers[registers_in_present_instruction[1]]|value_of_registers[registers_in_present_instruction[2]]; 
        break;
        case 5:

        break;  /////////////////////////////////////////////////////////////////////////////////////////////
        case 6: //slt
            value_of_registers[registers_in_present_instruction[0]]=value_of_registers[registers_in_present_instruction[1]]<value_of_registers[registers_in_present_instruction[2]];
        break; 
        case 7:  //addi
           execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]; 
            break;
        case 8: //subi
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]-registers_in_present_instruction[2]; 
            break;
        break;
        case 9:  // ori
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]|registers_in_present_instruction[2]; 
            break;
        break;
        case 10:

        break;
        case 11: //lw
            execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4; 
        break;
        case 12:  //sw
           execute_stage_value=value_of_registers[registers_in_present_instruction[1]]+registers_in_present_instruction[2]/4;
        break;
        case 13: //beq
            if(value_of_registers[registers_in_present_instruction[0]]==value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2];
            execute_stage_value = 100;
            }
            
            else{
               execute_stage_value = 100;
            }
        break;
        case 14: //bne
            if(value_of_registers[registers_in_present_instruction[0]]!=value_of_registers[registers_in_present_instruction[1]]){
                pc=registers_in_present_instruction[2]+1;
            }
            else{
            }
        break;
        case 15: //j
            pc = registers_in_present_instruction[0];
            execute_stage_value = 100;
        break;
        case 16: //la
            execute_stage_value = registers_in_present_instruction[1];
        break;
        case 17:

        break;
    }
    return execute_stage_value;
}

void simulator::valid_register(string s,int n){ // valid register method
    vector <string> tokens;
    stringstream commas(s);
    string intermediate;
    while(getline(commas,intermediate,',')){ // dividing the string by commas
        if(intermediate[0]=='$')
        tokens.push_back(intermediate.substr(1));
        else
        tokens.push_back(intermediate);
    }
    int x=tokens.size();
    int y=0;
    if(n==6 && x==3){  //add sub type
            for(int i=0;i<x;i++){
                for(int j=0;j<32;j++){
                    if(tokens[i]==registers[j]){
                        registers_in_present_instruction[i]=j; //we store the register here
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
        registers_in_present_instruction[2]=stoi(tokens[2]);
        y++;
       
        if(y!=3){
            cout<<"all registers given are not valid";
        }
    }
    
    else if(n==12 && x==2){
        type_of_offset lwsw;
         string s1=tokens[1];
        // cout<<s1<<"                 "<<"------------"<<endl;
        int offset=-1;
        int x=s1.find("(");
        char c=s1[0];
        int z=s1.find(")");
        string s2="";
        // 2 types we can have register or integer
        if(c=='0' || c=='1' || c=='2' || c=='3' || c=='4'|| c=='5' || c=='6' || c=='7' || c=='8' || c=='9'){ // if integer
           offset=stoi(s1.substr(0,x));
           lwsw.position=pc-1;
           lwsw.type=1;
           lwsw_offset_info.push_back(lwsw);
          s2=s1.substr(x+2,z-x-2);
         
        }
        else{ //register type
           s2=s1.substr(0,2);
          //  cout<<s2<<"ssssssss22222222"<<endl;
            lwsw.position=pc-1;
            lwsw.type=0;
            lwsw_offset_info.push_back(lwsw);
            for(int i=0;i<32;i++){
                if(s2==registers[i]){
                    offset=value_of_registers[i];
                  // cout<<offset<<"??????????????????????"<<endl;
                    break;
                }
            }
        } 
        string s3 = s1.substr(x+2,z-x-2);
        for(int i=0;i<32;i++){
            if(registers[i]==tokens[0]){
                registers_in_present_instruction[0]=i;
                y++;
            }
        }
        for(int i=0;i<32;i++){
            if(registers[i]==s3){
                registers_in_present_instruction[1]=i;
                y++;
            }
        }
        if(offset==-1){
            cout<<"wrong offset value";
        }
        registers_in_present_instruction[2]=offset;
        if(y!=2){
        cout<<"wrong"<<endl;
        }
   
    }
    else if(n==14 && x==3){
       // cout<<"bbbbbbbbbbbbeeeeeeeeeeeeqqqqqqqqqqqqqqbbbbbbbbbbbbbnnnnnnnnnnnnneeeeeeeeeeeeeeee"<<endl;
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
        cout<<"--------------------------------------------------"<<endl;
        for(int i=0;i<3;i++){
            cout<<registers_in_present_instruction[i]<<endl;
        }
        cout<<"--------------------------------------------------"<<endl;
        if(found==0){
            cout<<"wrong label name written";
        }

    }
    else if(n==15){
        int found=0;
        for(int i=0;i<labels_in_program.size();i++){
            if(s==labels_in_program[i].label){
                registers_in_present_instruction[0]=labels_in_program[i].line_number_of_label;
                found=1;
            }
        }
        cout<<"vvvvvvvaaaaaaallllliiiiiiiiiddddddiiiiiitttttttttttyyyyyyyyyyy"<<endl;
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

void simulator::display(){ // display function
     for(int i=0;i<32;i++){
         cout<<registers[i]<<"  "<<value_of_registers[i]<<endl;
     }
     cout<<"stalls in program"<<total_stalls<<endl;
     cout<<"clockcycles taken"<<clockcycle+3<<endl;
    cout<<data_forwarding_enabled<<endl;
    cout<<"......................................."<<endl;
    cout<<"========          "<<"instructions with satlls           "<<"=========="<<endl;
    for(int b=0;b<instructions_with_stalls.size();b++){
        cout<<instructions_with_stalls[b]<<endl;
    }
    cout<<"==========        "<<"hit rate              "<<"============="<<endl;
    float hitrate = (float)(hit)/(float)(lwsw_ins);
    cout<<hitrate<<endl;
    cout<<"==========        "<<"instructions per cycle           "<<"==========="<<endl;
    cout<<number_of_instructions_executed<<endl;
    cout<<clockcycle<<endl;
    cout<<(float)(number_of_instructions_executed)/clockcycle;
}
// we have 4kb memory so 4096 so 12 bits requireed to store addresses
int main(){
    ifstream file;
    int need;
    cout<<"need data forwarding 1/0";
    int cache_size = 8;
    int block_size = 2;
    int assosciativity = 2;
    int cache_size_2 = 16;
    int block_size_2 = 2;
    int assosciativity_2 = 2;
    Cache level1 = design_level_cache(cache_size,block_size,assosciativity);
    Cache level2 = design_level_cache(cache_size_2,block_size_2,assosciativity_2);
    int cache_latency1;
    int cache_latency2;
    int main_memory_latency;
//////////////////////////////////////////////////////////////////////////////////    
    cin>>need;
     simulator s(file,need,level1,level2,block_size,block_size_2,cache_latency1,cache_latency2,main_memory_latency);
     s.run();
     s.display();
     cout<<need<<endl;
     display_cache_level(level1);
     cout<<"^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"<<endl;
     display_cache_level(level2);
    return 0;
}