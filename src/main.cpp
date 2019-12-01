#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <stdlib.h>
#include "site.h"
#include "cell.h"

using namespace std;

int main()
{
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	srand(now.time_since_epoch().count());
	system("ulimit -s unlimited");

	cell cell1;
	ifstream input;
	ofstream output;
	ofstream occ;

	int nx, ny, nz;
	int n_big, n_small;
	double r_big, r_small;
	double act_big, act_dis;
	double k_in, k_out;
	int n_single_set_run, n_tot_set_run;
	string tmp;

	input.open("diffuse.in");
	input>>nx>>ny>>nz;
	getline(input,tmp);
	input>>n_small>>r_small;
	getline(input,tmp);
	input>>n_big>>r_big;
	getline(input,tmp);
	input>>act_big>>act_dis;
	getline(input,tmp);
	input>>k_in>>k_out;
	getline(input,tmp);
	input>>n_single_set_run>>n_tot_set_run;
	getline(input,tmp);
	input.close();

	cell1.init(nx,ny,nz);
	cell1.gen_hole_rand(n_small, r_small);
	cell1.gen_hole_rand(n_big, r_big);
	cell1.activate_hole(act_big, act_dis);
	cell1.assign_edge();
	cell1.count_edge();
	output.open("diffuse.out");
	for(size_t t1=0; t1<n_tot_set_run; t1++)
	{
		cell1.hopping_run(k_in,k_out,n_single_set_run);
		output<<(t1+1)*n_single_set_run<<'\t'<<cell1.count_fill()<<endl;
	}
	output.close();
	occ.open("filled.out");
	cell1.print_fill(occ);
	occ.close();
	return 0;
}
