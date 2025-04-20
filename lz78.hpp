#pragma once


#include "algs_compression.hpp"
#include "data_io.hpp"
#include "list.hpp"
#include <map>

class LZ78 : public AlgsCompression{
    private:
        std::map<unsigned, std::string> __key_substring;
        std::map<std::string, unsigned> __substring_key;

		unsigned __GetLenNumIn256sc(unsigned number){
			if(number==0)
					return static_cast<unsigned>(1);
				unsigned ans=0;
				while(number>0){
					ans++;
					number/=256;
				}
				return ans;
		}
    public:
        int encode(const std::string& filename_in, const std::string& filename_out) {
            __StartTime(__time.encode);
            __SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);

			#ifdef DEBUG
			std::cout << "-------------------START  ENCODE-------------------" << std::endl;
			#endif

            DataFile file_in(filename_in, std::ios::in);
            DataFile file_out(filename_out, std::ios::out | std::ios::binary);
			std::string substring;
			unsigned global_key=1;
			unsigned count_iterations=0;
			List<unsigned> temp_key_system256;

			#ifdef DEBUG
			unsigned num_step=1;
			#endif

            while(file_in.get_next_symbol()!=-1){
				unsigned temp_key=0;
				substring=static_cast<char>(file_in.get_cur_symbol());
				while((__substring_key.find(substring)!=__substring_key.end())&&(file_in.get_cur_symbol()!=-1)){
					temp_key=__substring_key[substring];
					if(file_in.get_next_symbol()!=-1) substring+=file_in.get_cur_symbol();
				}

				#ifdef DEBUG
				std::cout << "NUM STEP" << num_step << ':' << std::endl;
				std::cout << "NEW SUBSTRING IN MAP: {" << global_key << ": " << substring << '}' << std::endl;
				std::cout << "DISCOVERED SUBSTRING: {" << temp_key << ": " << __key_substring[temp_key] << std::endl;
				std::cout << "CODE CHAR IN FILE_OUT: " << file_in.get_cur_symbol() << std::endl;
				std::cout << "TEMP KEY: " << temp_key << " -> ";
				#endif

				while (temp_key>0){
					temp_key_system256.insert(temp_key%256, 0);
					temp_key/=256;
				}
				
				#ifdef DEBUG
				std::cout << temp_key_system256 << std::endl;
				#ifdef DEBUG_STEP
				std::cout << "Press enter for to proceed to the next step" << std::endl;
				getchar();
				#endif
				#endif
				
				for(int i=0; i<(__GetLenNumIn256sc(global_key)-temp_key_system256.size()); i++)
					file_out.write(0);
				
				for(auto it : temp_key_system256)
					file_out.write(it);

				if(file_in.get_cur_symbol()!=-1){
					__substring_key[substring]=global_key;
					__key_substring[global_key++]=substring;
					file_out.write(file_in.get_cur_symbol());
				}

				temp_key_system256.clear();
			}
            __EndTime(__time.encode);
            return 0;
        }
        int decode(const std::string& filename_in, const std::string& filename_out) {
            __StartTime(__time.decode);
			__SetFileName(__filename.binary, filename_in);
			__SetFileName(__filename.unzipped, filename_out);

			DataFile file_in(filename_in, std::ios::in | std::ios::binary);
			DataFile file_out(filename_out, std::ios::out);

			unsigned count_iterations=1;

			while(file_in.get_next_symbol()!=-1){
				// std::cout << "COUNT ITER: " << count_iterations << "\tCOUNT BYTES: " << __GetLenNumIn256sc(count_iterations) << "\t|\t";
				unsigned temp_key=file_in.get_cur_symbol();
				// std::cout << "I GET IN FILE_IN: " << temp_key << ' ';
				for(int i=0; i<__GetLenNumIn256sc(count_iterations)-1; i++) {temp_key=temp_key*256+file_in.get_next_symbol(); 
					// std::cout << file_in.get_cur_symbol() << ' ';
				}
				file_out.write(__key_substring[temp_key]);
				if(file_in.get_next_symbol()!=-1) file_out.write(file_in.get_cur_symbol());
				// std::cout << static_cast<char>(file_in.get_cur_symbol()) << "(char)" << std::endl;
				count_iterations++;
			}

			__EndTime(__time.decode);
			return 0;
        }
};