#ifndef ELEMENTS_H
#define ELEMENTS_H

#include <systemc.h>

SC_MODULE(mux_5to1) {
	sc_in<sc_lv<8>> a, b, c, d, e;
	sc_in<sc_lv<3>> sel;
	sc_out<sc_lv<8>> out;

	void eval();

	SC_CTOR(mux_5to1) {
		SC_METHOD(eval);
		sensitive << a << b << c << d << e << sel;
	}
};


SC_MODULE(mux) {
	// Ports
	sc_in<sc_lv<8>> a, b, c, d;   // 8-bit input vectors
	sc_in<sc_lv<2>> sel;          // 2-bit select input
	sc_out<sc_lv<8>> out;         // 8-bit output vector

	void eval();

	// Constructor
	SC_CTOR(mux) {
		SC_METHOD(eval);
		sensitive << a << b << c << d << sel;
	}
};


SC_MODULE(kernel_mux) {
	// Parameters
	static const int input_size = 8;  // Setting the input size as 8 bits

	// Ports
	sc_in<sc_lv<input_size>> a, b, c, d, e, f, g, h, k;
	sc_in<sc_lv<input_size>> i, j;
	sc_out<sc_lv<input_size>> output;

	// Process method
	void eval();

	// Constructor
	SC_CTOR(kernel_mux) {
		SC_METHOD(eval);
		sensitive << a << b << c << d << e << f << g << h << k << i << j;
	}
};



SC_MODULE(mult) {
	// Parameters
	static const int input_size = 8;

	// Ports
	sc_in<sc_lv<input_size>> a, b;
	sc_out<sc_lv<input_size>> output;

	// Internal signal
	sc_signal<sc_lv<2 * input_size>> temp;

	// Process method
	void eval();

	// Constructor
	SC_CTOR(mult) {
		SC_METHOD(eval);
		sensitive << a << b;
	}
};



SC_MODULE(adder) {
	// Parameters
	static const int input_size = 8;

	// Ports
	sc_in<sc_lv<input_size>> a, b;
	sc_out<sc_lv<input_size>> output;

	// Process method
	void eval();

	// Constructor
	SC_CTOR(adder) {
		SC_METHOD(eval);
		sensitive << a << b;
	}
};




SC_MODULE(counter) {
	// Parameters
	static const int counter_size = 8;
	int counter_limit;

	// Ports
	sc_in<sc_logic> clk, rst, en;
	sc_out<sc_lv<counter_size>> counter_out;
	sc_out<sc_logic> cout;

	// Process method
	void do_count();

	// Constructor
	SC_CTOR(counter) {
		SC_METHOD(do_count);
			sensitive << clk << rst;
	}
	counter(sc_module_name name, int size) : sc_module(name), counter_limit(size) {
		SC_METHOD(do_count);
		sensitive << clk << rst;
	}

};


SC_MODULE(maxpool) {
	// Parameters
	static const int data_width = 8;

	// Ports
	sc_in<sc_lv<data_width>> a, b, c, d;
	sc_out<sc_lv<data_width>> output;

	// Process method
	void do_maxpool();

	// Constructor
	SC_CTOR(maxpool) {
		SC_METHOD(do_maxpool);
		sensitive << a << b << c << d;
	}
};


SC_MODULE(relu) {
	// Parameters
	static const int data_width = 8;

	// Ports
	sc_in<sc_lv<data_width>> a, b, c, d;
	sc_out<sc_lv<data_width>> d1, d2, d3, d4;

	// Process method
	void do_relu();

	// Constructor
	SC_CTOR(relu) {
		SC_METHOD(do_relu);
		sensitive << a << b << c << d;
	}
};



SC_MODULE(resualt) {
	// Parameters
	static const int data_width = 8;

	// Ports
	sc_in<sc_lv<data_width>> a, b, c;
	sc_out<sc_lv<3>> output;

	// Process method
	void do_compare();

	// Constructor
	SC_CTOR(resualt) {
		SC_METHOD(do_compare);
		sensitive << a << b << c;
	}
};




//SC_MODULE(reg) {
//	// Parameters
//	int register_size;
//
//	// Ports
//	sc_in<sc_logic> clk, rst, en;
//	sc_in<sc_lv<16>> d;
//	sc_out<sc_lv<16>> q;
//
//	// Process method
//	void process_reg();
//
//	// Constructor
//	SC_CTOR(reg) : register_size(16) {
//		SC_METHOD(process_reg);
//		sensitive << clk << rst;
//	}
//
//	// Overloaded constructor for parameterized register size
//	SC_HAS_PROCESS(reg);
//	reg(sc_module_name name, int reg_size) : sc_module(name), register_size(reg_size) {
//		SC_METHOD(process_reg);
//		sensitive << clk.pos() << rst;
//	}
//};

template<int SIZE>
SC_MODULE(reg) {
	// Ports
	sc_in<sc_logic> clk, rst, en;
	sc_in<sc_lv<SIZE>> d;
	sc_out<sc_lv<SIZE>> q;

	// Process method
	void process_reg() {
		if (rst.read() == '1') {
			q.write(0);
		}
		else if ((clk->event() && clk.read() == '1') && en.read() == '1') {
			q.write(d.read());
		}
		//std::cout << name() << "d val = " << d << "reg_value = " << q << endl;
	}

	// Constructor
	SC_CTOR(reg) {
		SC_METHOD(process_reg);
		sensitive << clk << rst;
	}
};



#endif