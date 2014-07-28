#include "Genetic.h"

Genetic::~Genetic() {
}

Genetic::Genetic(std::string s) {
    // Inizializzo le variabili
    this->text = s;
    this->lenght = s.length();
    this->population = 0;
    this->max_rate = 0;
    this->passed = false;
    this->generation = 0;
    // Inizializzo il vettore
    this->popGenes.resize(Genetic::POPULATION_SIZE);
}

// Imposta la popolazione e avvia l'evoluzione
double Genetic::execute() {
    // Salvo il tempo iniziale
    this->init = clock();
    // Creo la prima popolazioni di individui
    this->firstPopulation();
    cout << "STARTING THE EVOLUTION" << endl;
    // Inizia l'evoluzione
    this->evolution();
    // Salvo il tempo finale
    this->final = difftime(clock(), this->init)/CLOCKS_PER_SEC;
    cout << "Execution time: " << this->final << "s" << endl;
	return this->final;
}

// Processo di evoluzione
void Genetic::evolution() {
    // Finchè non trova la parola
    while (!this->passed) {
    	// Se il vettore dei migliori è più grande della popolazione
    	if (this->best.size() > Genetic::POPULATION_SIZE) {
    		// Ordino il vettore e rimuovo i peggiori
            std::sort(this->best.begin(), this->best.end(), this->sortByRate);
            for (int i = this->best.size(); i > Genetic::POPULATION_SIZE; i--) {
            	this->best.pop_back();
            }
            // Salvo i migliori
            for (int i = 0; i < Genetic::POPULATION_SIZE; i++) {
            	if (this->popGenes[i].rate < this->best[i].rate) {
            		this->popGenes[i].rate = this->best[i].rate;
            		this->popGenes[i].word = this->best[i].word;
            		this->popGenes[i].code = this->best[i].code;
            	}
            }
		}
		// Ricombino la popolazione
        this->populate();
		// Ordino il vettore degli individui
		std::sort(this->popGenes.begin(), this->popGenes.end(), this->sortByRate);
	}
    // Ho trovato la parola
	cout << endl << "WORD FOUND AFTER: " << this->generation  << " GENERATIONS" << endl;
}

// Funzione di ricombinazione della popolazione
void Genetic::populate() {
    // Aumento la generazione
    this->generation++;
    cout << "START OF " << this->generation << " GENERATION"  << endl;
    Gene son;
    bool mutation;
    // Per ogni discendente
    for (int k = 0; k < Genetic::CHILDREN; k++) {
        for (int i = 0; i < Genetic::POPULATION_SIZE; i++) {
			mutation = false;
            this->population++;
            // Ogni MUTATION_RATE muto i nuovi individui
            if (this->population % Genetic::MUTATION_RATE == 0) {
                mutation = true;
            }
			// Ogni CROSS_RATE crosso due individui a caso
			if (this->population % Genetic::CROSS_RATE == 0) {
				int index1 = rand() % this->popGenes.size();
				int index2 = rand() % this->popGenes.size();
				son = this->generate(this->cross(this->popGenes[index1].code, this->popGenes[index2].code), mutation);
			}
            // Se l'individuo è vicino alla soluzione lo muto
            if (this->popGenes[i].rate >= this->max_rate) {
                son = this->generate(this->popGenes[i].code, true);
            } else {
                // Altrimenti faccio crossover tra due individui
                if (i <= (Genetic::POPULATION_SIZE - 2) && this->popGenes[i].code != this->popGenes[i + 1].code) {
                    son = this->generate(this->cross(this->popGenes[i].code, this->popGenes[i + 1].code), mutation);
                } else if (i <= (Genetic::POPULATION_SIZE - 3) && this->popGenes[i].code != this->popGenes[i + 2].code) {
                    son = this->generate(this->cross(this->popGenes[i].code, this->popGenes[i + 2].code), mutation);
                }            
            }
            
            // Se non ho fatto nessuna operazione muto l'individuo
            if ((son.code == this->popGenes[i].code && i != 0) || son.word == "") {
                son = this->generate(this->mutate(this->popGenes[i].code), mutation);
            }
            
            // Aggiorno la popolazione
            this->popGenes[i].code = son.code;
            this->popGenes[i].rate = son.rate;
            this->popGenes[i].word = son.word;

            // Aggiorno il vettore best con il nuovo individuo
            if (son.rate >= this->max_rate && !(this->in_array(son))) {
                cout << "FOUND BEST: " << son.word << endl;
                this->max_rate = son.rate;
                this->best.push_back(son);
            }
        }
    }
}

// Crea la popolazione iniziale
void Genetic::firstPopulation() {
    cout << "CREATE FIRST POPULATION" << endl;
    Gene new_ind;
    for (int i = 0; i < Genetic::POPULATION_SIZE; i++) {
        this->population++;
        bool mutation = true; // è la prima popolazione quindi muto tutti
        // Genero l'individuo
        new_ind = this->generate(this->genCode(), mutation);
        this->popGenes[i].code = new_ind.code;
        this->popGenes[i].rate = new_ind.rate;
        this->popGenes[i].word = new_ind.word;
    }
    cout << "CREATED FIRST POPULATION" << endl;
}

// Crea un individuo
Genetic::Gene Genetic::generate(std::vector<int> gene_code, bool mutation) {
    std::string new_word = "";
    Gene new_ind;
    new_ind.code.resize(this->lenght);
    if (mutation) {
        // Applico una mutazione al gene
        gene_code = this->mutate(gene_code);
    }
    for (int i = 0; i < this->lenght; i++) {
        // Prendo la lettera di posizione gene_code[i] nell'alfabeto
        std::string letter = this->LETTERS.substr(gene_code[i], 1);
        // Creo la parola
        new_word.append(letter);
    }
    // Creo l'individuo
    new_ind.code = gene_code;
    new_ind.word = new_word;
    // Controllo la parola appena creata
    new_ind.rate = this->check_success(new_word);
    return new_ind;
}

// Genera un array di numeri da 0 a lenght
std::vector<int> Genetic::genCode() {
    std::vector<int> code(this->lenght);
    for (int i = 0; i < this->lenght; i++) {
        // creo un numero
        code[i] = rand() % Genetic::LETTERS.size();
    }
    return code;
}

// Funzione per applicare una mutazione ad un individuo
std::vector<int> Genetic::mutate(std::vector<int> code) {
    // Quante lettere devo mutare
    int mutate_letter_num = floor(Genetic::MUTATION_SIZE * this->lenght / 100);
    int index;
    // Creo un nuovo codice
    if (mutate_letter_num > 0) {
        for (int i = 0; i < mutate_letter_num; i++) {
            // L'indirizzo della lettera da mutare
            index = rand() % this->lenght;
            // Eseguo la mutazione
            code[index] = rand() % this->LETTERS.size();
        }
    }
    return code;
}

// Funzione che esegue il crossover su due individui
std::vector<int> Genetic::cross(std::vector<int> g1, std::vector<int> g2) {
    std::vector<int> code(this->lenght);
    int index = rand() % this->lenght;
    // index definisce la linea di taglio
    for (int i = 0; i < this->lenght; i++) {
        if (i < index)
            code[i] = g1[i];
        else
            code[i] = g2[i];
    }
    return code;
}

// Controlla la fitness della stringa
double Genetic::check_success(std::string word) {
    double rate = 0.0;
    // Se ho trovato la parola fermo l'evoluzione
    if (this->text == word)
         this->passed = true;
	else {
    	// Controllo carattere per carattere se ci sono lettere uguali
    	for (int i = 0; i < this->lenght; i++) {
        	if (this->text.at(i) == word.at(i)) {
        	    rate++;
        	}
    	}
	}
    // Calcolo il valore di fitness
    rate = rate/this->lenght;
    return rate;
}


// Funzione per controllare se un individuo è nel vettore best
bool Genetic::in_array(Genetic::Gene needle) {
    int size = this->best.size();
    if (size == 0) return false;
    // Se ha code, rate e word uguali allora sono uguali
    for (int i = 0; i < size; i++)
        if (this->best[i].code == needle.code && this->best[i].rate == needle.rate && this->best[i].word == needle.word)
            return true;
    return false;
}

// Funzione di ordinamento dei geni per rate
bool Genetic::sortByRate(const Genetic::Gene &lhs, const Genetic::Gene &rhs) {
    return lhs.rate > rhs.rate;
}
