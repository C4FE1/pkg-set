#pragma once

#include "Globals.hpp"
void list_dir(const std::string generations_dir_path,const int& operator_index){
	const std::string list_operations[2] = {"user","program"};
	
	std::regex pattern("(Generation_)(\\d{2})(\\.toml)");

	std::filesystem::path generations_dir = generations_dir_path;
    	
	for (const auto& entry : std::filesystem::directory_iterator(generations_dir)) {
		
		if(list_operations[operator_index] == "user"){
     	
			std::cout << entry.path().filename() << std::endl;
		
		}else if(list_operations[operator_index] == "program"){
			std::smatch match;
			std::string filename = entry.path().filename().string();
			
			if (std::regex_search(filename, match, pattern)) {
				std::string generation_number_str = match[2];
                int generation_number = std::stoi(generation_number_str);
				generation_index.push_back(generation_number);
                std::cout << "Generation: " << generation_number << std::endl;
            }
		}
    }
}

int merge_file_A_to_B(const std::string File_A_path, const std::string File_B_path){
	



	return 0;
}
