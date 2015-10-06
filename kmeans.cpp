#include<iostream>
#include<cmath>
#include<fstream>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<limits>
using namespace std;

#define N_CLUSTERS 25	// No. of clusters
#define RAND_SEED 1		// Seed for random generator
#define N 40000
#define MAX_ITER 20

//typedef vector<long double> Point;
//typedef vector<Point> Points;
typedef struct {
	long double x, y;
} Point;


// Might need to check :P
long double dist_euc (vector<double> a,
					  vector<double> b)
{
	if (a.size() != b.size()) {
		cout<<"Shape mismatch!\n";
		return 0;
	}

	long double dist = 0, temp;
	for(int i=0; i<a.size(); ++i) {
		temp = a[i] - b[i];
		dist += (temp * temp);
	}

	return dist;
}

void cluster(vector<vector<double> > &data,
			 vector<vector<double> > &labels, int K)
{
	vector<vector<double> > centroids(K);

	// Initialize centroids by randomly selecting values

	// Stopping Criterion: less than x% change in cluster ass.
	// AND max_iters
	// Right now just like this :P


}

void get_data(vector<vector<long double> > &data)
{
	data.resize(N);
	ifstream file("/root/code/intel/data.csv");
	string line, word;

	stringstream cell;
	long double temp;

	int k =0;
	while(!file.eof()) {
	//{
		getline(file, line);
		if(line.compare("") == 0)
			continue;
		std:stringstream line_stream(line);

		while(getline(line_stream, word, ',')) {
			temp = stold(word.c_str());
			data[k].push_back(temp);
			//cout<<word<<" "<<temp<<endl;
		}

		//cout<<data[k].size()<<endl;
		k++;

	}
	cout<<k<<endl;
	file.close();
}

void write_to_csv(vector<int> &labels)
{
	ofstream fout("/root/code/intel/clabels.csv");

	for(int i=0; i<labels.size(); ++i) {
		fout<<labels[i]<<",";
	}

	fout.close();	
}

int main ()
{
	//Points data, centroids;
	vector<vector<long double> > data;
	cout.precision(100);

	get_data(data);
	vector<int> labels(data.size(), 0);

	//centroids = cluster(data, N_CLUSTERS);

	write_to_csv(labels);
	//cout<<centroids.size()<<endl;
	return 0;
}
