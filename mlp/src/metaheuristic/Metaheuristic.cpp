#include "Metaheuristic.hpp"
#include "Construction.hpp"
#include "LocalSearch.hpp"
#include <limits>
#include <algorithm>
#include <random>
#include <cassert>

Solution Metaheuristic::run(const Data &data, int max_iter_ils, int max_iter)
{
    Solution s_best;
    Construction cons;
    LocalSearch ls;
    s_best.cost = std::numeric_limits<double>::max();
    vector<double> R = {0.01, 0.05, 0.1, 0.15, 0.2, 0.25};

    for (int i = 0; i < max_iter; i++)
    {
        Solution s = cons.runMLP(data, R);

        cout << "após construção" << s;
        cout << s.cost << endl;
        getchar();

        int n = s.sequence.size();
        std::vector<std::vector<Subsequence>> subseq_matrix(n, std::vector<Subsequence>(n));

        UpdateAllSubseq(s, data, subseq_matrix);
        s.cost = subseq_matrix[0][s.sequence.size() - 1].C;

        cout << "após atualizar valor construçao" << s;
        cout << s.cost << endl;
        getchar();

        // cout << s;

        ls.run(data, s, subseq_matrix);
        assert(s.checkCost(data) == 0);

        cout << "após primeiro run" << s;
        cout << s.cost << endl;
        getchar();

        // cout << s;
        // getchar();


        Solution s_ils = s;
        int counter = 0;
        while (counter < max_iter_ils)
        {
            s = perturb(data, s_ils, subseq_matrix);

            cout << "após perturb" << s;
            cout << s.cost << endl;
            getchar();

            ls.run(data, s, subseq_matrix);

            cout << "após run" << s;
            cout << s.cost << endl;
            getchar();

            if (s.cost < s_ils.cost)
            {
                s_ils = s;
                counter = 0;
            }
            else
            {
                counter++;
            }

            if(counter == max_iter_ils) cout << "gils " << s_best.cost << endl;
        }
        if (s.cost < s_best.cost)
        {
            s_best = s;
        }
        assert(s.checkCost(data) == 0);
    }

    return s_best;
}

Solution Metaheuristic::perturb(const Data &data, const Solution &best, std::vector<std::vector<Subsequence>> &subseq_matrix)
{
    Solution s_pert = best;
    int n = s_pert.sequence.size() - 1;

    cout << "s_pert: " << s_pert << endl;
    cout << " best: "<< best << endl;
    getchar();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> indices;
    for (int i = 1; i < n; ++i)
    {
        indices.push_back(i);
    }
    std::shuffle(indices.begin(), indices.end(), gen); //consertar dps tb
    int a = indices[0];
    int b = indices[1];
    int c = indices[2];
    int d = indices[3];

    std::vector<int> selected = {a, b, c, d};
    std::sort(selected.begin(), selected.end());
    a = selected[0];
    b = selected[1];
    c = selected[2];
    d = selected[3];

    std::vector<int> new_seq;
    new_seq.reserve(s_pert.sequence.size());
    new_seq.insert(new_seq.end(), s_pert.sequence.begin(), s_pert.sequence.begin() + a);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + c, s_pert.sequence.begin() + d + 1);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + b, s_pert.sequence.begin() + c);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + a, s_pert.sequence.begin() + b);
    new_seq.insert(new_seq.end(), s_pert.sequence.begin() + d + 1, s_pert.sequence.end());
    s_pert.sequence = new_seq;

    UpdateAllSubseq(s_pert, data, subseq_matrix);
    s_pert.cost = subseq_matrix[0][s_pert.sequence.size() - 1].C;

    cout << "s_pert: " << s_pert << endl;
    cout << " best: " << best << endl;
    getchar();

    return s_pert;
}