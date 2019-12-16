#ifndef __CELL_FOR_CELL_WITH_HOLLOW__
#define __CELL_FOR_CELL_WITH_HOLLOW__
#include <fstream>
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
	int num_fill;										// number of filled sites
	int num_hole;										// number of defined holes
	int num_edge;										// number of edge sites
	int num_capacity;									// number of total to-be-filled sites
	int num_disconnected_hole;							// number of disconnected holes
	int num_site_hole;									// number of sites that are holes
	std::vector<std::vector<int> > hole_center;			// hole centers before merge
	std::vector<std::vector<int> > edges;				// edge sites
	std::vector<std::vector<int> > fills;				// edge sites
	std::vector<double> hole_radius;					// radius of the ith hole
public:
	void init(int Nx, int Ny, int Nz);
	//
	void gen_hole(int ix, int iy, int iz, double rx, double ry, double rz, int if_initiate);	// use self-growth, i is the center, r is the growth rate
	void gen_hole_sphere(int ix, int iy, int iz, double rr);	// use radius to control size
	void gen_hole_rand(int num, double rr);	// to be done, need to store contrl parameters of number of holes, radius, etc... in the class
	void hole_iter_add_number_sites(int ix, int iy, int iz);
	void clear_hole_iter(int ix, int iy, int iz);
	void clear_hole_iter();
	void activate_hole(int ix, int iy, int iz);		// auxiliary function
	void activate_hole(double rmin, double dmin);	// make holes activated, if either the radius is greater than rmin or the center is close to surface or big holes
	void assign_edge(int ix, int iy, int iz);		// find the edge sites
	void assign_edge();								// find the edge sites, either is hole and are edge or is not hole but on the surface
	void assign_edge(double rmin, double dmin);		// assign edge by holes, not connected holes
	void assign_edge_holes(int index);				// assign edge to edge of a single hole
	//
	int  count_edge();
	int  count_capacity();
	double count_average_hole_size();
	//
	void hopping_run(double k_ad, double k_rm);
	void hopping_run(double k_ad, double k_rm, int num_run);
//	void print_fill();
	int  return_fill();
	double return_fill_percent();
	int return_capacity();
	int return_edge();
	void print_fill(std::ofstream& out);
	void print_heatmap(std::ofstream& out);
	void print_heatmap_z_half(std::ofstream& out);
	void print_edge();
	void print_hole(int ix, int iy, int iz);
	void print_hole();
};
#endif
