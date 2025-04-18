#include <iostream>
#include <vector>
#include <functional>
#include <cmath>
#include "matplotlibcpp.h"
#include "random.h"

namespace plt = matplotlibcpp;

bool shot(double x, double y, const std::function<double(double)> &f) { return y <= f(x); }

auto Mc(int k, double a, double b, double M, const std::function<double(double)> &f) {
    std::vector<double> average{};
    std::vector<std::vector<double> > outcomes{};

    double x{};
    double y{};
    int n{50};
    while (n <= 5000) {

        int tmp_k{k};
        double tmp_av{};
        double formula_out{};
        std::vector<double> current_outcomes{};

        while (tmp_k-- > 0) {
            double C{};
            for (int i{1}; i <= n; ++i) {
                x = Random::get(a, b);
                y = Random::get(0.0, M);
                if (shot(x, y, f)) ++C;
            }
            formula_out = (C / n) * (b - a) * M;
            current_outcomes.push_back(formula_out);
            tmp_av += formula_out;
        }

        average.push_back(tmp_av / k);
        outcomes.push_back(current_outcomes);
        n += 50;
    }

    return std::make_pair(average, outcomes);
}

auto f1 = [](double x) { return std::pow(x, 1.0 / 3.0); };
auto f2 = [](double x) { return std::sin(x); };
auto f3 = [](double x) { return 4 * x * pow((1 - x), 3); };

auto McPI(int k) {
    double x{};
    double y{};
    double d{};
    std::vector<double> average{};
    std::vector<std::vector<double> > outcomes{};
    double estPi{};
    int n{50};
    while (n <= 5000) {
        std::vector<double> current_outcomes{};
        int tmp_k{k};
        double tmp_av{};
        while (tmp_k-- > 0) {
            int inCircle {};
            for (int i{1}; i <= n; ++i) {
                x = Random::get(-1.0, 1.0);
                y = Random::get(-1.0, 1.0);
                d = x * x + y * y;
                if (d <= 1)
                    ++inCircle;
            }
            estPi = 4.0 * inCircle / n;
            current_outcomes.push_back(estPi);
            tmp_av += estPi;
        }
        outcomes.push_back(current_outcomes);
        average.push_back(tmp_av/k);
        n += 50;
    }

    return std::make_pair(average, outcomes);
}

void plot_results(const std::pair<std::vector<double>, std::vector<std::vector<double>>>& results, double exact_value, const std::string& title) {
    std::vector<double> n_values;
    for (int n = 50; n <= 5000; n += 50) {
        n_values.push_back(n);
    }

    for (size_t i = 0; i < results.second.size(); ++i) {
        for (size_t j = 0; j < results.second[i].size(); ++j) {
            plt::scatter(std::vector<double>{n_values[i]}, std::vector<double>{results.second[i][j]}, 10.0, { {"color", "blue"} });
        }
    }

    for (size_t i = 0; i < results.first.size(); ++i) {
        plt::scatter(std::vector<double>{n_values[i]}, std::vector<double>{results.first[i]}, 50.0, { {"color", "red"} });
    }

    std::vector<double> exact_line(n_values.size(), exact_value);
    plt::plot(n_values, exact_line, "g-");

    std::vector<double> y_ticks = {exact_value - 2, exact_value - 1, exact_value, exact_value + 1, exact_value + 2};
    plt::yticks(y_ticks);

    std::vector<double> x_ticks = {1000, 2000, 3000, 4000, 5000};
    plt::xticks(x_ticks);

    plt::title(title);
    plt::xlabel("n (Number of Points)");
    plt::ylabel("Approximated Value");
    plt::show();
}



int main() {
    //w1 = 12
    auto w11 = Mc(5, 0, 8, 12 + 1, f1);
    auto w12 = Mc(50, 0, 8, 12 + 1, f1);
    plot_results(w11, 12.0, "Integral of x^(1/3) with k=5");
    plot_results(w12, 12.0, "Integral of x^(1/3) with k=50");

    //w2 = 2
    auto w21 = Mc(5, 0, M_PI, 2 + 1, f2);
    auto w22 = Mc(50, 0, M_PI, 2 + 1, f2);
    plot_results(w21, 2.0, "Integral of sin(x) with k=5");
    plot_results(w22, 2.0, "Integral of sin(x) with k=50");

    //w3 = 0.2
    auto w31 = Mc(5, 0, 1, 0.2 + 1, f3);
    auto w32 = Mc(50, 0, 1, 0.2 + 1, f3);
    plot_results(w31, 0.2, "Integral of 4x(1-x)^3 with k=5");
    plot_results(w32, 0.2, "Integral of 4x(1-x)^3 with k=50");
    //w4 = Pi
    auto w41 = McPI(5);
    auto w42 = McPI(50);
    plot_results(w41, M_PI, "Estimation of Pi with k=5");
    plot_results(w42, M_PI, "Estimation of Pi with k=50");

    return 0;
}
