# Combinatory-Optimization-Problem

This is an assignment problem that has been solved using a simulated annealing algorithm in C language.

We have 5 different csv files, containing each 6 columns (family_size, choice 1,...., choice 5), which respectively represent the size of the family, and its five day choices among the 7 days of the week.
The families' choices are ordered by preference (meaning the first choice is the most preferred choice,...the last choice is the least wanted one).
This means that there are some assignment costs depending on to which day each family has been assigned among their day choices.

There are also non-linear penalty costs that have to be taken into account, that depend on the family distribution (the more it is uniform, the lower the costs).

The aim is to minimize the objective function f = assignment_cost+penalty_cost, and return it.

To make the problem harder, we have also put a daily population constraint : no less than 125 people and more than 300 people per day ( we can lower the upperbound to 280, the problem gets harder but still has solutions, wherehas under 280 the code gives errors since there is not enough place for families in the first CSV file).
