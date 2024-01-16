#include <bits/stdc++.h>
using namespace std;

double lglr(double x, const vector<pair<double, double>>& points) {
    double result = 0.0;
    for (int i = 0; i < points.size(); i++) {
        double term = points[i].second;
        for (int j = 0; j < points.size(); j++) {
            if (i != j) {
                term *= (x - points[j].first) / (points[i].first - points[j].first);
            }
        }
        result += term;
    }
    return result;
}

int main() {
    double x;
    cin >> x;

    vector<pair<double, double>> points;
    string line;
    getline(cin, line);  
    while (getline(cin, line)) {
        istringstream iss(line);
        double x1, y1;
        if (iss >> x1 >> y1) {
            points.push_back(make_pair(x1, y1));
            if (points.size() >= 100) {
                break;
            }
        } else {
            break;
        }
    }

    double raw = lglr(x, points);
    double ans = ((int)(raw*10000))/10000.0; 
    cout << fixed << setprecision(4) << ans << endl;
    return 0;
}