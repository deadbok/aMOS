/* Section Header: Module magic */
#define		AMOD_MH_MAGIC0			0x61
#define		AMOD_MH_MAGIC1			0x4d
#define		AMOD_MH_MAGIC2			0x4f
#define		AMOD_MH_MAGIC3			0x53
#define		AMOD_MH_MAGIC4			0x6d
#define		AMOD_MH_MAGIC5			0x6f
#define		AMOD_MH_MAGIC6			0x64
#define		AMOD_MH_MAGIC7			0x75
#define		AMOD_MH_MAGIC8			0x6c
#define		AMOD_MH_MAGIC9			0x65

/* Module Header:  Module format version */
#define		AMOD_MH_CUR_VER0		0
#define		AMOD_MH_CUR_VER1		0
#define		AMOD_MH_CUR_VER2		0
#define		AMOD_MH_CUR_VER3		1

/* Module Header:  Architecture ID's */
#define		AMOD_MH_ARCH_UNKNOWN	0 /* Invalid */
#define		AMOD_MH_ARCH_I386		1

/* Module Header:  Architecture bits */
#define		AMOD_MH_NIT_UNKNOWN		0 /* Invalid */
#define		AMOD_MH_BIT_32			32
#define		AMOD_MH_BIT_64			64

/* Module Header */
struct module_header
{
	char			mh_magic[10];
	unsigned short	mh_version[4];
	unsigned long	mh_size;
	unsigned long	mh_id[2];
	unsigned char	mh_bit;
	unsigned long	mh_st_offset;
	unsigned long	mh_st_entries;
	unsigned long	mh_st_entsize;
}

/* Section Table: MD5 calculation */
#define		AMOD_ST_MD5_UNKNOWN	0
#define		AMOD_ST_MD5_CALC	1
#define		AMOD_ST_MD5_NOCALC	2

/* Section Table: entry */
struct st_entry
{
	unsigned long	st_offset;
	unsigned long	st_size;
	unsigned char	st_md5_calc;
}


