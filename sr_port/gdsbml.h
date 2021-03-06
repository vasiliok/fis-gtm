/****************************************************************
 *								*
 *	Copyright 2001, 2010 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#define SIXTEEN_BLKS_FREE	0x55555555
#define FOUR_BLKS_FREE		0x55
#define THREE_BLKS_FREE		0x54
#define LCL_MAP_LEVL		0xFF
#define BLK_BUSY		0x00
#define BLK_FREE		0x01
#define BLK_MAPINVALID		0x02
#define BLK_RECYCLED		0x03
#define BML_BITS_PER_BLK	2
#define	THREE_BLKS_BITMASK	0x3F
#define	TWO_BLKS_BITMASK	0x0F
#define	ONE_BLK_BITMASK		0x03
#define BMP_EIGHT_BLKS_FREE	255

/* returns the bitmap status (BLK_BUSY|BLK_FREE|etc.) of the "blknum"th block within the local bitmap block "bp" in bml_status */
#define	GET_BM_STATUS(bp, blknum, bml_status)								\
{													\
	sm_uc_ptr_t	ptr;										\
													\
	ptr = ((sm_uc_ptr_t)(bp) + SIZEOF(blk_hdr) + ((blknum * BML_BITS_PER_BLK) / BITS_PER_UCHAR));			\
	bml_status = (*ptr >> ((blknum * BML_BITS_PER_BLK) % BITS_PER_UCHAR)) & ((1 << BML_BITS_PER_BLK) - 1);	\
}

/* sets bitmap status (BLK_BUSY|BLK_FREE etc.) for the "blknum"th block within the local bitmap block "bp" from new_bml_status */
#define	SET_BM_STATUS(bp, blknum, new_bml_status)								\
{													\
	sm_uc_ptr_t	ptr;										\
													\
	assert(2 == BML_BITS_PER_BLK);									\
	ptr = ((sm_uc_ptr_t)(bp) + SIZEOF(blk_hdr) + ((blknum * BML_BITS_PER_BLK) / BITS_PER_UCHAR));			\
	*ptr = (*ptr & ~(0x03 << ((blknum * BML_BITS_PER_BLK) % BITS_PER_UCHAR)))			\
		| ((new_bml_status & 0x03) << ((blknum * BML_BITS_PER_BLK) % BITS_PER_UCHAR));		\
}

#define	BM_MINUS_BLKHDR_SIZE(bplm)	((bplm) / (BITS_PER_UCHAR / BML_BITS_PER_BLK))
#define BM_SIZE(bplm)			(SIZEOF(blk_hdr) + BM_MINUS_BLKHDR_SIZE(bplm))

#define	VALIDATE_BM_BLK(blk, bp, csa, region, status)								\
{														\
	error_def(ERR_DBBMLCORRUPT);										\
														\
	assert(BITS_PER_UCHAR % BML_BITS_PER_BLK == 0);	/* assert this for the BM_MINUS_BLKHDR_SIZE macro */	\
	if (IS_BITMAP_BLK(blk) && ((LCL_MAP_LEVL != (bp)->levl) || (BM_SIZE(csa->hdr->bplmap) != (bp)->bsiz)))	\
	{													\
		send_msg(VARLSTCNT(9) ERR_DBBMLCORRUPT, 7, DB_LEN_STR(region), 					\
				blk, (bp)->bsiz, (bp)->levl, &(bp)->tn, &csa->ti->curr_tn);			\
		status = FALSE;											\
		assert(FALSE);											\
	} else													\
		status = TRUE;											\
}

#define NO_FREE_SPACE		-1

/* MAP_RD_FAIL is hard coded into the file BML_GET_FREE.MAR */
#define MAP_RD_FAIL		-2
#define EXTEND_SUSPECT		-3
#define FILE_EXTENDED		-4
#define FINAL_RETRY_FREEZE_PROG	-5

#define	GET_CDB_SC_CODE(gdsfilext_code, status)			\
{								\
	if (MAP_RD_FAIL == gdsfilext_code)			\
		status = (enum cdb_sc)rdfail_detail;		\
	else if (EXTEND_SUSPECT == gdsfilext_code)		\
		status = (enum cdb_sc)cdb_sc_extend;		\
	else if (NO_FREE_SPACE == gdsfilext_code)		\
		status = cdb_sc_gbloflow;			\
	else if (FINAL_RETRY_FREEZE_PROG == gdsfilext_code)	\
		status = cdb_sc_needcrit;			\
}

#define MASTER_MAP_BITS_PER_LMAP	1

int4 bml_find_free(int4 hint, uchar_ptr_t base_addr, int4 total_bits);
int4 bml_init(block_id bml);
uint4 bml_busy(uint4 setbusy, sm_uc_ptr_t map);
uint4 bml_free(uint4 setfree, sm_uc_ptr_t map);
uint4 bml_recycled(uint4 setfree, sm_uc_ptr_t map);

