#include "tools.h"

// /!\ Toute la configuration du programme se trouve dans le fichier tools.h

int main(int argc, __attribute__((unused))char * argv[]) {

    if (argc > 1) {
        usage("Vous n'avez pas besoin d'entrer de paramètres supplémentaires pour exécuter le programme."
              "\nSi vous souhaitez modifier la configuration du programme, veuillez modifier le fichier tools.h."
              "\nUtilisation : ./som");
    }

    srand(time(NULL)); // pour bien avoir un résultat aléatoire, il ne faut l'appeler qu'une fois

    // on récupère le nombre de lignes de notre jeu de données
    int linesNum = countLines(DATA_FILENAME);

    // les indices de la carte vont nous permettre de modifier l'ordre de lecture des vecteurs
    // shuffle_order_matrix va ainsi contenir les indices d'accès au contenu des différentes cases de la carte
    int * shuffle_order_matrix;
    shuffle_order_matrix = malloc(linesNum*sizeof(int));

    if (!shuffle_order_matrix) {
        usage("Erreur lors de l'allocation de la mémoire pour le tableau d'indices de la carte.");
    }

    // on peut maintenant allouer de la mémoire pour notre vecteur de données,
    // on en profite pour récupérer les indices (faux pointeurs) pour shuffle_order_matrix
    vectorMalloc(linesNum, shuffle_order_matrix);

    // on initialise notre structure de données avec le jeu de données en entrée
    dataVectorInit(DATA_FILENAME, linesNum);

    //printf("\nDisplay data:\n");
    //showData(10, data_vector);

    // on normalise les vecteurs de données :
    normalize(linesNum);

    //printf("\nNormalized: \n");
    //showData(10, data_vector);

    // on initialise maintenant la carte de neurones avec des valeurs (float) aléatoires
    mapInit();

    // La formation de la carte topologique se déroule en 2 phases successives :
    // PHASE 1 : alpha (taux d'apprentissage) = 0.8, iterations = 100
    map_training(neural_network, data_vector, linesNum, ALPHA_PHASE_1, ITER_PHASE_1, shuffle_order_matrix);

    // PHASE 2 : alpha / 10 et 10 fois le nombre d'itérations de la 1e phase :
    map_training(neural_network, data_vector, linesNum, ALPHA_PHASE_2, ITER_PHASE_2, shuffle_order_matrix);

    // on assigne maintenant un label (classe de fleur) à chaque neurone
    label_map(neural_network, data_vector, linesNum);

    // on affiche les classes de notre carte de neurones :
    display_map(neural_network);

    // si on veut afficher les données de la carte de neurones :
    //print_map(neural_network);


    // FREEING MEMORY - 1 malloc() = 1 free()
    int i, j, k;
    for (i = 0; i < linesNum; i++) {
        free(data_vector[i].data);
        free(data_vector[i].name);
    }

    for (j = 0; j < ROW_NUM; j++) {
        for (k = 0; k < COL_NUM; k++) {
            free(neural_network->neurons[j][k].data);
        }
        free(neural_network->neurons[j]);
    }
    free(neural_network->neurons);
    free(neural_network);
    free(shuffle_order_matrix);
    free(data_vector);

    return 0;
}
