build : main.cpp dir_traverse.cpp file_operation.cpp screen_man.cpp
		g++ -std=c++11 main.cpp dir_traverse.cpp file_operation.cpp screen_man.cpp -o Xplorer
	
clean: 
		rm -f Xplorer