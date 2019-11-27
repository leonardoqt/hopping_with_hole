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
	real_num_hole = 0;
	hole_center.resize(num_sites);
	hole_radius.resize(num_sites);
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

void cell :: gen_hole_smooth(int ix, int iy, int iz, double rr)
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
			activate_site(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
		}
		// sites close to wall
		else if(hole_center[t1][0]<=dmin || hole_center[t1][1]<=dmin || hole_center[t1][2]<=dmin || hole_center[t1][0]>=nx-1-dmin || hole_center[t1][1]>=ny-1-dmin || hole_center[t1][2]>=nz-1-dmin)
		{
			activate_site(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
		}
		else
		{
			// sites close to big hole
			for(size_t t2=0; t2<num_hole;t2++)
			{
				if(hole_radius[t2] >= rmin && dis(hole_center[t1],hole_center[t2]) <= hole_radius[t2] + dmin)
				{
					activate_site(hole_center[t1][0],hole_center[t1][1],hole_center[t1][2]);
					break;
				}
			}
		}
	}
}

void cell :: activate_site(int ix, int iy, int iz)
{
	if(ix>=0 && iy>=0 && iz>=0 && ix<nx && iy<ny && iz<nz)
	if(sites[ix][iy][iz].if_hole && !sites[ix][iy][iz].if_hole_active)
	{
		sites[ix][iy][iz].if_hole_active = 1;
//		cout<<"He "<<ix<<'\t'<<iy<<'\t'<<iz<<endl;
		activate_site(ix-1,iy,iz);
		activate_site(ix+1,iy,iz);
		activate_site(ix,iy-1,iz);
		activate_site(ix,iy+1,iz);
		activate_site(ix,iy,iz-1);
		activate_site(ix,iy,iz+1);
	}
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
