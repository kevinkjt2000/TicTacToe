/*******************************************************************
 *   Filename: prog1.cpp                           Date: 2/12/2010 *
 *   Author: Kevin Tindall                                         * 
 *-----------------------------------------------------------------*
 *                      Tic-Tac-Toe Game                           *
 *-----------------------------------------------------------------*
 *   This program uses the TicTacToe and Player classes to play a  *
 * game of Tic-Tac-Toe between two people by reading input from    *
 * the user through the console.  Two names for the player are     *
 * entered and one of the players is randomly selected to go       *
 * first.  The players then take turns selecting spots on the      *
 * the game board until one of them has won or all the spots are   *
 * taken and neither player has won.                               *
 *******************************************************************/

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#ifdef WIN32
   #include <Winsock2.h>
#else
   #include <sys/types.h>
   #include <sys/socket.h>
   #include <netdb.h>
   #include <netinet/in.h>
   #include <unistd.h>

   #define closesocket close
   #define SOCKET int
#endif

#include "TicTacToe.h"

void graceful_exit(int exit_code)
{
   #ifdef WIN32
      WSACleanup();
   #endif
   exit(exit_code);
}

// u_long stringIP_to_intIP(char* chars)
// {
   // u_long ret_value = 0;
   // char *curr = strtok(chars, ".");
   // int shift = 24;
   // while(curr != NULL)
   // {
      // ret_value += atoi(curr) << shift;
      // curr = strtok(NULL , ".");
      // shift -= 8;
   // }
   // return ret_value;
// }

SOCKET start_server(char *IP, char *port)
{
   struct hostent* hent = gethostbyname(IP);
   if(hent == NULL)
   #ifdef WIN32
      return INVALID_SOCKET;
   #else
      return -1;
   #endif
   
   SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   #ifdef WIN32
      if(sockfd == INVALID_SOCKET)
         return INVALID_SOCKET;
   #else
      if(sockfd < 0)
         return -1;
   #endif
   
   struct sockaddr_in server_addr;
   memset((void *) &server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //accept any incomping connection address
   server_addr.sin_port = htons(atoi(port));
   
   if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
   #ifdef WIN32
      return INVALID_SOCKET;
   #else
      return -1;
   #endif
   
   if(listen(sockfd, 10) != 0)
   #ifdef WIN32
      return INVALID_SOCKET;
   #else
      return -1;
   #endif
   
   return sockfd;
}

SOCKET connect_to_server(char *IP, char *port)
{
   struct hostent* hent = gethostbyname(IP);
   if(hent == NULL)
   #ifdef WIN32
      return INVALID_SOCKET;
   #else
      return -1;
   #endif
   
   SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   #ifdef WIN32
      if(sockfd == INVALID_SOCKET)
         return INVALID_SOCKET;
   #else
      if(sockfd < 0)
         return -1;
   #endif
   
   struct sockaddr_in server_addr;
   memset((void *) &server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = AF_INET;
   server_addr.sin_addr = *((struct in_addr *)hent->h_addr);
   server_addr.sin_port = htons(atoi(port));
   
   #ifdef WIN32
      if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == SOCKET_ERROR)
   #else
      if(connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0)
   #endif
   {
      closesocket(sockfd);
      #ifdef WIN32
         return INVALID_SOCKET;
      #else
         return -1;
      #endif
   }
   return sockfd;
}

void send_msg(SOCKET sock, char *msg)
{
   #ifdef WIN32
      if((send(sock, msg, strlen(msg), 0)) == SOCKET_ERROR)
   #else
      if((send(sock, msg, strlen(msg), 0)) < 0)
   #endif
   {
      std::cerr << "Send error." << std::endl;
   }
   
   //closesocket(sock);
}  

char* get_msg(SOCKET sock)
{
   char *buffer = new char[1024];
   memset(buffer, '\0', 1024); // Clear the buffer.
   
   #ifdef WIN32
      if((recv(sock, buffer, 1023, 0)) == SOCKET_ERROR)
   #else
      if((recv(sock, buffer, 1023, 0)) < 0)
   #endif
   {
      std::cerr << "Receive error." << std::endl;
   }

   //closesocket(sock);
   return buffer;
}

SOCKET accept_connection(SOCKET sock)
{
   struct sockaddr_in cli_addr;
   #ifdef WIN32
      int clilen;
   #else
      unsigned int clilen;
   #endif
   clilen = sizeof(cli_addr);
   SOCKET sock_accept = accept(sock, (struct sockaddr *) &cli_addr, &clilen);
   return sock_accept;
}

int main(int argc, char** argv)
{
   // Check for correct commandline input.
   if(argc < 3)
   {
      std::cerr << "Usage: " << argv[0] << " [server name] [port]\n";
      exit(0);
   }
   
   srand(time(NULL)); //just to make things random
   
   bool isServer = false;
   
   std::cout << "Welcome to Network Tic-Tac-Toe\n------------------------------\n\n";
   
   //Attempt connecting to the server passed as argv[1] on port argv[2]
   #ifdef WIN32
      WSADATA wsaData;
      WSAStartup(MAKEWORD(2, 2), &wsaData);
   #endif
   SOCKET serverSock = connect_to_server(argv[1], argv[2]);
   SOCKET clientSock;
   #ifdef WIN32
      if(serverSock == INVALID_SOCKET)
   #else
      if(serverSock < 0)
   #endif
   {
      isServer = true;
      std::cout << "Setting up server...\n";
      closesocket(serverSock);
      serverSock = start_server(argv[1], argv[2]);
      #ifdef WIN32
         if(serverSock == INVALID_SOCKET)
      #else
         if(serverSock < 0)
      #endif
      {
         std::cerr << "Error starting server.\n";
         closesocket(serverSock);
         graceful_exit(1);
      }
      
      std::cout << "Waiting for someone to connect...\n";
      clientSock = accept_connection(serverSock);
      #ifdef WIN32
         if(clientSock == INVALID_SOCKET)
      #else
         if(clientSock < 0)
      #endif
      {
         std::cerr << "Accept error.\n";
         closesocket(serverSock);
         graceful_exit(1);
      }
   }
   
   bool goAgain = false;
   do{
   
   TicTacToe game;
      char msg_ready[] = "READY";
      char msg_yes[] = "YES";
      char msg_no[] = "NO";
      char msg_turn[] = "TURN";
   
   if(!isServer)
   {
      bool isReady = false;
      while(!isReady)
      {
         char msg[1024];
         std::cout << "Press Y <enter> when ready: ";
         std::cin.getline(msg, 1023);
         for(char *iter = msg; *iter != '\0'; iter++)
         {
            *iter = toupper(*iter);
         }
         isReady = (strcmp(msg, "Y") == 0);
      }
      send_msg(serverSock, msg_ready);
   }
   
   bool isPartnerReady = false;
   std::cout << "Waiting for partner to say ready...\n";
   while(!isPartnerReady)
   {
      char *msg;
      if(isServer)
         msg = get_msg(clientSock);
      else
         msg = get_msg(serverSock);
      for(char *iter = msg; *iter != '\0'; iter++)
      {
         *iter = toupper(*iter);
      }
      isPartnerReady = (strcmp(msg, "READY") == 0);
      delete msg;
   }
   
   if(isServer)
   {
      bool isReady = false;
      while(!isReady)
      {
         char msg[1024];
         std::cout << "Press Y <enter> when ready: ";
         std::cin.getline(msg, 1023);
         for(char *iter = msg; *iter != '\0'; iter++)
         {
            *iter = toupper(*iter);
         }
         isReady = (strcmp(msg, "Y") == 0);
      }
      send_msg(clientSock, msg_ready);
   }
   else
   {
      bool isNameGiven = false;
      while(!isNameGiven)
      {
         char msg[1024] = "NAME ";
         std::cout << "Please enter your name: ";
         std::cin.getline(msg+5*sizeof(char), 1016);
         isNameGiven = (strcmp(msg+5*sizeof(char), "") != 0);
         if(isNameGiven)
            send_msg(serverSock, msg);
      }
   }
   
   bool isPartnerNamed = false;
   std::cout << "Waiting for partner to enter their name...\n";
   while(!isPartnerNamed)
   {
      char *msg;
      if(isServer)
         msg = get_msg(clientSock);
      else
         msg = get_msg(serverSock);
      isPartnerNamed = (memcmp(msg, "NAME ", 5) == 0 && isgraph(msg[5]));
      if(isPartnerNamed)
         std::cerr << msg << std::endl;
      delete msg;
   }
   
   int turn;
   if(isServer)
   {
      bool isNameGiven = false;
      while(!isNameGiven)
      {
         char msg[1024] = "NAME ";
         std::cout << "Please enter your name: ";
         std::cin.getline(msg+5*sizeof(char), 1016);
         isNameGiven = (strcmp(msg+5*sizeof(char), "") != 0);
         if(isNameGiven)
            send_msg(clientSock, msg);
      }
      
      bool isClientReady = false;
      while(!isClientReady)
      {
         char *msg = get_msg(clientSock);
         isClientReady = (strcmp(msg, "TURN") == 0);
         delete msg;
      }
      
      std::cout << "Deciding who has the first turn...\n";
      turn = rand() % 2;
      char msg[] = {turn + '0', '\0'};
      send_msg(clientSock, msg);
   }
   else
   {
      send_msg(serverSock, msg_turn);
      std::cout << "The server is deciding who has the first turn...\n";
      bool isTurnOrderDecided = false;
      while(!isTurnOrderDecided)
      {
         char *msg = get_msg(serverSock);
         isTurnOrderDecided = (atoi(msg) == 0 || atoi(msg) == 1);
         if(isTurnOrderDecided)
            turn = atoi(msg);
         delete msg;
      }
   }
   
   bool isServerFirst = (turn == 0);
   
   // std::string name = ""; //for reading a name inputted by the user
   // while(name == "")
   // {
      // std::cout << "Please enter your name: ";
      // std::cin >> name;
      // std::cin.ignore(80, '\n');
   // }
   
   // std::string opponentName; //for storing the opponents name
   // while(opponentName == name)
   // {
      // std::cout << "Please enter a name that is different from your opponent's name: ";
      // std::cin >> name;
      // std::cin.ignore(80, '\n');
   // }
   // std::cout << "\n";
   
   //output who won the toss
   //std::cerr << turn;
   if((isServerFirst && isServer) || (!isServerFirst && !isServer))
      std::cout << "You won the first move.\n";
   else
      std::cout << "Your oppenent has the first move.\n";
   
   while(game.GetStatus() == 3) //status code 3 means no winner or tie, which implies the game is still going
   {
      //display the game board
      std::cout << game;
      
      //get and apply the next move
      if(turn % 2 == 0)
      {
         if(isServer)
         {
            //let the server make a move here
            std::cout << "It is your turn to make a move...\n";
            int row = 0, col = 0;
            while((row < 1 || row > 3 || col < 1 || col > 3) || (!game.SetValue(row-1, col-1, (turn%2) + 1)))
            {
               std::cout << "Enter row <space> column (both have to be 1-3 and the location must be empty): ";
               std::cin >> row >> col;
            }
            char msg[] = {row + '0', ' ', col + '0', '\0'};
            send_msg(clientSock, msg);
         }
         else
         {
            //client waits until server makes a move here
            std::cout << "Your opponent is making a move...\n";
            char *msg = get_msg(serverSock);
            msg[1] = '\0';
            int row = atoi(msg);
            int col = atoi(msg+2);
            delete msg;
            game.SetValue(row-1, col-1, (turn%2) + 1);
         }
      }
      else
      {
         if(isServer)
         {
            //server waits until client makes a move here
            std::cout << "Your opponent is making a move...\n";
            char *msg = get_msg(clientSock);
            msg[1] = '\0';
            int row = atoi(msg);
            int col = atoi(msg+2);
            delete msg;
            game.SetValue(row-1, col-1, (turn%2) + 1);
         }
         else
         {
            //let the client make a move here
            std::cout << "It is your turn to make a move...\n";
            int row = 0, col = 0;
            while((row < 1 || row > 3 || col < 1 || col > 3) || (!game.SetValue(row-1, col-1, (turn%2) + 1)))
            {
               std::cout << "Enter row <space> column (both have to be 1-3 and the location must be empty): ";
               std::cin >> row >> col;
            }
            char msg[] = {row + '0', ' ', col + '0', '\0'};
            send_msg(serverSock, msg);
         }
      }
      
      turn++;
   }
   
   std::cout << game;
   switch(game.GetStatus())
   {
      case 0 :
         std::cout << "Game tied!";
         break;
      case 1 :
         std::cout << "X has won!";
         break;
      case 2 :
         std::cout << "O has won!";
         break;
   }
   std::cout << std::endl << std::endl;
   
   if(isServer)
   {
      std::cout << "Would you like to play again (Y/N)? ";
      char again[1024];
      do{
         std::cin.getline(again, 1023);
         for(char *iter = again; *iter != '\0'; iter++)
         {
            *iter = toupper(*iter);
         }
      } while((strcmp(again, "Y") != 0) && (strcmp(again, "N") != 0));
      goAgain = (strcmp(again, "Y") == 0);
      if(goAgain)
      {
         send_msg(clientSock, msg_yes);
         std::cout << "Waiting on other player to decide whether they want to go again or not...\n";
         char *msg = get_msg(clientSock);
         goAgain = (strcmp(msg, "YES") == 0);
         delete msg;
      }
      else
      {
         send_msg(clientSock, msg_no);
      }
   }
   else
   {
      std::cout << "Waiting on other player to decide whether they want to go again or not...\n";
      char *msg = get_msg(serverSock);
      goAgain = (strcmp(msg, "YES") == 0);
      delete msg;
      if(goAgain)
      {
         std::cout << "Would you like to play again (Y/N)? ";
         char again[1024];
         do{
            std::cin.getline(again, 1023);
            for(char *iter = again; *iter != '\0'; iter++)
            {
               *iter = toupper(*iter);
            }
         } while((strcmp(again, "Y") != 0) && (strcmp(again, "N") != 0));
         goAgain = (strcmp(again, "Y") == 0);
         if(goAgain)
         {
            send_msg(serverSock, msg_yes);
         }
         else
         {
            send_msg(serverSock, msg_no);
         }
      }
   }
   
   } while(goAgain);
   
   if(isServer)
      closesocket(clientSock);
   closesocket(serverSock);
   graceful_exit(0);
}
