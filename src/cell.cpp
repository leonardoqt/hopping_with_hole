#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "cell.h"

using namespace std;

double dis(vector<int>A,vector<int>B)
{
	double dd=0;
	int num=min(A.size(),B.size());
	for(size_t t1=0; t1<num; t1++)
		dd+=pow(B[t1]-A[t1],2);
	return sqrt(dd);
}

void cell :: init(int Nx, int Ny, int Nz)
{
	site tmp;
	nx=Nx; ny=Ny; nz=Nz;
	sites.resize(nx);
	for(auto& m1:sites)
	{
		m1.resize(ny);
		for(auto& m2:m1)
		{
			m2.resize(nz);
			for(auto& m3:m2)
				m3 = tmp;
		}
	}
	num_sites = nx*ny*nz;
	num_hole = 0;
	num_edge = 0;
	hole_center.resize(num_sites);
	edges.resize(num_sites);
	hole_radius.resize(num_sites);
	for(size_t t1=0; t1<num_sites; t1++)
	{
		hole_center[t1].resize(3);
		edges[t1].resize(3);
	}
}

void cell :: gen_hole(int ix, int iy, int iz, double rx, double ry, double rz, int if_initiate)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(!sites[ix][iy][iz].if_hole)
	{
		if(if_initiate)
		{
			hole_center[num_hole][0] = ix;
			hole_center[num_hole][1] = iy;
			hole_center[num_hole][2] = iz;
			num_hole++;
		}
		sites[ix][iy][iz].if_hole = 1;
		if(rand()/(double)RAND_MAX < rx)
			gen_hole(ix-1, iy, iz, rx, ry, rz,0);
		if(rand()/(double)RAND_MAX < rx)
			gen_hole(ix+1, iy, iz, rx, ry, rz,0);
		if(rand()/(double)RAND_MAX < ry)
			gen_hole(ix, iy-1, iz, rx, ry, rz,0);
		if(rand()/(double)RAND_MAX < ry)
			gen_hole(ix, iy+1, iz, rx, ry, rz,0);
		if(rand()/(double)RAND_MAX < rz)
			gen_hole(ix, iy, iz-1, rx, ry, rz,0);
		if(rand()/(double)RAND_MAX < rz)
			gen_hole(ix, iy, iz+1, rx, ry, rz,0);
	}
}

void cell :: gen_hole_sphere(int ix, int iy, int iz, double rr)
{
	int x_min = max(0.0, ix-ceil(rr));
	int y_min = max(0.0, iy-ceil(rr));
	int z_min = max(0.0, iz-ceil(rr));
	int x_max = min(ix+ceil(rr)+1, (double)nx);
	int y_max = min(iy+ceil(rr)+1, (double)ny);
	int z_max = min(iz+ceil(rr)+1, (double)nz);

	hole_center[num_hole][0] = ix;
	hole_center[num_hole][1] = iy;
	hole_center[num_hole][2] = iz;
	hole_radius[num_hole] = rr;
	num_hole++;

	for(size_t t1=x_min; t1<x_max; t1++)
	for(size_t t2=y_min; t2<y_max; t2++)
	for(size_t t3=z_min; t3<z_max; t3++)
	{
		if((t1-ix)*(t1-ix)+(t2-iy)*(t2-iy)+(t3-iz)*(t3-iz) < rr*rr && !sites[t1][t2][t3].if_hole)
			sites[t1][t2][t3].if_hole = 1;
	}
}

void cell :: clear_hole_iter(int ix, int iy, int iz)
{	
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole && sites[ix][iy][iz].if_iter)
	{
		sites[ix][iy][iz].if_iter = 0;
		clear_hole_iter(ix-1,iy,iz);
		clear_hole_iter(ix+1,iy,iz);
		clear_hole_iter(ix,iy-1,iz);
		clear_hole_iter(ix,iy+1,iz);
		clear_hole_iter(ix,iy,iz-1);
		clear_hole_iter(ix,iy,iz+1);
	}
}

void cell :: clear_hole_iter()
{
	for(size_t t1=0; t1<num_hole; t1++)
		clear_hole_iter(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
}

void cell :: activate_hole(double rmin, double dmin)
{
	// clear active sites
	for(size_t t1=0; t1<nx; t1++)
	for(size_t t2=0; t2<ny; t2++)
	for(size_t t3=0; t3<nz; t3++)
		sites[t1][t2][t3].if_hole_active = 0;
	
	// activate sites
	for(size_t t1=0; t1<num_hole; t1++)
	{
		// big holes
		if(hole_radius[t1] >= rmin)
		{
			activate_hole(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
		}
		// sites close to wall
		else if(hole_center[t1][0]<=dmin || hole_center[t1][1]<=dmin || hole_center[t1][2]<=dmin || hole_center[t1][0]>=nx-1-dmin || hole_center[t1][1]>=ny-1-dmin || hole_center[t1][2]>=nz-1-dmin)
		{
			activate_hole(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
		}
		else
		{
			// sites close to big hole
			for(size_t t2=0; t2<num_hole;t2++)
			{
				if(hole_radius[t2] >= rmin && dis(hole_center[t1],hole_center[t2]) <= hole_radius[t2] + dmin)
				{
					activate_hole(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
					break;
				}
			}
		}
	}
}

void cell :: activate_hole(int ix, int iy, int iz)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole && !sites[ix][iy][iz].if_hole_active)
	{
		sites[ix][iy][iz].if_hole_active = 1;
		activate_hole(ix-1,iy,iz);
		activate_hole(ix+1,iy,iz);
		activate_hole(ix,iy-1,iz);
		activate_hole(ix,iy+1,iz);
		activate_hole(ix,iy,iz-1);
		activate_hole(ix,iy,iz+1);
	}
}

void cell :: assign_edge(int ix, int iy, int iz)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole_active && !sites[ix][iy][iz].if_iter)
	{
		sites[ix][iy][iz].if_iter = 1;
		if(ix>=1 && iy>=1 && iz>=1 && ix<nx-1 && iy<ny-1 && iz<nz-1)
		if(!sites[ix-1][iy][iz].if_hole_active || !sites[ix+1][iy][iz].if_hole_active || !sites[ix][iy-1][iz].if_hole_active || !sites[ix][iy+1][iz].if_hole_active || !sites[ix][iy][iz-1].if_hole_active || !sites[ix][iy][iz+1].if_hole_active)
		{
			sites[ix][iy][iz].if_edge = 1;
		}
		assign_edge(ix-1,iy,iz);
		assign_edge(ix+1,iy,iz);
		assign_edge(ix,iy-1,iz);
		assign_edge(ix,iy+1,iz);
		assign_edge(ix,iy,iz-1);
		assign_edge(ix,iy,iz+1);
	}
	
}

void cell :: assign_edge()
{
	// clear edge info
	for(size_t t1=0; t1<nx; t1++)
	for(size_t t2=0; t2<ny; t2++)
	for(size_t t3=0; t3<nz; t3++)
		sites[t1][t2][t3].if_edge = 0;
	
	// clear if_iter info
	clear_hole_iter();

	// assign edge from hole
	for(size_t t1=0; t1<num_hole; t1++)
	{
		assign_edge(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
	}
	// assign edge from wall
	for(size_t t1=0; t1<nx; t1++)
	for(size_t t2=0; t2<ny; t2++)
	{
		if(!sites[t1][t2][0].if_hole)
			sites[t1][t2][0].if_edge = 1;
		if(!sites[t1][t2][nz-1].if_hole)
			sites[t1][t2][nz-1].if_edge = 1;
	}
	for(size_t t1=0; t1<nx; t1++)
	for(size_t t3=0; t3<nz; t3++)
	{
		if(!sites[t1][0][t3].if_hole)
			sites[t1][0][t3].if_edge = 1;
		if(!sites[t1][ny-1][t3].if_hole)
			sites[t1][ny-1][t3].if_edge = 1;
	}
	for(size_t t2=0; t2<ny; t2++)
	for(size_t t3=0; t3<nz; t3++)
	{
		if(!sites[0][t2][t3].if_hole)
			sites[0][t2][t3].if_edge = 1;
		if(!sites[nx-1][t2][t3].if_hole)
			sites[nx-1][t2][t3].if_edge = 1;
	}
	clear_hole_iter();
}

void cell :: count_edge()
{
	num_edge = 0;
	for(size_t t1=0; t1<nx; t1++)
	for(size_t t2=0; t2<ny; t2++)
	for(size_t t3=0; t3<nz; t3++)
	if(sites[t1][t2][t3].if_edge)
	{	
		edges[num_edge][0] = t1;
		edges[num_edge][1] = t2;
		edges[num_edge][2] = t3;
		num_edge++;
	}
}

void cell :: print_edge()
{
	for(size_t t1=0; t1<num_edge; t1++)
	{
		cout<<"He "<<edges[t1][0]<<'\t'<<edges[t1][1]<<'\t'<<edges[t1][2]<<endl;
	}
}

void cell :: print_hole(int ix, int iy, int iz)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole && !sites[ix][iy][iz].if_iter)
	{
		cout<<"He "<<ix<<'\t'<<iy<<'\t'<<iz<<endl;
		sites[ix][iy][iz].if_iter = 1;
		print_hole(ix-1,iy,iz);
		print_hole(ix+1,iy,iz);
		print_hole(ix,iy-1,iz);
		print_hole(ix,iy+1,iz);
		print_hole(ix,iy,iz-1);
		print_hole(ix,iy,iz+1);
	}
}

void cell :: print_hole()
{
	for(size_t t1=0; t1<num_hole; t1++)
		print_hole(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
	clear_hole_iter();
}
