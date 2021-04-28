#include <iostream>

#include "muchosMutex.h"
#include "unMutex.h"
#include "mutexWriteRead.h"
#include <random>
#include <vector>
#include <chrono>

using namespace std;

vector <int> operaciones;
vector <int> numeros;
const int MAX_NUM_OPERATIONS = 100000;

template <class L>
class ListTester {
public:
	ListTester(L& lista, int modifier)
		: lista_(lista), modifier_(modifier) {}
	void operator()() {
		for (int i = 0; i < MAX_NUM_OPERATIONS; ++i)
			lista_.operacion(operaciones[i], numeros[i]);
			//lista_.operacion(2, numeros[i]);
	}

private:
	L& lista_;
	int modifier_;

	static std::uniform_real_distribution <double>
		sample_01_;
	static std::uniform_int_distribution<unsigned int>
		random_;
};




template <class L>
void RunTest(int num_threads) {
	

	L lista;

	std::vector<std::thread*> threads;

	for (int i = 0; i < num_threads; i++) {
		ListTester<L> tester(lista, i * MAX_NUM_OPERATIONS);

		std::thread* process = new std::thread(tester);
		threads.push_back(process);
	}
	for (int i = 0; i < num_threads; ++i) {
		threads[i]->join();
	}

}

void setRandomValues() {
	static const double CONTAINS_WEIGHT = 0.9;
	static const double INSERT_WEIGHT = 0.95;
	for (int i = 0; i < MAX_NUM_OPERATIONS; ++i) {
		//random
		static std::uniform_real_distribution <double> random_(0.000, 1.000);
		static std::uniform_int_distribution <int> sample_1(1000, 10000);

		std::random_device rd; // random device engine, usually based on /dev/random on UNIX-like systems
		// initialize Mersennes' twister using rd to generate the seed
		std::mt19937 rng(rd());

		if (i < MAX_NUM_OPERATIONS * 0.1) {
			operaciones.push_back(2);
			numeros.push_back(sample_1(rng));
		}
		else if (i < MAX_NUM_OPERATIONS * 0.2) {
			operaciones.push_back(3);
			numeros.push_back(sample_1(rng));
		}
		else {
			operaciones.push_back(1);
			numeros.push_back(sample_1(rng));
		}

		/*if (random_(rng) < CONTAINS_WEIGHT) {
			operaciones.push_back(1);
			numeros.push_back(sample_1(rng));
		}
		else {
			if (random_(rng) < INSERT_WEIGHT) {
				operaciones.push_back(2);
				numeros.push_back(sample_1(rng));
			}
			else {
				operaciones.push_back(3);
				numeros.push_back(sample_1(rng));
			}
		}*/
	}
}

int main() {
	setRandomValues();

	
	//Corriendo con 8 threads
	int thread_num = 8;
	cout << "------------------------THREADS: --------------------------" << thread_num << endl;
	auto start = std::chrono::system_clock::now();
	RunTest<lista_write_read>(thread_num);
	auto end = std::chrono::system_clock::now();
	chrono::duration<float, std::milli> duration = end - start;
	cout << "Pthread write and read: " << duration.count() << " ms" << endl;



	start = std::chrono::system_clock::now();
	RunTest<lista_one_mutex>(thread_num);
	end = std::chrono::system_clock::now();
	duration = end - start;
	std::cout << "List with only one Mutex: " << duration.count() << " ms" << endl;

	//RunTest<lista_mutex>(true);

	//Corriendo con 4
	
	thread_num = 4;
	cout << "------------------------THREADS: --------------------------" << thread_num << endl;
	start = std::chrono::system_clock::now();
	RunTest<lista_write_read>(thread_num);
	end = std::chrono::system_clock::now();
	duration = end - start;
	cout << "Pthread write and read: " << duration.count() << " ms" << endl;



	start = std::chrono::system_clock::now();
	RunTest<lista_one_mutex>(thread_num);
	end = std::chrono::system_clock::now();
	duration = end - start;
	std::cout << "List with only one Mutex: " << duration.count() << " ms" << endl;

	//Corriendo con 2

	thread_num = 2;
	cout << "------------------------THREADS: --------------------------" << thread_num << endl;
	start = std::chrono::system_clock::now();
	RunTest<lista_write_read>(thread_num);
	end = std::chrono::system_clock::now();
	duration = end - start;
	cout << "Pthread write and read: " << duration.count() << " ms" << endl;



	start = std::chrono::system_clock::now();
	RunTest<lista_one_mutex>(thread_num);
	end = std::chrono::system_clock::now();
	duration = end - start;
	std::cout << "List with only one Mutex: " << duration.count() << " ms" << endl;


	return 0;
}
