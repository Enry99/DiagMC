/**
 * @file setup.cpp
 * @brief Definitions of the functions to read settings from file, launch the calculations and save the results
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#include <diagmc/setup.h>
#include <diagmc/simulation.h>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdlib.h>
#include <exception>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

using json = nlohmann::json;

//defaults for optional settings parameters
#define N_THERMALIZATION_STEPS_DEFAULT 0
#define INITIAL_S0_DEFAULT 1
#define SAMPLES_PER_POINT_DEFAULT 1
#define NEW_SEED std::chrono::system_clock::now().time_since_epoch().count()



void check_required_keys_presence(const json &  settings, std::vector<std::string> list_of_keys)
{
    //loop over list of keys, checking if each is contained in settings, terminate the program if not present
    for (auto key : list_of_keys)
    {
        if(!settings.contains(key))
        {
            std::cerr << "Error: missing " << key << " in settings.json." << std::endl;
            exit(EXIT_FAILURE);        
        }
    }
}


std::vector<double> range_generator(const json & settings, std::string which)
{
    //vector to store the list of values of the 'which' parameter
    std::vector<double> range_vector; 

    //case where min, max and step keys are present for the 'which' parameter
    if (settings.contains(which+"_min") && settings.contains(which+"_max") && settings.contains(which+"_step"))
    {
        double min_value = settings[which+"_min"];
        double max_value = settings[which+"_max"];
        double step = settings[which+"_step"];
        
        //linearly spaced values between min, and max, spaced by step
        for (double x = min_value; x <= max_value; x += step)
            range_vector.push_back(x);
    }
    else if (settings.contains(which)) //case where only a single value of the parameter is specified
    {
        range_vector.push_back(settings[which]); //in this case the vector contains a single value
    }
    else //no values of the necessary parameter: terminate program
    {
        std::cerr << "Error: missing " << which << " in settings.json. "<< std::endl;
        exit(EXIT_FAILURE);        
    }

    return range_vector;
}


std::vector<double> log_range_generator(const json & settings, std::string which)
{
    //vector to store the list of values of the 'which' parameter
    std::vector<double> range_vector;

    //case where min, max and step keys are present for the 'which' parameter
    if (settings.contains(which+"_min") && settings.contains(which+"_max") && settings.contains(which+"_points_per_decade"))
    {
        double min_value = settings[which+"_min"];
        double max_value = settings[which+"_max"];
        double points_per_decade = settings[which+"_points_per_decade"];

        double logmin = std::log10(min_value);
        double logmax = std::log10(max_value);
        double number_of_decades = logmax - logmin; //e.g. log10(10^8) - log10(10^3) = 8 - 3 = 5 decades
        double logstep = 1. / points_per_decade; //distance between two subsequent points in logscale
        
        //log10-spaced values between min, and max, spaced by logstep, corresponding to the desired points_per_decade
        for (int n = 0; n <= number_of_decades / logstep; ++n)
            range_vector.push_back(std::pow(10, logmin + n * logstep)); //go back to linear scale
    }
    else if (settings.contains(which)) //case where only a single value of the parameter is specified
    {
        range_vector.push_back(settings[which]); //in this case the vector contains a single value
    }
    else //no values of the necessary parameter: terminate program
    {
        std::cerr << "Error: incorrect/missing " << which << " in settings.json. "<< std::endl;
        exit(EXIT_FAILURE);        
    }

    return range_vector;
}


void print_progress_bar(double progress)
{
    int barWidth = 70; //number of chars for the progress bar
    
    std::cout << "[";

    int current_position = barWidth * progress;
    for (int i = 0; i < barWidth; ++i) {
        if (i <= current_position) std::cout << "="; //full char if inside completed part
        else std::cout << " "; //empty char for the non-completed portion
    }
    std::cout << "] " << int(progress * 100.0) << "%\r"; //carriage return to overwrite line
    std::cout.flush();    
}




json read_settings(std::string filename)
{
    
    //open read-only filestream to read settings file
    std::ifstream filestream(filename);

    //check that file was opened correctly, terminate program otherwise
    if (!filestream) {

        std::cerr << "Unable to open the settings.json file. Make sure that it is present in the same folder of the executable." << std::endl;     
        exit(EXIT_FAILURE);
    }

    json settings; //dictionary-like object to parse and store the settings read from file

    //try to parse the json file, terminate program if fail
    try {
        settings = json::parse(filestream);
        filestream.close();       
    }
    catch(const std::exception & e)
    {
        std::cerr << "Error while parsing settings.json." << e.what() << std::endl;
        filestream.close(); 
        exit(EXIT_FAILURE);
    }

    //Check on main dictionary key. Terminate program if there is any error
    //here only CALC_TYPE is checked, since it is needed to determine which function to call
    //specific checks for required keys are then performed inside each function
    if(!settings.contains("CALC_TYPE"))
    {
        std::cerr << "Error: missing CALC_TYPE in settings.json." << std::endl;
        exit(EXIT_FAILURE);        
    }
    
    if(settings["CALC_TYPE"] != "single" && settings["CALC_TYPE"] != "sweep" && settings["CALC_TYPE"] != "convergence-test")
    {
        std::cerr << "Error: invalid CALC_TYPE argument in settings.json. Expected 'single'/'sweep'/'convergence-test', but "<< settings["CALC_TYPE"] << "was provided." << std::endl;
        exit(EXIT_FAILURE);        
    }

    std::cout<<"Simulation settings read from file.\n";

    return settings;
}


void single_run(const json & settings)
{

    //PARAMETERS#################################################################
    //check presence of required keys in settings.json
    check_required_keys_presence( settings,
        {
            "beta",
            "H",
            "GAMMA",
            "N_total_steps",
        }
    );

    //assign default values to optional keys if not present in settings.json
    int initial_s0 = settings.contains("initial_s0") ? (int) settings["N_thermalization_steps"] : INITIAL_S0_DEFAULT;
    unsigned long long N_thermalization_steps = settings.contains("N_thermalization_steps") ? (unsigned long long) settings["N_thermalization_steps"] : N_THERMALIZATION_STEPS_DEFAULT;
    unsigned long long int update_choice_seed = settings.contains("update_choice_seed") ? int(settings["update_choice_seed"]) : NEW_SEED;
    unsigned long long int diagram_seed = settings.contains("diagram_seed") ? int(settings["diagram_seed"]) : NEW_SEED;
    //############################################################################


    //SIMULATION#################################################################
    std::cout<<"Running single run simulation...\n";

    //execute single run simulation, and print results to terminal standard output
    run_simulation( 
        settings["beta"],
        initial_s0, 
        settings["H"], 
        settings["GAMMA"], 
        settings["N_total_steps"], 
        N_thermalization_steps, 
        update_choice_seed, 
        diagram_seed
    ).print_results();
    //############################################################################

}


void sweep(const json & settings)
{

    //PARAMETERS#################################################################
    //check existence of required keys in settings.json
    check_required_keys_presence( settings,
        {
            "N_total_steps",
            "output_file"
        }
    );
    
    //generates linearly-spaced list of values for the sweep parameters, based on min, max and step
    std::vector<double> beta_values = range_generator(settings, "beta");
    std::vector<double> H_values = range_generator(settings, "H");
    std::vector<double> GAMMA_values = range_generator(settings, "GAMMA");
    unsigned long long N_total_steps = settings["N_total_steps"];


    //assign default values to optional keys if not present in settings.json
    int initial_s0 = settings.contains("initial_s0") ? (int) settings["N_thermalization_steps"] : INITIAL_S0_DEFAULT;
    unsigned long long N_thermalization_steps = settings.contains("N_thermalization_steps") ? (unsigned long long) settings["N_thermalization_steps"] : N_THERMALIZATION_STEPS_DEFAULT;
    int samples_per_point = settings.contains("samples_per_point") ? int(settings["samples_per_point"]) : SAMPLES_PER_POINT_DEFAULT;
    //############################################################################

    
    //open file stream to write results in output file, write the header row containing the titles of the columns
    std::ofstream output_file_stream(static_cast<std::string>(settings["output_file"]));
    output_file_stream << SingleRunResults::ostream_output_header();



    //SIMULATION###################################################################
    std::cout<<"Running sweep simulation...\n";

    //calculates parameters for progress bar, and prints it on standard output
    int total_number_of_runs = beta_values.size() * H_values.size() * GAMMA_values.size() * samples_per_point;
    int current_run = 0;
    print_progress_bar(current_run/total_number_of_runs);
    
    //nested for loop for the sweep, running every combination of beta, H and GAMMA
    for (auto beta : beta_values)
    {
        for(auto H : H_values)
        {
            for (auto GAMMA : GAMMA_values)
            {       
                        
                //avoid GAMMA = 0, since it is not allowed: use a value extremely close to 0
                if(std::abs(GAMMA) < std::numeric_limits<double>::epsilon()) GAMMA = 1e-10;
                
                //possibility to run multiple times for the same combination of parameters, useful to compute average and stddev
                for(int i = 0; i < samples_per_point; ++i) 
                {
                    //launch run for the specific combination of parameters
                    SingleRunResults results = run_simulation(beta, initial_s0, H, GAMMA, N_total_steps, N_thermalization_steps);
                    output_file_stream << results; //immediately write results on file, to avoid losing data if program is interrupted

                    //update progress bar
                    ++current_run;
                    print_progress_bar( (double) current_run/total_number_of_runs);
                }                
            }
        }
    }
    std::cout<<std::endl<<"Sweep completed.\n";
    output_file_stream.close();
    //###############################################################################
    
}


void convergence_test(const json & settings)
{

    //PARAMETERS#################################################################
    //check existence of required keys in settings.json
    check_required_keys_presence( settings,
        {
            "beta",
            "H",
            "GAMMA",
            "output_file"
        }
    );

    //generates log-spaced list of values for the sweep parameters, based on min, max and step
    std::vector<double> N_total_steps_values = log_range_generator(settings, "N_total_steps");
    std::vector<double> N_thermalization_steps_values;

    //assign default values to optional keys if not present in settings.json
    int initial_s0 = settings.contains("initial_s0") ? (int) settings["N_thermalization_steps"] : 1;
    if( !settings.contains("N_thermalization_steps") && !settings.contains("N_thermalization_step_max")) N_thermalization_steps_values = {0};
    else N_thermalization_steps_values = log_range_generator(settings, "N_thermalization_steps");
    unsigned long long int update_choice_seed = settings.contains("update_choice_seed") ? int(settings["update_choice_seed"]) : NEW_SEED;
    unsigned long long int diagram_seed = settings.contains("diagram_seed") ? int(settings["diagram_seed"]) : NEW_SEED;
    //############################################################################


    //open file stream to write results in output file, write the header row containing the titles of the columns
    std::ofstream output_file_stream(static_cast<std::string>(settings["output_file"]));
    output_file_stream << SingleRunResults::ostream_output_header();


    //SIMULATION#################################################################
    std::cout<<"Running convergence test...\n";

    //calculates parameters for progress bar, and prints it on standard output
    int total_number_of_runs = N_total_steps_values.size() * N_thermalization_steps_values.size();
    int current_run = 0;
    print_progress_bar(current_run/total_number_of_runs);

    //nested for loop for the sweep, running every combination of N_total_steps, and N_thermalization_steps
    for (auto N_total_steps : N_total_steps_values)
    {
        for(auto N_thermalization_steps : N_thermalization_steps_values)
        {
            //launch run for the specific combination of parameters
            SingleRunResults results = run_simulation(
                settings["beta"],
                initial_s0, 
                settings["H"], 
                settings["GAMMA"], 
                N_total_steps, 
                N_thermalization_steps,
                update_choice_seed,
                diagram_seed
            );
            output_file_stream << results; //immediately write results on file, to avoid losing data if program is interrupted
            
            //update progress bar
            ++current_run;
            print_progress_bar( (double) current_run/total_number_of_runs);
        }
    }
    std::cout<<std::endl<<"Convergence test completed.\n";
    output_file_stream.close();    
    //############################################################################
}


void launch_calculations(std::string settings_filename)
{
    //read settings from json file, and store it in a json object (dictionary-like)
    json settings = read_settings(settings_filename);

    //select which kind of calculation to run, based on what was specified in the settings file
    if(settings["CALC_TYPE"] == "single" )
    {
        single_run(settings);
    }
    else if (settings["CALC_TYPE"] == "sweep")
    {
        sweep(settings);
    }
    else if (settings["CALC_TYPE"] == "convergence-test")
    {
        convergence_test(settings);
    }
    
}