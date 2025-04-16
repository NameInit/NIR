#pragma once

#include <set>

#include "algs_compression.hpp"
#include "data_io.hpp"
#include "list.hpp"

#define SHOW_LIST(list) {std::set<char> uv{'\n','\r'}; std::cout << '['; int i=0; for(auto it=list.begin(); it!=list.end(); ++it, ++i){if(uv.find(*it)!=uv.end()) std::cout<<"\'"<<(unsigned)*it<<"\'"; else std::cout<<*it; if((i+1)!=list.size()) std::cout << ", ";} std::cout<<']';}

class LZ77 : public AlgsCompression{
	private:
		unsigned __len_buffer_in;
		unsigned __len_buffer_out;
	public:
		LZ77() {} 
		LZ77(int len_buffer_in, int len_buffer_out) : __len_buffer_in(len_buffer_in), __len_buffer_out(len_buffer_out) {  }

		void init(int len_buffer_in, int len_buffer_out){
			__StartTime(__time.init);
			__len_buffer_in=len_buffer_in;
			__len_buffer_out=len_buffer_out;
			__EndTime(__time.init);
		}
		
        int encode(const std::string& filename_in, const std::string& filename_out) {
			__StartTime(__time.encode);
			__SetFileName(__filename.base,filename_in);
			__SetFileName(__filename.binary,filename_out);
			DataFile file_in(filename_in, std::ios::in);
			DataFile file_out(filename_out, std::ios::out | std::ios::binary);
			unsigned offset=0;
			unsigned len_subject_str=0;
			List<char> buff_in;
			List<char> buff_out;
			auto it_in=buff_in.begin();
			auto it_out=buff_out.begin();

			for(int i=0; (i<__len_buffer_in)&&(file_in.get_next_symbol()!=-1); i++){
				buff_in.append(file_in.get_cur_symbol());
			}

			while(buff_in.size()!=0){
				len_subject_str=0;
				offset=0;
				unsigned temp_offset=buff_out.size();
				for(it_out = buff_out.begin(); it_out!=buff_out.end(); ++it_out){
					auto temp_it_in = buff_in.begin();
					auto temp_it_out = it_out;
					unsigned temp_len_subject_str=0;
					while(*temp_it_out==*temp_it_in){
						++temp_len_subject_str;
						++temp_it_in;
						++temp_it_out;
						if((temp_it_in==buff_in.end())||(temp_it_out==buff_out.end()))
							break;
					}
					if(temp_len_subject_str>len_subject_str) {
						len_subject_str=temp_len_subject_str;
						offset=temp_offset;
					}
					--temp_offset;
				}
				// SHOW_LIST(buff_out);
				// SHOW_LIST(buff_in);
				// std::cout << std::endl << "LEN: " << len_subject_str << std::endl;
				// std::cout << "OFFSET: " << offset << std::endl;
				// std::cout << "LEN_OUT: " << buff_out.size() << std::endl << std::endl;
				file_out.write(offset);
				file_out.write(len_subject_str);
				// std::cout << '(' << offset << ", " << len_subject_str;
				for(int i=0; i<len_subject_str; i++){
					buff_out.append(buff_in.pop(0));
					if(file_in.get_next_symbol()!=-1) buff_in.append(file_in.get_cur_symbol());
				}
				if(buff_in.size()!=0){
					char temp_item=buff_in.pop(0);
					// std::cout << ", " << temp_item;
					file_out.write(temp_item);
					buff_out.append(temp_item);
					if(file_in.get_next_symbol()!=-1) buff_in.append(file_in.get_cur_symbol());
				}
				// std::cout << ')' << std::endl;
				while(buff_out.size()>__len_buffer_out) buff_out.pop(0);
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