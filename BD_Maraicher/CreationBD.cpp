#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <time.h>
#include <string.h>

typedef struct
{
  int   id;
  char  intitule[20];
  float prix;
  int   stock;  
  char  image[20];
} ARTICLE;

ARTICLE Elm[] = 
{
  {1,"carottes",2.16f,9,"carottes.jpg"},
  {2,"cerises",9.75f,8,"cerises.jpg"},
  {3,"artichaut",1.62f,15,"artichaut.jpg"},
  {4,"bananes",2.6f,8,"bananes.jpg"},
  {5,"champignons",10.25f,4,"champignons.jpg"},
  {6,"concombre",1.17f,5,"concombre.jpg"},
  {7,"courgette",1.17f,14,"courgette.jpg"},
  {8,"haricots",10.82f,7,"haricots.jpg"},
  {9,"laitue",1.62f,10,"laitue.jpg"},
  {10,"oranges",3.78f,23,"oranges.jpg"},
  {11,"oignons",2.12f,4,"oignons.jpg"},
  {12,"nectarines",10.38f,6,"nectarines.jpg"},
  {13,"peches",8.48f,11,"peches.jpg"},
  {14,"poivron",1.29f,13,"poivron.jpg"},
  {15,"pommes de terre",2.17f,25,"pommesDeTerre.jpg"},
  {16,"pommes",4.00f,26,"pommes.jpg"},
  {17,"citrons",4.44f,11,"citrons.jpg"},
  {18,"ail",1.08f,14,"ail.jpg"},
  {19,"aubergine",1.62f,17,"aubergine.jpg"},
  {20,"echalotes",6.48f,13,"echalotes.jpg"},
  {21,"tomates",5.49f,22,"tomates.jpg"}
};

int main(int argc,char *argv[])
{
  // Connection a MySql
  printf("Connection a la BD...\n");
  MYSQL* connexion = mysql_init(NULL);
  mysql_real_connect(connexion,"localhost","Student","PassStudent1_","PourStudent",0,0,0);

  // Creation d'une table UNIX_FINAL
  printf("Creation de la table articles...\n");
  mysql_query(connexion,"drop table articles;"); // au cas ou elle existerait deja
  mysql_query(connexion,"create table articles (id INT(4) auto_increment primary key, intitule varchar(20),prix FLOAT(4),stock INT(4),image varchar(20));");
  
  printf("Creation de la table clients...\n");
  mysql_query(connexion,"create table clients (id INT(4) auto_increment primary key, login varchar(20), password varchar(20));");
  mysql_query(connexion,"create table factures (id INT(4) auto_increment primary key, idClient INT(4), date DATETIME, montant FLOAT, paye BIT);");
  mysql_query(connexion,"create table ventes (idFacture INT(4) auto_increment primary key, idArticle INT(4), qunantie INT(4));");
  // Ajout de tuples dans la table UNIX_FINAL
  printf("Ajout de 21 articles la table articles...\n");
  char requete[256];
  for (int i=0 ; i<21 ; i++)
  {
	  sprintf(requete,"insert into articles values (NULL,'%s',%f,%d,'%s');",Elm[i].intitule,Elm[i].prix,Elm[i].stock,Elm[i].image);
	  mysql_query(connexion,requete);
  }

  // Deconnection de la BD
  mysql_close(connexion);
  exit(0);
}
