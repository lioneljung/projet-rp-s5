#ifndef __server_h
#define __server_h

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>


#define ERROR           -1    // Retour de fonction pour les erreurs
#define NTD             2     // Retour de fonction Nothing to do
#define MESS_MAX_SIZE   1049  // TYPE(1)+LENGTH(2)+IP(46)+DATA(1000)
#define TAILLE_MAX_HASH 1000  // taille max d'un hash
#define MAX_IPS         10    // nb max d'adresses IP à envoyer par le serveur
#define MAX_SERVER      10    // nombre max de serveur qu'on peut connaitre

#define PASS            "password123" // mot de passe serveur 

/* Définition des types pour les messages */
#define GET     1
#define PUT     2
#define WANT    3
#define HAVE    4
#define EXIT    5
#define NEW     6
#define DECO    7


/**
 * \def PERRORMSG(cmd, msg)
 * \brief Test une primitives systèmes.
 *
 * \param cmd La primitives systèmes qui est exécuté.
 * \param msg Le message que doit afficher perror.
 */
#define PERRORMSG(cmd, msg) if((cmd) == ERROR) \
    do{perror(msg); exit(EXIT_FAILURE);}while(0);


/*
 * \struct ip_cel dht.h
 * \brief Liste chainée de IP
 */
typedef struct ip_cel{
    char val[INET6_ADDRSTRLEN];
    struct ip_cel * next;
}IP;


/*
 * \struct hash_cel dht.h
 * \brief Table de hash.
 */
typedef struct hash_cel{
    char val[TAILLE_MAX_HASH];
    struct hash_cel * next;
    struct ip_cel * want;
    struct ip_cel * have;
}DHT;



/**
 * \fn int port_valide (int port)
 * \brief Vérifie si un numéro de port est valide.
 *
 * \param port Un numéro de port sous forme de string.
 * \return Le numero de port si le paramètre est 
 *  un numéro de port valide, -1 sinon.
 */
int port_valide(char * port);



/**
 * \fn int parse_hostname (char * hostname, char * ip)
 * \brief Détermine une adresse IPv6 pour un nom de hôte.
 *
 * \param hostname Un nom de hôte à résoudre
 * \param port Numero de port (en string)
 * \param ip Pointeur où l'on stock l'adresse IP
 * \return 0 si tout se passe bien, -1 sinon.
 */
int parse_hostname(char * hostname, char * port, char * ip);



/**
 * \fn convert_ipv6 (char * arg_ip, char * arg_port, struct sockaddr_in6 * addr)
 * \brief Vérifie qu'une adresse IPv6 en format char* est valide et la stock
 *  dans la structure addr.
 *
 * \param arg_ip Adresse IPv6 entrée par l'utilisateur.
 * \param arg_port Numéro de port entré par l'utilisateur.
 * \param addr Structure dans laquelle on stock l'adresse IP.
 * \return 0 si tout se passe bien, -1 sinon.
 */
int convert_ipv6(char * arg_ip, char * arg_port, struct sockaddr_in6 * addr);



/* 
 * \fn int check_hash (char * hash)
 * \brief Verifie qu'un hash est valide
 *
 * \param hash un hash au format string
 * \return 0 si le hash est valide, -1 sinon
 */
int check_hash(char * hash);


/*
 * \fn int check_access_code
 * \brief Vérifie si le code entré est valide
 *  permet d'acceder au serveur pour des manipulation critique
 *  (terminer le serveur par exemple)
 *
 * \param code Le mot de passe qu'on veut tester
 * \return 0 si le code est juste, -1 sinon
 */
int check_access_code(char * code);


/*
 * \fn void supp_server (char ** liste, int i, int *nb)
 * \brief Supprimer le ieme élement de la liste "liste"
 *
 * \param liste Une liste d'adresse IP de serveur
 * \param i L'élément de la liste à supprimer
 * \param nb Le nombre d'éléments de la liste
 */
void supp_server(char ** liste, int i, int *nb);



/* * *  TABLE DES HASHS * * */




/*
 * \fn DHT * init_dht (char * hash)
 * \brief Initialise une table pour stocker les hash
 *
 * \param hash Le premier hash de la table
 * \return Un pointeur vers la table créée
 */
DHT * init_dht(char * hash);



/*
 * \fn void supp_dht (DHT * table)
 * \brief Supprime une table de hash
 *
 * \param table Un pointeur vers une table
 */
void supp_dht(DHT * table);



/*
 * \fn void affiche_dht (DHT * table)
 * \brief Affiche les éléments d'un table
 *
 * \param table Un pointeur vers une table
 */
void affiche_dht (DHT * table);



/*
 * \fn char * get_hash (char * hash, DHT * table)
 * \brief Recupère les adresses IP qui possède un hash
 *
 * \param hash Un hash
 * \param table Un pointeur vers une table
 * \return Une chaine de caractère contenant la liste des IPs
 *         la forme de la chaine est " IP IP... IP"
 */
char * get_hash(char * hash, DHT * table);


/*
 * \fn int put_hash (char * hash, char * ip, DHT * table;
 * \brief Ajoute une adresse IP qui possède un hash
 *
 * \param hash Le hash associé à l'adresse IP
 * \param ip L'adresse IP de la machine qui possède le hash
 * \param table La table dans laquelle on stockera l'entrée
 * \return 0 si tu se passe bien, -1 en cas d'erreur, 2 si 
 *  l'adresse IP est déjà présente dans la liste.
 */
int put_hash(char * hash, char * ip, DHT ** table);



/*
 * \fn void delete_hash (char * hash, DHT * table)
 * \brief supprime un hash d'une table
 *
 * \param hash un hash
 * \param table une table de hash
 */
void delete_hash(char * hash, DHT ** table);


/*
 * \fn void delete_ip (char * hash, char * ip, DHT * table, int liste)
 * \brief Supprime une IP associé à un hash selon 
 *
 * \param hash un hash sous forme de string
 * \param ip une ip sous forme de string
 * \param table un pointeur vers une table de hash
 * \param liste la liste dans laquelle se trouve l'IP (WANT ou HAVE)
 */
void delete_ip(char * hash, char * ip, DHT * table, int liste);


#endif
