/**
 * @file simulation.cpp
 * @brief Definition of the MCMC algorithm function, and of the SingleRunResults class that stores the results
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#include <diagmc/simulation.h>
#include <diagmc/diagram.h>
#include <chrono>
#include <iostream>
#include <string>


SingleRunResults::SingleRunResults(
        double beta, 
        double initial_s0, 
        double H, 
        double GAMMA, 
        unsigned long long int N_total_steps, 
        unsigned long long int N_thermalization_steps,
        unsigned long long int update_choice_seed, 
        unsigned long long int diagram_seed
        )
    : beta(beta), initial_s0(initial_s0), H(H), GAMMA(GAMMA), N_total_steps(N_total_steps), 
      N_thermalization_steps(N_thermalization_steps), update_choice_seed(update_choice_seed), 
      diagram_seed(diagram_seed) { }


std::string SingleRunResults::ostream_output_header()
{
    return 
        "beta,"
        "initial_s0,"
        "H,"
        "GAMMA,"
        "measured_sigmax,"
        "measured_sigmaz,"
        "N_measures,"
        "N_attempted_flips,"
        "N_accepted_flips,"
        "N_attempted_addsegment,"
        "N_accepted_addsegment,"
        "N_attempted_removesegment,"
        "N_accepted_removesegment,"
        "max_diagram_order,"
        "avg_diagram_order,"
        "run_time,"
        "N_total_steps,"
        "N_thermalization_steps," 
        "update_choice_seed,"
        "diagram_seed\n";
}

std::ostream & operator<<(std::ostream &os, const SingleRunResults &results)
{
    return os << 
            results.beta << ',' <<
            results.initial_s0 << ',' <<
            results.H << ',' <<
            results.GAMMA << ',' <<
            results.measured_sigmax << ',' <<
            results.measured_sigmaz << ',' <<
            results.N_measures << ',' <<
            results.N_attempted_flips << ',' <<
            results.N_accepted_flips << ',' <<
            results.N_attempted_addsegment << ',' <<
            results.N_accepted_addsegment << ',' <<
            results.N_attempted_removesegment << ',' <<
            results.N_accepted_removesegment << ',' <<
            results.max_diagram_order << ',' <<
            results.avg_diagram_order << ',' <<
            results.run_time << ',' <<
            results.N_total_steps << ',' <<
            results.N_thermalization_steps << ',' << 
            results.update_choice_seed << ',' << 
            results.diagram_seed << std::endl;
}



void SingleRunResults::print_results() const
{
    //theoretical values for comparison
    double E = sqrt(H * H + GAMMA * GAMMA);
    double mz_exact = -H / E * tanh(beta * E);
    double mx_exact = -GAMMA / E * tanh(beta * E);

    std::cout << "\nResults:\n\n";

    std::cout << "Parameters:\n";
    std::cout << "beta  : " << beta << '\n';
    std::cout << "h     : " << H << '\n';
    std::cout << "gamma : " << GAMMA << '\n';
    

    std::cout << "\nMeasures:\n";
    std::cout << "sigma_z: " << measured_sigmaz << ".  exact mz: " << mz_exact << ".  diff: " << (measured_sigmaz - mz_exact) / mz_exact * 100<< "%\n";
    std::cout << "sigma_x: " << measured_sigmax << ".  exact mx: " << mx_exact << ".  diff: " << (measured_sigmax - mx_exact) / mx_exact * 100<< "%\n";


    std::cout << "\nStatistics:\n" <<
        "Accepted add   :  " << N_accepted_addsegment << "/" << N_attempted_addsegment << " = " << (double)N_accepted_addsegment / N_attempted_addsegment * 100 << "%\n" <<
        "Accepted remove:  " << N_accepted_removesegment << "/" << N_attempted_removesegment << " = " << (double)N_accepted_removesegment / N_attempted_removesegment * 100 << "%\n" <<
        "Accepted flips :  " << N_accepted_flips << "/" << N_attempted_flips << " = " << (double)N_accepted_flips / N_attempted_flips * 100 << "%\n" <<
        "Max order      :  " << max_diagram_order << '\n' <<
        "Average order  :  " << avg_diagram_order << '\n';
    
    std::cout << "\nPerformance:\n" <<
        "Run time:  " << run_time / 1e9 << " seconds (" << run_time/N_total_steps << " ns per step)\n";
}




SingleRunResults run_simulation(
    double beta, 
    double initial_s0, 
    double H, 
    double GAMMA, 
    unsigned long long int N_total_steps, 
    unsigned long long int N_thermalization_steps,
    unsigned long long int update_choice_seed, 
    unsigned long long int diagram_seed
    ) 
{

    //objects for random choice of the update
    std::mt19937 mt_generator(update_choice_seed);
    std::uniform_real_distribution<double> uniform_distribution(0, 1);
    #define RNG uniform_distribution(mt_generator)


    //initialize diagram object, to be a 0-order diagram with the given parameters
    Diagram diagram(beta, initial_s0, H, GAMMA, {}, diagram_seed);


    //initialize results object, inserting the simulation parameters, and setting to 0 the statistics variables.
    SingleRunResults results(beta, initial_s0, H, GAMMA, N_total_steps, N_thermalization_steps, update_choice_seed, diagram_seed);


    //define probabilities of choosing the updates. There is no need in principle for the user to 
    //modify them, hence they are not exposed as parameters.
    constexpr double attempt_flip_probability = 1./3;
    constexpr double attempt_add_probability = (1 - attempt_flip_probability)/2;
    constexpr double attempt_remove_probability = attempt_add_probability;


    //temporary variables to store the partial results
    double temp_sigmax = 0;
    double temp_sigmaz = 0;
    double temp_avgorder = 0;
    double temp_maxorder = 0;


    //Performance metrics of the run
    auto initial_time = std::chrono::high_resolution_clock::now();

    //main loop
    for (unsigned long long int loop_iteration = 0; loop_iteration < N_total_steps; ++loop_iteration)
    {

        double which_update = RNG; //ramdom extraction of the update

        //select the update and attempt to perform it using the proper Diagram method
        if (which_update < attempt_add_probability)
        {
            ++results.N_attempted_addsegment;
            results.N_accepted_addsegment += diagram.attempt_add_segment();
        }
        else if (which_update < attempt_add_probability + attempt_remove_probability)
        {
            ++results.N_attempted_removesegment;
            results.N_accepted_removesegment += diagram.attempt_remove_segment();
        }
        else
        {
            ++results.N_attempted_flips;
            results.N_accepted_flips += diagram.attempt_spin_flip();
        }
        

        //collect statistics
        if (loop_iteration >= N_thermalization_steps)   //measure samples only after thermalization steps (the = since counter starts from 0)
        {
            
            auto current_diagorder = diagram.order(); //local variable to avoid calling the method multiple times

            temp_sigmax += current_diagorder;
            temp_sigmaz += (beta - 2*diagram.sum_deltatau()) * diagram.get_s0() / beta;  

            temp_avgorder += current_diagorder;
            temp_maxorder = temp_maxorder > current_diagorder ? temp_maxorder : current_diagorder;

            ++results.N_measures;

        } 

    }
    auto final_time = std::chrono::high_resolution_clock::now();

    //caclulating final results
    results.run_time = std::chrono::duration_cast<std::chrono::nanoseconds>(final_time - initial_time).count();
    results.measured_sigmax = temp_sigmax / -(results.N_measures * beta * GAMMA);
    results.measured_sigmaz = temp_sigmaz / results.N_measures;  
    results.avg_diagram_order = temp_avgorder / results.N_measures;
    results.max_diagram_order = temp_maxorder;

    return results;

}






