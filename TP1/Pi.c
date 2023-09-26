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

int sendTCP(int sock, void *msg, int sizeMsg){
  int w = send(sock, msg, sizeMsg, 0);
  if(w == EBADF){return 0;} //Socket fermée
  else if(w == -1){return -1;}  //Erreur du dépôt
  else if(w == sizeMsg){return w;} //Dépôt réussi
  else {return w+sendTCP(sock,msg+w,sizeMsg-w);} //Dépôt incomplet
}

int recvTCP(int sock, void *msg, int sizeMsg){
  int r = recv(sock, msg, sizeMsg, 0);
  if(r == EBADF){return 0;} //Socket fermée
  else if(r == -1){return -1;}  //Erreur de lecture
  else return r; //Lecture réussi
}

int main(int argc, char *argv[]) {

  if (argc != 4){
    printf("utilisation : numProcess adresse_config port_config\n");
    exit(0);
  }



    //ETAPE 1 : CREATION DE LE SOCKET D'ECOUTE (TCP)






    //ETAPE 2 : CREATION DE L'ANNEAU - CONNEXION A Pconfig (UDP)

    int monNum = atoi(argv[1]);

    int dsUDP = socket(PF_INET, SOCK_DGRAM, 0);
    if (dsUDP == -1){
      perror("P%i : pb creation socket d'écoute\n",monNum);
      close(dsUDP);
      exit(1); // Arrêt du prog car le reste dépend de la réussite de cette instruction
    } else {printf("P%i : creation de la socket réussie \n",monNum);}

    struct sockaddr_in addrConfig;
    addrServeur.sin_family = AF_INET;
    addrServeur.sin_addr.s_addr = inet_ntoa(atoi(argv[2]));
    addrServeur.sin_port = htons(atoi(argv[3]));

    socklen_t lgA = sizeof(struct sockaddr_in);

    //Envoi des informations à Pconfig
    int sendtos = sendto(dSUDP,&monNum,sizeof(int),0, (sockaddr*)addrConfig, lgA);
    if(sendtos==-1){
        perror("P%i : Erreur à l'envoi\n",monNum);
        exit(1);
    }
    else {
        printf("P%i : Numéro et informations envoyés à Pconfig",monNum); 
    }

    struct sockaddr_in sockaddrRetour;
    struct sockaddr_in sockProcSuiv;

    //Récéption de la socket du Pi+1
    int recvfroms = recvfrom(dSUDP, (struct sockaddr *)&sockProcSuiv, 2024,0,(struct sockaddr *)&sockaddrRetour,&lgA);
    if(recvfroms==-1){
        perror("P%i : Erreur du retour\n",monNum);
        exit(1);
    } else {
        printf("P%i : Récéption des informations de mon successeur",monNum);
    }

    //ETAPE 3 : CALCUL DE LA TAILLE DE L'ANNEAU (TCP)



}