#include <exception>
#include <iostream>
#include "Program.hpp"

int main(){
	try{
		Program snakeGame;
		snakeGame.Run();
	}
	catch(const std::string & message){
		std::cout << message << std::endl;
	}
	catch(const std::exception & e){
		std::cout << e.what() << std::endl;
	}
	catch(...){
		std::cout << "Unexpected error!" << std::endl;
	}
	return 0;
}