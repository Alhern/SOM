#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>


// CONFIGURATION
#define DATA_FILENAME "data/iris.data"  // chemin du fichier de données
#define MIN 0.1  // valeur min pour les nombres aléatoires
#define MAX 0.5  // valeur max pour les nombres aléatoires
#define VEC_SIZE 4  // taille des vecteurs de données, nombre d'attributs des fleurs

// Kohonen : "les unités peuvent être arrangées en toute configuration plane (rectangulaire, hexagonale, ...)"
#define ROW_NUM 10 // nombre de lignes de la matrice
#define COL_NUM 7 // nombre de colonnes de la matrice

#define RADIUS 9 // rayon de voisinage
#define ALPHA_PHASE_1 0.8  // taux d'apprentissage pour la phase 1
#define ALPHA_PHASE_2 0.08 // taux d'apprentissage pour la phase 2
#define ITER_PHASE_1 100 // nombre d'itérations pour la phase 1
#define ITER_PHASE_2 1000 // nombre d'itérations pour la phase 2


// STRUCTURES

// Structure d'un neurone
typedef struct Neuron {
    char * label;
    float * data;
} Neuron;

// Structure du réseau de neurones
typedef struct Neural_net {
    int n_rows;
    int n_cols;
    Neuron ** neurons;
} Neural_net;

// Structure du vecteur de données
typedef struct Data_vector {
    float * data;   // les 4 attributs des fleurs (sepal length, width, petal length, width)
    char * name;   // l’étiquette, l’espèce des fleurs (setosa, versicolor, virginica)
    float norm;   // norme du vecteur de données
} Data_vector;

// Initialisation du réseau de neurones
Neural_net * neural_network;

// Initialisation du vecteur de données
Data_vector * data_vector;


// PROTOTYPES

// Nombres aléatoires
int randomInt(int, int);
float randomFloat(float, float);

// Initialisation des vecteurs de données
int countLines(char *);
void vectorMalloc(int, int *);
void convertLabel(char *, char *);
void dataVectorInit(char *, int);
void normCalc(int);
void normalize(int);

// Apprentissage
void mapInit();
void shuffle_vects(int *, int);
float get_distance(float *, float *);
int * get_BMU(Neural_net * neural_network, Data_vector * data);
void scale_neighborhood(Neural_net * neural_network, int *, int, int, float);
void map_training(Neural_net * neural_network, Data_vector * data, int, float, int, int *);
void label_map(Neural_net * neural_network, Data_vector * data, int);
void display_map(Neural_net * neural_network);

// Auxiliaires
void printVector(float *, int);
void showData(int, Data_vector * data_vector);
void usage(char *);
void print_map(Neural_net * map);