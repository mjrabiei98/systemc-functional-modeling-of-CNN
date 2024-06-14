#ifndef CN_H
#define CN_H


#include <systemc.h>

template<int NumClk, int DATA_WIDTH, int image_size>
SC_MODULE(convolution_functional) {

	// Parameters
	sc_signal<sc_lv<8>> kernels_array[9];
	sc_signal<sc_lv<8>> biase;


	// Ports
	sc_in<sc_logic> clk, rst, start;
	sc_in<sc_lv<8>> ram_data;
	sc_out<sc_lv<8>> ram_address;
	sc_out<sc_lv<8>> output[4];
	sc_out<sc_logic> done;
	//sc_out<sc_lv<8>> address_out;

	void cal_conv();
	void initial_ram();
	void initial_kernel_bias();

	// Constructor
	SC_CTOR(convolution_functional){
		SC_THREAD(initial_kernel_bias);
		SC_THREAD(cal_conv);
			sensitive << clk, rst, start;
	}
};


template<int NumClk, int DATA_WIDTH, int image_size>
void convolution_functional<NumClk, DATA_WIDTH, image_size>::initial_kernel_bias() {
	std::ifstream input_file("input_paterns/kernel.txt");
	if (!input_file.is_open()) {
		SC_REPORT_ERROR("KERNEL", "Failed to open input file.");
		return;
	}
	std::string line;
	int i = 0;
	while (i < 10 && std::getline(input_file, line)) {
		if (line.length() == 8) {
			sc_lv<8> text_data(line.c_str());
			if (i < 9){
				kernels_array[i] = text_data;
			}
			else{
				biase = text_data;
			}
			i++;
		}
	}
	std::cout << "sucsess reading file and initializing kernel and bias" << endl;
	input_file.close();
}




template<int NumClk, int DATA_WIDTH, int image_size, int number_of_conv>
void convolution_functional<NumClk, DATA_WIDTH, image_size, number_of_conv>::cal_conv(){
	while (true){
		if (rst == '1'){
			done.write(SC_LOGIC_0);
		}
		else if ((clk == '1') && (clk->event())){
			if (start == '1'){

				max_out = 0;
				int temp[4] = { 0, 0, 0, 0 };

				for (int x = 0; x < 2; x++){
					for (int y = 0; y < 2; y++){

						for (int i = 0; i < 3; i++){
							for (int j = 0; j < 3; j++){

								// address ro bezar ru khrooji
								ram_address.write(4 * (x + i) + y + j);
								wait(clk->posedge_event());

								temp[2 * x + y] = temp[2 * x + y] + (ram_data.read() *
												  (kernels_array[3 * i + j].read().to_int()));

							}

						}

						if (max_out < temp[2 * x + y] + biases[k].read().to_int()){
							max_out = temp[2 * x + y] + biases[k].read().to_int();
						}
					}
				}

				for (int i = 0; i < NumClk-36; i++){
					wait(clk->posedge_event());
				}

				
				output[0].write(temp[0]);
				output[1].write(temp[1]);
				output[2].write(temp[2]);
				output[3].write(temp[3]);
				done.write(SC_LOGIC_1);

			}
		}
	
		wait();
	}
}




#endif