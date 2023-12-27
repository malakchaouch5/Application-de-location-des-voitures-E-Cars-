
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char marque[50];
    char couleur[50];
    float prix_par_jour;
    int enpanne;//1 si oui 0 sinon
    int enlocation;//1 si oui 0 sinon
} Voiture ;

struct Date {
    int jour;
    int mois;
    int annee;
};

void afficher_voitures(Voiture *voitures, int total_voitures) {
    printf("\nListe des voitures disponibles :\n");
    for (int i = 0; i < total_voitures; ++i) {
        printf("ID: %d | Marque: %s | couleur: %s | Prix par jour: %.2f | En panne (1 si oui, 0 sinon) : %d | En Location(1 si oui 0 sinon) : %d \n",
               voitures[i].id, voitures[i].marque, voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
    }
}
void afficher_voitures_disponibles(Voiture *voitures, int total_voitures) {
    printf("\nListe des voitures disponibles :\n");
    for (int i = 0; i < total_voitures; ++i) {
        printf("ID: %d | Marque: %s | couleur: %s | Prix par jour: %.2f | En panne (1 si oui, 0 sinon) : %d | En Location(1 si oui 0 sinon) : %d \n",
               voitures[i].id, voitures[i].marque, voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
    }
}



void ajouter_voiture(FILE *fichier_voitures, Voiture *voitures, int *total_voitures) {
    int id;
    char marque[50];
    char couleur[50];
    float prix_par_jour;
    int enpanne;
    int enlocation;

    printf("Entrez l'ID de la nouvelle voiture : ");
    scanf("%d", &id);
    for (int i = 0; i < *total_voitures; ++i) {
        if (voitures[i].id == id) {
            printf("Une voiture avec cet ID existe déjà.\n");
            return;
        }
    }

    printf("Entrez la marque de la nouvelle voiture : ");
    scanf("%s", marque);

    printf("Entrez la couleur  de la nouvelle voiture : ");
    scanf("%s", couleur);

    printf("Entrez le prix par jour de la nouvelle voiture : ");
    scanf("%f", &prix_par_jour);

    printf("Entrez 1 si la voiture est en panne et 0 sinon : ");
    scanf("%d", &enpanne);

    printf("Entrez 1 si la voiture est en location et 0 sinon : ");
    scanf("%d", &enlocation);

    Voiture nouvelle_voiture = {id, "", "", prix_par_jour, enpanne, enlocation};
    strcpy(nouvelle_voiture.marque, marque);
    strcpy(nouvelle_voiture.couleur, couleur);

    if (fichier_voitures != NULL) {
        fseek(fichier_voitures, 0, SEEK_END);
        fprintf(fichier_voitures, "%d %s %s %.2f %d %d\n", nouvelle_voiture.id, nouvelle_voiture.marque,
                nouvelle_voiture.couleur, nouvelle_voiture.prix_par_jour, nouvelle_voiture.enpanne, nouvelle_voiture.enlocation);

        voitures[*total_voitures] = nouvelle_voiture;
        *total_voitures += 1;

        printf("Voiture ajoutée avec succès !\n");
    } else {
        printf("Erreur lors de l'ouverture du fichier.\n");
    }


}

int estBissextile(int annee) {
    if ((annee % 4 == 0 && annee % 100 != 0) || annee % 400 == 0)
        return 1;
    else
        return 0;
}

int joursDansMois(int mois, int annee) {
    switch (mois) {
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return (estBissextile(annee)) ? 29 : 28;
        default:
            return 31;
    }
}

int differenceEnJours(struct Date date_debut, struct Date date_fin) {
    int joursTotal = 0;

    joursTotal += date_debut.annee * 365 + date_debut.jour;
    for (int i = 0; i < date_debut.mois - 1; ++i) {
        joursTotal += joursDansMois(i + 1, date_debut.annee);
    }

    joursTotal -= date_fin.annee * 365 + date_fin.jour;
    for (int i = 0; i < date_fin.mois - 1; ++i) {
        joursTotal -= joursDansMois(i + 1, date_fin.annee);
    }


    int anneesBissextiles = (date_fin.annee - date_debut.annee) / 4 - (date_fin.annee / 100 - date_debut.annee / 100) + (date_fin.annee / 400 - date_debut.annee / 400);
    joursTotal += anneesBissextiles;

    return joursTotal;
}




void louer_voiture(FILE *fichier_voitures, FILE *fichier_locations, FILE *fichier_historique, Voiture *voitures, int total_voitures) {
    int id_client, id_voiture, jours_location;
    struct Date date_debut, date_fin;

    printf("entrez l'ID du client : ");
    scanf("%d", &id_client);



    printf("Les voitures disponibles :\n");
    afficher_voitures(voitures, total_voitures);

    printf("Entrez l'ID de la voiture à louer : ");
    scanf("%d", &id_voiture);

    int voiture_trouvee = 0;
    float prix_par_jour = 0.0f;

    for (int i = 0; i < total_voitures; ++i) {
        if (voitures[i].id == id_voiture && voitures[i].enpanne == 0 && voitures[i].enlocation == 0) {
            voiture_trouvee = 1;
            voitures[i].enlocation = 1;
            prix_par_jour = voitures[i].prix_par_jour;
            break;
        }
    }

    if (!voiture_trouvee) {
        printf("La voiture demandée n'existe pas ou n'est pas disponible pour la location.\n");
        return;
    }

    printf("Entrez la date de début de la location au format : JJ/MM/AAAA : ");
    scanf("%d/%d/%d", &date_debut.jour, &date_debut.mois, &date_debut.annee);

    printf("Entrez la date de fin de la location au format : JJ/MM/AAAA : ");
    scanf("%d/%d/%d", &date_fin.jour, &date_fin.mois, &date_fin.annee);

    int nbJoursLocation = differenceEnJours(date_fin, date_debut);
    printf("nb jours %f:\n",nbJoursLocation);
    float prix_total = prix_par_jour * nbJoursLocation;


    for (int i = 0; i < total_voitures; ++i) {
        if (voitures[i].id == id_voiture) {
            voitures[i].enlocation = 1;
            break;
        }
    }



     fprintf(fichier_historique, "%d %d %.2f %d/%d/%d %d/%d/%d\n", id_client, id_voiture, prix_total,
            date_debut.jour, date_debut.mois, date_debut.annee, date_fin.jour, date_fin.mois, date_fin.annee);

    printf("Voiture louée avec succès ! Le prix total est de %.2f\n", prix_total);
    sauvegarder_voitures(fichier_locations, voitures, total_voitures);


}







void supprimer_voiture_panne(FILE *fichier_voitures, Voiture *voitures, int *total_voitures, int id_voiture_panne) {
    afficher_voitures(voitures, *total_voitures);

    printf("Entrez l'ID de la voiture en panne à supprimer : ");
    if (scanf("%d", &id_voiture_panne) != 1) {
        printf("Erreur de saisie; Veuillez entrer un ID valide.\n");
        while (getchar() != '\n');
        return;
    }

    int found = 0;
    for (int i = 0; i < *total_voitures; ++i) {
        if (voitures[i].id == id_voiture_panne) {
            found = 1;
            for (int j = i; j < *total_voitures - 1; ++j) {
                voitures[j] = voitures[j + 1];
            }
            (*total_voitures)--;
            break;
        }
    }

    if (found) {
        fclose(fichier_voitures);
        fopen("voitures.txt", "w");
        for (int i = 0; i < *total_voitures; ++i) {
            fprintf(fichier_voitures, "%d %s %s %.2f %d %d\n", voitures[i].id, voitures[i].marque,
                    voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
        }
        fclose(fichier_voitures);
        printf("Voiture en panne supprimée avec succès !\n");
    } else {
        printf("Aucune voiture correspondante avec cet ID n'a été trouvée.\n");
    }

}


void afficher_contenu_fichier_historique(FILE *fichier_historique) {
    if (fichier_historique == NULL) {
        printf("Erreur lors de l'ouverture du fichier historique.\n");
        return;
    }

    printf("Contenu du fichier historique :\n");

    int id_client, id_voiture, num;
    float prix_total;
    struct Date date_debut, date_fin;

    while (fscanf(fichier_historique, "%d %d %f %d/%d/%d %d/%d/%d", &id_client, &id_voiture, &prix_total,
                  &date_debut.jour, &date_debut.mois, &date_debut.annee,
                  &date_fin.jour, &date_fin.mois, &date_fin.annee) == 9) {
        printf("ID Client: %d | ID Voiture: %d | Prix Total: %.2f | Date début: %d/%d/%d | Date fin: %d/%d/%d\n",
               id_client, id_voiture, prix_total,
               date_debut.jour, date_debut.mois, date_debut.annee,
               date_fin.jour, date_fin.mois, date_fin.annee);
    }

    rewind(fichier_historique);
}



void retourner_voiture_reclamation(FILE *fichier_historique, FILE *fichier_voitures, Voiture *voitures, int *total_voitures, int id_voiture_reclamation) {
    printf("Entrez l'ID de la voiture réclamée à retourner : ");
    if (scanf("%d", &id_voiture_reclamation) != 1) {
        printf("Erreur de saisie. Veuillez entrer un ID valide.\n");
        while (getchar() != '\n');
        return;
    }

    int found = 0;
    for (int i = 0; i < *total_voitures; ++i) {
        if (voitures[i].id == id_voiture_reclamation) {
            found = 1;
            voitures[i].enlocation = 0;
            voitures[i].enpanne = 1;
            break;
        }
    }

    if (found) {
        fseek(fichier_voitures, 0, SEEK_SET);
        for (int i = 0; i < *total_voitures; ++i) {
            fprintf(fichier_voitures, "%d %s %s %.2f %d %d\n", voitures[i].id, voitures[i].marque,
                    voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
        }



        printf("Voiture réclamée retournée avec succès !\n");
    } else {
        printf("Aucune voiture correspondante avec cet ID n'a été trouvée.\n");
    }
}





void modifier_voiture(FILE *fichier_voitures, Voiture *voitures, int total_voitures) {
    int id;
    int voiture_trouvee = 0;
    int x = -1;
    afficher_voitures(voitures, total_voitures);

    printf("\t\t\tSaisir ID : ");
    scanf("%d", &id);

    for (int i = 0; i < total_voitures; ++i) {
        if (voitures[i].id == id) {
            voiture_trouvee = 1;
            x = i;
            break;
        }
    }

    if (!voiture_trouvee) {
        printf("La voiture demandée n'existe pas.\n");
        return;
    }

    char test;
    Voiture V = voitures[x];
    printf("Informations de la voiture : \n");
    printf("Marque     : %s\n", V.marque);
    printf("couleur     : %s\n", V.couleur);
    printf("Prix/jour  : %.2f\n", V.prix_par_jour);
    printf("En panne   : %d\n", V.enpanne);
    printf("En location: %d\n", V.enlocation);
    printf("Qu'est-ce que vous voulez changer ?\n ");

       printf("\t\t\tMarque ? (o/n): ");
    scanf(" %c", &test);
    if (test == 'o') {
        printf("\t\t\tNouvelle Marque : ");
        scanf("%s", V.marque);
    }

    printf("\t\t\tModele ? (o/n): ");
    scanf(" %c", &test);
    if (test == 'o') {
        printf("\t\t\tNouvelle couleur : ");
        scanf("%s", V.couleur);
    }

    printf("\t\t\tPrix par jour ? (o/n): ");
    scanf(" %c", &test);
    if (test == 'o') {
        printf("\t\t\tNouveau Prix par jour : ");
        scanf("%f", &V.prix_par_jour);
    }

    printf("\t\t\tEn panne ? (o/n): ");
    scanf(" %c", &test);
    if (test == 'o') {
        printf("\t\t\tNouveau statut en panne (1 pour oui, 0 pour non) : ");
        scanf("%d", &V.enpanne);
    }

    printf("\t\t\tEn location ? (o/n): ");
    scanf(" %c", &test);
    if (test == 'o') {
        printf("\t\t\tNouveau statut en location (1 pour oui, 0 pour non) : ");
        scanf("%d", &V.enlocation);
    }

    voitures[x] = V;

    fseek(fichier_voitures, 0, SEEK_SET);
    for (int i = 0; i < total_voitures; ++i) {
        fprintf(fichier_voitures, "%d %s %s %.2f %d %d\n", voitures[i].id, voitures[i].marque,
                voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
    }


    printf("\t\t\tLa voiture a été modifiée avec succès !\n");

}
void sauvegarder_voitures(FILE *fichier_voitures, Voiture *voitures, int total_voitures) {
    fclose(fichier_voitures);
    fichier_voitures = fopen("voitures.txt", "w");

    if (fichier_voitures == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour sauvegarde.\n");
        return;
    }

    for (int i = 0; i < total_voitures; ++i) {
        fprintf(fichier_voitures, "%d %s %s %.2f %d %d\n", voitures[i].id, voitures[i].marque,
                voitures[i].couleur, voitures[i].prix_par_jour, voitures[i].enpanne, voitures[i].enlocation);
    }

    fclose(fichier_voitures);
}
void rechercher_par_marque(Voiture *voitures, int total_voitures, char *marque_recherchee) {
    printf("Entrez la marque de voiture à rechercher : ");
    scanf("%s", marque_recherchee);

    int found = 0;
    printf("\nVoitures de la marque %s :\n", marque_recherchee);
    for (int i = 0; i < total_voitures; ++i) {
        if (strcmp(voitures[i].marque, marque_recherchee) == 0) {
            printf("ID: %d | Marque: %s | couleur: %s | Prix par jour: %.2f | En panne: %d | En Location: %d\n",
                   voitures[i].id, voitures[i].marque, voitures[i].couleur, voitures[i].prix_par_jour,
                   voitures[i].enpanne, voitures[i].enlocation);
            found = 1;
        }
    }

    if (!found) {
        printf("Aucune voiture de la marque %s n'a été trouvée.\n", marque_recherchee);
    }
}
void trier_par_prix_de_location_croissant(Voiture *voitures, int total_voitures) {
    for (int i = 0; i < total_voitures - 1; ++i) {
        int min_idx = i;
        for (int j = i + 1; j < total_voitures; ++j) {
            if (voitures[j].prix_par_jour < voitures[min_idx].prix_par_jour) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Voiture temp = voitures[i];
            voitures[i] = voitures[min_idx];
            voitures[min_idx] = temp;
        }
    }

    printf("\nVoitures triées par prix par jour croissant :\n");
    for (int i = 0; i < total_voitures; ++i) {
        printf("ID: %d | Marque: %s | Couleur: %s | Prix par jour: %.2f | En panne: %d | En Location: %d\n",
               voitures[i].id, voitures[i].marque, voitures[i].couleur, voitures[i].prix_par_jour,
               voitures[i].enpanne, voitures[i].enlocation);
    }
}



int main() {
    FILE *fichier_voitures = fopen("voitures.txt", "a+");
    FILE *fichier_locations = fopen("voitures_louees.txt", "a+");
    FILE *fichier_historique = fopen("historique_locations.txt", "a+");
        char marque_recherchee[50];

    if (fichier_voitures == NULL || fichier_locations == NULL || fichier_historique == NULL) {
        printf("Erreur lors de l'ouverture des fichiers.\n");
        return 1;
    }

    Voiture voitures[100];
    int total_voitures = 0;

    while (fscanf(fichier_voitures, "%d %s %s %f %d %d", &voitures[total_voitures].id,
                  voitures[total_voitures].marque, voitures[total_voitures].couleur,
                  &voitures[total_voitures].prix_par_jour, &voitures[total_voitures].enpanne,
                  &voitures[total_voitures].enlocation) == 6) {
        total_voitures++;
    }

    int choix;
    do {
        printf("\t\t\t******************** Menu *******************************\n");
        printf("\t\t\t*    1. Ajouter une voiture                              *\n");
        printf("\t\t\t*    2. Louer une voiture                                *\n");
        printf("\t\t\t*    3. consulter l'etat des voitures disponibles        *\n");
        printf("\t\t\t*    4. Supprimer une voiture en panne                   *\n");
        printf("\t\t\t*    5. Retourner une voiture suite à une réclamation    *\n");
        printf("\t\t\t*    6. consulter l'historique de tous les locations     *\n");
        printf("\t\t\t*    7. Modifier une voiture                             *\n");
        printf("\t\t\t*    8. Rechercher voiture par marque                    *\n");
        printf("\t\t\t*    9. trier par ordre croissant de prix de location    *\n");
        printf("\t\t\t*   10. Quitter                                          *\n");
        printf("\t\t\t**********************************************************\n");
        printf("\t\t\t        Votre choix : ");

        if (scanf("%d", &choix) != 1) {
            printf("Erreur de saisie. Veuillez entrer un numero valide.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choix) {
            case 1:
                ajouter_voiture(fichier_voitures, voitures, &total_voitures);
                break;
            case 2:
                louer_voiture(fichier_voitures, fichier_locations, fichier_historique, voitures, total_voitures);


                break;
            case 3:
                afficher_voitures_disponibles(voitures, total_voitures);
                break;
            case 4:
                supprimer_voiture_panne(fichier_voitures, voitures, &total_voitures, 0);
                break;
            case 5:
                retourner_voiture_reclamation(fichier_locations,fichier_voitures, voitures, &total_voitures, 0);


                break;
            case 6:
                afficher_contenu_fichier_historique(fichier_historique) ;
                  break;
            case 7:
                modifier_voiture(fichier_voitures, voitures, total_voitures);
                break;
            case 8:
                  rechercher_par_marque(voitures, total_voitures, marque_recherchee);
                 break;
            case 9:
                 trier_par_prix_de_location_croissant(voitures, total_voitures);
                 break;

            case 10:
                printf("Fermeture de l'application.\n");
                break;
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
        }


    } while (choix != 10);
        sauvegarder_voitures(fichier_voitures, voitures, total_voitures);


        fclose(fichier_voitures);
        fclose(fichier_locations);
        fclose(fichier_historique);

    return 0;
}
