#include "dfs.h"
#include "common/vector.h"

int CompareCosts(Continuation const * A, Continuation const * B)
{
    if(A->cost == B->cost) return 0;
    
    bool A_is_larger = MinCost(A->cost, B->cost) == B->cost;
    return A_is_larger ? 1 : -1;
}

/**
 * Definition of quicksort macro-templated function
 *
 * void SortByCost(Continuation *begin, Continuation *end) { ... }
 * 
 * Uses a custom function to perform comparisons
 */
DEFINE_QUICKSORT_COMP(SortByCost, Continuation, CompareCosts)

cost_t DFS(
    GameState const * gamestate,
    RoutingTable const * routing,
    ProblemData const * pdata)
{
    cost_t best_cost = INF_COST;

    DFS_impl(gamestate, routing, pdata, 0, &best_cost);
    
    return best_cost;
}

void DFS_impl(
    GameState const * gamestate,
    RoutingTable const * routing,
    ProblemData const * pdata,
    cost_t acc_cost,
    cost_t * curr_best)
{
    ContinuationArray continuations;
    NEW_VECTOR(continuations);

    ComputePossibleContinuations(gamestate, routing, pdata, &continuations);

    SortByCost(continuations.begin, continuations.end);

    for(Continuation * it = continuations.begin; it != continuations.end; ++it)
    {
        cost_t total_cost = acc_cost + it->cost;
        
        if(MinCost(total_cost, *curr_best) == *curr_best)
        {
            break; // Prunning
        }

        if(WinningGamestate(&it->state, pdata))
        {
            *curr_best = total_cost;
            printf("Found a winning gamestate with cost %d\n", *curr_best);
            fflush(stdout);
        }
        else
        {
            DFS_impl(&it->state, routing, pdata, total_cost, curr_best);
        }
    }

    CLEAR(continuations);
}
