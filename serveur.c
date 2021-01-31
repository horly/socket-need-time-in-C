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
#include <time.h>

/* La fonction qui nous permet de récupérer le temps actuel */
char *getTime()
{
    static char time_str[12]; //le temps qui sera retourné en chaine de caractère
    time_t now = time(NULL); //Initialisation du temps actuelle à null
    struct tm tm_now; //On crée une structure qui nous permettra de stoker le temps actuelle
    localtime_r(&now, &tm_now);

    strftime(time_str, sizeof(time_str), "%H:%M", &tm_now);

    return time_str;
}

int main(int argc, char const *argv[])
{
    /* Configuarion de la l'adresse du serveur */
    struct sockaddr_in addrServer;
    addrServer.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    addrServer.sin_family = AF_INET;
    addrServer.sin_port = htons(30000);

    /* Création du socket */
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    
    /* On gère l'erreur*/
    if(socketServer <= 0)
    {
        printf("Erreur lors de la création du socket.\n");
    }

    /* Connecter le serveur */
    bind(socketServer, (const struct sockaddr *) &addrServer, sizeof(addrServer)); 
    printf("Serveur connecté\n");

    /* Le serveur doit écouter par exemple jusqu'à 5 connection en attente  */
    listen(socketServer, 5);
    printf("En écoute...\n");

    /* Récupération des connections des clients */
    struct sockaddr_in addrClient;
    socklen_t csize = sizeof(addrClient); //la taille de l'adresse du client

    /* Acceptation du client */
    int socketClient = accept(socketServer, (struct sockaddr *) &addrClient, &csize );
    /* On gère l'erreur */
    if(socketClient <= 0)
    {
        printf("Erreur lors de l'acceptation du client.\n");
    }
    /* Sinon on accepte */
    printf("Client accepté\n");


    /* Buffeur de reception de données du client (demande temps) */
    char buffer[64];
    
    /* reception de la requêtte du client */
    int receive = recv(socketClient, buffer, 512, 0);
    /* On gère l'erreur */
    if(receive <= 0)
    {
        printf("Erreur lors de la réception de la requêtte du client.\n");
    }

    /* La variable qui contiendra le temps */
    char time_str[12];

    /* On recupère le temps */
    stpcpy(time_str, getTime());
    printf("Heure du serveur : %s\n", time_str); //on affiche le temps

    /* Envoie des informations aux clients */
    send(socketClient, time_str, strlen(time_str), 0);

    close(socketClient); //on ferme le socket client
    close(socketServer); //on ferme le socket serveur
    printf("Fermé\n");

    return 0;
}
