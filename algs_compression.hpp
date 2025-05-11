#pragma once

#include <iostream>
#include <vector>
#include <vector>
#include <map>
#include <utility>
#include "timer.hpp"
#include "data_io.hpp"
#include "list.hpp"

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
		unsigned __occupied_memory_in_bytes=0;
		std::vector<std::pair<std::string, std::vector<double>>> __history;

		template<typename K, typename I>
		void __CalcMemoryMap(const std::map<K,I>& map) {
			__occupied_memory_in_bytes += map.size()*(sizeof(K)+sizeof(I)+sizeof(void*))+sizeof(std::map<K, I>);
		}

		void __CalcMemoryString(const std::string& str){
			__occupied_memory_in_bytes+=sizeof(str)+str.capacity();
		}

		template<typename I>
		void __CalcMemoryList(List<I>& list){
			__occupied_memory_in_bytes+=sizeof(list)+list.size()*(sizeof(I)+2*sizeof(I*));
		}

		template<typename I>
		void __CalcMemoryVector(const std::vector<I>& vector){
			__occupied_memory_in_bytes += sizeof(I) * vector.capacity() + sizeof(vector);
		}

		void __IncreaseUsageMemory(unsigned count_bytes){
			__occupied_memory_in_bytes+=count_bytes;
		} 

		void __SleepTime(Timer& timer) { timer.sleep(); }
		void __UnSleepTime(Timer& timer) { timer.unsleep(); }
		void __StartTime(Timer& timer) { timer.start(); }
		void __EndTime(Timer& timer) { timer.end(); }
		void __SetFileName(std::string& old_filename, const std::string& new_filename) { old_filename=new_filename; }
		void __AddInHistory(){ __history.push_back({__filename.base, {__time.init.duration_s()+__time.encode.duration_s(), __time.decode.duration_s()}}); }
	public:
		virtual ~AlgsCompression() {};

		virtual int encode(const std::string& filename_in, const std::string& filename_out)=0;
		virtual int decode(const std::string& filename_in, const std::string& filename_out)=0;
		
		void save_statistic(){ __AddInHistory(); }

		void show_history(){
			for(auto it : __history){
				std::cout << it.first << ':';
				for(auto it_item : it.second)
					std::cout << ' ' << it_item;
				std::cout << std::endl;
			}
		}

		void show_statistic(bool compres = false){
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
			if(compres){
				std::cout <<  __time.encode.duration_s() +  __time.init.duration_s() << ' ' << __time.decode.duration_s() << ' ' << __occupied_memory_in_bytes << ' ' << (double)file_base.size()/(double)file_binary.size() << ' ';
			}			
			else {
				std::cout << "---------------STATISTIC COMPRESSION---------------" << std::endl;
				std::cout << "START FILE: " << __filename.base << '(' << file_base.size() << " bytes" <<')' << std::endl;
				std::cout << "ARCHIVED FILE: " << __filename.binary << '(' << file_binary.size() << " bytes" <<')' << std::endl;
				std::cout << "UNZIPPED FILE: " << __filename.unzipped << '(' << file_unzipped.size() << " bytes" <<')' << std::endl << std::endl;

				std::cout << "TIME FOR ARCHIVING: " << __time.encode.duration_s() +  __time.init.duration_s() << 's' << std::endl;
				std::cout << "TIME FOR UNZIPPED: " << __time.decode.duration_s() << 's' << std::endl << std::endl;

				std::cout << "USAGE MEMORY: " << __occupied_memory_in_bytes << " bytes" << std::endl << std::endl;

				std::cout << "COMPRESSION RATIO: " << (double)file_base.size()/(double)file_binary.size() << std::endl;
				std::cout << "INTEGRITY: " << get_percent_diff_byte_in_files(file_base,file_unzipped) << '%' << std::endl;
				std::cout << "------------------------END------------------------" << std::endl;
			}
		}
};