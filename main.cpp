#include "huffman.hpp"
#include <thread>
#include <chrono>

void show_statistic(std::string,std::string,std::string);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <имя_файла>" << std::endl;
        return 1;
    }

    Huffman h(argv[1]);
    Timer run_code;

    // auto start_chrono = std::chrono::high_resolution_clock::now();
    // run_code.set_start();
    h.encode(argv[1], (static_cast<std::string>(argv[1])) + ".huff");
    h.decode((static_cast<std::string>(argv[1]))  + ".huff", (static_cast<std::string>(argv[1])) + ".dec");
    h.show_statistic();
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    // run_code.set_end();
    // std::cout << run_code.duration_s() << std::endl;

    // show_statistic(argv[1], (static_cast<std::string>(argv[1])) + "_huff.bin", (static_cast<std::string>(argv[1])) + "_huff.dec");


    // std::chrono::high_resolution_clock::time_point end_chrono = std::chrono::high_resolution_clock::now();
    
    // auto duration_chrono = std::chrono::duration_cast<std::chrono::milliseconds>(end_chrono - start_chrono).count();
    
    // std::cout << "Время выполнения (chrono): " << duration_chrono/1000. << " секунд" << std::endl;
    return 0;
}

void show_statistic(std::string filename_base, std::string filename_bin, std::string filename_dec){
    DataFile file_base(filename_base, std::ios::in);
    DataFile file_bin(filename_bin, std::ios::in);
    DataFile file_dec(filename_dec, std::ios::in);

    std::cout << "Коэффициент сжатия: " << (double)file_base.size()/(double)file_bin.size() << std::endl;
}