// On effectue les importations nécessaires
# include <string.h>
# include <stdio.h>
# include <stdlib.h>

# include "read_families.h"
// On définit notre méthode main
int main() {
    Family families[MAX_FAMILIES];
    int num_families = read_families(families);
    if (num_families == 0) {
        printf("Aucune famille n'a ete lue\n");
        return 1; }

    // On initialise ici les affectations des familles aux jours donnés
    int assignment[MAX_FAMILIES];
    assign_to_first_available_choice(assignment, num_families, MAX_DAYS, families);
    printf("Initial cost = %d\n", total_cost(families, assignment, num_families, MAX_DAYS));
    print_daily_population(assignment, num_families, MAX_DAYS, families);

    // On définit maintenant les paramètres d'optimisation associés au recuit simulé et qui peuvent être modifiés pour
    // améliorer l'optimisation
    int max_iterations = 100000;
    double initial_temperature = 100.0;
    double cooling_rate = 0.000000001;

    // On définit un nombrd d'itérations (à 100 par exemple) et sur ce nombre d'itérations on stocke à la fin le meilleur cout trouvé
    int num_runs = 100;
    int best_global_cost = INT_MAX;
    int best_global_assignment[MAX_FAMILIES];
    int sum_best_costs = 0;
    // voici la boucle d'itération
    for (int run = 0; run < num_runs; run++) {
        int temp_assignment[MAX_FAMILIES];
        memcpy(temp_assignment, assignment, num_families * sizeof(int));
        // On utilise l'algorithme du recuit simulé pour optimiser les affectations
        optimize_assignment(temp_assignment, num_families, MAX_DAYS, families, max_iterations, initial_temperature, cooling_rate);
        int current_cost = total_cost(families, temp_assignment, num_families, MAX_DAYS);
        sum_best_costs += current_cost;
        // On met à jour les couts et les affectations globaux si cela s'avère nécessaire
        if (current_cost < best_global_cost) {
            best_global_cost = current_cost;
            memcpy(best_global_assignment, temp_assignment, num_families * sizeof(int)); } }


    //On imprime le cout moyen optimal sur l'ensemble des itérations
    double average_best_cost = (double) sum_best_costs / num_runs;
    printf("Average optimal cost: %lf\n", average_best_cost);

    //On imprime le meilleur cout trouvé parmi les itérations
    printf("Best global cost found: %d\n", best_global_cost) ;
    memcpy(assignment, best_global_assignment, num_families * sizeof(int)) ;

    //on imprime le nombre de personnes par jour
    print_daily_population(assignment, num_families, MAX_DAYS, families);

    return 0;
}

