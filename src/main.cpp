#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <map>
#include "toml.hpp"


std::map<std::string, std::string> config;
std::map<std::string, std::string> packages_by_manager;
std::map<std::string, std::map<std::string, std::string>> package_manager_commands;

std::string to_lower_func(const std::string& UPPER_STRING){
	std::string lower_string;

	for (char chacter : UPPER_STRING){
		lower_string += std::tolower(chacter);
	}
	return lower_string;

}

void help_page(){
	std::cout << "Uso: config-sys pkg-set [opção] {pacotes}" << std::endl;
	std::cout << "exemplo: config-sys pkg-set load jonhspkgs" << std::endl;
	std::cout << "opções:" << std::endl;
	std::cout << "help   => mostra essa mensagem em seu terminal" << std::endl;
	//ToDo
	//std::cout << "list   => lista os conjutos de pacotes" << std::endl;
	std::cout << "load   => carrega um conjunto de pacotes" << std::endl;
	std::cout << "unload => descarrega um conjunto de pacotes" << std::endl;
}
/*
bool file_exists(const std::string& file_path) {
    std::ifstream file(file_path);
    return file.good();
}

int get_config_from_file() {
	std::string user_home = getenv("HOME");
	std::string user_config = "/.config/pkg-set/config.toml";
	std::string user_config_path = user_home + user_config; // Fixed the variable name
	std::string file_path;

	if (file_exists(user_config_path)) {
        	file_path = user_config_path;
    	} else {
        	file_path = "/etc/pkg-set/config.toml";
    	}

    	toml::table tbl;
    	try {
        	tbl = toml::parse_file(file_path);
        	auto paths = tbl["Paths"].as_table();
        	for (const auto& path : *paths) {
            		std::string path_name_by_func = std::string(path.first);
            		//std::cout << path_name_by_func << ":" << tbl["Paths"][path_name_by_func] << std::endl;
			config[path_name_by_func] = *tbl["Paths"][path_name_by_func].value<std::string>();
			std::cout << config[path_name_by_func] << std::endl;
        	}

    	} catch (const toml::parse_error& err) {
        	std::cerr << "Failed to read the file: " << err.what() << std::endl; // Changed err to err.what()
        	return 1;
    	}
    	return 0;
}*/

int get_data_from_file(const std::string& file_path){
	toml::table tbl;
	try{
		tbl = toml::parse_file( file_path );
		auto package_managers = tbl["PackageManagers"].as_table();
		for (const auto& package_manager : *package_managers ){
			
			//std::cout << package_manager.first << std::endl;
			
			std::map<std::string, std::string> commands_map;
			std::string package_manager_name = std::string(package_manager.first);

			auto packages = tbl["Packages"][package_manager_name];
			toml::array* package_array = packages.as_array();
			

			std::string package_list;
			//std::cout << packages << ":" << packages.type() << std::endl;
			
			for (toml::node& elem : *package_array){
    				std::string& str = elem.ref<std::string>();
				package_list += str + " ";
			}
			
			packages_by_manager[package_manager_name] = package_list;

			const std::string operations[3] = {"Install","Remove","Update"};
			
			for (auto operation : operations){
				commands_map[operation] = *tbl["PackageManagers"][package_manager_name][operation].value<std::string>();
				//std::cout << commands_map[operation] << std::endl;
				package_manager_commands[package_manager_name][operation] = commands_map[operation];
			}
		}

	} catch ( const toml::parse_error&  err ){
		std::cerr << "Falha ao ler o arquivo " << err<< std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]){
	//std::string user_home = getenv( "HOME" );
	
	//get_config_from_file();

	if (argc >= 3){
		get_data_from_file(argv[2]);
		if(to_lower_func(std::string(argv[1])) == "load"){
			for (const auto& manager : package_manager_commands) {
				std::string command_to_Install_by_manager = package_manager_commands[std::string(manager.first)]["Install"];
				std::string command_to_Install = command_to_Install_by_manager + " " + packages_by_manager[std::string(manager.first)];
				system(command_to_Install.c_str());
			}	
			return 0;		
		}else if (to_lower_func(std::string(argv[1])) == "unload"){
			for (const auto& manager : package_manager_commands){
				std::string command_to_Remove_by_manager = package_manager_commands[std::string(manager.first)]["Remove"];
				std::string command_to_Remove = command_to_Remove_by_manager + " " + packages_by_manager[std::string(manager.first)];
				system(command_to_Remove.c_str());
			}
			return 0;	
		}else{
			help_page();
			return 1;
		}
	}else if(argc == 2){
		if(to_lower_func(std::string(argv[1])) == "help"){
			help_page();
			return 0;
		}
	}else{
		std::cerr << "Agumentos insuficientes." << std::endl;
		help_page();
		return 1;
	}
	return 0;
}
