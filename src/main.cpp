#include <iostream>
#include <stdlib.h>
#include <chrono>
#include "site.h"
#include "cell.h"

using namespace std;

int main()
{
	chrono::high_resolution_clock::time_point now = chrono::high_resolution_clock::now();
	srand(now.time_since_epoch().count());
	cell a;
	a.init(101,101,101);
	a.gen_hole_rand(8000,5.01);
	a.activate_hole(10,6.0);
	a.assign_edge();
	a.count_edge();
	for(size_t t1=0; t1<100000; t1++)
	{
		a.hopping_run(1.0,0.0,1000);
		cout<<t1<<'\t'<<a.count_fill()<<endl;
	}
	return 0;
}
