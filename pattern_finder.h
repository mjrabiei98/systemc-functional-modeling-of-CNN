#ifndef PF_H
#define PF_H

#include <systemc.h>
#include "convolution.h"
#include "Elements.h"
#include "RAM.h"

// Define the kernel_array and bias_array types
typedef sc_lv<8> kernel_type[3][9];
typedef sc_lv<8> bias_array[3];

SC_MODULE(patter_finder) {
	// Parameters
	static const int data_width = 8;
	static const int number_of_conv = 3;
	static const int kernel_size = 3;

	// Inputs
	sc_signal<sc_lv<8>> kernels_array[3*9];
	sc_signal<sc_lv<8>> biases[3];

	sc_in<sc_lv<8>> image_size;

	// Ports
	sc_in<sc_logic> clk, rst, start, write_ram;
	sc_in<sc_lv<data_width>> data_in;
	sc_in<sc_lv<data_width>> address_in_wr;
	sc_out<sc_logic> done;
	sc_out<sc_lv<3>> output_pattern;

	// Internal signals
	sc_signal<sc_lv<data_width>> ram_data_out;
	sc_signal<sc_logic> write_en, read_en;
	sc_signal<sc_lv<data_width>> address_out;
	sc_signal<sc_lv<data_width>> convs_output[number_of_conv][4];
	sc_signal<sc_lv<data_width>> relus_output[number_of_conv][4];
	sc_signal<sc_lv<data_width>> maxpools_output[number_of_conv];

	sc_signal<sc_lv<8>> temp_adr[number_of_conv];
	sc_signal<sc_logic> temp_done[number_of_conv];

	// Submodule instances
	ram* ram1;
	convolution* conv[number_of_conv];
	relu* relu_units[number_of_conv];
	maxpool* maxpool_units[number_of_conv];
	resualt* result_unit;

	void do_kernel_bias();

	// Constructor
	SC_CTOR(patter_finder) {
		// Instantiate RAM module
		ram1 = new ram("ram1");
		ram1->clk(clk);
		ram1->rst(rst);
		ram1->address_in_read(address_out);
		ram1->data_in(data_in);
		ram1->address_in_wr(address_in_wr);
		ram1->data_out(ram_data_out);
		ram1->write_en(write_en);
		ram1->read_en(read_en);
		SC_METHOD(do_kernel_bias);

		

		// Instantiate convolution, relu, and maxpool modules
		for (int i = 0; i < number_of_conv; ++i) {
			std::string conv_name = "conv" + std::to_string(i);
			conv[i] = new convolution(conv_name.c_str());
			conv[i]->bias_value(biases[i]);
			conv[i]->image_size(image_size);
			conv[i]->kernet_1(kernels_array[i * 9 + 0]);
			conv[i]->kernet_2(kernels_array[i * 9 + 1]);
			conv[i]->kernet_3(kernels_array[i * 9 + 2]);
			conv[i]->kernet_4(kernels_array[i * 9 + 3]);
			conv[i]->kernet_5(kernels_array[i * 9 + 4]);
			conv[i]->kernet_6(kernels_array[i * 9 + 5]);
			conv[i]->kernet_7(kernels_array[i * 9 + 6]);
			conv[i]->kernet_8(kernels_array[i * 9 + 7]);
			conv[i]->kernet_9(kernels_array[i * 9 + 8]);
			conv[i]->clk(clk);
			conv[i]->rst(rst);
			conv[i]->start(start);
			conv[i]->data_in(ram_data_out);
			conv[i]->data_out1(convs_output[i][0]);
			conv[i]->data_out2(convs_output[i][1]);
			conv[i]->data_out3(convs_output[i][2]);
			conv[i]->data_out4(convs_output[i][3]);
			if (i != 0){
				conv[i]->done(temp_done[i]);
				conv[i]->address_out(temp_adr[i]);
			}

			

			std::string relu_name = "relu" + std::to_string(i);
			relu_units[i] = new relu(relu_name.c_str());
			relu_units[i]->a(convs_output[i][0]);
			relu_units[i]->b(convs_output[i][1]);
			relu_units[i]->c(convs_output[i][2]);
			relu_units[i]->d(convs_output[i][3]);
			relu_units[i]->d1(relus_output[i][0]);
			relu_units[i]->d2(relus_output[i][1]);
			relu_units[i]->d3(relus_output[i][2]);
			relu_units[i]->d4(relus_output[i][3]);

			std::string maxpool_name = "maxpool" + std::to_string(i);
			maxpool_units[i] = new maxpool(maxpool_name.c_str());
			maxpool_units[i]->a(relus_output[i][0]);
			maxpool_units[i]->b(relus_output[i][1]);
			maxpool_units[i]->c(relus_output[i][2]);
			maxpool_units[i]->d(relus_output[i][3]);
			maxpool_units[i]->output(maxpools_output[i]);
		}
		conv[0]->address_out(address_out);
		conv[0]->done(done);
		// Instantiate result unit
		result_unit = new resualt("result_unit");
		result_unit->a(maxpools_output[0]);
		result_unit->b(maxpools_output[1]);
		result_unit->c(maxpools_output[2]);
		result_unit->output(output_pattern);
	}
};

#endif // PF_H
