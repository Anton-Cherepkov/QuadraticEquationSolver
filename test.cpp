#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <cstdio>

#include "EquationSolver.h"

using namespace std;

const double eps = 1e-5;

void printResult(vector<double>& result) {
    cout << "roots:\n";
    for (auto el : result) {
        cout << el << " ";
    }
    cout << endl;
}

int main() {
    freopen("in.txt", "rt", stdin);
    int n;
    EquationSolver<double>& solver = EquationSolver<double>::GetInstance();
    cin >> n;
    for (int i = 0; i < n; i++) {
        double a, b, c;
        cin >> a >> b >> c;
        Polynomial<double> polynomial({c, b, a});
        const auto roots = solver.Solve(polynomial).GetRoots();

        vector<double> result;
        for (const Root<double> &root: roots) {
            for (int j = 0; j < root.GetMultiplicity(); j++) {
                result.push_back(root.GetValue());
            }
        }

        int cnt;
        cin >> cnt;
        if (cnt == -1) {
            if (result.size() == 1 && isinf(result[0])) {
                //cout << "Test #" << i << " is OK\n";
            } else {
                cout << "test #" << i << endl;
                cout << "true answer: inf\n";
                cout << "your answer:\n";
                printResult(result);
                throw 1;
            }
            continue;
        }

        vector<double> answer;
        for (int j = 0; j < cnt; ++j) {
            double root;
            cin >> root;
            answer.push_back(root);
        }

        sort(answer.begin(), answer.end());
        sort(result.begin(), result.end());

        bool flag = 1;
        if (answer.size() != result.size()) {
            flag = 0;
        } else {
            for (int j = 0; j < answer.size(); j++) {
                if (abs(answer[j] - result[j]) > eps) {
                    flag = 0;
                }
            }
        }
        if (flag) {
            cout << "URA\n";
        } else {
            cout << "test #" << i << endl;
            cout << "true answer:\n";
            printResult(answer);
            cout << "your answer:\n";
            printResult(result);
            throw 1;
        }
    }
}
