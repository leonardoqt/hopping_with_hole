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
	a.init(21,21,21);
	a.gen_hole_sphere(10,10,0,4.6);
	a.gen_hole_sphere(10,10,20,2.3);
	a.activate_hole(2.4,1.3);
	a.assign_edge();
	a.count_edge();
	a.print_edge();
	return 0;
}
