/* Top-level flocra simulation MaRCoS server file.
 *
 * When compiling standalone server, replace this with
 * marcos_server.cpp in the marcos_server library.
 *
 * Overall operation should remain compatible between both, apart from
 * the extra Verilator-related objects in flocra_sim_main which are
 * made use of in hardware.cpp for emulating PS<->PL communication.
 * 
 */

extern "C" {	
// Linux-related
#include <unistd.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
}

#include "version.hpp"
#include "hardware.hpp"
#include "iface.hpp"

// Verilator model
#include "flocra_model.hpp"

#include <iostream>
#include <sstream>

unsigned SERVER_VERSION_UINT;
std::string SERVER_VERSION_STR;

flocra_model *fm;
hardware *hw;
iface *ifa;

int main(int argc, char *argv[]) {
	std::cout << "MaRCoS server + FLOCRA sim model, " << __DATE__ << " " << __TIME__ << std::endl;

	// Global version string creation
	std::stringstream sv;
	sv << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_DEBUG;
	SERVER_VERSION_UINT = ((VERSION_MAJOR << 16) & 0xff0000) | ((VERSION_MINOR << 8) & 0xff00) | (VERSION_DEBUG & 0xff);
	SERVER_VERSION_STR = sv.str();
	
	std::cout << "Server version " << SERVER_VERSION_STR << std::endl;

	fm = new flocra_model(argc, argv);
	hw = new hardware();
	ifa = new iface();
	ifa->run_stream();

	// Cleanup
	delete hw;
	delete ifa;
	delete fm;
}
