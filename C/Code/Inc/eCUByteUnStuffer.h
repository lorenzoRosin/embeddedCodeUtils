/**
 * @file       eCUByteUnStuffer.h
 *
 * @brief      Byte unstuffer utils
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECUBYTEUNSTUFFER_H
#define ECUBYTEUNSTUFFER_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      INCLUDES
 **********************************************************************************************************************/
#include "eCUType.h"
#include "eCUByteUnStuffer_prv.h"



/***********************************************************************************************************************
 *      TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    DBUSTF_RES_OK = 0,
    DBUSTF_RES_BADPARAM,
    DBUSTF_RES_BADPOINTER,
	DBUSTF_RES_CORRUPTCTX,
    DBUSTF_RES_OUTOFMEM,
	DBUSTF_RES_FRAMEENDED,
    DBUSTF_RES_NOINITLIB,
}e_eCU_dBUStf_Res;

typedef struct
{
    bool_t   isInit;
	uint8_t* memArea;
	uint32_t memAreaSize;
	uint32_t memAreaCntr;
	e_eCU_dBUStf_sm_prv unStuffState;
}s_eCU_BUStuffCtx;



/***********************************************************************************************************************
 * GLOBAL PROTOTYPES
 **********************************************************************************************************************/
/**
 * @brief       Initialize the byte unStuffer context
 *
 * @param[in]   ctx         - Byte unStuffer context
 * @param[in]   memArea     - Pointer to a memory area that we will use to store the unstuffed data
 * @param[in]   memAreaSize - Dimension in byte of the memory area
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferInitCtx(s_eCU_BUStuffCtx* const ctx, uint8_t memArea[], const uint32_t memAreaSize);

/**
 * @brief       Start receiving a new frame, loosing the previous stored unstuffed frame
 *
 * @param[in]   ctx         - Byte unStuffer context
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferStartNewFrame(s_eCU_BUStuffCtx* const ctx);

/**
 * @brief       Retrive the pointer to the stored unstuffed data, and the data size of the frame. Keep in mind that
 *              the frame parsing could be ongoing, and if an error in the frame occour the retrivedLen could be
 *              setted to 0 again.
 *
 * @param[in]   ctx         - Byte unStuffer context
 * @param[out]  dataP       - Pointer to a Pointer pointing to the unstuffed data frame
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the unstuffed data will be placed
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferGetUnstufData(s_eCU_BUStuffCtx* const ctx, uint8_t** dataP, uint32_t* const retrivedLen);

/**
 * @brief       Retrive the current numbers of unstuffed data received. Keep in mind that the frame parsing could be
 *              ongoing, and if an error in the frame occour the retrivedLen could be setted to 0 again.
 *
 * @param[in]   ctx         - Byte unStuffer context
 * @param[out]  retrivedLen - Pointer to a uint32_t variable where the size of the unstuffed data will be placed
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferGetUnstufLen(s_eCU_BUStuffCtx* const ctx, uint32_t* const retrivedLen);

/**
 * @brief       Retrive if the Unstuffer is currently waiting for the Start of frame in order to start saving data.
 *
 * @param[in]   ctx          - Byte unStuffer context
 * @param[out]  isWaitingSof - Pointer to a bool_t variable that will be filled with true if we are waiting SOF
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferIsWaitingSof(const s_eCU_BUStuffCtx* ctx, bool_t* const isWaitingSof);

/**
 * @brief       Check if the current frame is finished or if we need to unstuff some more data to have the full frame
 *
 * @param[in]   ctx            - Byte unStuffer context
 * @param[out]  isFrameUnstuff - Pointer to a bool_t variable where we will store if the frame parsing is ongoing
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OK           - Operation ended correctly
 */
e_eCU_dBUStf_Res bUStufferIsAFullFrameUnstuff(s_eCU_BUStuffCtx* const ctx, bool_t* const isFrameUnstuff);

/**
 * @brief       Insert the stuffed data chunk that the alg will unstuff byte per byte
 *
 * @param[in]   ctx                - Byte unStuffer context
 * @param[in]   stuffArea          - Pointer to the stuffed Data that we will unstuff
 * @param[in]   stuffLen           - Size of the stuffArea
 * @param[out]  consumedStuffData  - Pointer to an uint32_t were we will store how many stuffed data byte has been
 *                                   analized. Keep in mind that unalized data were not unstuffed and will need to be
 *                                   reparsed. Un parsed data happens when the frame ended earlier
 *                                   ( DBUSTF_RES_FRAMEENDED is returned ) or when some error is returned. When the
 *                                   function return DBUSTF_RES_OK consumedStuffData will always be returned has
 *                                   stuffLen.
 * @param[out]  errSofRec          - Pointer to an uint32_t were we will store how many protocol error were detected.
 *                                   Even with some error detected, the protocol will continue parsing data discharging
 *                                   error. When an error is found the algoritms will restart searching for the SOF.
 *
 * @return      DBUSTF_RES_BADPOINTER   - In case of bad pointer passed to the function
 *		        DBUSTF_RES_NOINITLIB    - Need to init context before taking some action
 *		        DBUSTF_RES_BADPARAM     - In case of an invalid parameter passed to the function
 *		        DBUSTF_RES_CORRUPTCTX   - In case of an corrupted context
 *              DBUSTF_RES_OUTOFMEM     - Can not unstuff data, initial mem pointer was too small. The only way to
 *                                        resolve the issue is increasing the size of the memory area passed to init
 *		        DBUSTF_RES_FRAMEENDED   - Frame ended, restart context in order to parse a new frame. Every other call
 *                                        to this function will not have effect until we call bUStufferStartNewFrame.
 *                                        In this situation bear in mind that some data could be left out the parsing,
 *                                        and so we need to reparse that data after calling bUStufferStartNewFrame.
 *              DBUSTF_RES_OK           - Operation ended correctly. The chunk is parsed correclty but the frame is not
 *                                        finished yet. In this situation consumedStuffData is always reported with a
 *                                        value equals to stuffLen.
 */
e_eCU_dBUStf_Res bUStufferInsStufChunk(s_eCU_BUStuffCtx* const ctx, const uint8_t stuffArea[], const uint32_t stuffLen,
                                       uint32_t* const consumedStuffData, uint32_t* errSofRec);



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUBYTEUNSTUFFER_H */