/////// ACELERATION(in variable acele) OF PARTICLE i///////////
int aceleration(int i,double *acele);

/////////////CALCULATE OF GRAVITATIONAL POTENCIAL///////////////
double potcalc(int i);

/////////////////////////////TOTAL ENERGY/////////////////////////
int energy(double time);

///////////////////CENTER OF MASS POSITION//////////////////////
int centermasspos(void);

////////////////////CENTER OF MASS VELOCITY/////////////////////
int centermassvel(void);

////////IO/////////

///////////VERIFICATION OF INPUT FILES/////////////
int usage(void);

//REMOVE OLD outputfile AND CREATE A NEW ONE
int create_remove_dir(void);

//////////////////READ PARAMETERS OF THE STAR///////////////////
int read(void);

////////////READ INITIAL POSITIONS AND VELOCITYS////////////////
int read_input(void);

/////////WRITE RADIUS, POSITIONS, VELOCITIES AND MASS//////////////
int write_output(int j);

////////ALLOCATE MEMORY////////////
int allocate_memory(void);

/////////FREE MEMORY THAT WAS ALLOCATE IN THE PROGRAM///////////
int free_memory(void);


