/**
 * @file       eCUByteStuffer_prv.h
 *
 * @brief      Byte stuffer utils private definition
 *
 * @author     Lorenzo Rosin
 *
 **********************************************************************************************************************/

#ifndef ECUBYTESTUFFER_PRV_H
#define ECUBYTESTUFFER_PRV_H



#ifdef __cplusplus
extern "C" {
#endif



/***********************************************************************************************************************
 *      PRIVATE TYPEDEFS
 **********************************************************************************************************************/
typedef enum
{
    e_eCU_BSTFPRV_SM_NEEDSOF = 0,
    e_eCU_BSTFPRV_SM_NEEDRAWDATA,
    e_eCU_BSTFPRV_SM_NEEDNEGATEPRECDATA,
	e_eCU_BSTFPRV_SM_NEEDEOF,
    e_eCU_BSTFPRV_SM_STUFFEND
}e_eCU_BSTFPRV_SM;



#ifdef __cplusplus
} /* extern "C" */
#endif



#endif /* ECUBYTESTUFFER_PRV_H */