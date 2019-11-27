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
	a.gen_hole_smooth(50,50,50,2.6);
	a.gen_hole_smooth(50,50,56,2.3);
	a.activate_hole(2.4,3.3);
	return 0;
}
