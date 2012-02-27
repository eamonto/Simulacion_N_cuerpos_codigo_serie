/*
=========================================================================
lib_simulation.c
=========================================================================

Contiene una serie de rutinas estandares en la implementacion de N-Cuerpos
y las rutinas de entrada y salida de datos.

    Copyright (C) 2008  Edison Montoya, eamonto@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


Up to date: 27 Feb 2012					

*/

#include <header.h>

//////////////// ACELERATION(in variable acele) OF PARTICLE i////////////////
int aceleration(int i,double *acele)
{
  int j;
  double Xij,Yij,Zij,Rij;
  
  acele[0]=0.0;
  acele[1]=0.0;
  acele[2]=0.0;

  for(j=0; j<Num_par; j++)
    {
      if ( i!=j )	////////////CALCULATE ACELERATION OF PARTICLE
	{
	  Xij = (par[j].pos[0]-par[i].pos[0])*(par[j].pos[0]-par[i].pos[0]);
	  Yij = (par[j].pos[1]-par[i].pos[1])*(par[j].pos[1]-par[i].pos[1]);
	  Zij = (par[j].pos[2]-par[i].pos[2])*(par[j].pos[2]-par[i].pos[2]);

	  Rij = pow( Xij+Yij+Zij + EPS*EPS, 1.5);
	  
	  acele[0] = acele[0] - (par[i].pos[0]-par[j].pos[0])/Rij;
	  acele[1] = acele[1] - (par[i].pos[1]-par[j].pos[1])/Rij;
	  acele[2] = acele[2] - (par[i].pos[2]-par[j].pos[2])/Rij;
	}
    }

  acele[0] = acele[0]*G*par[0].mass;
  acele[1] = acele[1]*G*par[0].mass;
  acele[2] = acele[2]*G*par[0].mass;

  return 0;
}


/////////////CALCULATE OF GRAVITATIONAL POTENCIAL///////////////
double potcalc(int i)
{
  double pot,Xij,Yij,Zij;
  int j;

  pot=0.0;

  for(j=0; j<Num_par ; j++)
    {
      if ( i!=j )
	{
	  Xij = (par[i].pos[0] - par[j].pos[0])*(par[i].pos[0] - par[j].pos[0]);
	  Yij = (par[i].pos[1] - par[j].pos[1])*(par[i].pos[1] - par[j].pos[1]);
	  Zij = (par[i].pos[2] - par[j].pos[2])*(par[i].pos[2] - par[j].pos[2]);
	      
	  pot=pot- 1.0/sqrt(Xij + Yij + Zij + EPS*EPS);
	}
    }

  pot=pot*G*par[0].mass;

  return pot;
}


/////////////////////////////TOTAL ENERGY/////////////////////////
int energy(double time)
{
  int i;
  double v2;
  char output[100];  
  FILE *ppf;

  potencial=0.0;
  kinetic=0.0;
  totalenergy=0.0;
  printf("Calculating energy...\n");

  for(i=0; i<Num_par; i++)
    {
      potencial=potencial+potcalc(i);
      v2=par[i].vel[0]*par[i].vel[0] + par[i].vel[1]*par[i].vel[1] + par[i].vel[2]*par[i].vel[2];
      kinetic=kinetic+v2;
    }

  potencial=par[0].mass*0.5*potencial;
  kinetic=par[0].mass*0.5*kinetic;
  totalenergy=potencial+kinetic;

  strcpy(output,outputfile);
  strcat(output,"/energy.dat");
  
  ppf=fopen(output,"a");
  fprintf(ppf,"%lf \t %lf \t %lf \t %lf \n",time,totalenergy,potencial,kinetic);
  fclose(ppf);

  printf(" \n --> Total Energy=%lf U=%lf K=%lf later of %lf time\n",
	 totalenergy,potencial,kinetic,time);

  return 0;
}


////////////////////////CENTER OF MASS POSITION///////////////////////////////
int centermasspos(void)
{
  double cmx,cmy,cmz,Mass;
  int i;
  
  cmx=0.0;
  cmy=0.0;
  cmz=0.0;
  Mass=0.0;

  for ( i=0 ; i<Num_par ; i++) 
    {
      cmx = cmx + par[i].mass*par[i].pos[0];
      cmy = cmy + par[i].mass*par[i].pos[1];
      cmz = cmz + par[i].mass*par[i].pos[2];
      Mass = Mass + par[i].mass;
    }

  cmx=cmx/Mass;
  cmy=cmy/Mass;
  cmz=cmz/Mass;

  for ( i=0 ; i<Num_par ; i++) 
    {
      par[i].pos[0] = par[i].pos[0]-cmx;
      par[i].pos[1] = par[i].pos[1]-cmy;
      par[i].pos[2] = par[i].pos[2]-cmz;
    }

  return 0;
}


////////////////////////CENTER OF MASS VELOCITY/////////////////////////////
int centermassvel(void)
{
  double cmx,cmy,cmz,Mass;
  int i;
  
  cmx=0.0;
  cmy=0.0;
  cmz=0.0;
  Mass=0.0;
  
  for ( i=0 ; i<Num_par ; i++) 
    {
      cmx = cmx + par[i].mass*par[i].vel[0];
      cmy = cmy + par[i].mass*par[i].vel[1];
      cmz = cmz + par[i].mass*par[i].vel[2];
      Mass = Mass + par[i].mass;
    }

  cmx=cmx/Mass;
  cmy=cmy/Mass;
  cmz=cmz/Mass;

  for ( i=0 ; i<Num_par ; i++)
    {
      par[i].vel[0] = par[i].vel[0]-cmx;
      par[i].vel[1] = par[i].vel[1]-cmy;
      par[i].vel[2] = par[i].vel[2]-cmz; 
    }

  return 0;
}



 /// IO ROUTINES

///////////VERIFICATION OF INPUT FILES/////////////
int usage(void)
{
  printf("\n");
  printf("USAGE: ./exec <initfile> <inputfile> <outputfile> \n");
  printf("\n");
  exit(1);
}


//REMOVE OLD outputfile AND CREATE A NEW ONE
int create_remove_dir(void)
{
  char remove[200],copy[200],create[200];
  char *rm="rm -rf ",*cp="cp ",*mkdir="mkdir -p ";
  int out_return;

  strcpy(remove,rm);
  strcat(remove,outputfile);
  out_return = system(remove);

  strcpy(create,mkdir);
  strcat(create,outputfile);
  out_return = system(create);

  strcpy(copy,cp);
  strcat(copy,initfile);
  strcat(copy," ");
  strcat(copy,outputfile);
  out_return = system(copy);

  return out_return;
}


////////READ PARAMETERS OF THE STAR/////
int read(void)
{
  int int_out;
  FILE *pf;
  
  pf=fopen(initfile,"r");
  
  int_out = fscanf(pf,"%d %lf %lf %lf %lf %lf %lf %d",&Num_par,&t,&G,&w,
	 &Num_archive,&Num_simulation,&EPS,&INTEGRATOR);
  
  fclose(pf);

  return 0;
}


//////////////////////READ INITIAL POSITIONS AND VELOCITIES////////////////////
int read_input(void)
{
  int int_out;
  int i;
  FILE *ppf;
  
  ppf=fopen(inputfile,"r");
  
  for(i=0 ; i<Num_par ; i++)
    {
      int_out = fscanf(ppf,"%lf %lf %lf %lf %lf %lf %lf %lf",
	     &par[i].radius,&par[i].pos[0],&par[i].pos[1],&par[i].pos[2],
	     &par[i].vel[0],&par[i].vel[1],&par[i].vel[2],&par[i].mass);
    }

  fclose(ppf);

  return 0;
}


////////////WRITE RADIUS, POSITIONS, VELOCITIES AND MASS/////////////////////
int write_output(int j)
{
  int i;
  char output[100],param[30];
  char *char_out;
  FILE *pf;

  char_out = gcvt(j,2,param);
  
  strcpy(output,outputfile);
  strcat(output,"/");
  strcat(output,outputfile);
  strcat(output,"_");
  strcat(output,param);
  strcat(output,".dat");
  
  pf=fopen(output,"w");
  
  for (i=0 ; i < Num_par ; i++)
    { 
      par[i].radius=sqrt(par[i].pos[0]*par[i].pos[0] + 
			 par[i].pos[1]*par[i].pos[1] + 
			 par[i].pos[2]*par[i].pos[2]);

      fprintf(pf,"%lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\t %lf\n",
	      par[i].radius,par[i].pos[0],par[i].pos[1],par[i].pos[2],
	      par[i].vel[0],par[i].vel[1],par[i].vel[2],par[i].mass);     
    }

  fclose(pf);
  
  return 0;
}



////////ALLOCATE MEMORY ////////////
int allocate_memory(void)
{
  int i;

  par=(struct Particle *) malloc((size_t) Num_par*sizeof(struct Particle));

    for(i=0;i<MAXDIM;i++)
      {
        Auxiliar[i]=(double *) malloc((size_t) Num_par*sizeof(double));
      }
    
  if(INTEGRATOR==1){
    for(i=0;i<MAXDIM;i++)
      {
	F1[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	F2[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	F3[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	F4[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	G1[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	G2[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	G3[i]=(double *) malloc((size_t) Num_par*sizeof(double));
	G4[i]=(double *) malloc((size_t) Num_par*sizeof(double));
      }
  }

  return 0;
}


/////////FREE MEMORY THAT WAS ALLOCATE IN THE PROGRAM///////////
int free_memory(void)
{
  int i;

  free(par);

    for(i=0;i<MAXDIM;i++){
      free(Auxiliar[i]);
	}

  if(INTEGRATOR==1){
    for(i=0;i<MAXDIM;i++){
      free(F1[i]);
      free(F2[i]);
      free(F3[i]);
      free(F4[i]);
      free(G1[i]);
      free(G2[i]);
      free(G3[i]);
      free(G4[i]);
    }
  }

  return 0;
}


