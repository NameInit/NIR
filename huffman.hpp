#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <functional>

#include "data_io.hpp"
#include "algs_compression.hpp"


class Huffman : public AlgsCompression{
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

		Node *__root;
		std::map<char, std::string> __symbol_code;
		std::map<std::string, char> __code_symbol;
		std::map<char, unsigned> __symbol_frequency;

		unsigned __SizeTree(Node* root) {
			unsigned result = 0;

			std::function<void(Node*)> traversing_tree = [&](Node* node) {
				if (node == nullptr) {
					return;
				}

				result += 1;

				traversing_tree(node->left);
				traversing_tree(node->right);
			};

			traversing_tree(root);
			return result;
		}

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
	
		void __CreateMapCodeSymbol(){
			for(auto it : __symbol_code)
				__code_symbol[it.second]=it.first;
		}
	public:
		Huffman() : __root(nullptr) {}
		Huffman(const std::string& filename) {
			__StartTime(__time.init);
			__SetFileName(__filename.base, filename);
			
			#ifdef DEBUG
			std::cout << "-------------------START CONSTRUCTOR NUFFMAN-------------------" << std::endl;
			#endif
			
			__CreateMapSymbolFrequency(filename);
			__BuildTree(__symbol_frequency);
			__CreateMapSymbolCode(__root, "");
			__CreateMapCodeSymbol();
			__CalcMemoryMap(__symbol_code);
			__CalcMemoryMap(__code_symbol);
			__CalcMemoryMap(__symbol_frequency);
			__IncreaseUsageMemory(sizeof(Node*)*__SizeTree(__root)); //BTree

			#ifdef DEBUG
			std::cout << "MAP {CODE SYMBOL: FREQUENCY}:" << std::endl;
			for(auto it : __symbol_frequency)
			std::cout << '{' << static_cast<unsigned>(it.first) << ": " << it.second << '}' << std::endl; 
			
			std::cout << std::endl << "MAP {CODE SYMBOL: CODE BINARY}:" << std::endl;
			for(auto it : __symbol_code)
			std::cout << '{' << static_cast<unsigned>(it.first) << ": " << it.second << '}' << std::endl;
			
			std::cout << "------------------END CONSTRUCTOR NUFFMAN------------------" << std::endl;
			#endif

			__EndTime(__time.init);
		}

		~Huffman(){ __DestroyTree(__root); }

		void init(const std::string& filename){
			__StartTime(__time.init);
			__SetFileName(__filename.base, filename);

			#ifdef DEBUG
			std::cout << "--------------------START INIT  NUFFMAN--------------------" << std::endl;
			#endif

			if(__root!=nullptr){
				__DestroyTree(__root); 
				__symbol_code.clear(); 
				__symbol_frequency.clear();
			}
			__CreateMapSymbolFrequency(filename);
			__BuildTree(__symbol_frequency);
			__CreateMapSymbolCode(__root, 0);
			__CalcMemoryMap(__symbol_code);
			__CalcMemoryMap(__code_symbol);
			__CalcMemoryMap(__symbol_frequency);
			__IncreaseUsageMemory(sizeof(Node*)*__SizeTree(__root)); //BTree

			#ifdef DEBUG
			std::cout << "MAP {CODE SYMBOL: FREQUENCY}:" << std::endl;
			for(auto it : __symbol_frequency)
			std::cout << '{' << static_cast<unsigned>(it.first) << ": " << it.second << '}' << std::endl; 
			
			std::cout << "MAP {CODE SYMBOL: CODE BINARY}:" << std::endl << std::endl;
			for(auto it : __symbol_code)
			std::cout << '{' << static_cast<unsigned>(it.first) << ": " << it.second << '}' << std::endl << std::endl;
			
			std::cout << "---------------------END INIT  NUFFMAN---------------------" << std::endl;
			#endif

			__EndTime(__time.init);
		}

		void show_tree(){
			if(__root==nullptr)
				return ;
			std::vector<Node*> cur_nodes;
			std::vector<Node*> next_nodes;
			cur_nodes.push_back(__root);
			while(cur_nodes.size()>0){
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
			__IncreaseUsageMemory(sizeof(std::string)+8); //std::string buffer
			__IncreaseUsageMemory(sizeof(std::function<unsigned int(const std::string&)>)); //std::function<unsigned int(const std::string&)> strbin_to_int

			#ifdef DEBUG
			std::cout << "---------------START ENCODE  HUFFMAN---------------" << std::endl;
			#endif	

			DataFile file_in(filename_in, std::ios::in);
			DataFile file_out(filename_out, std::ios::out | std::ios::binary);

			std::function<unsigned int(const std::string&)> strbin_to_int = [](const std::string& str){
				unsigned num=0; 
				for(char symbol : str)
					num=num*2+(symbol=='1');
				return num;
			};
			std::string buffer;

			#ifdef DEBUG
			unsigned num_step=1;
			#endif

			while(file_in.get_next_symbol()!=-1){
				buffer+=__symbol_code[file_in.get_cur_symbol()];

				#ifdef DEBUG
				std::cout << "NUM STEP" << num_step++ << std::endl;
				std::cout << "CUR CODE CHAR: " << file_in.get_cur_symbol() << std::endl;
				std::cout << "BINARY CODE CHAR: " << __symbol_code[file_in.get_cur_symbol()] << std::endl;
				std::cout << "BUFFER: " << buffer << std::endl;
				#endif

				if(buffer.size()>=8){
					file_out.write(strbin_to_int(buffer.substr(0,8)));
					buffer=buffer.substr(8,buffer.size());
					#ifdef DEBUG
					std::cout << "BUFFER HAVE BEEN CROPPED, BUFFER: " << ((buffer.size()!=0) ? buffer : static_cast<std::string>("<void>")) << std::endl; 
					#endif
				}

				#ifdef DEBUG
				#ifdef DEBUG_STEP
				__SleepTime(__time.encode);
				std::cout << "Press enter for to proceed to the next step" << std::endl;
				getchar();
				__UnSleepTime(__time.encode);
				#endif
				#endif
			}
			if(buffer.size()!=0){
				file_out.write(buffer.size());
				file_out.write(strbin_to_int(buffer));
			}
			else{
				file_out.write(0);
				file_out.write(0);
			}

			#ifdef DEBUG
			std::cout << "----------------END ENCODE  HUFFMAN----------------" << std::endl;
			#endif

			__EndTime(__time.encode);
			return 0;
		}

		int decode(const std::string& filename_in, const std::string& filename_out){
			__StartTime(__time.decode);
			__SetFileName(__filename.binary, filename_in);
			__SetFileName(__filename.unzipped, filename_out);
			__IncreaseUsageMemory(sizeof(std::string)+16); //std::string data_file_in
			__IncreaseUsageMemory(sizeof(std::string)+8); //auto int_to_strbin = [](unsigned num, unsigned count_bit)

			#ifdef DEBUG
			std::cout << "---------------START DECODE  HUFFMAN---------------" << std::endl;
			#endif

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
			std::string temp_code;
			unsigned max_size_code=0;
			unsigned size_file=file_in.size();
			bool done=0;

			for(auto it=__symbol_code.begin(); it!=__symbol_code.end(); it++)
				max_size_code=(max_size_code<(it->second).size()) ? (it->second).size() : max_size_code;
			
			for(int i=0; i<size_file-2; i++)
				data_file_in+=int_to_strbin(file_in.get_next_symbol(),8);

			if(file_in.get_next_symbol()!=0){
				unsigned count_bit=file_in.get_cur_symbol();
				data_file_in+=int_to_strbin(file_in.get_next_symbol(),count_bit);
			}

			for(char bit : data_file_in){
				temp_code+=bit;
				
				#ifdef DEBUG
				std::cout << "NEW BIT: " << bit << std::endl;
				std::cout << "CURRENT BINARY CODE: " << temp_code << std::endl;
				#endif

				if(__code_symbol.find(temp_code)!=__code_symbol.end()){
					#ifdef DEBUG
					std::cout << "CODE "<< temp_code <<" HAVE BEEN FOUNDED" << std::endl;
					std::cout << temp_code << ": " << __code_symbol[temp_code] << "(char)" << std::endl;
					#endif
					file_out.write(__code_symbol[temp_code]);
					temp_code.clear();
				}
				else if(temp_code.size()>max_size_code){
					#ifdef DEBUG
					std::cout << "ERROR DECODE HUFFMAN!!!" << std::endl;
					#endif 
					done=1;
					break;
				}
				
				#ifdef DEBUG
				#ifdef DEBUG_STEP
				__SleepTime(__time.decode);
				std::cout << "Press enter for to proceed to the next step" << std::endl;
				getchar();
				__UnSleepTime(__time.decode);
				#endif
				#endif
			}

			#ifdef DEBUG
			std::cout << "--------------END DECODE  HUFFMAN--------------" << std::endl;
			#endif

			__EndTime(__time.decode);
			return done;
		}
};