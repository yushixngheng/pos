#ifndef _ISO8583TEST_H_
#define _ISO8583TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct ISO8583test {
       short bit_flag;                      /*    位图位       */
       char data_name[37];           /*    域    名       */
       short length;                        /*    长    度       */
       short attribute;                    /*     属    性      */
       short variable_flag;             /*     变     长      */
       short length_in_byte;          /*     变长长度   */
       char *data;                        /*     域的数据   */
};
extern struct ISO8583test master[128];

extern int unpack_master_8583(char *buf);
extern void protocal_master_8583(char *p);
extern int debug_master_8583();

#endif