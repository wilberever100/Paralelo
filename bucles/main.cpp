#include <iostream>
#include <string.h>
#include <random>
#include <chrono>

using namespace std;

const int MAX=500;

int main(int argc, char *argv[]) {
	
	double A[MAX][MAX],x[MAX],y[MAX];
	
	
	//random
	static std::uniform_real_distribution <double> sample_01_(10000.0, 50000.0);
	std::random_device rd;
	std::mt19937 rng(rd());
	
	for(int i=0; i<MAX;i++){
		x[i]=sample_01_(rng);
		for(int j=0; j<MAX;j++){
			A[i][j]=sample_01_(rng);
		}
	}
	//First Loop
	memset(y, 0.0, sizeof(y));
	chrono::duration<float,micro> duration;
	auto start = chrono::system_clock::now();
	for(int i=0; i<MAX;i++){
		for(int j=0; j<MAX;j++){
			y[i]+= A[i][j]*x[j];
		}
	}
	auto end = chrono::system_clock::now();
	duration = end - start;
	cout<<"The time 1st loop  "<<duration.count()<<endl; 	
	
	
	//SECOND LOOP
	memset(y, 0.0, sizeof(y));
	start = chrono::system_clock::now();
	for(int j=0; j<MAX;j++){
		for(int i=0; i<MAX;i++){
			y[i]+= A[i][j]*x[j];
		}
	}
	end = chrono::system_clock::now();
	duration = end - start;
	
	cout<<"The time 2nd loop  "<<duration.count()<<endl; 	
	
	return 0;
}

