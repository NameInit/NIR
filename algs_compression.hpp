#pragma once

#include <iostream>
#include "timer.hpp"
#include "data_io.hpp"

class AlgsCompression{
    protected:
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

        Times __time;
        Filename __filename;

        void __StartTime(Timer& timer) { timer.set_start(); }
        void __EndTime(Timer& timer) { timer.set_end(); }
        void __SetFileName(std::string& old_filename, const std::string& new_filename) { old_filename=new_filename; }
    public:
        virtual ~AlgsCompression() {};

        virtual int encode(const std::string& filename_in, const std::string& filename_out)=0;
        virtual int decode(const std::string& filename_in, const std::string& filename_out)=0;
        
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