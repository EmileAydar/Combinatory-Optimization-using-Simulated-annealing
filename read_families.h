// Nous faisons les importations nécessaires
# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <stdbool.h>
# include <time.h>

// On définit les variables globales, qui seront utilisées dans le reste du code
#define MAX_FAMILIES 1000
#define MAX_DAYS 7
#define DAILY_TOLERATED_LIMIT 300 //Vous pouvez modifier ce paramètres et le baisser à 280

// On définit un struct "Family" qui prend en compte les 6 colonnes du csv :
// un entier pour la taille d'une famille, et une liste de 5 éléments pour les choix d'une familles
typedef struct {
    int family_size;
    int choices[5];
} Family;


// On code la méthode read_families pour lire et utiliser les données du fichier csv
int read_families( Family *families) {
    // On déclare un pointeur de fichier
    FILE *FilePointer ;
    // On affecte au pointeur un lien (chemin absolu ou relatif) à ouvrir, et on met en mode lecture
    FilePointer = fopen("C:\\Users\\yunus\\Desktop\\Mini_project_ C\\pb10.csv", "r"); // changer le chemin pour que l'algorithme marche
    // Boucle d'exception qui retourne une erreur printée si jamais le csv n'est pas détecté
    if (FilePointer == NULL) {
        printf("Erreur d'ouverture du fichier\n");
        return 0;
    }
    // On déclare un compteur de lignes et un tampon chargé de stocker chaque ligne sous forme de charactères, avec au maximum 256 charactères par ligne
    int records = 0;
    char line[256];
    // On définit ici une boucle conditionnelle: tant qu'il y'a des lignes à lire dans le fichier...
    while (fgets(line, sizeof(line), FilePointer) ) {
        // On récupère les différentes données de la ligne et on les stocke dans la structure Family
        if (sscanf(line, "%d; %d; %d; %d; %d; %d",
                   &families[records].family_size,
                   &families[records].choices[0] ,
                   &families[records].choices[1],
                   &families[records].choices[2] ,
                   &families[records].choices[3],
                   &families[records].choices[4]) == 6) {
            // On incrémente de 1 le compteur dès qu'une famille a été lue, sachant que 1 ligne = 1 famille.
            records++;
        } else {
            // On ignore la première ligne car elle correspond à l'intitulé des colonnes -> pas utile
            printf("Premiere ligne (intitule des colonnes) ignore\n");
}
    }

    // Une fois que le travail de lecture a été effectué, on peut fermer le fichier
    fclose(FilePointer);
    // On retourne le nombre de lignes, donc de familles.
    return records;}


// On code ici une méthode pour calculer le cout d'affectation d'une famille en fonction du jour choisi
int family_cost(const Family *family, int day) {
    // Par défaut, le choix est initialisé à -1
    int choice = -1;
    // On déclare une variable entière nmb et on lui affecte la taille d'une famille, depuis le struct Family
    // (comme family est un pointeur, on utilise une flèche "->")
    int nmb = family->family_size;
    // On déclare une boucle "for" afin de pouvoir trouver le rang du jour choisi parmi les choix de la famille
    for (int i = 0; i < 5; i++) {
        if (family->choices[i] == day) {
            choice = i;
            break;}
    }
    // On fait une disjonction de cas dans laquelle on attribue des couts en fonction du rang du jour choisi
    switch (choice) {
        case 0:
            return 0;
        case 1:
            return 50;
        case 2:
            return 50 + 9 * nmb;
        case 3:
            return 100 + 9 * nmb;
        case 4:
            return 200 + 9 * nmb;}
}


// On code ici une méthode pour calculer les couts de pénalités entre deux jours donnés
int penalty(int pop1, int pop2) {
    return ((pop1 - 125) / 400) * pow(pop1, abs( (pop1 - pop2) /50));}


// On code ici une méthode pour calculer la fonction objectif, c'est-à-dire le cout d'affectation + pénalité
int total_cost(const Family *families, const int *assignment , int num_families, int num_days) {
    // On intialise une variable entière de cout à 0, et une liste de population sur la semaine aussi
    int cost = 0;
    int pop[MAX_DAYS] = {0} ;
    // On fait une boucle conditionnelle pour ajouter un cout donné associé au jour auquel la famille est associée
    // On met ensuite à jour la population
    for (int i = 0; i < num_families; i++) {
        int day = assignment[i];
        cost += family_cost(&families[i], day);
        pop[day - 1] += families[i].family_size;
    }
    // On appelle la méthode "penalty" pour associer un cout de pénalité à chaque paire de jours consécutifs
    for (int i = 0; i < num_days - 1; i++){
        cost += penalty(pop[i], pop[i + 1]);
    }
    // On prend en compte le cas du dernier jour, où la formule de pénalité est différente.
    cost += (pop[num_days-1]-125)/400;
    // On retourne la variable "cost" qui correspond à la somme du cout d'affectation et de la pénalité
    return cost; }


// On code maintenant une méthode qui va affecter les familles dans un premier temps
// en affectant les familles au premier choix choix disponible parmi ses choix
void assign_to_first_available_choice(int *assignment, int num_families, int num_days, Family *families) {
    // On initialise un tableau qui va stocker le nombre de personnes par jour, on l'initialise d'abord à 0.
    int daily_population[MAX_DAYS] = {0};
    //On va parcourir chaque famille dans cette boucle
    for (int i = 0; i < num_families; i++) {
        // On déclare une variable booléenne qui est fausse lorsque le nombre de personnes par jour >300
        // et vrai dans le cas contraire
        bool assigned = false;
        //On parcourt les familles et on essaie de les affecter au premier choix dispo
        for (int j = 0; j < 5; j++) {
            int choice_day = families[i].choices[j];
            // Contrainte sur le nombre de personnes par jour qui ne doit pas dépasser 300 !
            if (daily_population[choice_day] + families[i].family_size <= DAILY_TOLERATED_LIMIT) {
                assignment[i] = choice_day + 1 ;
                daily_population[choice_day ] += families[i].family_size;
                assigned = true;
                break;}
}

        // On affecte maintenant les familles restantes aux jours où il reste de la place
        if (!assigned) {
            for (int day = 0; day < num_days; day++) {
                if (daily_population[day] + families[i].family_size <= DAILY_TOLERATED_LIMIT) {
                    assignment[i] = day + 1;
                    daily_population[day] += families[i].family_size;
                    break; }}}
}}


// On code une méthode qui ne retourne rien et qui imprime le nombre de personnes disponible par jour.
void print_daily_population(const int *assignment, int num_families, int num_days, const Family *families) {
    //On initialise un tableau à 7 éléments destinés à être remplis en familles lorsqu'elles seront affectées
    int daily_population[MAX_DAYS] = {0};

    for (int i = 0; i < num_families; i++) {
        int day = assignment[i] - 1;
        daily_population[day] += families[i].family_size;
    }

    printf("Daily population:\n");
    for (int day = 0; day < num_days; day++) {
        //On imprime le nombre de personnes par jour
        printf("Day %d: %d people\n", day + 1, daily_population[day]);
    }
}
// On code ici une méthode booléenne qui définit si un permutation de deux familles est licite ou pas,
//c'est-à-dire si, après permutation, chaque famille ou groupe de famille va se retrouver dans un jour strictement
//compris dans ses choix, et si la limite à 300 personnes par jour est respectée
bool is_valid_swap(int *assignment, int family_index, int new_day, int num_families, int num_days, Family *families) {
    int daily_population[MAX_DAYS] = {0};
    // On détermine la population quotidienne pour chaque jour avant d'avoir affecté la famille actuelle
    for (int i = 0; i < num_families; i++) {
        if (i == family_index) continue;
        int day = assignment[i] - 1;
        daily_population[day] += families[i].family_size;
    }
    // On vérifie si l'échange respecte la limite de 300 personnes par jour
    int new_day_index = new_day - 1;
    if (daily_population[new_day_index] + families[family_index].family_size > DAILY_TOLERATED_LIMIT) {
        return false;
    }
    // On vérifie enfin si le jour affecté se trouve dans les choix de la famille
    bool choice_allowed = false;
    for (int i = 0; i < 5; i++) {
        if (families[family_index].choices[i] == new_day) {
            choice_allowed = true;
            break; } }
    return choice_allowed; }


// C'est la méthode la plus importante de ce code, qui optimise les affectations avec la métaheuristique du
// "recuit simulé"
void optimize_assignment(int *assignment, int num_families, int num_days, Family *families, int max_iterations, double initial_temperature, double cooling_rate) {
    // On initialise les variable de cout courant, meilleur cout et de meilleure affectation
    // et on crée une copie qui va être modifiée
    int current_cost = total_cost( families, assignment, num_families, num_days);
    int best_cost = current_cost ;
    int best_assignment[MAX_FAMILIES ];
    memcpy(best_assignment, assignment , num_families * sizeof(int));
    // On initialise la variable de température et la graine randome
    double temperature = initial_temperature;
    srand(time(NULL));

    // Voici la boucle d'optimisation
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        // On sélectionne aléatoirement les familles et leur nouveau jour
        int num_swaps = 1 + rand() % (num_families / 40); // On peut faire varier cette valeur manuellement pour améliorer l'optimisation
        int family_indices[num_swaps];
        int old_days[num_swaps];
        int new_days[num_swaps];
        // Les nouveaux jours doivent être compris parmi les choix des familles
        for (int i = 0; i < num_swaps; i++) {
            family_indices[i] = rand() % num_families;
            old_days[i] = assignment[family_indices[i]];
            new_days[i] = families[family_indices[i]].choices[rand() % 5]; }

        // On vérifie si les échanges sont valides, et on les effectue si c'est le cas
        bool valid_swap = true;
        for (int i = 0; i < num_swaps; i++) {
            if (!is_valid_swap(assignment, family_indices[i], new_days[i], num_families, num_days, families)) {
                valid_swap = false;
                break; } }

        if (valid_swap){
            for (int i = 0; i < num_swaps; i++) {
                assignment[family_indices[i]] = new_days[i]; }

            int new_cost =  total_cost(families, assignment, num_families, num_days);
            // On accepte ici les nouvelles affectations si elles ont un cout inférieur, ou si
            // la probabilité aléatoire est inférieure à la probabilité définie ci-dessous (critère de Metropolis).
            if (new_cost < current_cost || (double) rand() / RAND_MAX < exp((current_cost - new_cost) / temperature)) {
                current_cost = new_cost;
            } else {
                for (int i = 0; i < num_swaps; i++) {
                    assignment[family_indices[i]] = old_days[i]; } }
            // On met ici à jour les nouvelles affectations si le cout actuel est inférieur au meilleur cout jusqu'à présent
            if (current_cost < best_cost) {
                best_cost = current_cost;
                memcpy(best_assignment, assignment, num_families * sizeof(int)); } }
        // On multiplie  la variable de température par le coefficient de refroidissement
        temperature *= cooling_rate; }
    // On copie enfin les meilleures affectation trouvées
    memcpy(assignment, best_assignment, num_families * sizeof(int));
    //printf("Best cost found: %d\n", best_cost);
}

// On optimise encore davantage notre algorithme heuristique, en lui ajoutant un algo glouton (escalade de colline)
void hill_climbing(int *assignment, int num_families, int num_days, Family *families, int max_iterations) {
    int current_cost = total_cost(families, assignment, num_families, num_days);
    int best_cost = current_cost;
    int best_assignment[MAX_FAMILIES];
    memcpy(best_assignment, assignment, num_families * sizeof(int));

    srand(time(NULL));
    // On définit ci dessous la boucle d'optimisation de l'algorithme
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        int family_index = rand() % num_families;
        int old_day = assignment[family_index];
        int new_day = families[family_index].choices[rand() % 5];
    // On vérifie à nouveau si un échange est valide et on l'effectue si c'est le cas
        if (is_valid_swap(assignment, family_index, new_day, num_families, num_days, families)) {
            assignment[family_index] = new_day;
            int new_cost = total_cost(families, assignment, num_families, num_days);
            //On met à jour les couts actuels s'ils sont inférieurs au meilleur cout précédent
            if (new_cost < current_cost) {
                current_cost = new_cost;
            } else {
                assignment[family_index] = old_day; }
            //On met également à jour les affectations avec les couts
            if (current_cost < best_cost) {
                best_cost = current_cost;
                memcpy(best_assignment, assignment, num_families * sizeof(int)); } } }


// On copie et affiche les meilleures affectations et le meilleur cout associé que retourne la méthode hill_climbing
    memcpy(assignment, best_assignment, num_families * sizeof(int));
    printf("Best cost after hill climbing: %d\n", best_cost); }
