#pragma once

#include "Globals.hpp"

int get_collectives(const std::string& file_path){
	for(const auto& manager : manager_commands){
	tbl = toml::parse_file( file_path );
       	auto manager_collective_array = tbl[manager_collective[std::string(manager.first)]];
	std::string objects_list;
	objects_list.clear();
	toml::array* collective_array = manager_collective_array.as_array();
	for (toml::node& elem : *collective_array){
		std::string& str = elem.ref<std::string>();
		std::cout << str << std::endl;
		objects_list += str + " ";
	}
	
	objects_by_manager[std::string(manager.first)] = objects_list;
	}
	return 0;
}

int get_manager_info(const std::string& manager_path, const std::string& manager_name){
	try{
		tbl = toml::parse_file ( manager_path );
		manager_collective[manager_name] = *tbl["colective"].value<std::string>();
		std::map<std::string, std::string> commands_map;
		for (auto operation : operations){
			commands_map[operation] = *tbl[operation].value<std::string>();
			manager_commands[manager_name][operation] = commands_map[operation];
		}
	} catch ( const toml::parse_error& err ){
		std::cerr << "Falha ao ler o arquivo " << err<< std::endl;
		return 1;
	}
	return 0;
}

int get_configs(const std::string& config_path){

	try{
		tbl = toml::parse_file( config_path );
		auto managers = tbl["managers"];
		const std::string managers_path = config_path.substr(0, config_path.size() - 12);
		
		toml::array* managers_array = managers.as_array();
		std::vector<std::string> managers_as_vector;
		
		for ( toml::node& elem : *managers_array){
			
			std::string& str = elem.ref<std::string>();
			managers_as_vector.push_back(str); 
		}
		
		for (std::string elem : managers_as_vector){
			
			if (std::filesystem::exists(managers_path + elem + ".toml")){
				std::cout << elem << " existe" << std::endl;

				std::string manager_file = managers_path + elem + ".toml";
				get_manager_info(manager_file , elem);
			}else{
				std::cout << elem << " nao existe" << std::endl;
			}
		}

	} catch ( const toml::parse_error& err ){
		std::cerr << "Falha ao ler o arquivo " << err<< std::endl;
		return 1;
	}
	return 0;
}

void load_or_unload(const int& index){
	for (const auto& manager : manager_commands){
		std::string command_to_act_by_manager = manager_commands[std::string(manager.first)][operations[index]];
		std::string command_to_act = command_to_act_by_manager + " " + objects_by_manager[std::string(manager.first)];
		system(command_to_act.c_str());
	}
}
