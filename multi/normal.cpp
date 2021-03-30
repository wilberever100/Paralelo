#include <iostream>
#include <random>

using namespace std;

int m = 4, n = 600, p = 4;


void normal_mult(int **matA, int **matB, int **matC)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matC[i][j] = 0.0;
	for (int i = 0; i < m; ++i)
		for (int j = 0; j < p; ++j)
			for (int k = 0; k < n; ++k)
	{
				matC[i][j] += matA[i][k] * matB[k][j];
	}
}
void print_matrix(int **matX)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < p; j++)
		{
			cout << matX[i][j] << " ";
		}
		cout << endl;
	}
}
	int main(int argc, char *argv[])
	{
		m=n;
		p=n;
		
		//Creating Matrices
		int **matA = new int *[m];
		for (int i = 0; i < m; ++i)
			matA[i] = new int[n];
		
		int **matB = new int *[n];
		for (int i = 0; i < n; ++i)
			matB[i] = new int[p];
		
		int **matC = new int *[m];
		for (int i = 0; i < m; ++i)
			matC[i] = new int[p];
		
		//random
		static std::uniform_int_distribution<int> sample_01_(0, 100);
		std::random_device rd;
		std::mt19937 rng(rd());
		
		//fill data Matrices
		for (int i = 0; i < m; i++)
		{
			
			for (int j = 0; j < n; j++)
			{
				matA[i][j] = sample_01_(rng);
			}
		}
		for (int i = 0; i < n; i++)
		{
			
			for (int j = 0; j < p; j++)
			{
				matB[i][j] = sample_01_(rng);
			}
		}
		for (int i = 0; i < m; i++)
		{
			
			for (int j = 0; j < p; j++)
			{
				matC[i][j] = 0;
			}
		}
		
		//Multiplication
		
		
		cout << endl;
		normal_mult(matA, matB, matC);
		//print_matrix(matC);
		cout << endl;
		
		return 0;
	}
	
	
