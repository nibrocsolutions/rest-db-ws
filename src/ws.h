#include <stdlib.h> 

#ifndef ws_h
#define ws_h

int print_out_key(void *cls, enum MHD_ValueKind kind, const char *key, const char *value);
int handle_request(void *_request_handler,
                   struct MHD_Connection *connection, const char *url,
                   const char *method, const char *version, const char *upload_data,
                   size_t *upload_data_size, void **con_cls);

#endif