#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <stdlib.h>
#include <math.h>
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
	ofstream heatmap;

	int nx, ny, nz;
	int n_big, n_small;
	double r_big, r_small;
	double act_big, act_dis;
	double k_in, k_out;
	int n_single_set_run, n_tot_set_run;
	double rate_single_set_run;
	int if_heatmap;
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
	input>>rate_single_set_run>>n_tot_set_run;
	getline(input,tmp);
	input>>if_heatmap;
	getline(input,tmp);
	input.close();

	cell1.init(nx,ny,nz);
	cell1.gen_hole_rand(n_small, r_small);
	cell1.gen_hole_rand(n_big, r_big);
	cell1.activate_hole(act_big, act_dis);
	cell1.assign_edge();
	cell1.count_edge();
	cell1.count_capacity();
	cout<<"Average hole size: "<<cell1.count_average_hole_size()<<endl;
	cout<<"Capacity: "<<cell1.return_capacity()<<endl;
	cout<<"Number of interface sites: "<<cell1.return_edge()<<endl;
	output.open("diffuse.out");
	if(if_heatmap)
		heatmap.open("heatmap.dat");
	cout<<'['<<0<<'/'<<n_tot_set_run<<']'<<endl;
	for(size_t t1=0; t1<n_tot_set_run; t1++)
	{
		n_single_set_run = ceil((cell1.return_fill()+cell1.return_edge())*rate_single_set_run);
		cell1.hopping_run(k_in,k_out,n_single_set_run);
		//output<<(t1+1)*n_single_set_run<<'\t'<<cell1.return_fill_percent()<<endl;
		output<<(t1+1)<<'\t'<<cell1.return_fill_percent()<<endl;
		if(if_heatmap)
			cell1.print_heatmap_z_half(heatmap);
		cout<<"\e[A"<<'['<<t1+1<<'/'<<n_tot_set_run<<']'<<endl;
	}
	cout<<endl;
	output.close();
	if(if_heatmap)
		heatmap.close();
	occ.open("filled.out");
	cell1.print_fill(occ);
	occ.close();
	return 0;
}
