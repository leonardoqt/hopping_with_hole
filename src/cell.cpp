#include <iostream>
#include <stdlib.h>
#include "cell.h"

using namespace std;

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
	real_num_hole = 0;
	hole_center.resize(num_sites);
	for(auto& m1:hole_center)
		m1.resize(3);
	real_hole_center.resize(0);
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

void cell :: assign_hole_index(int ix, int iy, int iz, int index)
{	
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole && sites[ix][iy][iz].hole_index != index)
	{
		sites[ix][iy][iz].hole_index = index;
		assign_hole_index(ix-1,iy,iz,index);
		assign_hole_index(ix+1,iy,iz,index);
		assign_hole_index(ix,iy-1,iz,index);
		assign_hole_index(ix,iy+1,iz,index);
		assign_hole_index(ix,iy,iz-1,index);
		assign_hole_index(ix,iy,iz+1,index);
	}
}

void cell :: recover_hole_index(int ix, int iy, int iz)
{
	assign_hole_index(ix,iy,iz,temp_hole_index-1);
}

void cell :: hole_print(int ix, int iy, int iz, int if_initiate)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	{
	if(if_initiate)
	{
		temp_hole_index = sites[ix][iy][iz].hole_index+1;
	}
	if(sites[ix][iy][iz].if_hole && sites[ix][iy][iz].hole_index != temp_hole_index)
	{
		sites[ix][iy][iz].hole_index = temp_hole_index;
		cout<<ix<<'\t'<<iy<<'\t'<<iz<<endl;
		hole_print(ix-1,iy,iz,0);
		hole_print(ix+1,iy,iz,0);
		hole_print(ix,iy-1,iz,0);
		hole_print(ix,iy+1,iz,0);
		hole_print(ix,iy,iz-1,0);
		hole_print(ix,iy,iz+1,0);
	}
	}
}
