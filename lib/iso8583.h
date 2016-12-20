#ifndef ISO8583
#define ISO8583

struct  ISO_8583 {
        int             len;    /* data element max length */
        unsigned char   type;   /* bit 0--x, bit 1--n左对齐, bit 2--z bit 3--b*/
        unsigned char   flag;   /* length field length: 1--LLVAR型 2--LLLVAR型*/
};

struct data_element_flag {
        short bitf;
        short len;
        int   dbuf_addr;
};

typedef struct  {
        struct  data_element_flag f[128];
        short   off;
        char    dbuf[1024];
        char    message_id[10];
} ISO_data;

/*
struct len_str {
        short  len;
        char   *str;
};
*/

//struct element_struc {
//        short bitno;           /* element no */
//        short type;            /* 0--default value, 1,2--process function */
//        short len;             /* when type=0, len=length of defaule value */
//        void  *pointer;        /* pointer of value or process function */
//};
//
//struct  trans_bitmap_struc {
//        short trans_type;                /* transaction type */
//        char  message_id[10];
//        short element_number;            /* number of elememts */
//        struct element_struc *element;   /* transaction element ally */
//
//};

extern struct ISO_8583  *iso8583;

/**** for 64 ****/
int strtoiso ( unsigned char * dstr , ISO_data * iso ) ;
int isotostr ( unsigned char * dstr , ISO_data * iso ) ;
void WritePosData(ISO_data *iso, int flag);

/**** for 128 ****/
int strtoiso_128 ( unsigned char * dstr , ISO_data * iso ) ;
int isotostr_128 ( unsigned char * dstr , ISO_data * iso ) ;
void WritePosData_128(ISO_data *iso, int flag);


int setbit ( ISO_data * iso , int n , char * str , int len ) ;
int getbit ( ISO_data * iso , int n , char * str ) ;
void clearbit ( ISO_data * iso ) ;
void clearonebit ( ISO_data * iso, int bit ) ;
void WriteHexLog(char *spRawBuf, int vtLen);
int isotomacstr ( unsigned char * dstr, ISO_data * iso );

#endif
