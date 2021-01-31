#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <errno.h>

int main(int argc, char const *argv[])
{
    /* Configuarion de la l'adresse du serveur */
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(30000);

    /* Création du socket */
    int socketClient = socket(AF_INET, SOCK_STREAM, 0);
    
    /* On gère l'erreur*/
    if(socketClient <= 0)
    {
        printf("Erreur lors de la création du socket.\n");
    }

    /* Demande de connxion au serveur */
    connect(socketClient, (const struct sockaddr *) &addrServer, sizeof(addrServer));
    printf("Connecté\n");

    /* La variable qui contiendra le temps donc le buffeur des données réçu */
    char time[64];

    /* Buffeur de reception de données du client au serveur (demande temps) */
    char *buffer = "Quelle heure est it ?.";

    /* Envoie de la demande de l'heure */
    send(socketClient, buffer, strlen(buffer), 0);
    printf("Demande de l'heure envoyée.\n");

    /* reception de la réponse du serveur */
    int receive = recv(socketClient, time, 512, 0);
    /* on gère l'erreur */
    if(receive <= 0)
    {
        printf("Erreur lors de la réception de la réponse du serveur.\b");
    }
    /* On affiche la réponse du serveur */
    printf("Il est : %s\n", time);

    close(socketClient);
    
    return 0;
}
