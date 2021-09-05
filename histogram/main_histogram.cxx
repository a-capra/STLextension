#include <iostream>
#include <random>
#include <vector>

#include "histogram.hxx"

int main()
{
    std::default_random_engine generator;
    double mu = -7., sigma = 3;
    std::normal_distribution<double> distribution(mu,sigma);
    int ntrials = 100000;
    std::vector<double> measurements;
    std::cout << "Fake measurements with mean " << mu << " and standard deviation " << sigma << std::endl;
    histogram<double> H1(20, -17., 3.);
    for(int i = 0; i < ntrials; ++i) 
    {
        double result = distribution(generator);
        measurements.push_back(result);
        H1.Fill(result);
    }
    H1.Stats();
    H1.Print();
    H1.PrintStats();

    histogram<double> H2(20, -17., 3.);
    H2.Fill(measurements);
    H2.Print();
    H2.PrintStats();
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
