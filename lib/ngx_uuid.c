#include "ngx_core.h"

ngx_int_t
ngx_random_uuid(ngx_uuid_t* uuid) {
	char buf[37];
    const char *c = "89ab";
    char *p = buf;
    int n;

    memset(buf, 0, sizeof(buf));
    for( n = 0; n < 16; ++n ) {
        int b = rand()%255;
        switch( n ) {
            case 6:
                sprintf(p, "4%x", b%15 );
            break;
            case 8:
                sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
            break;
            default:
                sprintf(p, "%02x", b);
            break;
        }
        p += 2;
    }
    asc2hex(buf, NGX_UUID_LEN * 2, 0, uuid->bytes);
    //debug
    {
    	char auuid[37] = {0};
    	ngx_format_uuid(uuid, auuid);
    	ErrorLog(ERROR, "uuid:%s", auuid);
    }
    return NGX_OK;
}


ngx_int_t
ngx_format_uuid(ngx_uuid_t* uuid, char* str) {
		char buf[NGX_UUID_LEN * 2 + 1];
		char *p;
		int i;

		memset(buf, 0, sizeof(buf));
		hex2asc(uuid->bytes, NGX_UUID_LEN * 2, 0, buf);
		p = str;
		for (i = 0; i < strlen(buf); i++) {
			*p++ = buf[i];
	        switch(i) {
	            case 3:
	            case 5:
	            case 7:
	            case 9:
	                *p++ = '-';
	                break;
	        }
		}
		*p = 0;
	return NGX_OK;
}


