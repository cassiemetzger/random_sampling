#include "random.h"
#include <vector> 

random_number_generator rng; 
//defining function
double f(double x){
    return std::exp(-std::cos(x)*std::cos(x))/(std::sqrt(1-x*x)); 
}
//defining weight function
double w(double x){
    return 1/(std::sqrt(1-x*x)); 
}
//monte carlo average integration
double average(int N, const std::function<double(double)>& f){
    double sum = 0.0;
    std::vector<double> samples(N); 
    for(int i = 0; i <N; i++){
        samples[i] = rng.uniform(); 
    }
    for(int i = 1; i < N; i++){
        sum += f(samples[i]); 
    }
    return 2.0*sum/N; 
}
//monte carlo importance sampling 
double importance(int N, const std::function<double(double)>&f, const std::function<double(double)>&w){
    std::vector<double> samples(N); 
    //random_number_generator random;
    double x;
    for(int i = 0; i <N; i++){
        x = rng.uniform();
        samples[i] = std::sin(M_PI*x); 
        if(samples[i] *samples[i] == 1){ //if x = 1, then functions are undefined, so we need to resample
            x = rng.uniform();
            samples[i] = std::sin(M_PI*x); 
        }
    }
    double sum = 0.0; 
    for(int i = 1; i <N; i++){
        sum += f(samples[i])/w(samples[i]);
    }
    double I = 1.0/N*sum*(M_PI); //this is only true in the case where w(x) = 1/(sqrt(1-x^2)) bc integral of w(x) between -1 and 1 = pi
    return I;
}

int main() {
    int N =  100000; 
    int steps = 1000;
    double avg_sum = 0.0;
    double imp_sum = 0.0;
    for(int i = 0; i<steps;i++){
        double avg = average(N,f);
        double imp = importance(N,f,w);
        avg_sum += avg;
        imp_sum += imp; 
    }
    double avg_mean = avg_sum/steps;
    double imp_mean = imp_sum/steps;

    double avg_variance = 0.0; 
    for(int i = 1; i < steps; i++){
        double I = average(N,f); 
        avg_variance = avg_variance + (I-avg_mean)*(I-avg_mean);
    }
    avg_variance = std::sqrt(1.0/(steps-1.0)*avg_variance);

    double imp_variance = 0.0;
    for(int i = 1; i < steps; i++){
        double I = importance(N,f,w);
        imp_variance = imp_variance + (I-imp_mean)*(I-imp_mean);
    }
    imp_variance = std::sqrt(1.0/(steps-1)*imp_variance);
    
    std::cout << "Average: " << "\n" << "mean: " << avg_mean << '\n' << "std dev: " << avg_variance << '\n' << "Importance: \n" << "mean: " << imp_mean << '\n' << "std dev: " << imp_variance << '\n'<< std::endl; 



}
