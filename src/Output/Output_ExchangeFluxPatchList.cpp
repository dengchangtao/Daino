
#include "DAINO.h"

#ifndef SERIAL




//-------------------------------------------------------------------------------------------------------
// Function    :  Output_ExchangeFluxPatchList
// Description :  Output SendF_IDList[lv] or RecvF_IDList[lv]
//
// Option      :  0 -> SendF_IDList[lv]
//             :  1 -> RecvF_IDList[lv]
//-------------------------------------------------------------------------------------------------------
void Output_ExchangeFluxPatchList( const int option, const int lv, const char *comment )
{

   if ( option < 0  ||  option > 1 )
      Aux_Error( ERROR_INFO, "incorrect parameter %s = %d !!\n", "option", option );

   if ( lv < 0  ||  lv >= NLEVEL-1 )
      Aux_Error( ERROR_INFO, "incorrect parameter %s = %d !!\n", "lv", lv );


   char FileName[100];
   switch ( option )
   {
      case 0:  sprintf( FileName, "SendFluxPatchList_%d_%d",     MPI_Rank, lv );
               break;
      case 1:  sprintf( FileName, "RecvFluxPatchList_%d_%d",     MPI_Rank, lv );
               break;
   }

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


   FILE *File = fopen( FileName, "w" );

   fprintf( File, "Time = %13.7e  Step = %ld  Rank = %d  Level = %d\n\n", Time[0], Step, DAINO_RANK, lv );


   int NP    = 0;
   int *List = NULL;

   for (int s=0; s<6; s++)
   {
      switch ( option )
      {
         case 1:  NP    = patch->ParaVar->SendF_NList [lv][s];
                  List  = patch->ParaVar->SendF_IDList[lv][s];
                  break;

         case 2:  NP    = patch->ParaVar->RecvF_NList [lv][s];
                  List  = patch->ParaVar->RecvF_IDList[lv][s];
                  break;
      }

      fprintf( File, "Face = %d     Length = %d\n", s, NP );

      for (int P=0; P<NP; P++)    fprintf( File, "%5d ", List[P] );

      fprintf( File, "\n\n" );
   }

   fclose( File );

} // FUNCTION : Output_ExchangeFluxPatchList



#endif // #ifndef SERIAL
