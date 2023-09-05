/**
 * @file simulation.h
 * @brief Header file of the MCMC algorithm function, and of the SingleRunResults class that stores the results
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#pragma once

#include <diagmc/diagram.h>
#include <ostream>
#include <chrono>


/**
 * @brief Container class to store all the simulation parameters, and the results of a run.
 * It provides methods to print the results on standard output, and also to produce a 
 * formatted single-line output, e.g. for writing to file
 * 
 */
class SingleRunResults 
{

    private:

    //parameters of the simulation
    double beta;                                    ///< length of the diagram (here representing the thermondinamical $\beta$ = 1/T). Must be > 0.
    double initial_s0;                                      ///< spin of the 0-th segment of the diagram [0---t1] at the beginning of the simulation. Must be +1 or -1
    double H;                                       ///< value of the longitudinal component of magnetic field
    double GAMMA;                                   ///< Value of the transversal component of magnetic field. Must be != 0.
    unsigned long long int N_total_steps;           ///<Total number of steps of the MCMC algorithm
    unsigned long long int N_thermalization_steps;  ///<Number of initial steps for which statistics is not collected
    unsigned long long int update_choice_seed;      ///<Seed for the Mersenne-Twister random number generator to choose WHICH update to attempt
    unsigned long long int diagram_seed;            ///<Seed for the diagram, used INSIDE the updates


    public:
 
    //variables to store the statistics of the simulation
    unsigned long long int N_measures = 0;                  ///< Number of samples (iterations) for which the magnetizations and diagram order statstics were collected
    unsigned long long int N_attempted_flips = 0;           ///< Number of times the SPIN_FLIP update was attempted
    unsigned long long int N_accepted_flips = 0;            ///< Number of times the SPIN_FLIP update was accepted
    unsigned long long int N_attempted_addsegment = 0;      ///< Number of times the ADD_SEGMENT update was attempted
    unsigned long long int N_accepted_addsegment = 0;       ///< Number of times the ADD_SEGMENT update was accepted
    unsigned long long int N_attempted_removesegment = 0;   ///< Number of times the REMOVE_SEGMENT update was attempted
    unsigned long long int N_accepted_removesegment = 0;    ///< Number of times the REMOVE_SEGMENT update was accepted
    unsigned long long int max_diagram_order = 0;           ///< Maximum diagram order during the whole run
    unsigned long long int avg_diagram_order = 0;           ///< Average diagram order during the whole run
    unsigned long long int run_time = 0;                    ///< Execution time (in nanoseconds) for the Markov Chain loop (not the program run time)
    double measured_sigmax = 0;                             ///< Final value of the magnetization along x calculated through the MCMC algorithm
    double measured_sigmaz = 0;                             ///< Final value of the magnetization along z calculated through the MCMC algorithm



    /**
     * @brief Construct a new Single_Run_Results object, initializing the parameters of the simulation with the arguments,
     * and the statistics variables to 0.
     * 
     * @param beta       Length of the diagram (here representing 1/T). Must be > 0.
     * @param initial_s0         Spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
     * @param N_total_steps Total number of steps of the MCMC algorithm
     * @param N_thermalization_steps  Number of initial steps for which statistics is not collected
     * @param update_choice_seed  Seed for the Mersenne-Twister random number generator to choose WHICH update to attempt
     * @param diagram_seed Seed for the diagram, used INSIDE the updates
     */
    SingleRunResults(
        double beta, 
        double initial_s0, 
        double H, 
        double GAMMA, 
        unsigned long long int N_total_steps, 
        unsigned long long int N_thermalization_steps, 
        unsigned long long int update_choice_seed, 
        unsigned long long int diagram_seed
        );


    /**
     * @brief Prints a summary of the result of the run on the terminal standard output
     * 
     */
    void print_results() const;


    /**
     * @brief Returns a line containing the titles of the columns of the output file
     * 
     * @return std::string 
     */
    static std::string ostream_output_header();


    /**
     * @brief Output stream operator to write a single formatted line with all the parameters and results of the simulation
     * 
     * @param os std::ostream object, e.g. std::ofstream, or std::cout
     * @param results SingleRunResults object, containin all the parameters and results of the simulation
     * @return std::ostream&
     */
    friend std::ostream & operator << (std::ostream & os, const SingleRunResults & results);

};


/**
 * @brief Runs the Markov Chain Diagrammatic Monte Carlo algorithm for the 2-level spin sistem, with the given parameters,
 * returning the results statistics
 * 
 * @param beta       Length of the diagram (here representing 1/T). Must be > 0.
 * @param initial_s0         Spin of the 0-th segment of the diagram [0---t1] at the beginning of the simulation. Must be +1 or -1
 * @param H          Value of the longitudinal component of magnetic field
 * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
 * @param N_total_steps Total number of steps of the MCMC algorithm
 * @param N_thermalization_steps  Number of initial steps for which statistics is not collected
 * @param update_choice_seed  (optional) Seed for the Mersenne-Twister random number generator to choose WHICH update to attempt.
 * @param diagram_seed (optional) Seed for the diagram, used INSIDE the updates
 * @return SingleRunResults 
 */
SingleRunResults run_simulation(
        double beta, 
        double initial_s0, 
        double H, 
        double GAMMA,
        unsigned long long int N_total_steps, 
        unsigned long long int N_thermalization_steps,
        unsigned long long int update_choice_seed = std::chrono::system_clock::now().time_since_epoch().count(), 
        unsigned long long int diagram_seed = std::chrono::system_clock::now().time_since_epoch().count() 
    );
