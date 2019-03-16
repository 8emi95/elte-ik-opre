#include "Header.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <string>
#include <future> // ASYNC, FUTURE

std::array<int, 4> MatrixMultiplication(const std::array<std::array<int, 4>, 4>& matrix, const std::array<int, 4>& vector)
{
	std::array<int, 4> result;

	for (int i = 0; i < 4; i++)
	{
		result[i] = 0;
		for (int j = 0; j < 4; j++)
		{
			result[i] += matrix[i][j] * vector[j];
		}
	}

	return result;
}

void MatrixThread(const int& N, const std::array<std::array<int, 4>, 4>& matrix, 
				  Pipeline<std::array<int, 4> >& prePipe, Pipeline<std::array<int, 4> >& postPipe)
{
	for (int i = 0; i < N; i++)
	{
		postPipe.push(MatrixMultiplication(matrix, prePipe.pop()));
	}
}

int main()
{
	std::ifstream input("input_matrices.txt");
	unsigned int M = 0;
	input >> M;

	std::vector<std::array<std::array<int, 4>, 4> > matrices(M);
	int k;
	for (unsigned int i = 0; i < M; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				input >> matrices[i][j][k];
			}
		}
	}
	input.close();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cout << matrices[0][i][j] << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::ifstream input2("input_points.txt");
	unsigned int N = 0;
	input2 >> N;

	std::vector<std::array<int, 4> > vectors(N);
	for (unsigned int i = 0; i < N; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			input2 >> vectors[i][j];
		}
		vectors[i][3] = 1;
	}
	input2.close();

	for (int i = 0; i < 4; i++)
	{
		std::cout << vectors[0][i] << " ";
	}
	std::cout << std::endl;

	std::vector<Pipeline<std::array<int, 4> > > pipelines(M+1);

	for (unsigned int i = 0; i < N; i++)
	{
		pipelines[0].push(vectors[i]);
	}

	for (unsigned int i = 0; i < M; i++)
	{
		std::thread myThread(MatrixThread, N, std::ref(matrices[i]), std::ref(pipelines[i]), std::ref(pipelines[i + 1]));
		myThread.detach();
	}

	std::ofstream output("output.txt");

	for (int i = 0; i < N; i++)
	{
		std::array<int, 4> act = pipelines[M].pop();
		output << act[0] << " " << act[1] << " " << act[2] << std::endl;
	}

	output.close();
}