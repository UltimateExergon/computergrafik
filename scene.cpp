#include <iostream>
#include "model.h"

using namespace std;

int main() {
	
	cout << "Hello World!" << endl;
	
	Model testmodel;
	testmodel.loadModel("model_cube.stl");
	
	return 0;
}
