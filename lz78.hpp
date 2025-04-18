#pragma once


#include "algs_compression.hpp"
#include "data_io.hpp"
#include "list.hpp"
#include <map>

#ifdef DEBUG
#define SHOW_MAP(mymap) std::set<char> uv{'\n','\r'}; for(auto it=mymap.begin(); it!=mymap.end(); it++) {std::cout << '('; if(uv.find(it->first)!=uv.end()) std::cout<<'\''<<(unsigned)it->first<<'\''; else  std::cout<<it->first;std::cout<< ", " << it->second << ") ";} std::cout << std::endl;
#endif

class LZ78 : public AlgsCompression{
    private:
        std::map<unsigned, std::string> __key_substring;
        std::map<std::string, unsigned> __substring_key;
    public:
        int encode(const std::string& filename_in, const std::string& filename_out) {
            __StartTime(__time.encode);
            __SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);

            DataFile file_in(filename_in, std::ios::in);
            DataFile file_out(filename_out, std::ios::out | std::ios::binary);
			std::string substring;
			unsigned key=1;

			#ifdef DEBUG
			List<unsigned> debug;
			#endif

            while(file_in.get_next_symbol()!=-1){
				unsigned temp_key=0;
				substring=static_cast<char>(file_in.get_cur_symbol());
				while((__substring_key.find(substring)!=__substring_key.end())&&(file_in.get_cur_symbol()!=-1)){
					temp_key=__substring_key[substring];
					if(file_in.get_next_symbol()!=-1) substring+=file_in.get_cur_symbol();
				}
				if(temp_key==0){
					__substring_key[substring]=key;
					__key_substring[key++]=substring;
					file_out.write(temp_key);
					#ifdef DEBUG
					debug.append(temp_key);
					#endif
				}
				else{
					if(file_in.get_cur_symbol()!=-1){
						__substring_key[substring]=key;
						__key_substring[key++]=substring;
					}
					while(temp_key>0){
						file_out.write(temp_key%256);
						#ifdef DEBUG
						debug.append(temp_key%256);
						#endif
						temp_key/=256;
					}
				}
				if(file_in.get_cur_symbol()!=-1) file_out.write(file_in.get_cur_symbol());
				#ifdef DEBUG
				std::cout << "SUBSTRING: " << substring << std::endl;
				std::cout << "KEY: " << debug << "\tLAST CHAR: " << (file_in.get_cur_symbol()!=-1 ? file_in.get_cur_symbol() : -1) << std::endl;
				SHOW_MAP(__key_substring);
				std::cout << std::endl;
				debug.clear();
				#ifdef DEBUG_STEP
				std::cout << "For next step write enter" <<  std::endl;
				getchar()
				#endif
				std::cout << std::endl;
				#endif
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

			__EndTime(__time.decode);
			return 0;
        }
};