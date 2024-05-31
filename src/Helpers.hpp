#pragma once
#include "Globals.hpp"
// check if directory exists
bool dir_exists(const std::string& dir_path) {
	return std::filesystem::exists(dir_path) && std::filesystem::is_directory(dir_path);
}

// Check if file exists
bool file_exists(const std::string& file_path) {
    try {
        return std::filesystem::exists(file_path) && std::filesystem::is_regular_file(file_path);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return false;
    }
}

// same to files
//bool file_exists(const std::string& file_path) {
    	//std::ifstream file(file_path);
    	//return file.good();
	//return std::filesystem::exists(file_path);
//}

// equivalent to tolower() in C
std::string to_lower_func(const std::string& UPPER_STRING){
	std::string lower_string;

	for (char chacter : UPPER_STRING){
		lower_string += std::tolower(chacter);
	}
	return lower_string;

}
