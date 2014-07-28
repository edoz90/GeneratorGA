#ifndef GENETIC_H
#define	GENETIC_H

#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>

using namespace std;

class Genetic {
public:
    // costruttore
    Genetic(std::string);
	~Genetic();
    double execute();

private:
    // struct dei geni
    struct Gene {
        // la parola generata
        std::string word;
        // la bontà della parola
        double rate;
        // il codice genetico
        std::vector<int> code;
    };
    // Dimensioni della popolazione
    static const int POPULATION_SIZE = 300000;
    // Imposta la dimensione della mutazione
    static const int MUTATION_SIZE = 10;
    // Imposta la frequenza della mutazione
    static const int MUTATION_RATE = 400;
	// Imposta la frequenza del crossover occasionale
	static const int CROSS_RATE = 600;
    // L'alfabeto di partenza
    const std::string LETTERS = "abcdefghijklmnopqrstuvwxyz ";
    // Numero di discendenti da creare
    static const int CHILDREN = 2;
    // La parola da cercare
    std::string text;
    // La lunghezza della parola
    int lenght;
    // Numero di invididui totali
    int population;
    // Numero della generazione
    int generation;
    // Miglior valutazione, tempo finale d'esecuzione
    double max_rate, final;
    // Flag di fine esecuzione
    bool passed;
    // Vettore degli individui
    std::vector<Gene> popGenes;
    // Vettore degli individui migliori
    std::vector<Gene> best;
    // Tempo iniziale
    clock_t init;
    
    // Funzione di fitness
    double check_success(std::string);
    // Funzione di mutazione
    std::vector<int> mutate(std::vector<int>);
    // Funzione di crossover
    std::vector<int> cross(std::vector<int>, std::vector<int>);
    // Funzione di inizilizzazione della popolazione
    void firstPopulation();
    // Genera il codice genetico
    std::vector<int> genCode();
    // Genera un individuo
    Gene generate(std::vector<int>, bool);
    // Funzione di evoluzione della popolazione
    void evolution();
    // Funzione per la ricombinazione
    void populate();
    // Controlla se un individuo è gia stato inserito
    bool in_array(Gene);
    // Ordina gli individui
    static bool sortByRate (const Gene&, const Gene&);
};

#endif	/* GENETIC_H */

