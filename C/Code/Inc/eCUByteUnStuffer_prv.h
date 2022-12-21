/**
 * @file       eCUByteUnStuffer_prv.h
 *
 * @brief      Byte Unstuffer utils private definition
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECUBYTEUNSTUFFER_PRV_H
#define ECUBYTEUNSTUFFER_PRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_BUNSTFPRV_SM_NEEDSOF = 0,
    e_eCU_BUNSTFPRV_SM_NEEDRAWDATA,
    e_eCU_BUNSTFPRV_SM_NEEDNEGATEDATA,
    e_eCU_BUNSTFPRV_SM_UNSTUFFEND,
    e_eCU_BUNSTFPRV_SM_UNSTUFFFAIL
}e_eCU_BUNSTFPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUBYTEUNSTUFFER_PRV_H */
