#ifndef __CFD_H__
#define __CFD_H__

//constants
#define C_L				2.0
#define C_H				1.0
#define C_RHO			1.0
#define C_RE			10.0

//Nx, Ny (includes ghost cells)
#define NUM_XCELLS		53
#define NUM_YCELLS		53

#define DELTAT			0.0016

//over relaxation values
#define OMEGA			1.9
#define OMEGAU			1.0
#define OMEGAV			1.0
#define OMEGAP			1.0

//steady-state end condition
#define SS_EPSILONT		0.00001
#define SS_EPSILONS		0.5
#define SS_EPSILONP		0.0001

//speed of the animation
#define R_ANIMSCALE		10.0f

//0 -> solid; 1 -> smooth
int smoothing;

typedef enum
{
	OUT_SIMPLE_SS_U,
	OUT_SIMPLE_SS_V,
	OUT_SIMPLE_SS_P,
	OUT_ANALYTICAL_U,
	OUT_MAX
} outputs_t;

outputs_t scheme;

typedef struct tstep_s
{
	double t;
	double *u, *v, *p;
	struct tstep_s *prev;
	struct tstep_s *next;
} tstep_t;

typedef struct
{
	//constants
	double L, h, rho, Re;
	double delt;
	double oma, omu, omv, omp;
	double sset, sses, ssep;
	int numxc, numyc;
	float animscale;
	int elapsedtime;

	//global solution results
	double *slnau;				//analytical
								//pointers to the current time step SIMPLE values
	double *slnsu, *slnsv, *slnsp;
	tstep_t *slns;				//SIMPLE
	tstep_t *sslns;				//pointer to last time step (steady-state)
	tstep_t *curts;
	int ncx, ncy;
} cfd_info_t;

extern cfd_info_t ci;

void FinalProj_Analytical(int Ny);
void FinalProj_SIMPLE(int Nx, int Ny, double dt);
void FinalProject(void);
void FinalProj_Shutdown(void);
void FinalProj_NextTS(int time);
void FinalProj_Render(float wt, float ht);

#endif
