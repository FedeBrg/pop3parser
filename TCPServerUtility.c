#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <ctype.h>
#include "messi.h"
#include <unistd.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int SetupTCPServerSocket(const char *service) {
  // Construct the server address structure
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

  int servSock = -1;
  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create a TCP socket
    servSock = socket(addr->ai_family, addr->ai_socktype,
        addr->ai_protocol);
    if (servSock < 0)
      continue;       // Socket creation failed; try next address

    // Bind to the local address and set socket to listen
    if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
        (listen(servSock, MAXPENDING) == 0)) {
      // Print local address of socket
      struct sockaddr_storage localAddr;
      socklen_t addrSize = sizeof(localAddr);
      if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
        DieWithSystemMessage("getsockname() failed");
      fputs("Binding to ", stdout);
      PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
      fputc('\n', stdout);
      break;       // Bind and listen successful
    }

    close(servSock);  // Close and try again
    servSock = -1;
  }

  // Free address list allocated by getaddrinfo()
  freeaddrinfo(servAddr);

  return servSock;
}

int AcceptTCPConnection(int servSock) {
  struct sockaddr_storage clntAddr; // Client address
  // Set length of client address structure (in-out parameter)
  socklen_t clntAddrLen = sizeof(clntAddr);

  // Wait for a client to connect
  int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
  if (clntSock < 0)
    DieWithSystemMessage("accept() failed");

  // clntSock is connected to a client!

  fputs("Handling client ", stdout);
  PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
  fputc('\n', stdout);

  return clntSock;
}

// void HandleTCPClient(int clntSocket) {
//   char buffer[BUFSIZE]; // Buffer for echo string
//   // char s[4] = "110\0";
//   // int mailSock = SetupTCPServerSocket(s);


//   int mailSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   if (mailSock < 0)
//     DieWithSystemMessage("socket() failed");

//   // Construct the server address structure
//   struct sockaddr_in servAddr;            // Server address
//   memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
//   servAddr.sin_family = AF_INET;          // IPv4 address family
//   // Convert address
//   int rtnVal = inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);
//   if (rtnVal == 0)
//     DieWithUserMessage("inet_pton() failed", "invalid address string");
//   else if (rtnVal < 0)
//     DieWithSystemMessage("inet_pton() failed");
//   servAddr.sin_port = htons(110);    // Server port

//   // Establish the connection to the echo server
//   if (connect(mailSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
//     DieWithSystemMessage("connect() failed");


//   // Receive message from client
//   while(true){

//     ssize_t numBytesRcvd = recv(mailSock, buffer, BUFSIZE, 0);


//     if (numBytesRcvd < 0)
//       DieWithSystemMessage("recv() failed");

//     // Send received string and receive again until end of stream
//     while (numBytesRcvd > 0) { // 0 indicates end of stream
//       // Echo message back to client
//       ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
//       if (numBytesSent < 0){
//                 DieWithSystemMessage("HOLI");

//         DieWithSystemMessage("send() failed");
//       }
//       else if (numBytesSent != numBytesRcvd)
//         DieWithUserMessage("send()", "sent unexpected number of bytes");
//       // See if there is more data to receive
//       if(strstr(buffer, "out.") != NULL){
//         exit(0);
//       }
//       if(strstr(buffer, ".") != NULL){

//         break;
//       }

//       numBytesRcvd = recv(mailSock, buffer, BUFSIZE,0 );
//            fprintf(stderr,"NUMBYTESRECIEVED: %ld",numBytesRcvd);


//       if (numBytesRcvd < 0)
//         DieWithSystemMessage("recv() failed");
//     }






//      numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);


//     if (numBytesRcvd < 0)
//       DieWithSystemMessage("recv() failed");

//     // Send received string and receive again until end of stream
//     while (numBytesRcvd > 0) { // 0 indicates end of stream
//       // Echo message back to client
//       ssize_t numBytesSent = send(mailSock, buffer, numBytesRcvd, 0);
//       if (numBytesSent < 0){
//         DieWithSystemMessage("HOLIO");
//         DieWithSystemMessage("send() failed");
//       }
//       else if (numBytesSent != numBytesRcvd)
//         DieWithUserMessage("send()", "sent unexpected number of bytes");

//       // See if there is more data to receive

//       // if(strstr(buffer, '\n') != NULL){

//       //   break;
//       // }
//       numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, MSG_DONTWAIT);

//       // if (numBytesRcvd < 0)
//       //   DieWithSystemMessage("recv() failed");
//     }



//   }

//   close(clntSocket); // Close client socket
//   close(mailSock);
// }


void waiting_for_body(int mailSock,int clntSocket){
    char buffer[BUFSIZE]; // Buffer for echo string

  regex_t regex;
  int reti;

  reti = regcomp(&regex, ".*\r\n[.]\r\n", 0);
if (reti) {
    fprintf(stderr, "Could not compile regex\n");
    exit(1);
}

  while(true){
    fputs("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",stderr);
    ssize_t numBytesRcvd = recv(mailSock, buffer, BUFSIZE, 0);


    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() failed");

    // Send received string and receive again until end of stream
    while (numBytesRcvd > 0) { // 0 indicates end of stream
      // Echo message back to client
      for(int i = 0; i<BUFSIZE; i++){
        buffer[i] = toupper(buffer[i]);
      }
    fputs(buffer,stderr);

      ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
      if (numBytesSent < 0){
                DieWithSystemMessage("HOLI");

        DieWithSystemMessage("send() failed");
      }
      else if (numBytesSent != numBytesRcvd)
        DieWithUserMessage("send()", "sent unexpected number of bytes");
      // See if there is more data to receive

      reti = regexec(&regex, buffer, 0, NULL, 0);
      if (!reti) {
        fputs("EXIT",stderr);
        regfree(&regex);
        return;
      }
      numBytesRcvd = recv(mailSock, buffer, BUFSIZE,0 );
           fprintf(stderr,"NUMBYTESRECIEVED: %ld",numBytesRcvd);


      if (numBytesRcvd < 0)
        DieWithSystemMessage("recv() failed");
    }


  }
}


// void HandleTCPClient(int clntSocket) {
//   char buffer[BUFSIZE]; // Buffer for echo string
//   // char s[4] = "110\0";
//   // int mailSock = SetupTCPServerSocket(s);

//   int mailSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   if (mailSock < 0)
//     DieWithSystemMessage("socket() failed");

//   // Construct the server address structure
//   struct sockaddr_in servAddr;            // Server address
//   memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
//   servAddr.sin_family = AF_INET;          // IPv4 address family
//   // Convert address
//   int rtnVal = inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);
//   if (rtnVal == 0)
//     DieWithUserMessage("inet_pton() failed", "invalid address string");
//   else if (rtnVal < 0)
//     DieWithSystemMessage("inet_pton() failed");
//   servAddr.sin_port = htons(110);    // Server port

//   // Establish the connection to the echo server
//   if (connect(mailSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
//     DieWithSystemMessage("connect() failed");


//   // Receive message from client
//   while(true){

//     ssize_t numBytesRcvd = recv(mailSock, buffer, BUFSIZE, 0);


//     if (numBytesRcvd < 0)
//       DieWithSystemMessage("recv() failed");

//     // Send received string and receive again until end of stream
//     while (numBytesRcvd > 0) { // 0 indicates end of stream
//       // Echo message back to client
//       ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
//       if (numBytesSent < 0){
//                 DieWithSystemMessage("HOLI");

//         DieWithSystemMessage("send() failed");
//       }
//       else if (numBytesSent != numBytesRcvd)
//         DieWithUserMessage("send()", "sent unexpected number of bytes");
//       // See if there is more data to receive

//       if(strstr(buffer, ".") != NULL){

//         break;
//       }

//       numBytesRcvd = recv(mailSock, buffer, BUFSIZE,0 );
//            fprintf(stderr,"NUMBYTESRECIEVED: %ld",numBytesRcvd);


//       if (numBytesRcvd < 0)
//         DieWithSystemMessage("recv() failed");
//     }






//      numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);


//     if (numBytesRcvd < 0)
//       DieWithSystemMessage("recv() failed");

//     // Send received string and receive again until end of stream
//     while (numBytesRcvd > 0) { // 0 indicates end of stream
//       // Echo message back to client
//       ssize_t numBytesSent = send(mailSock, buffer, numBytesRcvd, 0);
//       if (numBytesSent < 0){
//         DieWithSystemMessage("HOLIO");
//         DieWithSystemMessage("send() failed");
//       }
//       else if (numBytesSent != numBytesRcvd)
//         DieWithUserMessage("send()", "sent unexpected number of bytes");

//       // See if there is more data to receive
//       fputs(buffer,stderr);
//       if(strstr(buffer,"RETR")){
//         waiting_for_body(mailSock,clntSocket);
//         break;
//       }
//       numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, MSG_DONTWAIT);

//       // if (numBytesRcvd < 0)
//       //   DieWithSystemMessage("recv() failed");
//     }

//   }

//   close(clntSocket); // Close client socket
//   close(mailSock);
// }


enum parser_state mail_server_read(struct pop3_parser * p,char c, enum parser_state state){

  switch(state){
    case parsing_single_line:
      return single_line_read(p,c);
    case parsing_multi_line:
      return multi_line_read(p,c);
  }

}

void enum_to_str(int i){
  switch(i){
    case parsing_cmd:
      printf("PARSING CMD\n");
    break;

    case parsing_single_line:
      printf("PARSING SINGLE LINE\n");
    break;

    case parsing_multi_line:
      printf("PARSING MULTI LINE\n");
    break;

    case parsing_retr:
      printf("PARSING RETR\n");
    break;

    case parser_error:
      printf("PARSER ERROR");
    break;

    case exit_parser:
      printf("EXIT PARSER\n");
        break;

  }
}

void clear_buffer(char * buff){
  for(int i = 0; i<BUFSIZE;i++){
    buff[i] = '\0';
  }
}


void HandleTCPClient(int clntSocket) {
  char buffer[BUFSIZE]; // Buffer for echo string
  // char s[4] = "110\0";
  // int mailSock = SetupTCPServerSocket(s);

  int mailSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (mailSock < 0)
    DieWithSystemMessage("socket() failed");

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // Convert address
  int rtnVal = inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr.s_addr);
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(110);    // Server port

  // Establish the connection to the echo server
  if (connect(mailSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("connect() failed");

    struct single_line_state_machine slsm = {
    .state = reading,
  };

  struct multi_line_state_machine mlsm = {
    .state = reading1,
  };

  struct cmd_state_machine cmsm = {
    .state = reading,
  };

  struct pop3_parser p = {
    .sl_sm = slsm,
    .ml_sm = mlsm,
    .cmd_sm = cmsm,
    .state = parsing_single_line,
    .cmd = "\0\0\0\0\0\0\0\0\0\0",
  };

  int i = 0;

    enum parser_state initial_state = p.state;



    enum parser_state new_state = p.state;
    // fprintf(stderr, "INITIAL STATE: " );
    // enum_to_str(initial_state);

    // fprintf(stderr, "NEW STATE: " );
    // enum_to_str(new_state);

    bool quit = false;

  while(true){

    fprintf(stderr,"1\n");


    while(new_state == initial_state){
    fprintf(stderr,"2\n");

      ssize_t numBytesRcvd = recv(mailSock, buffer, BUFSIZE, 0);
    fprintf(stderr,"3\n");

      while(i<numBytesRcvd){

        new_state = mail_server_read(&p,buffer[i],new_state);
        i++;
      }

          if(quit){
        break;
    }
          fprintf(stderr,"4\n");
              fprintf(stderr, "INITIAL STATE: " );
    enum_to_str(initial_state);

    fprintf(stderr, "NEW STATE: " );
    enum_to_str(new_state);

      //fprintf(stderr, "BUFFER: %s SIZE: %d\n",buffer,numBytesRcvd );
      send(clntSocket, buffer, numBytesRcvd, 0);
      clear_buffer(buffer);
      i=0;
    }

          fprintf(stderr,"5\n");


    i = 0;

    if(quit){
        break;
    }

    // fprintf(stderr, "INITIAL STATE: " );
    // enum_to_str(initial_state);

    // fprintf(stderr, "NEW STATE: " );
    // enum_to_str(new_state);

    initial_state = new_state;


    while(new_state == initial_state){
    fprintf(stderr,"6\n");

      ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
          fprintf(stderr,"7\n");

      while(i<numBytesRcvd){
        //fprintf(stderr, "%c\n",buffer[i] );

        new_state = cmd_read(&p,buffer[i]);
        i++;
      }
      //fprintf(stderr, "%s\n", buffer);
                fprintf(stderr,"8\n");

          fprintf(stderr, "INITIAL STATE: " );
    enum_to_str(initial_state);

    fprintf(stderr, "NEW STATE: " );
    enum_to_str(new_state);
    sleep(1);

      send(mailSock, buffer, numBytesRcvd, 0);
            clear_buffer(buffer);

      i = 0;
    }
        fprintf(stderr,"9\n");

        initial_state = new_state;
        i=0;

        if(initial_state == exit_parser){
          quit = true;
        }

  }

  close(clntSocket); // Close client socket
  close(mailSock);
  fprintf(stderr,"Chauchis");
  exit(0);
}