#include "experiment.h"
#include "random.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>


std::vector<std::vector<std::array<int, 5>>> experiment() {
    int birthday_paradox;        // Moment of first collision
    int n_empty_bins;            // Number of empty bins after n balls
    int coupon_collector_problem;// First moment when there's no empty bins (Cn)
    int m_two_balls;             // First moment when all bins have at least 2 balls (Dn)
    int mtb_ccp;                 // m_two_balls - coupon_collector_problem (Dn - Cn)
    std::vector<std::vector<std::array<int, 5>>> results;

    for(int n = 1000; n <= 100000; n += 1000) {
        std::vector<std::array<int, 5>> r_exp;
        std::cout<<n<<'\n';
        for(int k = 0; k < 50; ++k) {
            std::vector<int> bins(n, 0);
            birthday_paradox = n_empty_bins = coupon_collector_problem = m_two_balls = mtb_ccp = 0;
            bool first_collision = false, first_all_filled = false, first_all2_filled = false;
            int how_many_balls = 0, current_empty_bins = n, bins_less_than_two = n;

            // Experiment
            while(!first_all2_filled) {
                ++how_many_balls;
                if (how_many_balls == std::numeric_limits<int>::max()) {
                    std::cerr << "Reached maximum int capacity for how_many_balls." << std::endl;
                    break; // or handle the situation as needed
                }
                int r_bin = Random::get(0, n - 1); // Select random bin for the ball

                // Check for first collision (Birthday Paradox)
                if(!first_collision && bins[r_bin] != 0) {
                    first_collision = true;
                    birthday_paradox = how_many_balls;
                }

                // Update empty bins count
                if(bins[r_bin] == 0) {
                    --current_empty_bins;
                }

                // Update bins less than two count if the bin will reach two balls
                if(bins[r_bin] == 1) {
                    --bins_less_than_two;
                }

                // Increment the bin's ball count
                ++bins[r_bin];

                // Check for Coupon Collector's Problem (all bins have at least one ball)
                if(!first_all_filled && current_empty_bins == 0) {
                    first_all_filled = true;
                    coupon_collector_problem = how_many_balls;
                }

                // Check for all bins having at least two balls (Dn)
                if(bins_less_than_two == 0) {
                    first_all2_filled = true;
                    m_two_balls = how_many_balls;
                }

                // Record number of empty bins after n balls have been thrown
                if(how_many_balls == n) {
                    n_empty_bins = std::count(bins.begin(), bins.end(), 0);
                }
            }
            mtb_ccp = m_two_balls - coupon_collector_problem; // Dn - Cn
            // Save data from the experiment
            r_exp.push_back({birthday_paradox, n_empty_bins, coupon_collector_problem, m_two_balls, mtb_ccp});
        }
        results.push_back(r_exp);
    }
    return results;
}

