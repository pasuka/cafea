#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

int main(int argc, char* argv[])
{
	if(1<argc){
		std::ifstream infile(argv[1]);
		if(infile.is_open()){
			std::string line;
			while(std::getline(infile, line)){
			boost::trim(line);
			if(!line.empty()){
				if(std::equal(line.begin(), line.begin() + 4, "$END")){
					std::cout << "This is END\n";
				}
				else if(std::equal(line.begin(), line.begin()+5, "$NODE")){
					std::cout << "This is keyword NODE begin\n";
					std::getline(infile, line);
					std::vector<std::string> list;
					boost::split(list, line, boost::is_any_of(","));
					if(2<list.size())std::cout << "Node Num: " << list[0] << " Max Id. " << list[1] << "\n";
				}
				else{
					std::cout << line << "\n";
				}
				
				
			}
		}
		}
		else{
			std::cout << "Could not find file: " << argv[1] << "!\n";
		}
		
		
	}
	return 0;
}