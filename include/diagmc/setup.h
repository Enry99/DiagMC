/**
 * @file setup.h
 * @brief Header file of the functions to read settings from file, launch the calculations and save the results
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;


/**
 * @brief Check that all keys in list_of_keys are present in settings, otherwise 
 * terminate the program with EXIT_FAILURE
 * 
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 * @param list_of_keys List of strings containing the keys whose presence in settings dictionary has to be checked
 */
void check_required_keys_presence(const json &  settings, std::vector<std::string> list_of_keys);


/**
 * @brief If "which"_min, "which"_max and "which"_step parameter are present in settings,
 * returns a vector of linearly spaced values from min to max (spaced by step).
 * If "which"_min, "which"_max and "which"_step are not present but "which" is,
 * returns a vector with the single value specified by the setting parameter "which".
 * If both previous conditions are not satisfied, terminates the program with EXIT_FAILURE
 * 
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 * @param which Name of the parameter for the algorithm, e.g. "H", or "GAMMA"
 * @return std::vector<double> containing a the (one or more) values of the 'which' parameter
 */
std::vector<double> range_generator(const json &  settings, std::string which);


/**
 * @brief If "which"_min, "which"_max and "which"_points_per_decade parameter are present in settings,
 * returns a vector of log10-spaced values from min to max (with points_per_decade between each power of 10).
 * If "which"_min, "which"_max and "which"_step are not present but "which" is,
 * returns a vector with the single value specified by the setting parameter "which".
 * If both previous conditions are not satisfied, terminates the program with EXIT_FAILURE
 * 
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 * @param which Name of the parameter for the algorithm, e.g. "N_total_steps"
 * @return std::vector<double> containing a the (one or more) values of the 'which' parameter
 */
std::vector<double> log_range_generator(const json & settings, std::string which);


/**
 * @brief Prints a progress bar on standard output
 * 
 * @param progress Fraction of completion (in range [0,1])
 */
void print_progress_bar(double progress);


/**
 * @brief Read settings for the simulation from json file, and returns them as a json dictionary-like object
 * If file cannot be opened, or it is not correctly parsed, or the "CALC_TYPE" key is missing,
 * terminates the program with EXIT_FAILURE
 * 
 * @param filename Name (path) of the json file containing the settings for the calculation
 * @return json (dictionry-like object containing the settings for the simulation)
 */
json read_settings(std::string filename = "settings.json");


/**
 * @brief Perform a single run for the given parameters in setting,
 * printing a summary of the results on standard output.
 * 
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 */
void single_run(const json & settings);


/**
 * @brief Perform multiple runs for different values of the beta, H and GAMMA parameters,
 * sweeping the range between the min and max values indicated in settings,
 * with the step value also specified in settings.
 * If min/max/step values for more than one parameter are provided, all combinations are calculated.
 * The results are written in a csv file, with each row corresponding to a single run.
 * 
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 */
void sweep(const json & settings);


/**
 * @brief Perform a convergence test on the number of steps for the MCMC algorithm, by
 * performing multiple runs for different number of steps (keeping the same seed), to avoid
 * random fluctuations that could make the result unclear.
 * The values of the N_total_steps are taken the range between the min and max values indicated in settings,
 * in log10-scale, with the specified "points_per_decade".
 * The results are written in a csv file, with each row corresponding to a single run.
 * @param settings dictionary-like nlohmann::json object, with the settings for running the algorithm
 */
void convergence_test(const json & settings);


/**
 * @brief Call the read_settings function to read settings from file, and select which calculation to run
 * 
 * @param settings_filename Name (path) of the json file containing the settings for the calculation
 */
void launch_calculations(std::string settings_filename);