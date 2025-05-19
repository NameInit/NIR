#pragma once

#include "algs_compression.hpp"

#include <functional>

class MyAlgs : public AlgsCompression{
	private:
		unsigned __min_code_letter_ascii;
		unsigned __count_bit_in_len_code;
		std::map<unsigned, unsigned> __symbol_shift;
		std::map<unsigned, std::string> __symbol_code;
		std::map<std::string, unsigned> __code_symbol;


		std::string __InBString(unsigned num, unsigned len){
			std::string ans = "";
				
			if (num == 0) {
				ans = "0";
			}
			else {
				while (num > 0) {
					ans = static_cast<char>(num % 2 + '0') + ans;
					num /= 2;
				}
			}

			if (ans.size() < len) {
				ans = std::string(len - ans.size(), '0') + ans;
			}
			return ans;
		}
		unsigned __InNum(const std::string& str){
			unsigned ans=0;
			for(char letter : str){
				ans*=2;
				ans+=(letter=='1');
			}
			return ans;
		}
	public:
		MyAlgs(const std::string& filename) {
			__StartTime(__time.init);
			__SetFileName(__filename.base, filename);
			__IncreaseUsageMemory(4*sizeof(unsigned)); //min, max, diff, count

			#ifdef DEBUG
			std::cout << "-----------------START CONSTRUCTOR NUFFMAN-----------------" << std::endl;
			#endif
			
			DataFile file(filename, std::ios::in);
			__min_code_letter_ascii=256;
			unsigned max_code_letter = 0;

			std::function<unsigned(unsigned)> count_bit=[](unsigned num){
				unsigned ans=1;
				while(num>1){
					num/=2;
					++ans;
				}
				return ans;
			};

			#ifdef DEBUG
			std::cout << "MAP SYMBOL-SHIFT: " << std::endl;
			#endif
			while(file.get_next_symbol()!=-1){
				if(__min_code_letter_ascii>file.get_cur_symbol())
					__min_code_letter_ascii=file.get_cur_symbol();
				if(max_code_letter<file.get_cur_symbol())
					max_code_letter=file.get_cur_symbol();
				if(__symbol_shift.find(file.get_cur_symbol())==__symbol_shift.end()){
					__symbol_shift[file.get_cur_symbol()]=__symbol_shift.size();
					#ifdef DEBUG
					std::cout << file.get_cur_symbol() << ' ' << __symbol_shift[file.get_cur_symbol()] << std::endl;
					#endif
				}
			}

			__count_bit_in_len_code = count_bit(__symbol_shift.size()-1);

			#ifdef DEBUG
			std::cout << "MAP SYMBOL-CODE:" << std::endl;
			#endif
			for(auto it : __symbol_shift){
				std::string code = __InBString(it.second, __count_bit_in_len_code);
				__symbol_code[it.first] = code;
				
				#ifdef DEBUG
				std::cout << it.first << ' ' << __symbol_code[it.first] << std::endl; 
				#endif

				__code_symbol[code] = it.first;
			}


			#ifdef DEBUG
			std::cout << "COUNT BIT IN LEN CODE: " << __count_bit_in_len_code << std::endl;
			#endif

			#ifdef DEBUG
			std::cout << "------------------END CONSTRUCTOR NUFFMAN------------------" << std::endl;
			#endif

			__EndTime(__time.init);
		}

		int encode(const std::string& filename_in, const std::string& filename_out){
			__StartTime(__time.encode);
			__SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);

			#ifdef DEBUG
			std::cout << "---------------START ENCODE  MYALGS---------------" << std::endl;
			#endif
			
			DataFile file_in(filename_in, std::ios::in);
			DataFile file_out(filename_out, std::ios::out | std::ios::binary);
			std::string buffer="";

			while(file_in.get_next_symbol()!=-1){
				#ifdef DEBUG
				std::cout << "CUR_LETTER: " << file_in.get_cur_symbol() << std::endl;
				std::cout << "CODE: " << __symbol_code[file_in.get_cur_symbol()] << std::endl;
				#endif
				buffer+=__symbol_code[file_in.get_cur_symbol()];
				#ifdef DEBUG
				std::cout << "BEFORE BUFFER: " << buffer << std::endl;
				#endif
				if(buffer.size()>=8){
					file_out.write(__InNum(buffer.substr(0,8)));
					#ifdef DEBUG
					std::cout << "ADD IN FILE: " << buffer.substr(0,8) << " == " << __InNum(buffer.substr(0,8)) << std::endl;
					#endif
					buffer=buffer.substr(8,buffer.size());
				}
				#ifdef DEBUG
				std::cout << "AFTER BUFFER: " << buffer << std::endl;
				#ifdef DEBUG_STEP
				__SleepTime(__time.encode);
				std::cout << "Press enter for to proceed to the next step" << std::endl;
				getchar();
				__UnSleepTime(__time.encode);
				#endif
				std::cout << std::endl;
				#endif
			}
			// std::cout << "UPAL: " << buffer << std::endl;
			// std::cout << "LEN: " << buffer.size() << std::endl;
			// std::cout << "BUFFER: " << 8-buffer.size() << std::endl; 

			file_out.write(buffer.size());
			file_out.write(__InNum(std::string(8-buffer.size(), '0')+buffer));
			#ifdef DEBUG
			std::cout << "LAST_PART_BUFFER: " << buffer << std::endl;
			std::cout << "SIZE_BUFFER: " << buffer.size() << std::endl;
			std::cout << "APPEND: " << std::string(8-buffer.size(), '0')+buffer<<" == "<<__InNum(std::string(8-buffer.size(), '0')+buffer)<<std::endl;
			#endif

			#ifdef DEBUG
			std::cout << "----------------END ENCODE  MYALGS----------------" << std::endl;
			#endif

			__EndTime(__time.encode);
			return 0;
		}

		int decode(const std::string& filename_in, const std::string& filename_out){
			__StartTime(__time.decode);
			__SetFileName(__filename.binary,filename_in);
			__SetFileName(__filename.unzipped,filename_out);
			__CalcMemoryMap(__symbol_shift);
			__CalcMemoryMap(__symbol_code);
			__CalcMemoryMap(__code_symbol);
			__IncreaseUsageMemory(3*sizeof(unsigned)+(__count_bit_in_len_code*2-1)*sizeof(char));

			#ifdef DEBUG
			std::cout << "---------------START DECODE  MYALGS---------------" << std::endl;
			#endif
			
			DataFile file_in(filename_in, std::ios::in | std::ios::binary);
			DataFile file_out(filename_out, std::ios::out);
			std::string buffer="";
			unsigned len_last_byte;
			unsigned size_file_in = file_in.size(); 

			for(int i=0; i<size_file_in-2; i++){
				unsigned num = file_in.get_next_symbol();
				buffer+=__InBString(num, 8);
				while(buffer.size()>=__count_bit_in_len_code){
					file_out.write(__code_symbol[buffer.substr(0,__count_bit_in_len_code)]);
					buffer=buffer.substr(__count_bit_in_len_code,buffer.size());
				}
			}

			len_last_byte=file_in.get_next_symbol();
			buffer+=__InBString(file_in.get_next_symbol(), len_last_byte);

			while(buffer.size()>=__count_bit_in_len_code){
				file_out.write(__code_symbol[buffer.substr(0,__count_bit_in_len_code)]);
				buffer=buffer.substr(__count_bit_in_len_code,buffer.size());
			}

			#ifdef DEBUG
			std::cout << "----------------END DECODE  MYALGS----------------" << std::endl;
			#endif

			__EndTime(__time.decode);
			return 0;
		}
};