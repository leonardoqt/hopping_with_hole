#ifndef __CELL_FOR_CELL_WITH_HOLLOW__
#define __CELL_FOR_CELL_WITH_HOLLOW__
#include <vector>
#include "site.h"

class cell;

// hole growth can be defined as growth from center to three directions
// the rate can be different
class cell
{
private:
	int nx, ny, nz;										// size of the bulk in three dimensions
	std::vector<std::vector<std::vector<site> > > sites;// the material
	int num_sites;										// number of sites
	int num_hole, real_num_hole;						// number of hole before/after merge
	std::vector<std::vector<int> > hole_center;			// hole centers before merge
	std::vector<double> hole_radius;					// radius of the ith hole
	std::vector<std::vector<int> > real_hole_center;	// ............ after merge
	int temp_hole_index;
public:
	void init(int Nx, int Ny, int Nz);
	void gen_hole(int ix, int iy, int iz, double rx, double ry, double rz, int if_initiate);	// use self-growth, i is the center, r is the growth rate
	void gen_hole_auto();	// to be done, need to store contrl parameters in the class
	void gen_hole_smooth(int ix, int iy, int iz, double rr);	// use radius to control size
	void assign_hole_index(int ix, int iy, int iz, int index);
	void recover_hole_index(int ix, int iy, int iz);
	void activate_hole(double rmin, double dmin);	// make holes activated, if either the radius is greater than rmin or the center is close to surface or big holes
	void activate_site(int ix, int iy, int iz);		// auxiliary function
	void assign_edge();		// find the edge sites
	void hole_print(int ix, int iy, int iz, int if_initiate);
};
#endif
