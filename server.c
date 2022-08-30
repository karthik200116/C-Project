#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "interface.h"


#define MAX 3
#define ARRIVE 0
#define DEPART 1




//Intializes queue values
void initqueue ( struct queue *pq )
{
    pq -> count = 0 ;
    pq -> front = 0 ;
    pq -> rear = -1 ;
}
//Appends to the rear of a queue
void addqueue ( struct queue *pq, struct plane item )
{
    if ( pq -> count >= MAX )
    {
        printf ( "\nQueue is full.\n" ) ;
        return ;
    }
    ( pq -> count )++ ;

    pq -> rear = ( pq -> rear + 1 ) % MAX ;
    pq -> p[pq -> rear] = item ;
}
//Deletes from the front of a queue
struct plane delqueue ( struct queue *pq )
{
    struct plane p1 ;

    if ( pq -> count <= 0 )
    {
        printf ( "\nQueue is empty.\n" ) ;
        p1.id = 0 ;
        p1.tm = 0 ;
  
    }
    else
    {
        ( pq -> count )-- ;
        p1 = pq -> p[pq -> front] ;
        pq -> front = ( pq -> front + 1 ) % MAX ;
    }
    return p1 ;
}
//Takes the frontmost value from within a queue
struct plane takefront(struct queue *pq){
     struct plane p1;
     p1 = pq -> p[pq -> front];
     return p1;

}
//Returns the count of a queue
int size ( struct queue q )
{
    return q.count ;
}
//Checks if queue count = 0
int empty ( struct queue q )
{
    return ( q.count <= 0 ) ;
}
//Checks if queue count = MAX
int full ( struct queue q )
{
    return ( q.count >= MAX ) ;
}

//Initializes the airport structure
void initairport ( struct airport *ap )
{
    initqueue ( &( ap-> landing ) ) ;
    initqueue ( &( ap -> takeoff ) ) ;
   
    ap->pln.id =0;
    ap->pln.tm =0;
   
    ap -> pl = &( ap -> landing ) ;
    ap -> pt = &( ap -> takeoff ) ;
    ap -> nplanes = ap -> nland = ap -> ntakeoff = ap -> nrefuse = 0 ;
    ap -> landwait = ap -> takeoffwait = ap -> idletime = 0 ;
}

//This function takes in the total units of time to pass, expected number of planes arrving/unit time and expected number of planes departing/unit
void start ( int *endtime, double *expectarrive, double *expectdepart )
{
    int flag = 0 ;
    char wish ;

    printf ( "\nProgram that simulates an airport with two runways.\n" ) ;
    printf ( "Planes can land and depart from their respective runways in each unit of time.\n" ) ;
    printf ( "Up to %d planes can be waiting to land or take off at any time.\n", MAX ) ;
    printf ( "How many units of time will the simulation run?" ) ;
    scanf ( "%d", endtime ) ;
    myrandomize( ) ; //seeds a new number for the function randomnumber()
    do
    {
        printf ( "\nExpected number of arrivals per unit time? 0.30<i<40 " ) ;
        scanf ( "%lf", expectarrive ) ;
        printf ( "\nExpected number of departures per unit time? 0.30<i<0.40 " ) ;
        scanf ( "%lf", expectdepart ) ;

        if ( *expectarrive < 0.0 || *expectdepart < 0.0 )
        {
            printf ( "These numbers must be nonnegative.\n" ) ;
            flag = 0 ;
        }
        else
        {
            if ( *expectarrive + *expectdepart > 1.0 )
            {
                printf ( "The airport will become saturated. Read new numbers? " ) ;
                fflush ( stdin ) ;
                scanf ( "%c", &wish ) ;
                if ( tolower ( wish ) == 'y' )
                    flag = 0 ;
                else
                    flag = 1 ;
            }
            else {
                
                if(*expectarrive < 0.30 || *expectdepart < 0.30 || *expectarrive >= 0.40 || *expectdepart >= 0.40){
                     printf("\nDigits must be between 0.30 and 0.40");
                     flag = 0;
                }
                else
                {
                    flag =1;
                }
                
              }  
                 
        }
    } while ( flag == 0 ) ;
}

// Adds a new plane to either ready to land or ready to depart from the aiport
void newplane ( struct airport *ap, int curtime, int action ){
    
 struct plane t;
    switch ( action )
    {
        //If the plane is arriving, creates a new plane whose id is equal to that of current nplanes value
        case ARRIVE :
        ( ap -> nplanes )++ ;
        ap -> pln.id = ap -> nplanes ;
        ap -> pln.tm = curtime ;
            printf ( "\n" ) ;
            printf ( "Plane %d ready to land.\n", ap -> nplanes ) ;
            break ;

        //If the plane is departing , takes the plane at the front of the takeoff queue and prints it 
        case DEPART :
            t = takefront(ap->pt);
            printf ( "\nPlane %d ready to take off.\n",t.id) ;
            break ;
    }
}

//Refuses a plane
void refuse ( struct airport *ap, int action )
{
    struct plane t;
    switch ( action )
    {   //If both runways are full, the plane is redirected and refused completely
        case ARRIVE :
             printf ( "\tplane  %d directed to another airport.\n", ap -> pln.id ) ;
             break ;
 
        //If takeoff queue is full, the plane is told to try later
        case DEPART :
             t = apdelqueue(ap, 't');
             printf ( "\tplane %d told to try later.\n", t.id) ;
             break ;
    }
    ( ap -> nrefuse )++ ;
}

//Lands a plane and increments landing wait time
void land ( struct airport *ap, struct plane pl, int curtime )
{
    int wait ;

    wait = curtime - pl.tm ; 
    printf ( "%d: Plane %d landed ", curtime, pl.id ) ;
    printf ( "in queue %d units \n", wait ) ;
    ( ap -> nland ) ++ ;
    ( ap -> landwait ) += wait ;
}

//The plane takes off and the takeoff wait time is incremented
void fly ( struct airport *ap, struct plane pl, int curtime )
{
    int wait ;

    wait = curtime - pl.tm ;
    printf ( "%d: Plane %d took off ", curtime, pl.id ) ;
    printf ( "in queue %d units \n", wait ) ;
    ( ap -> ntakeoff )++ ;
    ( ap -> takeoffwait ) += wait ;
}

//Checks if either runway is idle
void idle ( struct airport *ap, int curtime, int t )
{
    switch (t)
    {
    case 1:
        printf( "%d: Runway 1 is idle.\n", curtime ) ;
        printf("\nRunway 1 will be used for takeoffs");
        ap -> idletime++ ;
        break;
    
    case 2:
         printf ( "%d: Runway 2 is idle.\n", curtime ) ;
         printf("\nRunways 2 will be used for landings");
         ap -> idletime++ ;
         break;
    }
   
}

//Prints the end tally
void conclude ( struct airport *ap, int endtime )
{
    printf ( "\tSimulation has concluded after %d units.\n", endtime ) ;
    printf ( "\tTotal number of planes processed: %d\n", ap -> nland + ap -> ntakeoff + ap -> nrefuse + apsize ( *ap, 't' ) +  apsize ( *ap, 'l' )) ;
    printf ( "\tNumber of planes landed: %d\n", ap -> nland ) ;
    printf ( "\tNumber of planes taken off: %d\n", ap -> ntakeoff ) ;
    printf ( "\tNumber of planes refused use: %d\n", ap -> nrefuse ) ;
    printf ( "\tNumber left ready to land: %d\n", apsize ( *ap, 'l' ) ) ;
    printf ( "\tNumber left ready to take off: %d\n", apsize ( *ap, 't' ) ) ;

    if ( endtime > 0 )
        printf ( "\tPercentage of time runway idle: %lf \n", ( ( double ) ap -> idletime / endtime ) * 100.0 ) ;

    if ( ap -> nland > 0 )
        printf ( "\tAverage wait time to land: %lf \n", ( ( double ) ap -> landwait / ap -> nland ) ) ;

    if ( ap -> ntakeoff > 0 )
        printf ( "\tAverage wait time to take off: %lf \n", ( ( double ) ap -> takeoffwait / ap -> ntakeoff ) ) ;
}

//Adds a plane to either landing or takeoff queue
void apaddqueue ( struct airport *ap, char type )
{
    struct plane t;
    switch ( tolower( type ) )
    {   
        //Adds a plane from new pln value from airport struct.
        case 'l' :
              addqueue ( ap -> pl, ap -> pln ) ;
              break ;
        
        //Removes a plane from landing queue and adds it to takeoff queue
        case 't' :
              t = delqueue(ap->pl);
              addqueue( ap -> pt,t);
              break;
    }
}

//Removes a plane from either landng or takeoff queue
struct plane apdelqueue ( struct airport *ap, char type )
{
    struct plane p1 ;

    switch ( tolower ( type ) )
    {
        case 'l' :
              //Removes from landing and adds the same to takeoff
              p1 = delqueue ( ap -> pl ) ;
              addqueue(ap->pt,p1);
              break ;
 
              //Removes a plane from takeoff queue
        case 't' :
              p1 = delqueue ( ap -> pt ) ;
              break ;
    }

    return p1 ;
}

//Returns the plane at the front of the landing/takeoff queues
struct plane aptakefront(struct airport *ap, char c){
     struct plane p1;
     switch (tolower(c))
     {
     case 'l':
         p1 = takefront(ap->pl);
         break;
     
     case 't':
         p1 = takefront(ap->pt);
         break;
     }
     return p1;
}

//Returns the count of either queue
int apsize ( struct airport ap, char type )
{
    switch ( tolower ( type ) )
    {
        case 'l' :
              return ( size ( *( ap.pl ) ) ) ;

        case 't' :
              return ( size ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}

//Checks if either runway is full
int apfull ( struct airport ap, char type )
{
    switch ( tolower ( type ) )
    {
        case 'l' :
              return ( full ( *( ap.pl ) ) ) ;

        case 't' :
              return ( full ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}

//Checks if either runway is empty
int apempty ( struct airport ap, char type )
{
    switch ( tolower ( type ) )
    {
        case 'l' :
              return ( empty ( *( ap.pl ) ) ) ;

        case 't' :
              return ( empty ( *( ap.pt ) ) ) ;
    }

    return 0 ;
}

//Creates a new seed for the random number function
void myrandomize( )
{
    srand ( ( unsigned int ) ( time ( NULL ) % 10000 ) ) ;
}

/* This function takes a random number based on the poisson distribution curve.        
   
   Lets say the average of n numbers is 4.6 . The list of n numbers whose average may be 4.6 
   will be from a set  S = {3,5,4,5,2,6.....} and this function picks a random number from that list.

   Lets suppose 2 planes arrive in every 5 units of time. The expected number of arrivals is therefore 0.4 .
   Since our average is 0.4, we use this function ot pick out random numbers that may eventually 
   average out to 0.4 . 

   Unfortunately, this function only works on a LINUX machine, because rand()/ (double) INT_MAX 
   always returns zero for windows, making the runways idle at all times. Please execute this program on a LINUX 
   machine.
*/
int randomnumber ( double expectedvalue )
{
    int n = 0 ;
    double em ;
    double x ;

    em = exp ( -expectedvalue ) ;
    x = rand( ) / ( double ) INT_MAX ;

    while ( x > em )
    {
        n++ ;
        x *= rand( ) / ( double ) INT_MAX ;
    }

    return n ;
}