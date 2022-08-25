/**
 * @file eCUCircularQueue.c
 *
 */

/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUCircularQueue.h"

/***********************************************************************************************************************
 *      DEFINES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   PRIVATE STATIC FUNCTIONS PROTOTYPES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  STATIC VARIABLES
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *      MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *   GLOBAL FUNCTIONS
 **********************************************************************************************************************/
e_eCU_Res circQInit(s_eCU_circQCtx* const ctx, uint8_t* const memPool, const uint32_t memPoolSize)
{
	/* Local variable */
	e_eCU_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx) || ( NULL ==  memPool) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check data validity */
		if( memPoolSize <= 0u )
		{
			result = ECU_RES_BADPARAM;
		}
		else
		{
			/* Check Init */
			if( true == ctx->isInit )
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				ctx->isInit = true;
				ctx->memPool = memPool;
				ctx->memPoolSize = memPoolSize;
				ctx->memPoolUsedSize = 0u;
				ctx->memPoolFrstFreeIdx = 0u;
				ctx->memPoolFrstOccIdx = 0u;
				
				result = ECU_RES_OK;
			}			
		}

	return result;
}

e_eCU_Res circQReset(s_eCU_circQCtx* const ctx)
{
	/* Local variable */
	e_eCU_Res result;

	/* Check pointer validity */
	if( NULL == ctx )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{			
			/* Update index in order to discharge all saved data */
			ctx->memPoolUsedSize = 0u;
			ctx->memPoolFrstFreeIdx = 0u;
			ctx->memPoolFrstOccIdx = 0u;
			result = ECU_RES_OK;		
		}

	return result;
}

e_eCU_Res circQGetFreeSapce(s_eCU_circQCtx* const ctx, uint32_t* const freeSpace)
{
	/* Local variable */
	e_eCU_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == freeSpace ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( (datalen <= 0u) || ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || 
			    ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) || ( ctx->memPoolSize < ctx->memPoolUsedSize ))
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				*freeSpace = ctx->memPoolSize - ctx->memPoolUsedSize;
				result = ECU_RES_OK;				
			}
		}

	return result;
}

e_eCU_Res circQGetOccupiedSapce(s_eCU_circQCtx* const ctx, uint32_t* const usedSpace)
{
	/* Local variable */
	e_eCU_Res result;

	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == usedSpace ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( (datalen <= 0u) || ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || 
			    ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) || ( ctx->memPoolSize < ctx->memPoolUsedSize ))
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				*usedSpace = ctx->memPoolUsedSize;
				result = ECU_RES_OK;					
			}
		}

	return result;
}

e_eCU_Res circQInsertData(s_eCU_circQCtx* const ctx, const uint32_t* data, const uint32_t datalen)
{
	/* Local variable */
	e_eCU_Res result;
	uint32_t freeSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen
	
	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( (datalen <= 0u) || ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || 
			    ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) || ( ctx->memPoolSize < ctx->memPoolUsedSize ))
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				/* Check for free memory */
				result = circQGetFreeSapce(ctx, &freeSpace);
				
				if( ECU_RES_OK == result )
				{
					if( datalen > freeSpace )
					{
						/* No memory avaiable */
						result = ECU_RES_OUTOFMEM;
					}
					else
					{
						/* Can insert data */
						if( ( datalen +  ctx->memPoolFrstFreeIdx ) <= ctx->memPoolSize )
						{
							/* Direct copy */
							memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], data, datalen);
							
							/* Update free index */
							ctx->memPoolFrstFreeIdx += datalen;
							if(ctx->memPoolFrstFreeIdx >= ctx->memPoolSize)
							{
								ctx->memPoolFrstFreeIdx = 0u;
							}
						}
						else
						{
							/* Multicopy */
				
							/* First round */
							firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstFreeIdx;
							memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], data, firstTranshLen);
							ctx->memPoolFrstFreeIdx = 0u;
				
							/* Second round */
							secondTranshLen = datalen - firstTranshLen;
							memcpy(&ctx->memPool[ctx->memPoolFrstFreeIdx], &data[firstTranshLen], secondTranshLen);
							ctx->memPoolFrstFreeIdx += secondTranshLen;
						}
				
						ctx->memPoolUsedSize += datalen;
						result = ECU_RES_OK;						
					}					
				}
			}
		}

	return result;
}

e_eCU_Res circQRetriveData(s_eCU_circQCtx* const ctx, uint32_t* const data, const uint32_t datalen)
{
	/* Local variable */
	e_eCU_Res result;
	uint32_t usedSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen
	
	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( (datalen <= 0u) || ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || 
			    ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) || ( ctx->memPoolSize < ctx->memPoolUsedSize ))
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				result = circQGetOccupiedSapce(ctx, &usedSpace);
				if( ECU_RES_OK == result )
				{
					if( datalen > usedSpace)
					{
						/* No enoght data in the queue */
						result = ECU_RES_OUTOFMEM;
					}
					else
					{
						/* Can retrive data */
						if( ( datalen +  ctx->memPoolFrstOccIdx ) <= ctx->memPoolSize )
						{
							/* Direct copy */
							memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], datalen);
							
							/* Update used index */
							ctx->memPoolFrstOccIdx += datalen;
							if(ctx->memPoolFrstOccIdx >= ctx->memPoolSize)
							{
								ctx->memPoolFrstOccIdx = 0u;
							}
						}
						else
						{
							/* Multicopy */
				
							/* First round */
							firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstOccIdx;
							memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], firstTranshLen);
							ctx->memPoolFrstOccIdx = 0u;
				
							/* Second round */
							secondTranshLen = datalen - firstTranshLen;
							memcpy(&data[firstTranshLen], &ctx->memPool[ctx->memPoolFrstOccIdx], secondTranshLen);
							ctx->memPoolFrstOccIdx += secondTranshLen;
						}
				
						ctx->memPoolUsedSize -= datalen;
						result = ECU_RES_OK;						
					}					
				}
			}
		}

	return result;
}
 
e_eCU_Res circQPeekData(s_eCU_circQCtx* const ctx, uint32_t* const data, const uint32_t datalen)
{
	/* Local variable */
	e_eCU_Res result;
	uint32_t usedSpace;
	uint32_t firstTranshLen;
	uint32_t secondTranshLen
	
	/* Check pointer validity */
	if( ( NULL == ctx ) || ( NULL == data ) )
	{
		result = ECU_RES_BADPOINTER;
	}
	else
	{
		/* Check Init */
		if( false == ctx->isInit )
		{
			result = ECU_RES_NOINITLIB;
		}
		else
		{
			/* Check data validity an queue integrity */
			if( (datalen <= 0u) || ( ctx->memPoolFrstFreeIdx >= ctx->memPoolSize ) || 
			    ( ctx->memPoolFrstOccIdx >= ctx->memPoolSize ) || ( ctx->memPoolSize < ctx->memPoolUsedSize ))
			{
				result = ECU_RES_BADPARAM;
			}
			else
			{
				result = circQGetOccupiedSapce(ctx, &usedSpace);
				if( ECU_RES_OK == result )
				{
					if( datalen > usedSpace)
					{
						/* No enoght data in the queue */
						result = ECU_RES_OUTOFMEM;
					}
					else
					{
						/* Can retrive data */
						if( ( datalen +  ctx->memPoolFrstOccIdx ) <= ctx->memPoolSize )
						{
							/* Direct copy */
							memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], datalen);
						}
						else
						{
							/* Multicopy */
				
							/* First round */
							firstTranshLen = ctx->memPoolSize - ctx->memPoolFrstOccIdx;
							memcpy(data, &ctx->memPool[ctx->memPoolFrstOccIdx], firstTranshLen);
				
							/* Second round */
							secondTranshLen = datalen - firstTranshLen;
							memcpy(&data[firstTranshLen], &ctx->memPool[0u], secondTranshLen);
						}
				
						result = ECU_RES_OK;						
					}					
				}
			}
		}

	return result;
} 