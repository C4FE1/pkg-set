#pragma once
#include <iostream>
#include <unistd.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <cctype>
#include <cstdlib>
#include <regex>
#include <map>
#include <vector>
#include <set>

std::vector<int> generation_index; 
std::map<std::string, std::string> config;
std::map<std::string, std::string> objects_by_manager;
//std::map<std::string, std::map<std::string, std::string>> commands_by_order;
std::map<std::string, std::map<std::string, std::string>> manager_commands;
std::map<std::string, std::string> manager_collective;
toml::table tbl;
const std::string operations[3] = {"add","remove","update"};
