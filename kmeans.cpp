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

#define N_CLUSTERS 25			// No. of clusters
#define RAND_SEED 696716708		// Seed for random generator
#define N_POINTS 40000
#define MAX_ITER 100
#define MIN_CHANGE 0.005		// Less than 0.5% change => Terminate

#define DATA_FILE "data.csv"
#define OUTPUT_FILE "kcm_phase1.csv"

/* Read data from the file
 */
void get_data(vector<vector<long double> > &data)
{
	data.resize(N_POINTS);
	ifstream file(DATA_FILE);
	string line, word;

	stringstream cell;
	long double temp;

	int k =0;
	while(!file.eof()) {
		getline(file, line);
		if(line.compare("") == 0)
			continue;
		std:stringstream line_stream(line);

		while(getline(line_stream, word, ',')) {
			temp = stold(word.c_str());
			data[k].push_back(temp);
		}

		k++;
	}
	file.close();
}

/* Write output to csv file
 */
void write_to_csv(vector<int> &labels, string file_name)
{
	ofstream fout(file_name.c_str());

	for(int i=0; i<labels.size() - 1; ++i) {
		/* Start labels with 1 */
		fout<<labels[i] + 1<<" ,";
	}
	fout<<labels[labels.size()-1];

	fout.close();	
}

/* Return euclidean distance between two points
 */
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

/* returns the index of the nearest centroid */
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

/* Updates cluster centroids based on distance
 */
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

/* Returns the percentage of different values
 */
double get_labels_change_percent(vector<int> &new_labels,
								 vector<int> &old_labels)
{
	int N = new_labels.size();

	double count=0;
	for(int i=0; i<N; ++i) {
		if(new_labels[i] != old_labels[i])
			count++;
	}

	return count / N;
}

/* K-Means clustering
 * Serial implementation
 */
void cluster(vector<vector<long double> > &data,
			 vector<int> &labels, int K)
{
	vector<vector<long double> > centroids(K);

	/* Initialize centroids by randomly selecting values */
	srand(RAND_SEED);					  /* FIXED SEED */
	vector<int> r;
	for(int i=0; i<N_POINTS; ++i)
		r.push_back(i);
	
	random_shuffle(r.begin(), r.end());
	for(int i=0; i<K; ++i) {
		centroids[i] = data[r[i]];
	}

	/* Store last labeling to compute change */
	vector<int> old_labels(labels.size(), 0);
    
	for(int i=0; i<MAX_ITER; ++i) {
    
		/* Update labels by finding min distance centroid */
		for(int ind=0; ind<N_POINTS; ++ind) {
			labels[ind] = min_dist_centroid_index(centroids, data[ind]);
		}

		/* Stopping Criterion: less than MIN_CHANGE change in cluster ass */
		double change_percent = get_labels_change_percent(labels, old_labels);
		//cout<<i<<" "<<change_percent<<endl;
		cout<<"Iteration "<<i<<endl;
		if(change_percent < MIN_CHANGE)
			break;

		old_labels = labels;

		/* Update centroids by finding mean of points in cluster */
		update_cluster_centroids(data, centroids, labels);
	}
}

int main ()
{
	vector<vector<long double> > data;
	cout<<"Getting the data..\n";

	get_data(data);

	cout<<"Clustering the data..\n";
	vector<int> labels(data.size(), 0);
	cluster(data, labels, N_CLUSTERS);

	write_to_csv(labels, OUTPUT_FILE);
	cout<<"Done!\n";

	return 0;
}
