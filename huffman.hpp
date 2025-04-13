#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <set>

#include "timer.hpp"
#include "data_io.hpp"

#define SHOW_MAP(mymap) std::set<char> uv{'\n','\r'}; for(auto it=mymap.begin(); it!=mymap.end(); it++) {std::cout << '('; if(uv.find(it->first)!=uv.end()) std::cout<<'\''<<(unsigned)it->first<<'\''; else  std::cout<<it->first;std::cout<< ", " << it->second << ") ";} std::cout << std::endl;
#define SHOW_VECTOR_NODES(myvector) for(auto it=myvector.begin(); it!=myvector.end(); it++) std::cout<<**it<<' ';std::cout<<std::endl;


class Huffman{
    private:
        typedef struct Node{
            std::string str;
            unsigned frequency;
            Node* left;
            Node* right;
            
            Node(const std::string& string, const unsigned count) : str(string), frequency(count), left(nullptr), right(nullptr) {}
            
            int operator<(const Node& other) { return frequency<other.frequency; }
            
            friend std::ostream& operator<<(std::ostream& out, const Node& other){
                out<<'(';
                std::set<char> uv{'\n','\r'};
                for(char symbol : other.str){
                    if(uv.find(symbol)!=uv.end())
                        out<<'\''<<(unsigned)symbol << '\'';
                    else
                        out << symbol;
                }
                out<<", "<<other.frequency<<')';
                return out;
            }
        }Node;

        typedef struct Times{
            Timer encode;
            Timer decode;
            Timer init;
        }Times;

        typedef struct Filename{
            std::string base;
            std::string binary;
            std::string unzipped;
        }Filename;

        Node *__root;
        std::map<char, std::string> __symbol_code;
        std::map<char, unsigned> __symbol_frequency;
        Times __time;
        Filename __filename;

        void __StartTime(Timer& timer) { timer.set_start(); }
        void __EndTime(Timer& timer) { timer.set_end(); }
        void __SetFileName(std::string& old_filename, const std::string& new_filename) { old_filename=new_filename; }

        void __DestroyTree(Node* node){
            if(node!=nullptr){
                __DestroyTree(node->left);
                __DestroyTree(node->right);
                delete node;
            }
        }

        void __BuildTree(std::map<char, unsigned>& dict){
            std::vector<Node*> array;
            __root=nullptr;
            for(auto it=dict.begin(); it!=dict.end(); it++){
                char buff[1]={it->first};
                std::string string(buff);
                Node* temp_node=new Node(string,it->second);
                array.push_back(temp_node);
            }
            if(array.size()==0)
                __root=nullptr;
            else if(array.size()==1){
                __root=array.back();
                array.pop_back();
            }
            else{
                Node *min1, *min2, *temp_node;
                unsigned imin1, imin2, freq_min1, freq_min2;
                while(array.size()>1){
                    imin1=(*array[0]<*array[1]) ? 0 : 1;
                    imin2=1-imin1;
                    for(int i=2; i<array.size(); i++){
                        if(*array[i]<*array[imin1]){
                            imin2=imin1;
                            imin1=i;
                        }
                        else if(*array[i]<*array[imin2])
                            imin2=i;
                    }
                    min1=array[imin1];
                    min2=array[imin2];
                    array.erase(array.begin()+imin1);
                    array.erase(array.begin()+imin2-(imin1<imin2));
                    temp_node=new Node(min1->str+min2->str,min1->frequency+min2->frequency);
                    temp_node->left=min1;
                    temp_node->right=min2;
                    array.push_back(temp_node);
                }
                __root=array[0];
                array.pop_back();
            }
        }

        void __CreateMapSymbolCode(Node *node, std::string code){
            if(__root==nullptr)
                return ;
            if(__root!=nullptr && __root->left==nullptr && __root->right==nullptr){
                __symbol_code[__root->str[0]]="0";
                return;
            }


            if((node->left==nullptr)&&(node->right==nullptr)){
                __symbol_code[node->str[0]]=code;
                return ;
            }
            __CreateMapSymbolCode(node->left, code+"0");
            __CreateMapSymbolCode(node->right, code+"1");
        }

        void __CreateMapSymbolFrequency(const std::string& filename){
            DataFile file(filename, std::ios::in);
            while((file.get_next_symbol())!=-1){
                __symbol_frequency[file.get_cur_symbol()]+=1;
            }
        }
    public:
        Huffman() : __root(nullptr) {}
        Huffman(const std::string& filename) {
            __StartTime(__time.init);
            __SetFileName(__filename.base, filename);
            __CreateMapSymbolFrequency(filename);
            __BuildTree(__symbol_frequency);
            __CreateMapSymbolCode(__root, "");
            __EndTime(__time.init);
        }

        ~Huffman(){ __DestroyTree(__root); __symbol_code.clear(); __symbol_frequency.clear(); }

        void init(const std::string& filename){
            __StartTime(__time.init);
            __SetFileName(__filename.base, filename);
            if(__root!=nullptr){
                __DestroyTree(__root); 
                __symbol_code.clear(); 
                __symbol_frequency.clear();
            }
            __CreateMapSymbolFrequency(filename);
            __BuildTree(__symbol_frequency);
            __CreateMapSymbolCode(__root, 0);
			__EndTime(__time.init);
        }

        void show_tree(){
            if(__root==nullptr)
                return ;
            std::vector<Node*> cur_nodes;
            std::vector<Node*> next_nodes;
            cur_nodes.push_back(__root);
            while(cur_nodes.size()>0){
                SHOW_VECTOR_NODES(cur_nodes);
                for(int i=0; i<cur_nodes.size(); i++)
                    if(cur_nodes[i]->left!=nullptr){
                        next_nodes.push_back(cur_nodes[i]->left);
                        next_nodes.push_back(cur_nodes[i]->right);
                    }
                cur_nodes=next_nodes;
                next_nodes.clear();
            }
            return ;
        }

        int encode(const std::string& filename_in, const std::string& filename_out){            
            __StartTime(__time.encode);
			__SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);
			DataFile file_in(filename_in, std::ios::in);
            DataFile file_out(filename_out, std::ios::out | std::ios::binary);

            auto strbin_to_int = [](const std::string& str){
                unsigned num=0; 
                for(char symbol : str)
                    num=num*2+(symbol=='1');
                return num;
            };
            std::string buffer;

            while(file_in.get_next_symbol()!=-1){
                buffer+=__symbol_code[file_in.get_cur_symbol()];
                if(buffer.size()>=8){
                    file_out.write(strbin_to_int(buffer.substr(0,8)));
                    buffer=buffer.substr(8,buffer.size());
                }
            }
            if(buffer.size()!=0){
                file_out.write(buffer.size());
                file_out.write(strbin_to_int(buffer));
            }
            else{
                file_out.write(0);
                file_out.write(0);
            }
			__EndTime(__time.encode);
            return 0;
        }

        int decode(const std::string& filename_in, const std::string& filename_out){
			__StartTime(__time.decode);
			__SetFileName(__filename.binary, filename_in);
			__SetFileName(__filename.unzipped, filename_out);
            DataFile file_in(filename_in, std::ios::in | std::ios::binary);
            DataFile file_out(filename_out, std::ios::out);

            auto int_to_strbin = [](unsigned num, unsigned count_bit){
                std::string strbin;
                while(num>0){
                    strbin=(char)(num%2+'0')+strbin;
                    num/=2;
                }
                while(strbin.size()<count_bit){
                    strbin=(char)(num%2+'0')+strbin;
                }
                return strbin;
            };
            std::string data_file_in;
            std::string temp_data;
            std::map<std::string, unsigned> code_symbol;
            unsigned max_size_code=0;
            unsigned size_file=file_in.size();
            bool done=0;

            for(auto it=__symbol_code.begin(); it!=__symbol_code.end(); it++){
                code_symbol[it->second]=it->first;
                max_size_code=(max_size_code<(it->second).size()) ? (it->second).size() : max_size_code;
            }
            
            for(int i=0; i<size_file-2; i++)
                data_file_in+=int_to_strbin(file_in.get_next_symbol(),8);

            if(file_in.get_next_symbol()!=0){
                unsigned count_bit=file_in.get_cur_symbol();
                data_file_in+=int_to_strbin(file_in.get_next_symbol(),count_bit);
            }

            for(char bit : data_file_in){
                temp_data+=bit;
                if(code_symbol.find(temp_data)!=code_symbol.end()){
                    file_out.write(code_symbol[temp_data]);
                    temp_data.clear();
                }
                else if(temp_data.size()>max_size_code){
                    done=1;
                    break;
                }
            }
			__EndTime(__time.decode);
            return done;
        }

        void show_map_code(){
            SHOW_MAP(__symbol_code);
        }

        void show_map_frequency(){
            SHOW_MAP(__symbol_frequency);
        }
        
		void show_statistic(){
			auto get_percent_diff_byte_in_files = [](DataFile& file1, DataFile& file2){
				unsigned count=0;
				unsigned offset=0;
				while((file1.get_next_symbol()!=-1)&&(file2.get_next_symbol()!=-1)){
					if(file1.get_cur_symbol()!=file2.get_cur_symbol()) ++count;
					++offset;
				}
				if((file1.get_next_symbol()!=-1)||(file2.get_next_symbol()!=-1)){
					count+=(file1.size()>file2.size() ? file1.size() : file2.size())-offset;
				}
				return ((double)(file1.size()>file2.size() ? file1.size() : file2.size() - count))/((double)(file1.size()>file2.size() ? file1.size() : file2.size()))*100.;
			};

			DataFile file_base(__filename.base, std::ios::in);
			DataFile file_binary(__filename.binary, std::ios::in);
			DataFile file_unzipped(__filename.unzipped, std::ios::in);

			std::cout << "-----------------STATISTIC HUFFMAN-----------------" << std::endl;
			std::cout << "START FILE: " << __filename.base << '(' << file_base.size() << " bytes" <<')' << std::endl;
			std::cout << "ARCHIVED FILE: " << __filename.binary << '(' << file_binary.size() << " bytes" <<')' << std::endl;
			std::cout << "UNZIPPED FILE: " << __filename.unzipped << '(' << file_unzipped.size() << " bytes" <<')' << std::endl << std::endl;

			std::cout << "TIME FOR ARCHIVING: " << __time.decode.duration_s() +  __time.init.duration_s() << "s (" << __time.init.duration_s() << "s spent for creating dict)" << std::endl;
			std::cout << "TIME FOR UNZIPPED: " << __time.decode.duration_s() << 's' << std::endl << std::endl;

			std::cout << "COMPRESSION RATIO: " << (double)file_base.size()/(double)file_binary.size() << std::endl;
			std::cout << "INTEGRITY: " << get_percent_diff_byte_in_files(file_base,file_unzipped) << '%' << std::endl;
			std::cout << "------------------------END------------------------" << std::endl;
		}
};