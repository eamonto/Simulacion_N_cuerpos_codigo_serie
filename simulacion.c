/*
========================================================================
simulacion.c
========================================================================
Programa principal

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

int main (int argc,char *argv[])
{
  extern char *inputfile,*initfile,*outputfile;

  if(argc != 4) usage(); //VERIFICATION OF INPUT FILES

  initfile=argv[1];      //ASIGNATION
  inputfile=argv[2];     //OF FILE'S 
  outputfile=argv[3];    //NAMES

  create_remove_dir();   //REMOVE OLD OUTPUT FILE CREATES A NEW ONE
  
  read();                //READ INITIAL CONDITIONS

  allocate_memory();     //ALLOCATE THE MEMORY

  read_input();          //READ INITIAL POSITIONS AND VELOCITIES 

  emulator();            //SIMULATION 

  free_memory();         //FREE THE MEMORY

  return 0;
}
