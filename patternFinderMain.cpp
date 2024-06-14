#include "TB.h"
#include "convolution.h"

int sc_main(int argc, char* argv[]) {

	// Instantiate the testbench
	testbench tb("testbench");

	sc_trace_file* vcdfile;
	vcdfile = sc_create_vcd_trace_file("patternFinder_test");
	sc_trace(vcdfile, tb.start, "start");
	sc_trace(vcdfile, tb.rst, "rst");
	sc_trace(vcdfile, tb.clk, "clk");
	sc_trace(vcdfile, tb.output_pattern, "output_pattern");
	sc_trace(vcdfile, tb.done, "done");

	// Start simulation
	sc_start(10000, SC_NS);

	return 0;
}