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
	std::vector<std::vector<int> > real_hole_center;	// ............ after merge
	int temp_hole_index;
public:
	void init(int Nx, int Ny, int Nz);
	void gen_hole(int ix, int iy, int iz, double rx, double ry, double rz, int if_initiate);
	void gen_hole_auto();	// to be done, need to store contrl parameters in the class
	void assign_hole_index(int ix, int iy, int iz, int index);
	void recover_hole_index(int ix, int iy, int iz);
	void hole_print(int ix, int iy, int iz, int if_initiate);
};
#endif
