#ifndef __SITE_FOR_CELL_WITH_HOLE__
#define __SITE_FOR_CELL_WITH_HOLE__
class site;

class site
{
public:
//	double hole_size;				// zero means the bulk
	int if_iter;				// if the hole is iterated
	int if_hole;				// if belongs to a hole
	int if_hole_active;			// if a hole is active
	int if_edge;				// edge serve as source and sink
	int if_fill;				// if already filled with ions
	int num_filled_neighbor;	// how many neighbors are filled

	site();
};
#endif
