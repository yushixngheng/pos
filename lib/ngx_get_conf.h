#ifndef _NGX_GET_CONF_H_
#define _NGX_GET_CONF_H_


int ngx_get_conf(char* file, char* val_name, char* value);
int read_conf( char *path, char * in , char * out );
int GetConf(char *file, char *sec, char *key, char *result);
int get_param( char *path, char * val_name, char * value);
int file_line_change(char* mht_path, char *str1, char* str2);

#endif