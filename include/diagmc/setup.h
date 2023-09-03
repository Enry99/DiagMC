/**
 * @file setup.cpp
 * @brief Header file of the functions to read settings from file, launch the calculations and save the results
 * @author Enrico Pedretti
 * @date 2023-09-03
 */

#pragma once

#include <nlohmann/json.hpp>
using json = nlohmann::json;

void check_required_keys_presence(const json &  settings, std::vector<std::string> list_of_keys);

std::vector<double> range_generator(const json &  settings, std::string which);

std::vector<double> log_range_generator(const json & settings, std::string which);

void print_progress_bar(double progress);


json read_settings(std::string = "settings.json");

void single_run(const json &);

void sweep(const json &);

void convergence_test(const json &);

void launch_calculations(std::string settings_filename);