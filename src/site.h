#ifndef __SITE_FOR_CELL_WITH_HOLE__
#define __SITE_FOR_CELL_WITH_HOLE__
class site;

class site
{
public:
	int hole_size;				// zero means the bulk
	int hole_index;				// site with the same hole index are in the same hole
	int if_hole;				// if belongs to a hole
	int if_hole_active;			// if a hole is active
	int if_edge;				// edge serve as source and sink
	int if_fill;				// if already filled with ions
	int num_filled_neighbor;	// how many neighbors are filled

	site();
};
#endif
