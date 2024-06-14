#ifndef TB_H
#define TB_H
#include <systemc.h>
#include "pattern_fincder_functional.h"
#include <iostream>

SC_MODULE(testbench) {







	sc_signal<sc_lv<8>> sig_image_size;
	sc_signal<sc_logic> start;
	sc_signal<sc_logic> clk;
	sc_signal<sc_logic> rst;
	sc_signal<sc_logic> write_ram;
	sc_signal<sc_lv<3>> output_pattern;
	sc_signal<sc_logic> done;
	sc_signal<sc_lv<8>> data_in;
	sc_signal<sc_lv<8>> address_in_wr;


	//int NumClk, int DATA_WIDTH, int image_size

	// Instance of the patter_finder module
	pf_functional<12, 8, 4, 3>* p_finder;

	// Constructor
	SC_CTOR(testbench) {
		// Instantiate the patter_finder module
		p_finder = new pf_functional<12, 8, 4, 3>("p_finder");
		p_finder->clk(clk);
		p_finder->rst(rst);
		p_finder->start(start);
		p_finder->done(done);
		p_finder->output_pattern(output_pattern);






		// Initialize signals
		start.write(SC_LOGIC_0);
		clk.write(SC_LOGIC_0);
		rst.write(SC_LOGIC_0);
		write_ram.write(SC_LOGIC_0);
		data_in.write("00000000");
		address_in_wr.write("00000000");
		sig_image_size.write("00000100");

		// Register processes
		SC_THREAD(resetting);
		SC_THREAD(clocking);
		SC_THREAD(inGenerating);
		SC_THREAD(displaying);
		sensitive << clk;
		//sensitive << done;
	}

	void resetting();
	void clocking();
	void inGenerating();
	void displaying();

};


#endif
