#include <stdio.h> 
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_BUFFER_SIZE 146980

int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("utilisation : monAdresse monPort nbProcess\n");
    exit(0);
  }

int nbProcess = atoi(argv[3]);

int dsUDP = socket(PF_INET, SOCK_DGRAM, 0);
  if (dsUDP == -1){
    perror("Pconfig : pb creation socket d'écoute\n");
    close(dsUDP);
    exit(1); // Arrêt du prog car le reste dépend de la réussite de cette instruction
} else {printf("Pconfig : creation de la socket réussie \n");}


  struct sockaddr_in addrConfig;
  addrConfig.sin_family = AF_INET;
  addrConfig.sin_addr.s_addr = inet_ntoa(argv[1]);
  addrConfig.sin_port = htons(atoi(argv[2]));

  int bindReturn = bind(dsUDP,(struct sockaddr*)&addrConfig, lgA);
   if(bindReturn==-1){
    perror("Pconfig : Erreur à l'attribution du port\n");
    close(dsUDP);
    exit(1);
  } else {
    std::cout << "Pconfig : Attribution du port réussie\n";
  }

  struct sockaddr_in socketPi[nbProcess];
  int recvfroms,sendtos,rep;

  struct sockaddr_in sockaddrRetour;
  socklen_t lgRetour = sizeof(sockaddrRetour);
  socklen_t lgA = sizeof(struct sockaddr_in);

  for(int i=0;i<nbProcess;i++){
    //On attend l'arrivée d'un client UDP
    recvfroms = recvfrom(dsUDP, &rep, 2024,0,(struct sockaddr *)&sockaddrRetour,&lgRetour);
    if(recvfroms==-1){
        perror("Pconfig : Erreur du retour\n");
        exit(1);
    } else {
        printf("P%i a envoyé son numéro et ses informations",rep);
        socketPi[rep] = sockaddrRetour;
    }
  }

  for(int i=0;i<nbProcess-1;i++){
    //On envoie à tous le monde la socket du suivant
    sendtos = sendto(dsUDP,socketPi[i+1],sizeof(int),0, (sockaddr*)socketPi[i], lgA);
    if(sendtos==-1){
        perror("Moi : Erreur à l'envoi\n");
        exit(1);
    }
    else {
        printf("Pconfig : Successeur (%i) envoyé à P%i",i+1,i); 
    }
  }
  sendtos = sendto(dsUDP,socketPi[0],sizeof(int),0, (sockaddr*)socketPi[nbProcess], lgA);
    if(sendtos==-1){
        perror("Moi : Erreur à l'envoi\n");
        exit(1);
    }
    else {
        printf("Pconfig : Successeur (0) envoyé à P%i",nbProcess); 
    }

    printf("Pconfig : J'ai terminé mon travail, je termine");
    close(dsUDP);
}