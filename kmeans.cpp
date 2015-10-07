#include<iostream>
#include<cmath>
#include<fstream>
#include<vector>
#include<sstream>
#include<cstdlib>
#include<limits>
#include<algorithm>
#include<cfloat>
using namespace std;

#define N_CLUSTERS 25	// No. of clusters
#define RAND_SEED 696716708		// Seed for random generator
#define N_POINTS 40000
#define MAX_ITER 20

typedef struct {
	long double x, y;
} Point;

void get_data(vector<vector<long double> > &data)
{
	data.resize(N_POINTS);
	ifstream file("/root/code/intel/data/data.csv");
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
	//cout<<k<<endl;
	file.close();
}

void write_to_csv(vector<int> &labels, string file_name)
{
	ofstream fout(file_name.c_str());

	for(int i=0; i<labels.size(); ++i) {
		fout<<labels[i]<<",";
	}

	fout.close();	
}

// Might need to check :P
long double dist_euc (vector<long double> a,
					  vector<long double> b)
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

int min_dist_centroid_index(vector<vector<long double> > &centroids,
					  		vector<long double> &point)
{
	int c_ind = 0;
	long double dist, min_dist = LDBL_MAX;

	for(int i=0; i<centroids.size(); ++i) {
		dist = dist_euc(centroids[i], point);

		if(dist < min_dist) {
			min_dist = dist;
			c_ind = i;
		}
	}

	return c_ind;
}
	
void update_cluster_centroids(vector<vector<long double> > &data,
							  vector<vector<long double> > &centroids,
							  vector<int> &labels)
{
	int K = centroids.size();
	int M = centroids[0].size();

	vector<long double> zero(M, 0.0);
	vector<int> counts(K, 0);

	for(int i=0; i<K; ++i) {
		centroids[i] = zero;
	}

	for(int i=0; i<N_POINTS; ++i) {
		for(int j=0; j<M; ++j) {
			centroids[labels[i]][j] += data[i][j];
		}
		counts[labels[i]]++;
	}

	for(int i=0; i<K; ++i) {
		for(int j=0; j<M; ++j) {
			centroids[i][j] /= (1.0 * counts[i]);
		}
	}
}


void cluster(vector<vector<long double> > &data,
			 vector<int> &labels, int K)
{
	vector<vector<long double> > centroids(K);

	// Initialize centroids by randomly selecting values
	srand(RAND_SEED);				// FIXED SEED
	vector<int> r;
	for(int i=0; i<N_POINTS; ++i)
		r.push_back(i);
	
	random_shuffle(r.begin(), r.end());
	for(int i=0; i<K; ++i) {
		centroids[i] = data[r[i]];
	}
    
	//for(int k=0; k<centroids[0].size(); ++k)
	//	cout<<centroids[0][k]<<"\t"<<data[r[0]][k]<<endl;
    
	// Stopping Criterion: less than x% change in cluster ass.
	// AND max_iters
    
	for(int i=0; i<MAX_ITER; ++i) {
    
		cout<<i<<endl;
		// Update centroids by finding min distance centroid
		for(int ind=0; ind<N_POINTS; ++ind) {
			labels[ind] = min_dist_centroid_index(centroids, data[ind]);
		}
    
		// Update centroids by finding mean of points in cluster
		update_cluster_centroids(data, centroids, labels);
	}
}

int main ()
{
	vector<vector<long double> > data;
	cout.precision(100);

	//cout<<"hey1\n";
	get_data(data);
	vector<int> labels(data.size(), 0);
	//cout<<"hey2\n";

	cluster(data, labels, N_CLUSTERS);
	//cout<<"hey3\n";

	write_to_csv(labels, "output.csv");
	//cout<<"hey4\n";
	return 0;
}
