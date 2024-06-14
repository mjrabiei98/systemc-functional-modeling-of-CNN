#include "pattern_finder.h"



void patter_finder::do_kernel_bias() {
	std::ifstream input_file("input_paterns/kernel.txt");
	if (!input_file.is_open()) {
		SC_REPORT_ERROR("RAM", "Failed to open input file.");
		return;
	}
	std::string line;
	int i = 0;
	while (i < 30 && std::getline(input_file, line)) {
		if (line.length() == 8) {
			sc_lv<8> text_data(line.c_str());
			if (i < 27){
				kernels_array[i] = text_data;
			}
			else{
				biases[i-27] = text_data;
			}
			i++;
		}
	}
	std::cout << "sucsess reading file and initializing kernel and bias" << endl;
	input_file.close();
}
