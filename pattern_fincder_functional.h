#ifndef CN_H
#define CN_H


#include <systemc.h>

template<int NumClk, int DATA_WIDTH, int image_size, int number_of_conv>
SC_MODULE(pf_functional) {

	// Parameters
	sc_signal<sc_lv<8>> kernels_array[3 * 9];
	sc_signal<sc_lv<8>> biases[3];

	sc_signal<sc_lv<DATA_WIDTH>> mem[image_size*image_size];

	// Ports
	sc_in<sc_logic> clk, rst, start;
	sc_out<sc_lv<3>> output_pattern;
	sc_out<sc_logic> done;
	//sc_out<sc_lv<8>> address_out;

	void cal_conv();
	void initial_ram();
	void initial_kernel_bias();

	// Constructor
	SC_CTOR(pf_functional){
		SC_THREAD(initial_ram);
		SC_THREAD(initial_kernel_bias);
		SC_THREAD(cal_conv);
		sensitive << clk, rst, start;
	}
};





template<int NumClk, int DATA_WIDTH, int image_size, int number_of_conv>
void pf_functional<NumClk, DATA_WIDTH, image_size, number_of_conv>::initial_ram() {
	std::ifstream input_file("input_paterns/input6.txt");
	if (!input_file.is_open()) {
		SC_REPORT_ERROR("RAM", "Failed to open input file.");
		return;
	}
	std::string line;
	int i = 0;
	while (i < image_size*image_size && std::getline(input_file, line)) {
		if (line.length() == DATA_WIDTH) {
			sc_lv<DATA_WIDTH> text_data(line.c_str());
			mem[i] = text_data;
			//cout << "read file " << text_data << endl;
			i++;
		}
	}
	//std::cout << "sucsess reading file and initializing ram" << endl;
	input_file.close();
}


template<int NumClk, int DATA_WIDTH, int image_size, int number_of_conv>
void pf_functional<NumClk, DATA_WIDTH, image_size, number_of_conv>::initial_kernel_bias() {
	std::ifstream input_file("input_paterns/kernel.txt");
	if (!input_file.is_open()) {
		SC_REPORT_ERROR("KERNEL", "Failed to open input file.");
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
				biases[i - 27] = text_data;
			}
			i++;
		}
	}
	std::cout << "sucsess reading file and initializing kernel and bias" << endl;
	input_file.close();
}




template<int NumClk, int DATA_WIDTH, int image_size, int number_of_conv>
void pf_functional<NumClk, DATA_WIDTH, image_size, number_of_conv>::cal_conv(){
	while (true){
		int max_out[number_of_conv];
		if (rst == '1'){
			done.write(SC_LOGIC_0);
		}
		else if ((clk == '1') && (clk->event())){
			if (start == '1'){

				for (int k = 0; k < number_of_conv; k++){
					max_out[k] = 0;
					int temp[4] = { 0, 0, 0, 0 };

					for (int x = 0; x < 2; x++){
						for (int y = 0; y < 2; y++){

							for (int i = 0; i < 3; i++){
								for (int j = 0; j < 3; j++){

									temp[2 * x + y] = temp[2 * x + y] + ((mem[4 * (x + i) + y + j].read().to_int()) * (kernels_array[3 * i + j].read().to_int()));
									std::cout << "address" << 4 * (x + i) + y + j << "memvalue out" << kernels_array[3 * i + j] << endl;
									std::cout << "temp val = " << temp[2 * x + y] << endl;
								}

							}

							if (max_out[k] < temp[2 * x + y] + biases[k].read().to_int()){
								max_out[k] = temp[2 * x + y] + biases[k].read().to_int();
							}
						}
					}

				}

				for (int i = 0; i < NumClk; i++){
					wait(clk->posedge_event());
				}
				std::cout << "conv 0 out" << max_out[0] << endl;
				std::cout << "conv 1 out" << max_out[1] << endl;
				std::cout << "conv 2 out" << max_out[2] << endl;
				std::cout << "bias 2 out" << biases[1] << endl;

				if (max_out[0] > max_out[1] && max_out[0] > max_out[2])
					output_pattern.write("001");
				else if (max_out[1] > max_out[0] && max_out[1] > max_out[2])
					output_pattern.write("010");
				else if (max_out[2] > max_out[1] && max_out[2] > max_out[0])
					output_pattern.write("100");
				else
					output_pattern.write("000");

				done.write(SC_LOGIC_1);

			}
		}

		wait();
	}
}




#endif