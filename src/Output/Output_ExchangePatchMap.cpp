
#include "DAINO.h"

#ifndef SERIAL




//-------------------------------------------------------------------------------------------------------
// Function    :  Output_ExchangePatchMap
// Description :  Output the map which marks the patches prepared for sending and receiving data from 
//                neighbor ranks 
//
// Note        :  ( S  .  R )  <-->  (Send  Nothing  Receive)
//
// xyz         :  ( 0, 1, 2 )  -->  output (yz, xz, xy) slice
//-------------------------------------------------------------------------------------------------------
void Output_ExchangePatchMap( const int lv, const int xyz, const char *comment )
{

// set up parameters for outputing different slice
   const int NPatch1D[3] = { NX0[0]/PATCH_SIZE*(1<<lv)+4, 
                             NX0[1]/PATCH_SIZE*(1<<lv)+4, 
                             NX0[2]/PATCH_SIZE*(1<<lv)+4  };
   const char *Dim[2]    = { NULL, NULL };
   int *ii = NULL, *jj = NULL, *kk = NULL;
   int i, j, k, i_end, j_end, k_end; 

   i_end = j_end = k_end = 0;

   switch ( xyz )
   {
      case 0:
         Dim[0] = "X";  Dim[1] = "YZ";
         ii = &k;    jj = &i;    kk = &j; 
         i_end = NPatch1D[1];
         j_end = NPatch1D[2];
         k_end = NPatch1D[0];
         break;

      case 1:
         Dim[0] = "Y";  Dim[1] = "XZ";
         ii = &i;    jj = &k;    kk = &j; 
         i_end = NPatch1D[0];
         j_end = NPatch1D[2];
         k_end = NPatch1D[1];
         break;

      case 2:
         Dim[0] = "Z";  Dim[1] = "XY";
         ii = &i;    jj = &j;    kk = &k; 
         i_end = NPatch1D[0];
         j_end = NPatch1D[1];
         k_end = NPatch1D[2];
         break;

      default:
         Aux_Error( ERROR_INFO, "incorrect parameter %s = %d !!\n", "xyz", xyz );
   }


   char FileName[100];
   sprintf( FileName, "ExchangePatchMap_%d_%d_%2s", DAINO_RANK, lv, Dim[1] );
   if ( comment != NULL )       
   {
      strcat( FileName, "_" );
      strcat( FileName, comment );
   }

   FILE *File_Check = fopen( FileName, "r" );
   if ( File_Check != NULL )
   {
      Aux_Message( stderr, "WARNING : the file \"%s\" already exists and will be overwritten !!\n", FileName );
      fclose( File_Check );
   }


// allocate the Map
   char *Map = new char [ NPatch1D[0]*NPatch1D[1]*NPatch1D[2] ];


// initialize the Map as ''
   for (int P=0; P<NPatch1D[0]*NPatch1D[1]*NPatch1D[2]; P++)    Map[P] = '.';


// record the patches for sending data in the Map
   const int scale0 = patch->scale[ 0];
   const int scale  = patch->scale[lv];
   int ip, jp, kp, ID, PID;

   for (int s=0; s<26; s++)
   for (int P=0; P<patch->ParaVar->SendP_NList[lv][s]; P++)
   {
      PID = patch->ParaVar->SendP_IDList[lv][s][P];

      ip = ( patch->ptr[0][lv][PID]->corner[0]-MPI_Rank_X[0]*NX0[0]*scale0 ) / ( PATCH_SIZE*scale ) + 2;
      jp = ( patch->ptr[0][lv][PID]->corner[1]-MPI_Rank_X[1]*NX0[1]*scale0 ) / ( PATCH_SIZE*scale ) + 2;
      kp = ( patch->ptr[0][lv][PID]->corner[2]-MPI_Rank_X[2]*NX0[2]*scale0 ) / ( PATCH_SIZE*scale ) + 2;

      ID = kp*NPatch1D[1]*NPatch1D[0] + jp*NPatch1D[0] + ip;

      Map[ID] = 'S';
   }


// record the patches for receiving data in the Map
   for (int s=0; s<26; s++)
   for (int P=0; P<patch->ParaVar->RecvP_NList[lv][s]; P++)
   {
      PID = patch->ParaVar->RecvP_IDList[lv][s][P];

      ip = ( patch->ptr[0][lv][PID]->corner[0]-MPI_Rank_X[0]*NX0[0]*scale0 ) / ( PATCH_SIZE*scale ) + 2;
      jp = ( patch->ptr[0][lv][PID]->corner[1]-MPI_Rank_X[1]*NX0[1]*scale0 ) / ( PATCH_SIZE*scale ) + 2;
      kp = ( patch->ptr[0][lv][PID]->corner[2]-MPI_Rank_X[2]*NX0[2]*scale0 ) / ( PATCH_SIZE*scale ) + 2;

      ID = kp*NPatch1D[1]*NPatch1D[0] + jp*NPatch1D[0] + ip;

//    the ID should never repeat for receiving data
      if ( Map[ID] != '.' )      
         Aux_Message( stderr, "WARNING : repeated ID (Rank = %d, P = %d, (ip, jp, kp) = (%d,%d,%d)) !!\n", 
                      DAINO_RANK, P, ip, jp, kp );

      Map[ID] = 'R';
   }


// output the Map slice by slice 
   FILE *File = fopen( FileName, "w" );
   fprintf( File, "Time = %13.7e  Step = %ld  Rank = %d  Level = %d\n\n", Time[0], Step, DAINO_RANK, lv );

   for (k=0; k<k_end; k++)
   {
      fprintf( File, "%s = %d\n\n", Dim[0], k );

      for (j=j_end-1; j>=0; j--)
      {
         for (i=0; i<i_end; i++)   
         {
            ID = (*kk)*NPatch1D[1]*NPatch1D[0] + (*jj)*NPatch1D[0] + (*ii);

            fprintf( File, "  %c", Map[ID] );
         }

         fprintf( File, "\n" );
      }

      fprintf( File, "\n\n\n" );
   }

   fclose( File );


   delete [] Map;

} // FUNCTION : Output_ExchangePatchMap



#endif // #ifndef SERIAL
