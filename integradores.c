/*
=========================================================================
integradores.c
=========================================================================
Esta libreria contiene la implementacion de un integrador simplectico 
de segundo orden (leapfrog) para N-cuerpos, y un integrador Runge-Kutta 4.

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

//////////////////////////RUNGE-KUTTA4/////////////////////////////
int RK4(void)
{
  int i,j;
  double acele[3];

//////////////CALCULA K1/////////////////
  for(j=0;j<Num_par;j++)
    {
      aceleration(j,acele);

      for(i=0;i<MAXDIM;i++)
	{
	  F1[i][j]=t*acele[i];
	  G1[i][j]=t*par[j].vel[i];
	}
    }

//////////////CALCULA K2/////////////////

  for(j=0;j<Num_par;j++)
    {
      for(i=0;i<MAXDIM;i++)
	{
	  par[j].pos[i]=Auxiliar[i][j]+0.5*G1[i][j];
	}
    }


  for(j=0;j<Num_par;j++)
    {
      aceleration(j,acele);

      for(i=0;i<MAXDIM;i++)
	{
	  F2[i][j]=t*acele[i];
	  G2[i][j]=t*(par[j].vel[i]+0.5*F1[i][j]);
	}
    }

//////////////CALCULA K3/////////////////

  for(j=0;j<Num_par;j++)
    {
      for(i=0;i<MAXDIM;i++)
	{
	  par[j].pos[i]=Auxiliar[i][j]+0.5*G2[i][j];
	}
    }

  for(j=0;j<Num_par;j++)
    {
      aceleration(j,acele);

      for(i=0;i<MAXDIM;i++)
	{
	  F3[i][j]=t*acele[i];
	  G3[i][j]=t*(par[j].vel[i]+0.5*F2[i][j]);
	}
    }

//////////////CALCULA K4/////////////////

  for(j=0;j<Num_par;j++)
    {
      for(i=0;i<MAXDIM;i++)
	{
	  par[j].pos[i]=Auxiliar[i][j]+G3[i][j];
	}
    }


  for(j=0;j<Num_par;j++)
    {
      aceleration(j,acele);

      for(i=0;i<MAXDIM;i++)
	{
	  F4[i][j]=t*acele[i];
	  G4[i][j]=t*(par[j].vel[i]+F3[i][j]);
	}
    }

  return 0;
}



//////////////////////RK4 INTEGRATOR//////////////////////
int RK4_integration(void)
{
  int i,j;
  double C,D;

  ////MAKE A COPY OF THE ORIGINAL POSITIONS
  for(j=0;j<Num_par;j++){
    for(i=0;i<MAXDIM;i++){
      Auxiliar[i][j]=par[j].pos[i];
    }
  }

  ////CALCULATE OF F's AND G's FOR ALL PARTICLE IN ALL DIRECTION
  RK4();  

  //////////////// PARTICLE j VELOCITY AND POSITION IN DIRECTION i ////////////
  for(j=0;j<Num_par;j++)
    {
      for(i=0;i<MAXDIM;i++)
	{
	  C = F1[i][j]+2.0*F2[i][j]+2.0*F3[i][j]+F4[i][j];
	  par[j].vel[i] = par[j].vel[i]+C/6.0;

	  D = G1[i][j]+2.0*G2[i][j]+2.0*G3[i][j]+G4[i][j];
	  par[j].pos[i] = Auxiliar[i][j]+D/6.0;
	}
    }
    
  return 0;
}


///////////////////SYMPLECTIC INTEGRATOR//////////////////
int symplectic_integration(void)
{
  int i,j;
  double acele[3];

  for(j=0;j<Num_par;j++){
    for(i=0;i<MAXDIM;i++){
      par[j].pos[i] = par[j].pos[i] + 0.5*t*par[j].vel[i];
    }
  }
  
  for(j=0;j<Num_par;j++){
    aceleration(j,acele);
      
    for(i=0;i<MAXDIM;i++){
      par[j].vel[i] = par[j].vel[i] + t*acele[i];
    }
  }
	
  for(j=0;j<Num_par;j++){
    for(i=0;i<MAXDIM;i++){
      par[j].pos[i]=  par[j].pos[i] + 0.5*t*par[j].vel[i];
    }
  }
  
  return 0;
}


////////THE BOSS OF INTEGRATOR AND EXIT DATES//////////
int emulator(void)
{
  int i,l;
  double Ttotal,time;
  
  Ttotal = t*Num_simulation*Num_archive;
  printf("\nTotal time of simulation =%lf\n\n",Ttotal);

  centermasspos();    
  centermassvel();    

  time=0.0;


  ////THIS "for" GENERATE "Num_archive" ARCHIVES WHIT POSITION AND VELOCITY
  for(i=0 ; i<Num_archive ; i++)
    {
      for (l=0; l<Num_simulation; l++) ///NUMBER OF SIMULATION IN ONE ARCHIVE
	{ 
	  if(l%10==0)  energy(time);

	  printf("Time of Simulation: %lf\n",time);

	  if(INTEGRATOR==0)
	    {
	      symplectic_integration(); ////SYMPLECTIC INTEGRATION//
	    }else
	    {
	      RK4_integration();        ////RK4 INTEGRATION ////////
	    }

	  time=time + t;	
	}
      
      centermasspos();    
      centermassvel();    

      write_output(i);
    }
  
  return 0;
}

