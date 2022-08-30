
#define MAX 3  //Maximum members in a queue
#define ARRIVE 0
#define DEPART 1

//This structure defines a plane within a queue
struct plane
{
    int id ; //plane no.
    int tm ; //plane time, which is the curtime unit at which it arrives
} ;

//This structure is an abstract queue 
struct queue
{
    int count ;
    int front ;
    int rear ;
       struct plane p[MAX] ; //Planes within a queue, limited to a maxmimum defined by MAX
} ;

//Structure encompassing ALL airport details
struct airport
{
    struct queue landing ; //Runway 1 queue
    struct queue takeoff ; //Runway 2 queue
    
    struct queue *pl ;     //Pointer to runway 1 landing queue
    struct queue *pt ;     //Pointer to runway 2 landing queue
    
    int idletime ;         //Time spent idle
    int landwait, takeoffwait ;     //Wait times
    int nland, nplanes, nrefuse, ntakeoff ;  //total landings ,new entering planes, total refusals, total takeoffs
    
    
    struct plane pln ; //New planes entering the aiport.
} ;


void initqueue ( struct queue * ) ;  
void addqueue ( struct queue *, struct plane ) ;
struct plane delqueue ( struct queue * ) ;
struct plane takefront(struct queue * );
int size ( struct queue ) ;
int empty ( struct queue ) ;
int full ( struct queue ) ;
void initairport ( struct airport * ) ;
void start ( int *, double *, double * ) ;
void newplane ( struct airport *, int, int ) ;
void refuse ( struct airport *, int ) ;
void land ( struct airport *, struct plane, int ) ;
void fly ( struct airport *, struct plane, int ) ;
void idle ( struct airport *, int , int) ;
void conclude ( struct airport *, int ) ;
int randomnumber ( double ) ;
void apaddqueue ( struct airport *, char ) ;
struct plane apdelqueue ( struct airport *, char ) ;
struct plane aptakefront(struct airport * , char  );
int apsize ( struct airport, char ) ;
int apfull ( struct airport, char ) ;
int apempty ( struct airport, char ) ;
void myrandomize ( ) ;

