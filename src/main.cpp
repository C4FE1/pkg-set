#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <map>
#include <set>
#include "toml.hpp"


std::map<std::string, std::string> config;
std::map<std::string, std::string> objects_by_manager;
std::map<std::string, std::map<std::string, std::string>> commands_by_order;
std::map<std::string, std::map<std::string, std::string>> manager_commands;

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
	//std::cout << "set => habilita uma geração especifica" << std::endl;
	//std::cout << "merge => Une um conjunto de pacotes a sua geração atual" << std::endl; 
	std::cout << "list   => lista as suas gerações" << std::endl;
	//std::cout << "load   => desabilita um conjunto de pacotes e instruções" << std::endl;
	//std::cout << "unload => habilita um conjunto de pacotes e instruções" << std::endl;
}

bool dir_exists(const std::string& dir_path) {
	return std::filesystem::exists(dir_path) && std::filesystem::is_directory(dir_path);
}

bool file_exists(const std::string& file_path) {
    	std::ifstream file(file_path);
    	return file.good();
}

void list_dir(std::string generations_dir_path){
	std::filesystem::path generations_dir = generations_dir_path;
	// Iterate over each file in the directory
    	for (const auto& entry : std::filesystem::directory_iterator(generations_dir)) {
        	// Print the file name
        	std::cout << entry.path().filename() << std::endl;
    	}
}

int get_data_from_file(const std::string& file_path){
	toml::table tbl;
	try{
		//std::map<std::string, std::string> commands_map;
		tbl = toml::parse_file( file_path );
		auto managers = tbl["Managers"].as_table();

		//auto commands = tbl["cmd"];

		for (const auto& manager : *managers ){
			
			//std::cout << manager.first << std::endl;
			
			std::map<std::string, std::string> commands_map;
			std::string manager_name = std::string(manager.first);
			std::string manager_collective = *tbl["Managers"][manager_name]["collective"].value<std::string>();

			auto manager_collective_array = tbl[manager_collective][manager_name];
			std::string objects_list;
			objects_list.clear();
			toml::array* collective_array = manager_collective_array.as_array();

			for (toml::node& elem : *collective_array){
				std::string& str = elem.ref<std::string>();
				objects_list += str + " ";
			}
			objects_by_manager[manager_name] = objects_list;
			const std::string operations[3] = {"add","remove","update"};
			
			for (auto operation : operations){
				commands_map[operation] = *tbl["Managers"][manager_name][operation].value<std::string>();
				//std::cout << commands_map[operation] << std::endl;
				manager_commands[manager_name][operation] = commands_map[operation];
			}
		}

	} catch ( const toml::parse_error&  err ){
		std::cerr << "Falha ao ler o arquivo " << err<< std::endl;
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]){
	std::string user_home = getenv( "HOME" );

	if (argc >= 3){
		if (!file_exists(argv[1])){
			get_data_from_file(argv[2]);
		}else{
			std::cerr << "Erro Arquivo não existe" << std::endl;
			return 1;
		}
		
		if(to_lower_func(std::string(argv[1])) == "load"){
			
			for (const auto& manager : manager_commands) {
				std::string command_to_add_by_manager = manager_commands[std::string(manager.first)]["add"];
				std::string command_to_add = command_to_add_by_manager + " " + objects_by_manager[std::string(manager.first)];	
				//system(command_to_add.c_str());
			}	
			return 0;		
		}else if (to_lower_func(std::string(argv[1])) == "unload"){
			
			for (const auto& manager : manager_commands){
				std::string command_to_remove_by_manager = manager_commands[std::string(manager.first)]["remove"];
				std::string command_to_remove = command_to_remove_by_manager + " " + objects_by_manager[std::string(manager.first)];
				//system(command_to_remove.c_str());
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
		}else if(to_lower_func(std::string(argv[1])) == "list"){
			std::string user_generation_dir = user_home + "/.config/pkg-set/";
			if (dir_exists(user_generation_dir) == true){
				list_dir(user_generation_dir);
				return 0;
			}else{
				std::cout << "Erro: ~/.config/pkg-set/ não foi encontrado." << std::endl;
				return 1;
			}
		}
	}else{
		std::cerr << "Agumentos insuficientes." << std::endl;
		help_page();
		return 1;
	}
	return 0;
}
