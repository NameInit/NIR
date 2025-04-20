#pragma once

#include <set>

#include "algs_compression.hpp"
#include "data_io.hpp"
#include "list.hpp"

#ifdef DEBUG
#define SHOW_LIST(list) {std::set<char> uv{'\n','\r'}; std::cout << '['; int i=0; for(auto it=list.begin(); it!=list.end(); ++it, ++i){if(uv.find(*it)!=uv.end()) std::cout<<"\'"<<(unsigned)*it<<"\'"; else std::cout<<*it; if((i+1)!=list.size()) std::cout << ", ";} std::cout<<']';}
#endif

class LZ77 : public AlgsCompression{
	private:
		unsigned __len_buffer_in=255;
		unsigned __len_buffer_out=255;
		int __CheckParam(int len){
			return (1<=len)&&(len<=255);
		}
	public:
		LZ77() {} 
		LZ77(int len_buffer_in, int len_buffer_out) {
			__StartTime(__time.init);
			if(__CheckParam(len_buffer_in) && __CheckParam(len_buffer_out)){
				__len_buffer_in=len_buffer_in;
				__len_buffer_out=len_buffer_out;
			}
			#ifdef DEBUG
			std::cout << "-------------CONSTRUCTOR LZ77 START-------------" << std::endl;
			std::cout << "LEN_IN: " << __len_buffer_in << std::endl;
			std::cout << "LEN_OUT: " << __len_buffer_out << std::endl;
			std::cout << "--------------CONSTRUCTOR LZ77 END--------------" << std::endl;
			#endif
			__EndTime(__time.init);
		}

		void init(int len_buffer_in, int len_buffer_out){
			__StartTime(__time.init);
			if(__CheckParam(len_buffer_in) && __CheckParam(len_buffer_out)){
				__len_buffer_in=len_buffer_in;
				__len_buffer_out=len_buffer_out;
			}
			#ifdef DEBUG
			std::cout << "----------------INIT  LZ77 START----------------" << std::endl;
			std::cout << "LEN_IN: " << __len_buffer_in << std::endl;
			std::cout << "LEN_OUT: " << __len_buffer_out << std::endl;
			std::cout << "-----------------INIT  LZ77 END-----------------" << std::endl;
			#endif
			__EndTime(__time.init);
		}
		
        int encode(const std::string& filename_in, const std::string& filename_out) {
			#ifdef DEBUG
			std::cout << "-------------------START  ENCODE-------------------" << std::endl;
			#endif
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
				#ifdef DEBUG
				SHOW_LIST(buff_out);
				SHOW_LIST(buff_in);
				std::cout << std::endl << "LEN: " << len_subject_str << std::endl;
				std::cout << "OFFSET: " << offset << std::endl;
				std::cout << "LEN_OUT: " << buff_out.size() << std::endl;
				#endif
				file_out.write(offset);
				file_out.write(len_subject_str);
				#ifdef DEBUG
				std::cout << '(' << offset << ", " << len_subject_str;
				#endif
				for(int i=0; i<len_subject_str; i++){
					buff_out.append(buff_in.pop(0));
					if(file_in.get_next_symbol()!=-1) buff_in.append(file_in.get_cur_symbol());
				}
				if(buff_in.size()!=0){
					char temp_item=buff_in.pop(0);
					#ifdef DEBUG
					std::cout << ", " << temp_item;
					#endif
					file_out.write(temp_item);
					buff_out.append(temp_item);
					if(file_in.get_next_symbol()!=-1) buff_in.append(file_in.get_cur_symbol());
				}
				#ifdef DEBUG
				std::cout << ')' << std::endl;
				#ifdef DEBUG_STEP
				std::cout << "Press enter for to proceed to the next step" <<  std::endl;
				getchar();
				#endif
				std::cout << std::endl;
				#endif
				while(buff_out.size()>__len_buffer_out) buff_out.pop(0);
			}
			__EndTime(__time.encode);
			#ifdef DEBUG
			std::cout << "--------------------END  ENCODE--------------------" << std::endl;
			#endif
			return 0;
		}

        int decode(const std::string& filename_in, const std::string& filename_out) {
			#ifdef DEBUG
			std::cout << "-------------------START  DECODE-------------------" << std::endl;
			#endif
			__StartTime(__time.decode);
			__SetFileName(__filename.binary, filename_in);
			__SetFileName(__filename.unzipped, filename_out);
			DataFile file_in(filename_in, std::ios::in | std::ios::binary);
			DataFile file_out(filename_out, std::ios::out);
			List<char> buffer_out;
			auto it_out=buffer_out.begin();
			while(file_in.get_next_symbol()!=-1){
				unsigned offset = file_in.get_cur_symbol();
				unsigned len_subject_str = file_in.get_next_symbol();
				#ifdef DEBUG
				SHOW_LIST(buffer_out);
				std::cout << std::endl << "OFFSET: " << offset << std::endl;
				std::cout << "LEN: " << len_subject_str << std::endl;
				std::cout << "STRING: ";
				#endif
				if(offset!=0){
					it_out=buffer_out.begin();
					for(int i=0; i!=(buffer_out.size()-offset); i++) ++it_out;
					for(int i=0; i!=len_subject_str; i++) {
						#ifdef DEBUG
						if(*it_out<=13)
							std::cout << '\'' << (unsigned)(*it_out) << '\'';
						else
							std::cout << (char)(*it_out);
						#endif
						buffer_out.append(*it_out); ++it_out;
					}
				}
				if(file_in.get_next_symbol()!=-1) {
					#ifdef DEBUG
					if(file_in.get_cur_symbol()<=13)
						std::cout << '\'' << (unsigned)(file_in.get_cur_symbol()) << '\'';
					else
						std::cout << (char)(file_in.get_cur_symbol());
					#endif
					buffer_out.append(file_in.get_cur_symbol());
				}
				#ifdef DEBUG
				std::cout << std::endl << std::endl;
				#endif
				while(buffer_out.size()>__len_buffer_out) file_out.write(buffer_out.pop(0));
			}
			while(buffer_out.size()!=0) file_out.write(buffer_out.pop(0));
			__EndTime(__time.decode);
			#ifdef DEBUG
			std::cout << "--------------------END  DECODE--------------------" << std::endl;
			#endif
			return 0;
		}
};