#include <math.h>
#include "opengl.h"
#include "main.h"
#include "project.h"
#include "cfd.h"

cfd_info_t ci;

//double precision Pi
#define C_PI			3.14159265358979323846

//max number of loops (to prevent infinit loop condition)
#define MAXTLOOPS		10000
#define MAXSLOOPS		10000
#define MAXPLOOPS		10000

/*
----------------
FinalProj_Analytical

- compute the analytical solution for validation
----------------
*/
void FinalProj_Analytical(int Ny)
{
	double dy, y;
	int i;

	//make these 16-byte aligned for SSE2 optimization
	if(!(ci.slnau = _malloc_aligned(Ny * sizeof(double), 16)))
		Sys_Log("Failed to allocate array slnau");

	dy = ci.h / (double)(Ny - 3);
	y = 0.0;

	ci.slnau[0] = ci.slnau[Ny - 1] = 0.0;
	for(i = 1; i < (Ny - 1); i++)
	{
		ci.slnau[i] = ci.Re * (-0.6) * ((y * y) - y  * ci.h);
		y += dy;
	}
}

/*
----------------
FinalProj_SIMPLE

- use Semi-Implicit Method for Pressure-Linked Equations
----------------
*/
void FinalProj_SIMPLE(int Nx, int Ny, double dt)
{
	double dx, dy, beta2, Cij;
	double pt0, pt, cpt0, cpt;		//for p' convergence
	double ps0, ps, cps0, cps;		//for p convergence
	double uss0, uss, cuss0, cuss;	//for u convergence to ss
	double vss0, vss, cvss0, cvss;	//for v convergence to ss
	double pss0, pss, cpss0, cpss;	//for p convergence to ss
	double uE, uP, un, us, ue, uw;
	double vN, vP, vn, vs, ve, vw;
	double clump1, clump2, clump3, clump4, clump5;
	int i, j, k = 0, l, m;
	double *pru, *prv, *prp;		//previous time step pointers
	double up, vp, *pp;				//correction factors
	int tc;
	tstep_t *ts, *temp;

	//NOTE: Nx and Ny include the ghost cells on every boundary

	dx = ci.L / (double)(Nx - 3);
	dy = ci.h / (double)(Ny - 3);
	beta2 = dx / dy;
	beta2 *= beta2;

	if(!(ci.slns = calloc(1, sizeof(tstep_t))))
		Sys_Error("Failed to allocate first time step");

	//make these 16-byte aligned for SSE2 optimization
	//only allocated once (temporary storage for each iteration)
	if(!(pp = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
		Sys_Log("Failed to allocate array pp");

	//allocated for first time step
	if(!(ci.slnsu = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
		Sys_Log("Failed to allocate array slnsu");

	if(!(ci.slnsv = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
		Sys_Log("Failed to allocate array slnsv");

	if(!(ci.slnsp = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
		Sys_Log("Failed to allocate array slnsp");

	ci.slns->u = ci.slnsu;
	ci.slns->v = ci.slnsv;
	ci.slns->p = ci.slnsp;
	ts = ci.slns;

	//initial conditions
	for(j = 0; j < Ny; j++)
	{
		for(i = 0; i < Nx; i++)
		{
			if(i == 0)
			{
				ci.slnsu[j * Nx] = 1.0;
				ci.slnsv[j * Nx] = 0.0;
			}
			else
			{
				ci.slnsu[j * Nx + i] = 0.0;
				ci.slnsv[j * Nx + i] = 0.0;
			}

			ci.slnsp[j * Nx + i] = 0.0;
		}
	}

	//time the computations (CPU cycles)
	StartTimer();

	//time loop
	for(k = 0; k < MAXTLOOPS; k++)	//iteration loop, exits when ||f - f0|| / ||f0|| is <= epsilon
	{
		cuss0 = cuss = 0.0;		//cumulative u values for the entire domain
		cvss0 = cvss = 0.0;		//cumulative v values for the entire domain
		cpss0 = cpss = 0.0;		//cumulative p values for the entire domain

		if(!(temp = calloc(1, sizeof(tstep_t))))
			Sys_Error("Failed to allocate next time step");

		pru = ci.slnsu;
		prv = ci.slnsv;
		prp = ci.slnsp;

		//make these 16-byte aligned for SSE2 optimization
		if(!(ci.slnsu = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
			Sys_Log("Failed to allocate array slnsu");

		if(!(ci.slnsv = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
			Sys_Log("Failed to allocate array slnsv");

		if(!(ci.slnsp = _malloc_aligned(Nx * Ny * sizeof(double), 16)))
			Sys_Log("Failed to allocate array slnsp");

		memcpy((void *)ci.slnsu, (void *)ts->u, Nx * Ny * sizeof(double));
		memcpy((void *)ci.slnsv, (void *)ts->v, Nx * Ny * sizeof(double));
		memcpy((void *)ci.slnsp, (void *)ts->p, Nx * Ny * sizeof(double));

		temp->u = ci.slnsu;
		temp->v = ci.slnsv;
		temp->p = ci.slnsp;

		ts->next = temp;
		temp->prev = ts;

		memset((void *)pp, 0, Nx * Ny * sizeof(double));

		//SIMPLE loop
		for(j = 0; j < MAXSLOOPS; j++)
		{
			cps0 = cps = 0.0;	//cumulative p values for the entire domain

			for(m = 1; m < (Ny - 1); m++)
			{
				for(l = 1; l < (Nx - 1); l++)
				{
					uE = (pru[m * Nx + l] + pru[m * Nx + (l + 1)]) / 2.0;
					uP = (pru[m * Nx + l] + pru[m * Nx + (l - 1)]) / 2.0;
					un = (pru[m * Nx + l] + pru[(m + 1) * Nx + l]) / 2.0;
					us = (pru[m * Nx + l] + pru[(m - 1) * Nx + l]) / 2.0;
					uw = (pru[m * Nx + (l - 1)] + pru[(m + 1) * Nx + (l - 1)]) / 2.0;
					ue = un;
					vN = (prv[m * Nx + l] + prv[(m + 1) * Nx + l]) / 2.0;
					vP = (prv[m * Nx + l] + prv[(m - 1) * Nx + l]) / 2.0;
					vn = (prv[m * Nx + l] + prv[m * Nx + (l + 1)]) / 2.0;
					vs = (prv[(m - 1) * Nx + l] + prv[(m - 1) * Nx + (l + 1)]) / 2.0;
					vw = (prv[m * Nx + l] + prv[m * Nx + (l - 1)]) / 2.0;
					ve = vn;

					//update u with new pressures
					clump1 = (((uE * uE) - (uP * uP)) * dt) / dx;
					clump2 = (((un * vn) - (us * vs)) * dt) / dy;
					clump3 = ((ci.slnsp[m * Nx + (l + 1)] - ci.slnsp[m * Nx + l]) * dt) / dx;
					clump4 = (pru[m * Nx + (l + 1)] - 2.0 * pru[m * Nx + l] + pru[m * Nx + (l - 1)]) / (dx * dx);
					clump5 = (pru[(m + 1) * Nx + l] - 2.0 * pru[m * Nx + l] + pru[(m - 1) * Nx + l]) / (dy * dy);
					ci.slnsu[m * Nx + l] = pru[m * Nx + l] - clump1 - clump2 - clump3 + (dt / ci.Re) * (clump4 + clump5);

					//update v with new pressures
					clump1 = (((ue * ve) - (uw * vw)) * dt) / dx;
					clump2 = (((vN * vN) - (vP * vP)) * dt) / dy;
					clump3 = ((ci.slnsp[(m + 1) * Nx + l] - ci.slnsp[m * Nx + l]) * dt) / dy;
					clump4 = (prv[m * Nx + (l + 1)] - 2.0 * prv[m * Nx + l] + prv[m * Nx + (l - 1)]) / (dx * dx);
					clump5 = (prv[(m + 1) * Nx + l] - 2.0 * prv[m * Nx + l] + prv[(m - 1) * Nx + l]) / (dy * dy);
					ci.slnsv[m * Nx + l] = prv[m * Nx + l] - clump1 - clump2 - clump3 + (dt / ci.Re) * (clump4 + clump5);
				}
			}

			//boundary conditions
			for(l = 1; l < (Ny - 1); l++)
			{
				ci.slnsu[l * Nx] = 1.0;
				ci.slnsu[(l + 1) * Nx - 2] = ci.slnsu[(l + 1) * Nx - 3];
				ci.slnsv[l * Nx] = -ci.slnsv[l * Nx + 1];
				ci.slnsv[(l + 1) * Nx - 2] = ci.slnsv[(l + 1) * Nx - 3];
			}

			for(l = 0; l < Nx; l++)
			{
				ci.slnsu[l] = -ci.slnsu[l + Nx];
				ci.slnsu[(Ny - 1) * Nx + l] = -ci.slnsu[(Ny - 2) * Nx + l];
				ci.slnsv[l] = 0;
				ci.slnsv[(Ny - 2) * Nx + l] = 0;
			}

			//PSOR
			for(i = 0; i < MAXPLOOPS; i++)
			{
				cpt0 = cpt = 0.0;	//cumulative p' values for the entire matrix

				for(m = 0; m < Ny; m++)
				{
					for(l = 0; l < Nx; l++)
					{
						if(l == 0)
						{
							//left BC
							pp[m * Nx] = pp[m * Nx + 1];
						}
						else if(l == (Nx - 1))
						{
							//right BC
							pp[m * Nx + l] = 2.0 * pp[m * Nx + (l - 1)] - pp[m * Nx + (l - 2)];
						}
						else if(m == 0)
						{
							//top BC
							pp[l] = pp[l + Nx];
						}
						else if(m == (Ny - 1))
						{
							//bottom BC
							pp[m * Nx + l] = pp[(m - 1) * Nx + l];
						}
						else
						{
							//Gauss-Seidel
							pt0 = pt = pp[m * Nx + l];
							Cij = ((ci.slnsu[m * Nx + l] - ci.slnsu[m * Nx + (l - 1)]) * (dx / dt)) + ((ci.slnsv[m * Nx + l] - ci.slnsv[(m - 1) * Nx + l]) * beta2 * (dy / dt));
							pt = ((1.0 - ci.oma) * pt) + (ci.oma / (2.0 * (1.0 + beta2)) * (pp[m * Nx + (l + 1)] + pp[m * Nx + (l - 1)] + beta2 * (pp[(m + 1) * Nx + l] + pp[(m - 1) * Nx + l]) - Cij));
							pp[m * Nx + l] = pt;
							cpt0 += (pt0 * pt0);
							cpt += ((pt - pt0) * (pt - pt0));
						}
					}
				}

				if(sqrt(cpt / cpt0) <= ci.ssep)
					break;		//convergence
			}

			//use p' to calculate p
			for(m = 0; m < Ny; m++)
			{
				for(l = 0; l < Nx; l++)
				{
					ps0 = ci.slnsp[m * Nx + l];
					ps = prp[m * Nx + l] + ci.omp * pp[m * Nx + l];
					ci.slnsp[m * Nx + l] = ps;
					cps0 += (ps0 * ps0);
					cps += ((ps - ps0) * (ps - ps0));
				}
			}

			if(sqrt(cps / cps0) <= ci.sses)
				break;		//convergence
		}

		for(m = 1; m < (Ny - 1); m++)
		{
			for(l = 1; l < (Nx - 1); l++)
			{
				pss0 = prp[m * Nx + l];
				uss0 = uss = ci.slnsu[m * Nx + l];
				vss0 = vss = ci.slnsv[m * Nx + l];
				up = (pp[m * Nx + l] - pp[m * Nx + (l + 1)]) * dt / dx;
				vp = (pp[m * Nx + l] - pp[(m + 1) * Nx + l]) * dt / dy;
				uss = uss + ci.omu * up;
				vss = vss + ci.omv * vp;
				ci.slnsu[m * Nx + l] = uss;
				ci.slnsv[m * Nx + l] = vss;
				pss = ci.slnsp[m * Nx + l];
				cuss0 += (uss0 * uss0);
				cuss += ((uss - uss0) * (uss - uss0));
				cvss0 += (vss0 * vss0);
				cvss += ((vss - vss0) * (vss - vss0));
				cpss0 += (pss0 * pss0);
				cpss += ((pss - pss0) * (pss - pss0));
			}
		}

		//boundary conditions
		for(l = 1; l < (Ny - 1); l++)
		{
			ci.slnsu[l * Nx] = 1.0;
			ci.slnsu[(l + 1) * Nx - 2] = ci.slnsu[(l + 1) * Nx - 3];
			ci.slnsv[l * Nx] = -ci.slnsv[l * Nx + 1];
			ci.slnsv[(l + 1) * Nx - 2] = ci.slnsv[(l + 1) * Nx - 3];
		}

		for(l = 0; l < Nx; l++)
		{
			ci.slnsu[l] = -ci.slnsu[l + Nx];
			ci.slnsu[(Ny - 1) * Nx + l] = -ci.slnsu[(Ny - 2) * Nx + l];
			ci.slnsv[l] = 0;
			ci.slnsv[(Ny - 2) * Nx + l] = 0;
		}

		if(sqrt(cuss / cuss0) <= ci.sset && sqrt(cvss / cvss0) <= ci.sset && sqrt(cpss / cpss0) <= ci.sset)
			break;		//steady-state reached

		ts = temp;
	}

	ci.sslns = temp;

	//grab the number of CPU cycles the computation took
	tc = StopTimer();

	//free temporary arrays
	_free_aligned(pp);

	Sys_Log("\n===================================");
	Sys_Log("SIMPLE");
	Sys_Log("%d Iterations Performed", k);
	Sys_Log("%d CPU Cycles Performed", tc);
	Sys_Log("%f CPU Time (Seconds)", (float)tc / 1400.0f / 1000000.0f);
	Sys_Log("===================================\n");
}

/*
----------------
FinalProject
----------------
*/
void FinalProject(void)
{
	//solution contains a linked list of time-steps, each containing velocity profiles
	FILE *fp;
//	double *mflow;
//	double dy;
	int i, j;

	//defaults
	scheme = 0;
	smoothing = 0;
//	ncx = numxc;
//	ncy = numyc;

	//Analytical Solution
	//------------------------------------

	FinalProj_Analytical(ci.numyc);

#pragma warning(push)
#pragma warning(disable: 4996)

	if(fp = fopen("results_analytical.txt", "w"))
	{
		fprintf(fp, "Ny = %d\n", ci.numyc);
		fprintf(fp, "deltay = %f\n", ci.h / (double)(ci.numyc - 3));

		fprintf(fp, "u\n");
		for(i = 0; i < ci.numyc; i++)
			fprintf(fp, "%.3f ", ci.slnau[i]);

		fclose(fp);
	}

#pragma warning(pop)

	//SIMPLE Solution
	//------------------------------------

	FinalProj_SIMPLE(ci.numxc, ci.numyc, ci.delt);

	//reprocess the data to get the interpolated velocities at the centers of the cells
/*	if(!(sslns->u = _malloc_aligned(numxc * numyc * sizeof(double), 16)))
		Sys_Log("Failed to allocate array u");

	if(!(sslns->v = _malloc_aligned(numxc * numyc * sizeof(double), 16)))
		Sys_Log("Failed to allocate array v");

	for(j = 0; j < numyc; j++)
	{
		for(i = 1; i < (numxc - 1); i++)
			sslns->u[j * numxc + i] = (slnsu[j * numxc + i] + slnsu[j * numxc + (i - 1)]) / 2.0;
	}

	for(j = 1; j < (numyc - 1); j++)
	{
		for(i = 0; i < numxc; i++)
			sslns->v[j * numxc + i] = (slnsv[j * numxc + i] + slnsv[(j - 1) * numxc + i]) / 2.0;
	}

	_free_aligned(slnsu);
	_free_aligned(slnsv);

	slnsu = sslns->u;
	slnsv = sslns->v;
*/
#pragma warning(push)
#pragma warning(disable: 4996)

/*	if(!(mflow = calloc(numxc - 2, sizeof(double))))
		Sys_Error("Failed to allocate array mflow");

	dy = h / (double)(numxc - 3);
	for(j = 1; j < (numyc - 1); j++)
	{
		for(i = 1; i < (numxc - 1); i++)
			mflow[i - 1] += (slnsu[j * numxc + i] + slnsu[j * numxc + (i + 1)]) * dy / 2.0;
	}

	fp = fopen("results_massflow.txt", "w");

	for(i = 0; i < numxc; i++)
		fprintf(fp, "%.3f ", mflow[i]);

	fclose(fp);
	free(mflow);
*/
	if(fp = fopen("results_simple.txt", "w"))
	{
		fprintf(fp, "Nx = %d, Ny = %d\n", ci.numxc, ci.numyc);
		fprintf(fp, "deltax = %f\n", ci.L / (double)(ci.numxc - 3));
		fprintf(fp, "deltay = %f\n", ci.h / (double)(ci.numyc - 3));

		fprintf(fp, "u\n");
		for(j = 0; j < ci.numyc; j++)
		{
			for(i = 0; i < ci.numxc; i++)
				fprintf(fp, "%.3f ", ci.slnsu[j * ci.numxc + i]);

			fprintf(fp, "\n");
		}

		fprintf(fp, "v\n");
		for(j = 0; j < ci.numyc; j++)
		{
			for(i = 0; i < ci.numxc; i++)
				fprintf(fp, "%.3f ", ci.slnsv[j * ci.numxc + i]);

			fprintf(fp, "\n");
		}

		fprintf(fp, "p\n");
		for(j = 0; j < ci.numyc; j++)
		{
			for(i = 0; i < ci.numxc; i++)
				fprintf(fp, "%.3f ", ci.slnsp[j * ci.numxc + i]);

			fprintf(fp, "\n");
		}

		fclose(fp);
	}

#pragma warning(pop)

	ci.curts = ci.sslns;
}

/*
----------------
FinalProj_Shutdown
----------------
*/
void FinalProj_Shutdown(void)
{
	tstep_t *tmp;

	//memory cleanup
	if(ci.slnau)
	{
		_free_aligned(ci.slnau);
		ci.slnau = NULL;
	}

	tmp = ci.slns;
	while(tmp)
	{
		_free_aligned(tmp->u);
		_free_aligned(tmp->v);
		_free_aligned(tmp->p);
		tmp = tmp->next;
		if(tmp)
			free(tmp->prev);
	}
}

/*
----------------
FinalProj_NextTS
----------------
*/
void FinalProj_NextTS(int time)
{
	float res;

	ci.elapsedtime += time;
	res = (float)ci.delt * 1000.0f * ci.animscale;
	if((float)ci.elapsedtime >= res)
		ci.elapsedtime = 0;
	else
		return;

	ci.curts = ci.curts->next;
	if(!ci.curts)
	{
		//go back to the beginning
		ci.curts = ci.slns;
		if(!ci.curts)
			return;
	}

	ci.slnsu = ci.curts->u;
	ci.slnsv = ci.curts->v;
	ci.slnsp = ci.curts->p;
}

/*
----------------
FinalProj_Render
----------------
*/
void FinalProj_Render(float wt, float ht)
{
	float x1, y1, x2, y2, dxx, dyy, x, y;
	float r, g, b, ma, mi, mh, v;
	int i, j;
	double *sln = NULL;

	switch(scheme)
	{
		case OUT_SIMPLE_SS_U:
			sln = ci.slnsu;
			break;

		case OUT_SIMPLE_SS_V:
			sln = ci.slnsv;
			break;

		case OUT_SIMPLE_SS_P:
			sln = ci.slnsp;
			break;

		case OUT_ANALYTICAL_U:
			sln = ci.slnau;
			break;
	}

	gglDisable(GL_DEPTH_TEST);
	gglDisable(GL_CULL_FACE);

	gglViewport(0, 0, (GLsizei)si.width, (GLsizei)si.height);
	gglMatrixMode(GL_PROJECTION);

	gglLoadIdentity();
	gglOrtho(0.0, (GLdouble)wt, (GLdouble)ht, 0.0, -1.0, 1.0);

	gglMatrixMode(GL_MODELVIEW);
	gglLoadIdentity();

	gglClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	gglClear(GL_COLOR_BUFFER_BIT);

	//-----------------------------------------------------------------

	if(smoothing)
		gglShadeModel(GL_SMOOTH);
	else
		gglShadeModel(GL_FLAT);

	gglBegin(GL_QUADS);

	//find min and max values
	if(scheme == OUT_ANALYTICAL_U)
	{
		ma = 0.0f;
		mi = 999999.0f;
		for(i = 1; i < (ci.numyc - 1); i++)
		{
			if((float)sln[i] > ma)
				ma = (float)sln[i];

			if((float)sln[i] < mi)
				mi = (float)sln[i];
		}
	}
	else
	{
		ma = 0.0f;
		mi = 999999.0f;
		for(j = 1; j < (ci.numyc - 1); j++)
		{
			for(i = 1; i < (ci.numxc - 1); i++)
			{
				if((float)sln[j * ci.numxc + i] > ma)
					ma = (float)sln[j * ci.numxc + i];

				if((float)sln[j * ci.numxc + i] < mi)
					mi = (float)sln[j * ci.numxc + i];
			}
		}
	}

	//halfway point
	mh = ((ma - mi) / 2.0f) + mi;

	//mins/maxs for the overall channel verticies
	x1 = wt / 8.0f;
	y1 = ht / 4.0f;
	x2 = 7.0f * (wt / 8.0f);
	y2 = 3.0f * (ht / 4.0f);
	dxx = (x2 - x1) / (float)(ci.numxc - 3);
	dyy = (y2 - y1) / (float)(ci.numyc - 3);

	y = 0.0f;
	if(scheme == OUT_ANALYTICAL_U)
	{
		for(i = 1; i < (ci.numyc - 2); i++)
		{
			v = (float)sln[i];
			if(v > mh)
			{
				//red to green
				if((mh - mi) != 0.0f)
					r = (v - mh) / (mh - mi);
				else
					r = 0.0f;

				g = 1.0f - r;
				b = 0.0f;
			}
			else
			{
				//green to blue
				r = 0.0f;
				if((mh - mi) != 0.0f)
					g = (v - mi) / (mh - mi);
				else
					g = 0.0f;

				b = 1.0f - g;
			}

			gglColor4f(r, g, b, 1.0f);
			gglVertex2f(x2, ht - (y1 + y));

			gglColor4f(r, g, b, 1.0f);
			gglVertex2f(x1, ht - (y1 + y));

			v = (float)sln[i + 1];
			if(v > mh)
			{
				//red to green
				if((mh - mi) != 0.0f)
					r = (v - mh) / (mh - mi);
				else
					r = 0.0f;

				g = 1.0f - r;
				b = 0.0f;
			}
			else
			{
				//green to blue
				r = 0.0f;
				if((mh - mi) != 0.0f)
					g = (v - mi) / (mh - mi);
				else
					g = 0.0f;

				b = 1.0f - g;
			}

			gglColor4f(r, g, b, 1.0f);
			gglVertex2f(x1, ht - (y1 + y + dyy));

			gglColor4f(r, g, b, 1.0f);
			gglVertex2f(x2, ht - (y1 + y + dyy));

			y += dyy;
		}
	}
	else
	{
		for(j = 1; j < (ci.numyc - 2); j++)
		{
			x = 0.0f;
			for(i = 1; i < (ci.numxc - 2); i++)
			{
				v = (float)sln[j * ci.numxc + i];
				if(v > mh)
				{
					//red to green
					if((mh - mi) != 0.0f)
						r = (v - mh) / (mh - mi);
					else
						r = 0.0f;

					g = 1.0f - r;
					b = 0.0f;
				}
				else
				{
					//green to blue
					r = 0.0f;
					if((mh - mi) != 0.0f)
						g = (v - mi) / (mh - mi);
					else
						g = 0.0f;

					b = 1.0f - g;
				}

				gglColor4f(r, g, b, 1.0f);
				gglVertex2f(x1 + x, ht - (y1 + y));

				v = (float)sln[(j + 1) * ci.numxc + i];
				if(v > mh)
				{
					//red to green
					if((mh - mi) != 0.0f)
						r = (v - mh) / (mh - mi);
					else
						r = 0.0f;

					g = 1.0f - r;
					b = 0.0f;
				}
				else
				{
					//green to blue
					r = 0.0f;
					if((mh - mi) != 0.0f)
						g = (v - mi) / (mh - mi);
					else
						g = 0.0f;

					b = 1.0f - g;
				}

				gglColor4f(r, g, b, 1.0f);
				gglVertex2f(x1 + x, ht - (y1 + y + dyy));

				v = (float)sln[(j + 1) * ci.numxc + (i + 1)];
				if(v > mh)
				{
					//red to green
					if((mh - mi) != 0.0f)
						r = (v - mh) / (mh - mi);
					else
						r = 0.0f;

					g = 1.0f - r;
					b = 0.0f;
				}
				else
				{
					//green to blue
					r = 0.0f;
					if((mh - mi) != 0.0f)
						g = (v - mi) / (mh - mi);
					else
						g = 0.0f;

					b = 1.0f - g;
				}

				gglColor4f(r, g, b, 1.0f);
				gglVertex2f(x1 + x + dxx, ht - (y1 + y +dyy));

				v = (float)sln[j * ci.numxc + (i + 1)];
				if(v > mh)
				{
					//red to green
					if((mh - mi) != 0.0f)
						r = (v - mh) / (mh - mi);
					else
						r = 0.0f;

					g = 1.0f - r;
					b = 0.0f;
				}
				else
				{
					//green to blue
					r = 0.0f;
					if((mh - mi) != 0.0f)
						g = (v - mi) / (mh - mi);
					else
						g = 0.0f;

					b = 1.0f - g;
				}

				gglColor4f(r, g, b, 1.0f);
				gglVertex2f(x1 + x + dxx, ht - (y1 + y));

				x += dxx;
			}

			y += dyy;
		}
	}

	gglEnd();
}
