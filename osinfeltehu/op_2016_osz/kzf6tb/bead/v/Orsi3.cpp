#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <deque>

template <typename T>
class Pipeline
{
private:
	std::queue<T> _q;
	std::mutex _m;
	std::condition_variable _cv;
	int counter;

public:
	Pipeline()
	{
		counter = 0;
	}
	void push(const T& d)
	{
		std::unique_lock<std::mutex> lk(_m);
		_q.push(d);
		_cv.notify_all();
	}

	T pop()
	{
		std::unique_lock<std::mutex> lk(_m);
		if (_q.empty())
		{
			_cv.wait(lk, [=] { return !_q.empty(); });
		}

		T data = _q.front();
		_q.pop();
		counter++;
		return data;
	}

	int getCounter()
	{
		return counter;
	}


};

std::vector<int> matrixTransform(const std::vector < std::vector < int> > &matrix, const std::vector<int> &vector)
{
	std::vector<int> resVector;

	for (int i = 0; i <4; ++i)
	{
		int element = 0;
		for (int j = 0; j < 4; ++j)
		{
			element += matrix.at(i).at(j) * vector.at(j);
		}
		resVector.push_back(element);
	}
	return(resVector);
}

void transmission(int where, std::deque < Pipeline< std::vector<int> > > &csatorna, const std::vector < std::vector < std::vector <int> > > &matrices, const int &N)
{

	while (csatorna[where].getCounter() < N)
	{
		csatorna[where + 1].push(matrixTransform(matrices.at(where), csatorna[where].pop()));
	}

}


int main(int argc, char* argv[])
{
	std::ifstream matrix_file;
	matrix_file.open("input_matrices.txt ");

	int M;

	matrix_file >> M;

	std::vector < std::vector < std::vector <int> > > matrices;

	for (int i = 0; i < M; i++)
	{
		std::vector < std::vector <int> > oneMatrix;
		for (int j = 0; j < 4; ++j)
		{
			std::vector<int> oneRow;
			for (int k = 0; k < 4; k++)
			{
				int element;
				matrix_file >> element;
				oneRow.push_back(element);
			}
			oneMatrix.push_back(oneRow);
		}
		matrices.push_back(oneMatrix);
	}
	matrix_file.close();

	std::ifstream vector_file;
	vector_file.open("input_points.txt ");

	int N;

	vector_file >> N;

	std::vector < std::vector <int> >  vectors;

	for (int i = 0; i < N; i++)
	{
		std::vector <int> oneVector;
		for (int j = 0; j < 3; ++j)
		{
			int element;
			vector_file >> element;
			oneVector.push_back(element);
		}
		oneVector.push_back(1);
		vectors.push_back(oneVector);
	}
	vector_file.close();

	std::deque < Pipeline< std::vector<int> > >  csatorna;

	for (int i = 0; i <= M; i++)
	{
		csatorna.emplace_back();
	}

	for (auto vec : vectors)
	{
		csatorna[0].push(vec);
	}

	std::vector< std::thread > szalak;
	for (int i = 0; i < M; ++i)
	{
		szalak.emplace_back(transmission, i, std::ref(csatorna), std::ref(matrices), N);
	}

	for (auto &it : szalak)
	{
		it.join();
	}

	std::ofstream output("output.txt");
	for (int i = 0; i < N; ++i)
	{
		std::vector <int> actVec = csatorna[M].pop();
		output << actVec[0] << " " << actVec[1] << " " << actVec[2] << std::endl;
	}
	output.close();

	return 0;
}

