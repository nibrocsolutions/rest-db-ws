#include <mongoc/mongoc.h>
#include <stdio.h>

int pingDB(char **inarr, size_t *inarr_len) {
    const char *uri_string = "mongodb://172.17.0.2:27017";
    mongoc_uri_t *uri = NULL;
    mongoc_client_t *client = NULL;
    bson_t *command, reply;
    bson_error_t error;
    char *str;
    bool retval;
    mongoc_init();
    uri = mongoc_uri_new_with_error(uri_string, &error);
    if (!uri) {
        fprintf(stderr, "failed to parse URI: %s\n"
                        "error message:       %s\n",
                uri_string, error.message);
        fflush(stdout);
        //return EXIT_FAILURE;
    }

    client = mongoc_client_new_from_uri (uri);
    if (!client) {
      //return EXIT_FAILURE;
    }

    command = BCON_NEW ("ping", BCON_INT32 (1));

    retval = mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error);

    if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      fflush(stdout);
      //return EXIT_FAILURE;
    }

    str = bson_as_json (&reply, NULL);

    char *result = malloc((strlen(str) + 1 ) * sizeof(char));
    strcpy(result, str);

    bson_destroy(&reply);
    bson_destroy(command);
    bson_free(str);

    mongoc_uri_destroy(uri);
    mongoc_client_destroy(client);
    //mongoc_cleanup();

    *inarr = result;
    *inarr_len = sizeof(result);

    return 0;
}