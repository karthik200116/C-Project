#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include "interface.h"

//NOTE: This program can only be run on a linux machine. See the defintiion of randomnumber()
//in server.c for details.
void main( )
{
    struct airport a ;
    int i, pri, curtime, endtime ;
    double expectarrive, expectdepart ;
    struct plane temp ;

    initairport ( &a );

    start ( &endtime, &expectarrive, &expectdepart ) ;

    for ( curtime = 1 ; curtime <= endtime ; curtime++ )
    {
        pri = randomnumber ( expectarrive ) ;

        for ( i = 1 ; i <= pri ; i++ )
        {
            newplane1:
            newplane ( &a, curtime, ARRIVE ) ;
            if ( apfull ( a, 'l' ) &&  apfull ( a, 't' ) ){
                 refuse ( &a, ARRIVE ) ;
            }
            else {
             if (apfull ( a, 'l' )){
            
              printf("\nTakeoffs stopped temporarily. Runway 2 will be used for landings");
              temp = aptakefront( &a, 'l' ) ;
              land ( &a, temp, curtime ) ;
              apaddqueue(&a,'t');
            }
            else{
                apaddqueue( &a, 'l' ) ;
            }
          }
        }

        pri = randomnumber ( expectdepart ) ;
        for ( i = 1 ; i <= pri ; i++ )
        {
         newplane2:
            newplane ( &a, curtime, DEPART ) ;
            if ( apfull ( a, 't' ) )
               refuse ( &a, DEPART ) ;
            else if( apempty ( a, 'l' )){
               goto newplane1;
            }else{
              apaddqueue( &a, 't' ) ;
            }
        }

         if (  ! ( apempty ( a, 'l' ) ) )
        {
            temp = aptakefront( &a, 'l' ) ;
            land ( &a, temp, curtime ) ;
            apaddqueue(&a,'t');
        }
        else{
          idle(&a,curtime,1);
          goto newplane1;
          
        }
        
        if ( ! ( apempty ( a, 't' ) ) )
            {
                temp = apdelqueue ( &a, 't' ) ;
                fly ( &a, temp, curtime ) ;
            }
        else{
            idle ( &a, curtime,2 ) ;
            goto newplane2;

        }
        
    }

    conclude ( &a, endtime ) ;

}
