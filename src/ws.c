#include <stdio.h>
#include <stdbool.h>
#include <microhttpd.h>
#include "db.h"

struct postStatus {
    bool status;
    char *buff2;
};

static int print_out_key (void *cls, enum MHD_ValueKind kind, const char *key, const char *value) {
  //printf("%s: %s\n", key, value);
  //fflush(stdout);
  return MHD_YES;
}

int handle_request (void *_request_handler, 
      struct MHD_Connection *connection, const char *url,
      const char *method, const char *version, const char *upload_data,
      size_t *upload_data_size, void **con_cls) {

  struct MHD_Response *response;

  int code = 200;
  int ret = MHD_YES;

  MHD_get_connection_values(connection, MHD_HEADER_KIND, print_out_key, NULL);

  char *buff = NULL;

  struct postStatus *post = NULL;
  post = (struct postStatus*)*con_cls;

  if(post == NULL) {
    post = malloc(sizeof(struct postStatus));
    post->status = false;
    *con_cls = post;
  }

  if(!post->status) {
    post->status = true;
    return MHD_YES;
  } else {
    if(*upload_data_size != 0) {
        post->buff2 = malloc(*upload_data_size + 1);
        strncpy(post->buff2, upload_data, *upload_data_size);
        *upload_data_size = 0;
        return MHD_YES;
    } else {
        buff = malloc(strlen(post->buff2));
        strncpy(buff, post->buff2, strlen(post->buff2));
        free(post->buff2);
    }
  } 

  if(post != NULL)
    free(post);

  char *pingResult;
  size_t pingResult_len;
  int pingReturnValue = pingDB(&pingResult, &pingResult_len);
  if(pingResult) {
    free(buff);
    buff = malloc(strlen(pingResult) + 1);
    strcpy(buff, pingResult);
    free(pingResult);
  }

  response = MHD_create_response_from_buffer(strlen(buff), (void*) buff, MHD_RESPMEM_MUST_FREE);
  if (code == 200)
      MHD_add_response_header(response, "Content-Type", "application/json; charset=utf8");
  else
    MHD_add_response_header (response, "Content-Type", "text/plain; charset=utf8");

  MHD_add_response_header (response, "Cache-Control", "no-cache");
  ret = MHD_queue_response (connection, code, response);
  MHD_destroy_response (response);

  return ret;
}