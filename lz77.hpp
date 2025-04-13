#pragma once

#include "algs_compression.hpp"
#include "data_io.hpp"
#include "list.hpp"

class LZ77 : AlgsCompression{
	private:
		unsigned __len_buffer_in;
		unsigned __len_buffer_out;
	public:
		LZ77() {} 
		LZ77(int len_buffer_in, int len_buffer_out) : __len_buffer_in(len_buffer_in), __len_buffer_out(len_buffer_out) {  }

		void init(int len_buffer_in, int len_buffer_out){
			__len_buffer_in=len_buffer_in;
			__len_buffer_out=len_buffer_out;
		}
		
        int encode(const std::string& filename_in, const std::string& filename_out) {
			__StartTime(__time.encode);
			__SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);
			DataFile file_in(filename_in, std::ios::in);
			DataFile file_out(filename_out, std::ios::out | std::ios::binary);
			unsigned ptr_in=0;
			unsigned ptr_out=0;
			List<char> buff_in;
			List<char> buff_out;
			for(int i=0; (i<__len_buffer_in)&&(file_in.get_next_symbol()!=-1); i++){
				buff_in.append(file_in.get_cur_symbol());
			}
			std::cout << buff_in << std::endl;
			__EndTime(__time.encode);
			return 0;
		}

        int decode(const std::string& filename_in, const std::string& filename_out) {
			__StartTime(__time.decode);
			__SetFileName(__filename.binary, filename_in);
			__SetFileName(__filename.unzipped, filename_out);

			__EndTime(__time.decode);		
			return 0;
		}
};