/**
 * @file diagram.h
 * @brief Diagram class
 * @author Enrico Pedretti
 * @date 2023-08-31
 */

#pragma once
#include <list>
#include <random>
#include <chrono>



/**
 * @class Diagram_core 
 * 
 * @brief This class is the base class of Diagram, and should not be used directly, aside from testing functionalities. 
 * It contains the variables defining the diagram and the DETERMINISTIC part of the routines to perform updates, 
 * excluding the random number extraction, which is instead handled by the Diagram class.
 */
class Diagram_core
{

    public:

    /**
     * @brief Construct a new diagram, setting its defining parameters. The list of vertices is optional: 
     * by default it is the 0-th order diagram [0]-------[beta]
     * 
     * @param beta       Length of the diagram (here representing 1/{k_bT}). Must be > 0.
     * @param s0         Spin of the first segment of the diagram. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field
     * @param vertices   (optional) List containing the times of diagram vertices, with t1<t2<t3... (they need to be already sorted)
     */
    Diagram_core(double beta, int s0, double H, double GAMMA, std::list<double> vertices=std::list<double>() );

    /**
     * @brief Returns the acceptance rate for the ADD_SEGMENT update for the given parameters
     * 
     * @param tau1      time of the first vertex of the segment to be added
     * @param tau2      time of the second vertex of the segment to be added
     * @param tau2max   maximum value of tau2 for the random extraction
     * @param new_segment_spin spin of the segment to be added
     * @return double 
     */
    double acceptance_rate_add(double tau1, double tau2, double tau2max, double new_segment_spin) const;

    /**
     * @brief Returns the acceptance rate for the REMOVE_SEGMENT update for the given parameters
     * 
     * @param tau1      time of the first vertex of the segment to be removed
     * @param tau2      time of the second vertex of the segment to be removed
     * @param tau2max   maximum value of tau2 for the random extraction
     * @param segment_toberemoved_spin spin of the segment to be removed
     * @return double 
     */
    double acceptance_rate_remove(double tau1, double tau2, double tau2max, double segment_toberemoved_spin) const;

    /**
     * @brief Returns the acceptance rate for the SPIN_FLIP update for the given parameter
     * 
     * @param sum_deltatau alternate sign sum of the vertices (... +t4-t3 + t2-t1)
     * @return double 
     */
    double acceptance_rate_flip(double sum_deltatau) const;

    /**
     * @brief Attemps the ADD_SEGMENT update for the current status of the diagram, 
     * using the three random numbers given in input.
     * 
     * @param RN1 Random number for the extraction of tau1, must be in range [0, 1]
     * @param RN2 Random number for the extraction of tau2, must be in range [0, 1]
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update wa rejected
     */
    bool attempt_add_segment(double RN1, double RN2, double RNacc);

    /**
     * @brief Attemps the REMOVE_SEGMENT update for the current status of the diagram, 
     * using the two random numbers given in input.
     * 
     * @param RN1 Random number for the extraction of first vertex, must be in range [0, 1]
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update wa rejected
     */
    bool attempt_remove_segment(double RN1, double RNacc);

    /**
     * @brief Attemps the SPIN_FLIP update for the current status of the diagram, 
     * using the random number given in input.
     * 
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update wa rejected 
     */
    bool attempt_spin_flip(double RNacc);


    protected:

    double beta;                 ///< length of the diagram. Must be > 0.
    int s0;                      ///< spin of the first segment of the diagram. Must be +1 or -1
    double H;                    ///< value of the longitudinal component of magnetic field
    double GAMMA;                ///< value of the transversal component of magnetic field
    std::list<double> vertices;  ///< list containing the times of the diagram vertices

};


/**
 * @class Diagram
 * 
 * @brief This is the main Diagram class, containing the variables defining the diagram - inherited from Diagram_core -
 * and the functions to attempt the updates.
 */
class Diagram: public Diagram_core
{
    public:

    /**
     * @brief Construct a new Diagram object, setting its defining parameters. The list of vertices is optional: 
     * by default it is the empty list, corresponding to the 0-th order diagram [0]-------[beta].
     * Optionally, a seed for the Mersenne-Twister random number generator can be explicitly set.
     * 
     * @param beta       Length of the diagram (here representing 1/{k_bT}). Must be > 0.
     * @param s0         Spin of the first segment of the diagram. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field
     * @param vertices   (optional) List containing the times of diagram vertices, with t1<t2<t3... (they need to be already sorted)
     * @param seed       (optional) Seed to initialize the random number generator
     */
    Diagram(double beta, int s0, double H, double GAMMA, 
        std::list<double> vertices=std::list<double>(), 
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count());


    /**
     * @brief Get the value of beta (length of the diagram)
     * 
     * @return double (>0)
     */
    double get_beta() const;

    /**
     * @brief Get the value of the spin of the first segment of the diagram
     * 
     * @return int (+1 or -1)
     */
    int get_s0() const;

    /**
     * @brief Get the value of the longitudinal field H
     * 
     * @return double 
     */
    double get_H() const;

    /**
     * @brief Get the value of the transverse field GAMMA
     * 
     * @return double 
     */
    double get_GAMMA() const;

    /**
     * @brief Get a copy of the list of vertices
     * 
     * @return std::list<double> 
     */
    std::list<double> get_vertices() const;

    /**
     * @brief Get the order of the diagram (number of vertices)
     * 
     * @return size_t 
     */
    size_t order() const; 


    /**
     * @brief Attemps the ADD_SEGMENT update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update wa rejected
     */
    bool attempt_add_segment();

    /**
     * @brief Attemps the REMOVE_SEGMENT update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update wa rejected
     */
    bool attempt_remove_segment(); 

    /**
     * @brief Attemps the SPIN_FLIP update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update wa rejected
     */
    bool attempt_spin_flip();

    /**
     * @brief Reset all diagram parameters with the new values.
     * 
     * @param _beta       Length of the diagram (here representing 1/{k_bT}). Must be > 0.
     * @param _s0         Spin of the first segment of the diagram. Must be +1 or -1.
     * @param _H          Value of the longitudinal component of magnetic field
     * @param _GAMMA      Value of the transversal component of magnetic field
     * @param _vertices   (optional) List containing the times of diagram vertices, with t1<t2<t3... (they need to be already sorted)
     * @param _seed       (optional) Seed to initialize the random number generator
     */
    void reset_diagram(double _beta, int _s0, double _H, double _GAMMA, 
        std::list<double> _vertices=std::list<double>(), 
        unsigned int _seed = std::chrono::system_clock::now().time_since_epoch().count());

    private:
        std::uniform_real_distribution<double> uniform_dist; ///< uniform distribution for random number generation
        std::mt19937 mt_generator;                           //Mersenne-Twister random number generator

};