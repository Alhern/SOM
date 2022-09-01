#include "tools.h"

// Générateur de nombre aléatoire qui retourne un entier
int randomInt(int min, int max) {
    return min + (rand() % (max - min + 1));
}

// Générateur de nombre aléatoire qui retourne un flottant
float randomFloat(float min, float max) {
    return min + ((float) rand() / RAND_MAX) * (max - min);
}


// INITIALISATION DES VECTEURS DE DONNEES

// Compte le nombre de lignes dans le fichier de données
int countLines(char * filename) {
    FILE * f = fopen(filename, "r");
    if (!f) {
        usage("Impossible d'ouvrir le fichier de données.\nVeuillez vérifier le chemin du fichier.");
    }
    int counter = 0;
    size_t len = 0;
    char * line = NULL;
    while (getline(&line, &len, f) != -1) {
        counter++;
    }
    free(line);
    fclose(f);
    return counter;

}


// Allocation de la mémoire pour les vecteurs de données
void vectorMalloc(int size, int idx[]) {
    data_vector = malloc(size*sizeof(Data_vector));

    if (!data_vector) {
        usage("Erreur d'allocation de mémoire pour le vecteur de données.");
    }

    int i;
    for (i = 0; i < size; i++) {
        data_vector[i].data = malloc(sizeof(float) * VEC_SIZE);

        if (!data_vector[i].data) {
            usage("Erreur d'allocation de mémoire pour le vecteur de données.");
        }

        data_vector[i].name = malloc(sizeof(char) * 30);

        if (!data_vector[i].name) {
            usage("Erreur d'allocation de mémoire pour le vecteur de données.");
        }

        data_vector[i].norm = 0;
        idx[i] = i;
    }
}


// Convertit le nom de la fleur en un chiffre, utile pour afficher la matrice de neurones à la fin
void convertLabel(char * label, char * token) {
    if (strcmp(token, "Iris-setosa") == 0) {
        strcpy(label, "1");
    }
    else if (strcmp(token, "Iris-versicolor") == 0) {
        strcpy(label, "2");
    }
    else {
        strcpy(label, "3");
    }
}


// Initialisation du vecteur de données
// on lit les données dans le fichier soumis et on les stocke dans le vecteur de données
void dataVectorInit(char * filename, int linesNum) {
    FILE * f = fopen(filename, "r");
    if (!f) {
        usage("Impossible d'ouvrir le fichier de données.\nVeuillez vérifier le chemin du fichier.");
    }

    char * line = malloc(sizeof(char) * 200);
    if (!line) {
        usage("Erreur d'allocation de mémoire pour la variable line dans dataVectorInit().");
    }

    int i, j;
    for (i = 0; i < linesNum; i++) {
        fscanf(f, "%s", line);
        char * token = strtok(line, ",");
        char * tail;
        for (j = 0; j < VEC_SIZE; j++) {
            data_vector[i].data[j] = strtof(token, &tail);
            token = strtok(NULL, ",");
        }

        // conversion du nom de la fleur en un chiffre
        convertLabel(data_vector[i].name, token);

        // normalisation des données
        normCalc(i);

    }

    free(line);
    fclose(f);

}


// La normalisation se déroule en 2 étapes :
// Etape 1 : D'abord le calcul de la norme

void normCalc(int idx) {
    float res = 0;
    int i;
    for (i = 0; i < VEC_SIZE; i++) {
        res += pow((data_vector[idx].data[i]), 2);
    }
    data_vector[idx].norm = sqrt(res);
}

// Etape 2 : Puis la division de chaque attribut par la norme :
void normalize(int lines) {
    int i, j;
    for (i = 0; i < lines; i++) {
        for (j = 0; j < VEC_SIZE; j++) {
            data_vector[i].data[j] /= data_vector[i].norm;
        }
    }
}


// APPRENTISSAGE

// Initialisation du réseau de neurones
void mapInit() {
    neural_network = malloc(sizeof(Neural_net));
    if (!neural_network) {
        usage("Erreur d'allocation de mémoire pour le réseau de neurones.");
    }
    neural_network->neurons = malloc(sizeof(Neuron *) * ROW_NUM);
    if (!neural_network->neurons) {
        usage("Erreur d'allocation de mémoire pour les neurones.");
    }
    neural_network->n_rows = ROW_NUM;
    neural_network->n_cols = COL_NUM;
    int i, j, k;

    for (i = 0; i < ROW_NUM; i++) {
        neural_network->neurons[i] = malloc(sizeof(Neuron) * COL_NUM);
        if (!neural_network->neurons[i]) {
            usage("Erreur d'allocation de mémoire pour les neurones.");
        }
        for (j = 0; j < COL_NUM; j++) {
            neural_network->neurons[i][j].data = malloc(sizeof(float) * VEC_SIZE);
            if (!neural_network->neurons[i][j].data) {
                usage("Erreur d'allocation de mémoire pour les données des neurones.");
            }
            for (k = 0; k < VEC_SIZE; k++) {
                neural_network->neurons[i][j].data[k] = randomFloat(MIN, MAX);
            }
        }
    }
}


// Fisher–Yates shuffle
// on s'en sert pour mélanger les indices des vecteurs de données
void shuffle_vects(int * vector, int n) {
    int i, j, tmp;
    for (i = n - 1; i > 0; i--) {
        j = randomInt(0, i-1);
        tmp = vector[j];
        vector[j] = vector[i];
        vector[i] = tmp;
    }
}


// Calcul de la distance euclidienne entre deux vecteurs
// Utilisé pour trouver le BMU
float get_distance(float * weight_vector, float * input_vector){
    float distance = 0;
    int i;
    for (i = 0; i < VEC_SIZE; i++) {
        distance += pow((weight_vector[i] - input_vector[i]), 2);
    }
    return sqrt(distance);
}


// Pour déterminer le BMU on va itérer sur tous les neurones et
// calculer la distance euclidienne entre le neurone et le vecteur de données actuel
// le neurone qui a la distance la plus petite avec le vecteur de données est le BMU
int * get_BMU(Neural_net * map, Data_vector * data) {
    int i, j;
    int * bmu = malloc(sizeof(int) * 2); // tableau de 2 entiers pour accueillir les coordonnées du BMU
    if (!bmu) {
        usage("Erreur d'allocation de mémoire pour le BMU.");
    }
    float curr_dist;
    float prev_dist = get_distance(map->neurons[0][0].data, data->data);
    for (i = 0; i < ROW_NUM; i++) {
        for (j = 0; j < COL_NUM; j++) {
            curr_dist = get_distance(map->neurons[i][j].data, data->data);
            if (curr_dist < prev_dist) {
                bmu[0] = i; // row
                bmu[1] = j; // col
                prev_dist = curr_dist;
            }
        }
    }
    return bmu;
}

// Le voisinage est l'ensemble des neurones situés dans le rayon (donc distance du voisin <= radius) du BMU, il décroit à chaque itération.
// La fonction suivante a pour but de déterminer le voisinage du BMU pour ensuite ajuster les poids de chaque voisin.
// Les coordonnées du BMU sont utilisés pour déterminer les limites du voisinage (1e étape)
// Si un neurone se trouve dans le voisinage, on ajuste son poids en fonction du vecteur de données sélectionné (2e étape)
void scale_neighborhood(Neural_net * map, int * bmu, int radius, int idx, float alpha) {
    int row_min = bmu[0] - radius;
    int row_max = bmu[0] + radius;
    int col_min = bmu[1] - radius;
    int col_max = bmu[1] + radius;
    int i, j, k;
    for (i = row_min; i <= row_max; i++) {
        for (j = col_min; j <= col_max; j++) {
            if (i >= 0 && i < ROW_NUM && j >= 0 && j < COL_NUM) {
                for (k = 0; k < VEC_SIZE; k++) {
                    // nouveau poids = ancien poids + alpha (taux d'apprentissage) * (valeur actuelle du vecteur de données - ancien poids)
                    map->neurons[i][j].data[k] += alpha * (data_vector[idx].data[k] - map->neurons[i][j].data[k]);
                }
            }
        }
    }
}


// Entrainement du réseau de neurones
void map_training(Neural_net * map, Data_vector * data, int linesNum, float alpha, int iter, int * idxs) {
    int * bmu;
    // alpha module les taux de modifications des vecteurs,
    // on garde une copie de l'alpha initial, on en aura besoin pour le diminuer
    float alpha_init = alpha;
    int n_init = RADIUS;  // on fait de même avec le radius de voisinage initial
    int i, j;

    for (i = 0; i < iter; i++) {
        // on shuffle les indices (faux pointeurs) des vecteurs à chaque itération
        // afin d'avoir un vecteur de données aléatoire à chaque itération
        shuffle_vects(idxs, linesNum);
        for (j = 0; j < VEC_SIZE; j++) {
            // on calcule le BMU pour chaque vecteur de données
            bmu = get_BMU(map, &data[idxs[j]]);
            // maintenant que le BMU est déterminé, on calcule son voisinage
            scale_neighborhood(map, bmu, n_init, idxs[j], alpha);
        }

        // à chaque iteration, alpha (taux d'apprentissage) et le radius de voisinage diminuent linéairement
        // puisque selon Hecht-Nielsen (1991) : "une diminution linéaire de ces valeurs
        // avec le temps semble fonctionner correctement".
        alpha = alpha_init * (1 - ((float)i / iter));
        n_init = RADIUS * (1 - ((float)i / iter));
    }
    // on libère la mémoire allouée pour le BMU
    free(bmu);
}


// On assigne un label à chaque neurone
void label_map(Neural_net * map, Data_vector * data, int linesNum) {
    float prev_dist, curr_dist;
    int idx, i, j, k;
    for (i = 0; i < ROW_NUM; i++) {
        for (j = 0; j < COL_NUM; j++) {
            idx = 0;
            prev_dist = get_distance(map->neurons[i][j].data, data[0].data);
            for (k = 1; k < linesNum; k++) {
                curr_dist = get_distance(map->neurons[i][j].data, data[k].data);
                if (curr_dist < prev_dist) {
                    prev_dist = curr_dist;
                    idx = k;  // position du BMU
                }
            }
            // on assigne un label à chaque neurone en fonction de la classe du vecteur de données le plus proche
            map->neurons[i][j].label = data[idx].name;
        }
    }
}


// affichage du réseau de neurones
void display_map(Neural_net * map) {
    int i, j;
    printf("[1] = Setosa\n[2] = Versicolor\n[3] = Virginica\n\n");
    for (i = 0; i < ROW_NUM; i++) {
        for (j = 0; j < COL_NUM; j++) {
            if (strcmp(map->neurons[i][j].label, "1") == 0) {
                printf("\033[1;34m [1] \033[0m");
            }
            else if (strcmp(map->neurons[i][j].label, "2") == 0) {
                printf("\033[1;37m [2] \033[0m");
            }
            else if (strcmp(map->neurons[i][j].label, "3") == 0) {
                printf("\033[1;31m [3] \033[0m" );
            }
        }
        printf("\n");
    }
}




void usage(char * message) {
    fprintf(stderr, "Usage : %s\n", message);
    exit(1);
}


// Affiche N lignes du fichier de données
void showData(int lines, Data_vector * data_vector) {
    int i, j;
    for (i = 0; i < lines; i++) {
        for (j = 0; j < VEC_SIZE; j++) {
            printf(" %f ", data_vector[i].data[j]);
        }
        printf("Label: %s ", data_vector[i].name);
        printf("Norm: %f \n", data_vector[i].norm);
    }
}


// Affiche le réseau de neurones
void print_map(Neural_net * map) {
    int i, j, k;
    for (i = 0; i < ROW_NUM; i++) {
        printf("Row %d\n", i+1);
        for (j = 0; j < COL_NUM; j++) {
            for (k = 0; k < VEC_SIZE; k++) {
                printf("%f ", map->neurons[i][j].data[k]);
            }
            printf("\n");
        }
    }
}

