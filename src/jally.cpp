#define _SECURE_SCL 0
#define _SCL_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

#define BOOST_DISABLE_ASSERTS

#ifdef PROFILE
#include "gperftools/profiler.h"
#endif

#include "version.h"
#include "region.h"

using namespace wallysworld;

inline void
displayUsage() {
  std::cout << "Usage: Jally <command> <arguments>" << std::endl;
  std::cout << std::endl;
  std::cout << "    region         plot genomic region" << std::endl;
  std::cout << "    version        show jally and requirments version" << std::endl; 
  std::cout << "    help           show jally help" << std::endl; 
  std::cout << std::endl;
}

int main(int argc, char **argv) {
    if (argc < 2) { 
      printTitle("Jally");
      displayUsage();
      return 0;
    }

    if ((std::string(argv[1]) == "version") || (std::string(argv[1]) == "--version") || (std::string(argv[1]) == "--version-only") || (std::string(argv[1]) == "-v")) {
      std::cout << "Jally version: v" << jallyVersionNumber << std::endl;
      std::cout << " using Boost: v" << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100 << std::endl;
      std::cout << " using HTSlib: v" << hts_version() << std::endl;
      std::cout << " using OpenCV: v" << CV_VERSION  << std::endl;
      return 0;
    }
    else if ((std::string(argv[1]) == "help") || (std::string(argv[1]) == "--help") || (std::string(argv[1]) == "-h") || (std::string(argv[1]) == "-?")) {
      printTitle("Jally");
      displayUsage();
      return 0;
    }
    else if ((std::string(argv[1]) == "region")) {
      return region(argc-1,argv+1);
    }
    std::cerr << "Unrecognized command " << std::string(argv[1]) << std::endl;
    return 1;
}
