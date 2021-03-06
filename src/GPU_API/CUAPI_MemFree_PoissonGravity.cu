
#include "DAINO.h"

#if ( defined GPU  &&  defined GRAVITY )



extern real (*d_Rho_Array_P    )[ RHO_NXT*RHO_NXT*RHO_NXT ];
extern real (*d_Pot_Array_P_In )[ POT_NXT*POT_NXT*POT_NXT ];
extern real (*d_Pot_Array_P_Out)[ GRA_NXT*GRA_NXT*GRA_NXT ];
extern real (*d_Flu_Array_G    )[GRA_NIN][ PATCH_SIZE*PATCH_SIZE*PATCH_SIZE ];




//-------------------------------------------------------------------------------------------------------
// Function    :  CUAPI_MemFree_PoissonGravity
// Description :  Free device and host memory previously allocated by the function
//                "CUAPI_MemAllocate_PoissonGravity"
//
// Parameter   :  None
//-------------------------------------------------------------------------------------------------------
void CUAPI_MemFree_PoissonGravity()
{

// free the device memory
   if ( d_Rho_Array_P     != NULL )    CUDA_CHECK_ERROR(  cudaFree( d_Rho_Array_P     )  );
   if ( d_Pot_Array_P_In  != NULL )    CUDA_CHECK_ERROR(  cudaFree( d_Pot_Array_P_In  )  );
   if ( d_Pot_Array_P_Out != NULL )    CUDA_CHECK_ERROR(  cudaFree( d_Pot_Array_P_Out )  );
   if ( d_Flu_Array_G     != NULL )    CUDA_CHECK_ERROR(  cudaFree( d_Flu_Array_G     )  );

   d_Rho_Array_P     = NULL;
   d_Pot_Array_P_In  = NULL;
   d_Pot_Array_P_Out = NULL;
   d_Flu_Array_G     = NULL;


// free the host memory allocated by CUDA
   for (int t=0; t<2; t++)
   {
      if ( h_Rho_Array_P    [t] != NULL )    CUDA_CHECK_ERROR(  cudaFreeHost( h_Rho_Array_P    [t] )  );
      if ( h_Pot_Array_P_In [t] != NULL )    CUDA_CHECK_ERROR(  cudaFreeHost( h_Pot_Array_P_In [t] )  );
      if ( h_Pot_Array_P_Out[t] != NULL )    CUDA_CHECK_ERROR(  cudaFreeHost( h_Pot_Array_P_Out[t] )  );
      if ( h_Flu_Array_G    [t] != NULL )    CUDA_CHECK_ERROR(  cudaFreeHost( h_Flu_Array_G    [t] )  );

      h_Rho_Array_P    [t] = NULL;
      h_Pot_Array_P_In [t] = NULL;
      h_Pot_Array_P_Out[t] = NULL;
      h_Flu_Array_G    [t] = NULL;
   }

} // FUNCTION : CUAPI_MemFree_PoissonGravity



#endif // #if ( defined GPU  &&  defined GRAVITY )
