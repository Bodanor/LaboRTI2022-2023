#include "windowclient.h"
#include "ui_windowclient.h"
#include <QMessageBox>
#include <string>

using namespace std;

extern WindowClient *w;
int articlesencours = 0;

#define REPERTOIRE_IMAGES "images/"

WindowClient::WindowClient(QWidget *parent) : QMainWindow(parent), ui(new Ui::WindowClient)
{
    ui->setupUi(this);

    setSocket(Server_connect("localhost", 4444));

    // Configuration de la table du panier (ne pas modifer)
    ui->tableWidgetPanier->setColumnCount(3);
    ui->tableWidgetPanier->setRowCount(0);
    QStringList labelsTablePanier;
    labelsTablePanier << "Article"
                      << "Prix à l'unité"
                      << "Quantité";
    ui->tableWidgetPanier->setHorizontalHeaderLabels(labelsTablePanier);
    ui->tableWidgetPanier->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetPanier->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetPanier->horizontalHeader()->setVisible(true);
    ui->tableWidgetPanier->horizontalHeader()->setDefaultSectionSize(160);
    ui->tableWidgetPanier->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetPanier->verticalHeader()->setVisible(false);
    ui->tableWidgetPanier->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    ui->pushButtonPayer->setText("Confirmer achat");
    setPublicite("!!! Bienvenue sur le Maraicher en ligne !!!");

    // Exemples à supprimer
    //setArticle("pommes", 5.53, 18, "pommes.jpg");
    //ajouteArticleTablePanier("cerises", 8.96, 2);
    //articlesencours = 1;

}

WindowClient::~WindowClient()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles : ne pas modifier /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setNom(const char *Text)
{
    if (strlen(Text) == 0)
    {
        ui->lineEditNom->clear();
        return;
    }
    ui->lineEditNom->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *WindowClient::getNom()
{
    strcpy(nom, ui->lineEditNom->text().toStdString().c_str());
    return nom;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setMotDePasse(const char *Text)
{
    if (strlen(Text) == 0)
    {
        ui->lineEditMotDePasse->clear();
        return;
    }
    ui->lineEditMotDePasse->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *WindowClient::getMotDePasse()
{
    strcpy(motDePasse, ui->lineEditMotDePasse->text().toStdString().c_str());
    return motDePasse;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setPublicite(const char *Text)
{
    if (strlen(Text) == 0)
    {
        ui->lineEditPublicite->clear();
        return;
    }
    ui->lineEditPublicite->setText(Text);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setImage(const char *image)
{
    // Met à jour l'image
    char cheminComplet[80];
    sprintf(cheminComplet, "%s%s", REPERTOIRE_IMAGES, image);
    QLabel *label = new QLabel();
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);
    QPixmap *pixmap_img = new QPixmap(cheminComplet);
    label->setPixmap(*pixmap_img);
    label->resize(label->pixmap()->size());
    ui->scrollArea->setWidget(label);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::isNouveauClientChecked()
{
    if (ui->checkBoxNouveauClient->isChecked())
        return 1;
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setArticle(const char *intitule, float prix, int stock, const char *image)
{
    ui->lineEditArticle->setText(intitule);
    if (prix >= 0.0)
    {
        char Prix[20];
        sprintf(Prix, "%.2f", prix);
        ui->lineEditPrixUnitaire->setText(Prix);
    }
    else
        ui->lineEditPrixUnitaire->clear();
    if (stock >= 0)
    {
        char Stock[20];
        sprintf(Stock, "%d", stock);
        ui->lineEditStock->setText(Stock);
    }
    else
        ui->lineEditStock->clear();
    setImage(image);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::getQuantite()
{
    return ui->spinBoxQuantite->value();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::setTotal(float total)
{
    if (total >= 0.0)
    {
        char Total[20];
        sprintf(Total, "%.2f", total);
        ui->lineEditTotal->setText(Total);
    }
    else
        ui->lineEditTotal->clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::loginOK()
{
    ui->pushButtonLogin->setEnabled(false);
    ui->pushButtonLogout->setEnabled(true);
    ui->lineEditNom->setReadOnly(true);
    ui->lineEditMotDePasse->setReadOnly(true);
    ui->checkBoxNouveauClient->setEnabled(false);

    ui->spinBoxQuantite->setEnabled(true);
    ui->pushButtonPrecedent->setEnabled(true);
    ui->pushButtonSuivant->setEnabled(true);
    ui->pushButtonAcheter->setEnabled(true);
    ui->pushButtonSupprimer->setEnabled(true);
    ui->pushButtonViderPanier->setEnabled(true);
    ui->pushButtonPayer->setEnabled(true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::logoutOK()
{
    ui->pushButtonLogin->setEnabled(true);
    ui->pushButtonLogout->setEnabled(false);
    ui->lineEditNom->setReadOnly(false);
    ui->lineEditMotDePasse->setReadOnly(false);
    ui->checkBoxNouveauClient->setEnabled(true);

    ui->spinBoxQuantite->setEnabled(false);
    ui->pushButtonPrecedent->setEnabled(false);
    ui->pushButtonSuivant->setEnabled(false);
    ui->pushButtonAcheter->setEnabled(false);
    ui->pushButtonSupprimer->setEnabled(false);
    ui->pushButtonViderPanier->setEnabled(false);
    ui->pushButtonPayer->setEnabled(false);

    setNom("");
    setMotDePasse("");
    ui->checkBoxNouveauClient->setCheckState(Qt::CheckState::Unchecked);

    setArticle("", -1.0, -1, "");

    w->videTablePanier();
    w->setTotal(-1.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table du panier (ne pas modifier) /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::ajouteArticleTablePanier(const char *article, float prix, int quantite)
{
    char Prix[20], Quantite[20];

    sprintf(Prix, "%.2f", prix);
    sprintf(Quantite, "%d", quantite);

    // Ajout possible
    int nbLignes = ui->tableWidgetPanier->rowCount();
    nbLignes++;
    ui->tableWidgetPanier->setRowCount(nbLignes);
    ui->tableWidgetPanier->setRowHeight(nbLignes - 1, 10);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(article);
    ui->tableWidgetPanier->setItem(nbLignes - 1, 0, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Prix);
    ui->tableWidgetPanier->setItem(nbLignes - 1, 1, item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(Quantite);
    ui->tableWidgetPanier->setItem(nbLignes - 1, 2, item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::videTablePanier()
{
    ui->tableWidgetPanier->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int WindowClient::getIndiceArticleSelectionne()
{
    QModelIndexList liste = ui->tableWidgetPanier->selectionModel()->selectedRows();
    if (liste.size() == 0)
        return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue (ne pas modifier ////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::dialogueMessage(const char *titre, const char *message)
{
    QMessageBox::information(this, titre, message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::dialogueErreur(const char *titre, const char *message)
{
    QMessageBox::critical(this, titre, message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// CLIC SUR LA CROIX DE LA FENETRE /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::closeEvent(QCloseEvent *event)
{

    exit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonLogin_clicked()
{
    int error_check;
    OVESP *result;

    error_check = OVESP_Login(getNom(), getMotDePasse(), isNouveauClientChecked(), getSocket());
    printf("Error_check : %d\n" ,error_check);
    switch(error_check)
    {
        case 0 :
            /*Succesfull*/
            dialogueMessage("COOOL", "Tu es connecte le zin");
            w->loginOK();
            w->on_pushButtonSuivant_clicked();
            break;
        case 1 :
            /*USERNAME DOESNT EXIS*/ 
            dialogueErreur("Eh merte", "L'user existe pas");
            break;
        case 2 : 
            /*PASSWORD INCORRECT*/
            dialogueErreur("Retape un peu", "Mdp incorrect");
            break;
        case 3 : 
            /*DB ERROR*/
            dialogueErreur("ABORT THE MISSION", "Database error");
            break;

        case 4 : 
            /*USER ALREADY EXIST*/
            dialogueErreur("Eh merte *2", "Change de pseudo");
            break;
        case -1 : 
            /*SERVER RIP*/
            dialogueErreur(":(", "Ce n'est qu'un au revoir");
            break;
        case -2 : 
            /*MALLOC ERROR AND CORRPTED DATA*/
            dialogueErreur("MAINFRAME HACKED", "Memory error | Corrupted DATA");
            break;
        case -3 : 
            dialogueErreur("BIBBOUP", "I/O Error");
            break;
        case -4 : 

            /*BAD ANSWERS SERVER T NUL OOOUUUH*/
            dialogueErreur("Allo t ou", "Reveille toi ducon");
            break;
        
            
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonLogout_clicked()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonSuivant_clicked()
{
    int error_check;
    OVESP *res;

    int id = articlesencours;
    
    error_check = OVESP_Consult(++articlesencours, getSocket(), &res);
   
        
    if(error_check == 1)
    {
        dialogueErreur("Probleme", "Article introuvable");
        --articlesencours;
    }
    else {
        setArticle(res->data[0][1],atof(res->data[0][2]),atoi(res->data[0][3]),res->data[0][4]);
    }
    

    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonPrecedent_clicked()
{
    int error_check;
    OVESP *res;


    error_check = OVESP_Consult(--articlesencours, getSocket(), &res);
    if(error_check == 1)
    {
        dialogueErreur("Probleme", "Article introuvable");
        ++articlesencours;
    }
    else {
        setArticle(res->data[0][1],atof(res->data[0][2]),atoi(res->data[0][3]),res->data[0][4]);
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonAcheter_clicked()
{
    int error_check;
    int i;

    OVESP * res;


    error_check = OVESP_Achat(articlesencours, ui->spinBoxQuantite->value(), getSocket(), &res);
    if (error_check == 1) {
        dialogueErreur("Erreur", "l'article n'existe pas !");
    }
    else if (error_check == 2) {
        dialogueErreur("Erreur", "La quantite est trop elevee");
    }
    else if (error_check == 3) {
        dialogueErreur("Erreur", "La quantite ne peux pas valoir 0 !");
    }
    else if (error_check == -2) {
        dialogueErreur("Erreur", "Une erreur interne est survenue !");
    }
    else {
        setArticle(res->data[0][1],atof(res->data[0][2]),atoi(ui->lineEditStock->text().toLocal8Bit().data()) - atoi(res->data[0][3]),res->data[0][4]);
    }

    w->videTablePanier();


    error_check = OVESP_Caddie(getSocket(), &res);
    if (error_check == -1 || error_check == 1) {
        dialogueErreur("Erreur", "FATAL ERROR !");
    }

    printf("Nombre d'article dans panier : %ld\n", res->rows);
    for(i = 0; i < res->rows; i++) {
        
        ajouteArticleTablePanier(res->data[i][1], atof(res->data[i][2]), atoi(res->data[i][3]));
    }
    

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonSupprimer_clicked()
{
    int error_check;
    int i;

    OVESP * res;

    error_check = OVESP_Caddie(getSocket(), &res);
    if (error_check == -1 || error_check == 1) {
        dialogueErreur("Erreur", "FATAL ERROR !");
    }


    error_check = OVESP_Cancel(res->data[getIndiceArticleSelectionne()][0], getSocket()); 
    /*Il faudrait d'aures erreurs je suppose */ 
    if (error_check == -2) {
        dialogueErreur("Erreur", "Une erreur interne est survenue !");
    }
    else {
        
        setArticle(res->data[0][1],atof(res->data[0][2]),atoi(ui->lineEditStock->text().toLocal8Bit().data()) + atoi(res->data[0][3]),res->data[0][4]);
    }

    w->videTablePanier();


    error_check = OVESP_Caddie(getSocket(), &res);
    if (error_check == -1 || error_check == 1) {
        dialogueErreur("Erreur", "FATAL ERROR !");
    }

    printf("Nombre d'article dans panier : %ld\n", res->rows);
    for(i = 0; i < res->rows; i++) {
        
        ajouteArticleTablePanier(res->data[i][1], atof(res->data[i][2]), atoi(res->data[i][3]));
    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonViderPanier_clicked()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowClient::on_pushButtonPayer_clicked()
{
}

int WindowClient::getSocket()
{
    return this->socket_server;
}
void WindowClient::setSocket(int socket)
{
    socket_server = socket;
}
