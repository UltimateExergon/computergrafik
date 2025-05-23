#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "model.h"

using namespace std;

Model::Model(){}; //Constructor (empty atm)

//Load Model file
void Model::loadModel(string file_name) {
	ifstream file(file_name);
			
	if (file.is_open()) {
		string line;
		
		while (getline(file, line)) {
			cout << line.c_str() << endl;
		}
			
	file.close();
	system("pause");
	}
}	
