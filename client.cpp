#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAXBUF 65536

#include "pubsubpeer.h"


int main(int argc, char*argv[])
{

    //PubSubPeer peer("192.168.13.78", 55555);



 /* int sock, status, buflen, sinlen;
  char buffer[MAXBUF];
  struct sockaddr_in sock_in;
  int yes = 1;

  sinlen = sizeof(struct sockaddr_in);
  memset(&sock_in, 0, sinlen);
  buflen = MAXBUF;

  sock = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  sock_in.sin_addr.s_addr = htonl(INADDR_ANY);
  sock_in.sin_port = htons(0);
  sock_in.sin_family = PF_INET;

  status = bind(sock, (struct sockaddr *)&sock_in, sinlen);
  printf("Bind Status = %d\n", status);


  status = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int) );
  printf("Setsockopt Status = %d\n", status);



  sock_in.sin_addr.s_addr=htonl(-1);
  sock_in.sin_port = htons(atoi(argv[1]));
  sock_in.sin_family = PF_INET;

  sprintf(buffer, "Ciao");
  buflen = strlen(buffer);
  status = sendto(sock, buffer, buflen, 0, (struct sockaddr *)&sock_in, sinlen);

  sock_in.sin_addr.s_addr;
  printf("sendto Status = %d\n", status);

  shutdown(sock, 2);
  close(sock);*/
}
