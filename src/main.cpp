#include "IncludesWrapper.hpp"

void help_page(){
	std::cout << "Uso: config-sys pkg-set [opção] {pacotes}" << std::endl;
	std::cout << "exemplo: config-sys pkg-set load jonhspkgs" << std::endl;
	std::cout << "opções:" << std::endl;
	std::cout << "help    => mostra essa mensagem em seu terminal" << std::endl;
	//ToDo
	//std::cout << "set     => habilita uma geração especifica" << std::endl;
	//std::cout << "merge   => Une um conjunto de pacotes a sua geração atual" << std::endl;
	//std::cout << "unmerge => Separa um conjuto de pacotes de sua geração atual" << std::endl;
	std::cout << "list    => lista as suas gerações" << std::endl;
	std::cout << "load    => desabilita um conjunto de pacotes e instruções" << std::endl;
	std::cout << "unload  => habilita um conjunto de pacotes e instruções" << std::endl;
}

int main(int argc, char* argv[]){
	std::string user_home = getenv( "HOME" );
	int latest = std::stoi(getenv( "DEFAULT_GEN" ));
	std::string user_config_dir = user_home + "/.config/pkg-set/";
	//std::string user_generation_dir = user_config_dir;

	if (argc >= 3){
		if (file_exists(argv[1]) == false){
			get_configs(user_config_dir + "pkg-set.toml");
			get_collectives(std::string(argv[2]));	
		}else{
			std::cerr << "Erro Arquivo não existe" << std::endl;
			return 1;
		}
		
		if (to_lower_func(std::string(argv[1])) == "load"){
			load_or_unload(0);
			return 0;

		}else if (to_lower_func(std::string(argv[1])) == "unload"){
			load_or_unload(1);
			return 0;	
		
		}else if (to_lower_func(std::string(argv[1])) == "merge"){
			if (latest != 0){
				std::cout << latest << std::endl;
				return 0;
			}else{
				//list_dir(user_generation_dir,0);
				//std::cout << "Sem valor para ACTUAL_GEN" << std::endl;
				return 1;
			}
		}else if (to_lower_func(std::string(argv[1])) == "set"){
			std<< "Sei lá vo ler o help()." << std::endl;
			return 1;	
		}else{
			help_page();
			return 1;
		}

	}else if(argc == 2){
		if(to_lower_func(std::string(argv[1])) == "help"){
			help_page();
			return 0;
		}else if(to_lower_func(std::string(argv[1])) == "list"){
			//std::string user_generation_dir = user_home + "/.config/pkg-set/";
			
			//if (dir_exists(user_generation_dir) == true){
				//list_dir(user_generation_dir,0);
				return 0;
			//}else{
				//std::cout << "Erro: ~/.config/pkg-set/ não foi encontrado." << std::endl;
				//return 1;
			//}
		}
	}else{
		std::cerr << "Agumentos insuficientes." << std::endl;
		help_page();
		return 1;
	}
	return 0;
}
