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
	a.init(11,11,11);
	cout<<"finished init"<<endl;
	a.gen_hole(5,5,5,0.2,0.2,0.2,1);
	cout<<"finished gen hole"<<endl;
	a.assign_hole_index(5,5,5,1);
	cout<<"finished assign index"<<endl;
	a.hole_print(5,5,5,1);
	return 0;
}
