/**
 * @file diagram.h
 * @brief Header file for Diagram and Diagram_core classes
 * @author Enrico Pedretti
 * @date 2023-08-31
 */

#pragma once

#include <list>
#include <random>
#include <chrono>

#define EPSILON 1e-10  //theshold for floating point comparison

/**
 * @class Diagram_core 
 * 
 * @brief This class is the base class of Diagram, and should not be used directly, aside from testing functionalities. 
 * It contains the variables defining the diagram and the DETERMINISTIC part of the routines to perform updates, 
 * excluding the random number extraction, which is instead handled by the Diagram class.
 */
class Diagram_core
{

    protected:

    double _beta;                 ///< length of the diagram (here representing the thermondinamical beta = 1/T)). Must be > 0.
    int _s0;                      ///< spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1
    double _H;                    ///< value of the longitudinal component of magnetic field
    double _GAMMA;                ///< Value of the transversal component of magnetic field. Must be != 0.
    std::list<double> _vertices;  ///< list containing the times of the diagram vertices


    /**
     * @brief Internal (non-public) member function that checks wether all the parameters are within the allowed values.
     * Throws an std::invalid_argument exception otherwise. It is used in the Diagram_core constructor and in Diagram::reset_diagram
     * 
     * @param beta       Length of the diagram (here representing the thermondinamical $\beta$ = 1/T). Must be > 0.
     * @param s0         Spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
     * @param vertices   (optional) List containing the times of diagram _vertices, with t1<t2<t3... < _beta (they need to be already sorted)
     */
    void assert_parameters_validity(double beta, int s0, double H, double GAMMA, std::list<double> vertices) const;


    public:

    /**
     * @brief Construct a new diagram, setting its defining parameters. The list of vertices is optional: 
     * by default it is the 0-th order diagram [0]-------[beta]
     * 
     * @param beta       Length of the diagram (here representing the thermondinamical $\beta$ = 1/T). Must be > 0.
     * @param s0         Spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
     * @param vertices   (optional) List containing the times of diagram _vertices, with t1<t2<t3... < _beta (they need to be already sorted)
     */
    Diagram_core(double beta, int s0, double H, double GAMMA, std::list<double> vertices=std::list<double>() );

    /**
     * @brief operator to test wether two Diagram_core objects are equal. It is intended for TESTING purposes only, and not to be used within the program.
     * It checks that all values defining a diagram (beta, s0, H, GAMMA and each vertex in the vertices list) 
     * of the two Diagram_core objects are within EPSILON = 1e-10 from each other. EPSILON is defined at the beginning of diagram.h
     * 
     * @param other other Diagram_core object
     * @return true 
     * @return false 
     */
    bool operator==(const Diagram_core & other) const;


    /**
     * @brief operator to test wether two Diagram_core objects are different. It is the negation of operator==.
     * It is intended for TESTING purposes only, and not to be used within the program.
     * 
     * @param other other Diagram_core object
     * @return true 
     * @return false 
     */
    bool operator!=(const Diagram_core & other) const;

    /**
     * @brief Returns the ratio of the weights of the two diagrams, i.e. this->value()/other.value()
     * 
     * @return double 
     */
    double operator/(const Diagram_core & other) const;

    /**
     * @brief Small helper function, performing the sum (... +t4-t3 + t2-t1)
     * 
     * @return double 
     */
    double sum_deltatau() const;


    /**
     * @brief Returns the value ("weight") of the current diagram
     * 
     * @return double 
     */
    double value() const;

    /**
     * @brief Get the order of the diagram (number of _vertices)
     * 
     * @return size_t 
     */
    size_t order() const; 

    /**
     * @brief Get the value of _beta (length of the diagram)
     * 
     * @return double (>0)
     */
    double get_beta() const;

    /**
     * @brief Get the value of the spin of the 0-th segment of the diagram [0---t1]
     * 
     * @return int (+1 or -1)
     */
    int get_s0() const;

    /**
     * @brief Get the value of the longitudinal field _H
     * 
     * @return double 
     */
    double get_H() const;

    /**
     * @brief Get the value of the transverse field _GAMMA
     * 
     * @return double 
     */
    double get_GAMMA() const;

    /**
     * @brief Get a copy of the list of _vertices
     * 
     * @return std::list<double> 
     */
    std::list<double> get_vertices() const;


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
     * @return double 
     */
    double acceptance_rate_flip() const;

    /**
     * @brief Attemps the ADD_SEGMENT update for the current status of the diagram, 
     * using the three random numbers given in input.
     * 
     * @param RN1 Random number for the extraction of tau1, must be in range [0, 1]
     * @param RN2 Random number for the extraction of tau2, must be in range [0, 1]
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update was rejected
     */
    bool attempt_add_segment(double RN1, double RN2, double RNacc);

    /**
     * @brief Attemps the REMOVE_SEGMENT update for the current status of the diagram, 
     * using the two random numbers given in input.
     * 
     * @param RN1 Random number for the extraction of first vertex, must be in range [0, 1]
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update was rejected
     */
    bool attempt_remove_segment(double RN1, double RNacc);

    /**
     * @brief Attemps the SPIN_FLIP update for the current status of the diagram, 
     * using the random number given in input.
     * 
     * @param RNacc Random number for the acceptance, should be in range [0,1]
     * @return true if update was accepted,
     * @return false if update was rejected 
     */
    bool attempt_spin_flip(double RNacc);


};


/**
 * @class Diagram
 * 
 * @brief This is the main Diagram class, containing the variables defining the diagram - inherited from Diagram_core -
 * and the functions to attempt the updates.
 */
class Diagram: public Diagram_core
{

    private:
        std::uniform_real_distribution<double> _uniform_dist; ///< uniform distribution for random number generation
        std::mt19937 _mt_generator;                           ///< Mersenne-Twister random number generator


    public:

    /**
     * @brief Construct a new Diagram object, setting its defining parameters. The list of vertices is optional: 
     * by default it is the empty list, corresponding to the 0-th order diagram [0]-------[beta].
     * Optionally, a seed for the Mersenne-Twister random number generator can be explicitly set.
     * 
     * @param beta       Length of the diagram (here representing the thermondinamical beta = 1/T). Must be > 0.
     * @param s0         Spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
     * @param vertices   (optional) List containing the times of diagram _vertices, with t1<t2<t3... (they need to be already sorted)
     * @param seed       (optional) Seed to initialize the random number generator
     */
    Diagram(double beta, int s0, double H, double GAMMA, 
        std::list<double> _vertices=std::list<double>(), 
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count());

    using Diagram_core::operator/ ;
    //using Diagram_core::operator== ;


    /**
     * @brief Attemps the ADD_SEGMENT update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update was rejected
     */
    bool attempt_add_segment();

    /**
     * @brief Attemps the REMOVE_SEGMENT update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update was rejected
     */
    bool attempt_remove_segment(); 

    /**
     * @brief Attemps the SPIN_FLIP update for the current status of the diagram.
     * 
     * @return true if update was accepted,
     * @return false if update was rejected
     */
    bool attempt_spin_flip();

    /**
     * @brief Reset all diagram parameters with the new values.
     * 
     * @param beta       Length of the diagram (here representing the thermondinamical beta = 1/T). Must be > 0.
     * @param s0         Spin of the 0-th segment of the diagram [0---t1]. Must be +1 or -1.
     * @param H          Value of the longitudinal component of magnetic field
     * @param GAMMA      Value of the transversal component of magnetic field. Must be != 0.
     * @param vertices   (optional) List containing the times of diagram _vertices, with t1<t2<t3... < _beta (they need to be already sorted)
     * @param seed       (optional) Seed to initialize the random number generator
     */
    void reset_diagram(double beta, int s0, double H, double GAMMA, 
        std::list<double> vertices=std::list<double>(), 
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count());

};


/**
 * @brief Small helper function that checks if two lists of floating points numbers are equal,
 * performing element-by-element comparison.
 * If list1[i] and list2[i] are within epsilon for every i, the two lists are considered equal
 * 
 * @param list1 First list
 * @param list2 Second list
 * @param epsilon absolute value of the difference below which two elements are considered equal
 * @return true if lists are equal,
 * @return false otherwise
 */
bool lists_are_float_equal(const std::list<double>& list1, const std::list<double>& list2, double epsilon);