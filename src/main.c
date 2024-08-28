#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "ws.h"
#include <microhttpd.h>
#include <stdbool.h>

int main (int argc, char **argv) {
  bool shutdown_requested = false;
  char *host = strdup("0.0.0.0");
  int port = 8080;
  
  sigset_t base_mask, waiting_mask;

  sigemptyset (&base_mask);
  sigaddset (&base_mask, SIGINT);
  sigaddset (&base_mask, SIGTSTP);
  sigaddset (&base_mask, SIGHUP);
  sigaddset (&base_mask, SIGQUIT);
  sigprocmask (SIG_SETMASK, &base_mask, NULL);

  struct MHD_Daemon *daemon = MHD_start_daemon 
	(MHD_USE_THREAD_PER_CONNECTION, port, NULL, NULL,
	handle_request, NULL, MHD_OPTION_END);

  if (daemon) {
      printf("HTTP server started.\n");
      fflush(stdout);
      while (!shutdown_requested) {
	      usleep(1000000);
	      sigpending (&waiting_mask);
	      if (sigismember (&waiting_mask, SIGINT) ||
	        sigismember (&waiting_mask, SIGTSTP) ||
	        sigismember (&waiting_mask, SIGQUIT) ||
	        sigismember (&waiting_mask, SIGHUP)) {
	          shutdown_requested = true;
	      }
	    }

      printf("HTTP server stopping\n");
      fflush(stdout);

      MHD_stop_daemon (daemon);
  } else {
      printf("Can't start HTTP server (check port %d is not in use)\n", port);
  }
  free(host);
}