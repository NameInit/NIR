#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>

class DataFile{
    private:
        std::fstream __file;
        std::string __filename;
        int __cur_symbol;

    public:
        DataFile(std::string filename, std::ios::openmode flags) {
            __file.open(filename, flags);
            if(!__file.is_open())
                throw std::logic_error("Cannot open file");
            __filename=filename;
            __cur_symbol=-1;
        }

        ~DataFile(){
            __file.close();
        }

        int get_next_symbol(){
            if(!__file.is_open())
                throw std::logic_error("File is not open");
            __cur_symbol=__file.get();
            return __cur_symbol;
        }

        int get_cur_symbol(){
            return __cur_symbol;
        }

        std::map<char, unsigned> get_map_symbol_frequency(){
            if(!__file.is_open())
                throw std::logic_error("File is not open");
            char symbol;
            unsigned position=__file.tellg();
            __file.seekg(0,__file.beg); 
            std::map<char, unsigned> symbol_frequency;

            while((symbol=__file.get())!=-1)
                symbol_frequency[symbol]+=1;

            __file.seekg(position, __file.beg);
            return symbol_frequency;
        }

        void record(unsigned symbol){
            if(!__file.is_open())
                throw std::logic_error("File is not open");
            unsigned position = __file.tellp();
            __file.seekp(0, __file.end);
            __file.put(symbol);
            __file.seekp(position, __file.beg);
        }

        void write(unsigned symbol){
            if(!__file.is_open())
                throw std::logic_error("File is not open");
            __file.put(symbol);
        }

        unsigned size(){
            unsigned position = __file.tellg();
            unsigned size;
            __file.seekg(0, __file.end);
            size=__file.tellg();
            __file.seekg(position, __file.beg);
            return size;
        }
};