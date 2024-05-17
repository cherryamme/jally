#ifndef VERSION_H
#define VERSION_H

#include <boost/version.hpp>

#include <htslib/sam.h>

namespace wallysworld
{


  std::string jallyVersionNumber = "0.0.1";

  inline 
    void printTitle(std::string const& title) 
    {
      std::cout << "**********************************************************************" << std::endl;
      std::cout << "Program: Jally" << std::endl;
      std::cout << "This program Use for plot reads from bam" << std::endl;
      std::cout <<  std::endl;
      std::cout <<  title << " (Version: " << jallyVersionNumber << ")" << std::endl;
      std::cout << "Contact: jiangchen2 (jiangchen2@genomics.com)" << std::endl;
      std::cout << "**********************************************************************" << std::endl;
      std::cout << std::endl;
    }

  inline
    void displayWarranty()
    {
      std::cout << "SHOW DAMAGE." << std::endl;
      std::cout << std::endl;
    }


    inline void
    bsd() {
      std::cout << "Copyright (c) 2021, European Molecular Biology Laboratory (EMBL)." << std::endl;
      std::cout << "All rights reserved." << std::endl;
      std::cout << std::endl;
      std::cout << "Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:" << std::endl;
      std::cout << "    1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer." << std::endl;
      std::cout << "    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution." << std::endl;
      std::cout << "    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission." << std::endl;
      std::cout << std::endl;
      std::cout << "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, ";
      std::cout << "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR ";
      std::cout << "CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, ";
      std::cout << "PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, ";
      std::cout << "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ";
      std::cout << "ADVISED OF THE POSSIBILITY OF SUCH DAMAGE." << std::endl;
      std::cout << std::endl;
    }


}

#endif
