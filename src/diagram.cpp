/**
 * @file diagram.cpp
 * @brief Definitions file for Diagram and Diagram_core classes
 * @author Enrico Pedretti
 * @date 2023-08-31
 */

#include <iostream>
#include <diagmc/diagram.h>
#include <stdexcept>
#include <string>
#include <random>
#include <limits>
#include <cmath>
#include <algorithm>

#define RNG _uniform_dist(_mt_generator)


//Methods definitions for class Diagram_core -------------------------------------------------------
Diagram_core::Diagram_core(double beta, int s0, double H, double GAMMA, std::list<double> vertices) 
    : _beta(beta), _s0(s0), _H(H), _GAMMA(GAMMA), _vertices(vertices) {

    if(! (beta > 0)) 
    {
        throw std::invalid_argument( 
            std::string("beta must be > 0, but ") 
            + std::to_string(_beta) + std::string(" was provided.") 
            );
    }

    if(s0 != 1 && s0 != -1) 
    {
        throw std::invalid_argument( 
            std::string("The spin can either be +1 or -1, but ") 
            + std::to_string(s0) + std::string(" was provided.") 
            );
    }
    
    if(std::abs(H) < std::numeric_limits<double>::epsilon() && std::abs(GAMMA) < std::numeric_limits<double>::epsilon() ) 
    {
        throw std::invalid_argument( 
            std::string("H and GAMMA cannot both be 0.")
            ); 
    }

    if(vertices.size() % 2 != 0)
    {
        throw std::invalid_argument( 
            std::string("The vertices list must contain an even number of elements.")
            );         
    }

    for(auto v : vertices)
    {
        if (v > beta) throw std::invalid_argument("The vertices list contains values > beta."); 
    }

    if(!std::is_sorted(vertices.begin(), vertices.end()))
    {
        throw std::invalid_argument("The list used to initialize the diagram was not sorted.");
    }    

}

bool Diagram_core::operator==(const Diagram_core &other) const
{
    if (this->_beta == other._beta 
        && this->_s0 == other._s0
        && this->_H == other._H
        && this->_GAMMA == other._GAMMA
        && this->_vertices == other._vertices) return true;
    else return false;
}


double Diagram_core::operator/(const Diagram_core &other) const
{
    return this->value()/other.value();
}

double Diagram_core::value() const
{
    //sum (... +t4-t3 + t2-t1)
    double sum_deltatau = 0;
    for (auto it = _vertices.begin(); it != _vertices.end();)
    {
        sum_deltatau -= *it;  //-t1
        ++it;
        sum_deltatau += *it;  //+t2
        ++it;
    }              
    double val = std::pow(_GAMMA, order()) * std::exp(_H * _s0 *( -_beta + 2*sum_deltatau));

    return val;
}

size_t Diagram_core::order() const {
    return _vertices.size();
}


//acceptance rates for the updates
double Diagram_core::acceptance_rate_add(double tau1, double tau2, double tau2max, double new_segment_spin) const {
    return _GAMMA*_GAMMA * std::exp(-2 * _H * new_segment_spin * (tau2-tau1)) * _beta * (tau2max - tau1) / (_vertices.size() + 1);
}

double Diagram_core::acceptance_rate_remove(double tau1, double tau2, double tau2max, double segment_toberemoved_spin) const {
    return std::exp(2 * _H * segment_toberemoved_spin * (tau2-tau1)) * (_vertices.size() - 1) / ( _GAMMA*_GAMMA * _beta * (tau2max-tau1) );
}

double Diagram_core::acceptance_rate_flip(double sum_deltatau) const {
    return std::exp(2*_H*_s0*(_beta - 2 *sum_deltatau));
}


//update functions
bool Diagram_core::attempt_add_segment(double RN1, double RN2, double RNacc) {

    //extract the time tau1 of the first vertex to be added in uniform([0, _beta])
    double tau1 = RN1 * _beta; 

    
    //determine the nearest vertex (tau3) after the extracted tau1 (which will become tau3 after adding (tau1, tau2))----------------------
    //, and the index of its segment

    std::list<double>::iterator tau3_it = _vertices.end();   //iterator to sweep the list and find the nearest vertex with tau > tau1
    int new_segment_index = 0;                              //index that the segment we want to add will have, corresponding to the index of the current tau3 segment

    for (auto i = _vertices.begin(); i != _vertices.end(); ++i)
    {  
        if (*i > tau1) //stop at the nearest value of the vertex greater than tau1
        {
            tau3_it = i;
            break;
        }
        ++new_segment_index;       
    }
    double tau2max = tau3_it != _vertices.end() ? *tau3_it : _beta ;
    //--------------------------------------------------------------------------------------------------------------------------------------

    //select second vertex in uniform([tau1, tau2max])
    double tau2 = tau1 + RN2 * (tau2max - tau1);  

    //spin of the segment that we will add
    double new_segment_spin = _s0*std::pow(-1, new_segment_index + 1); 

    //attempt update, adding segment if accepted (and returning true); doing nothing (and returning false) if rejected
    if (RNacc < acceptance_rate_add(tau1, tau2, tau2max, new_segment_spin))
    {
        _vertices.insert(tau3_it, tau1);
        _vertices.insert(tau3_it, tau2);       
        return true;
    }
    return false;

}

bool Diagram_core::attempt_remove_segment(double RN1, double RNacc) {

    //cannot remove segment if diagram is 0 order, so reject update right away
    if (order() == 0) return false;


    //randomly choose segment to be removed
    int segment_toberemoved_index = RN1 * (order() - 1) + 1; //it starts from 1, since the first segment [0,t1] cannot be removed 


    //determine the _vertices (tau1, tau2) of the segment to be removed, and their location in the list---
    auto tau1_it = _vertices.begin();
    std::advance(tau1_it, segment_toberemoved_index - 1); //does not advance if segment 1, since t1 is already the first element of the list

    auto tau2_it = tau1_it; ++tau2_it; //tau2 is the vertex just after tau1

    double tau1 = *tau1_it;
    double tau2 = *tau2_it;
    //---------------------------------------------------------------------------------------------------

    //tau3 is the vertex just after tau2
    auto tau3_it = tau2_it; ++tau3_it;
    double tau2max = tau3_it != _vertices.end() ? *tau3_it : _beta;

    //spin of the segment to be removed
    double segment_toberemoved_spin = _s0 * std::pow(-1, segment_toberemoved_index);


    //attempt update, removing segment if accepted (and returning true); doing nothing (and returning false) if rejected
    if (RNacc < acceptance_rate_remove(tau1, tau2, tau2max, segment_toberemoved_spin))
    {    
        _vertices.erase(tau1_it, tau3_it); //erase between [1, 3), so 1 and 2
        return true;
    }
    return false;
}

bool Diagram_core::attempt_spin_flip(double RNacc) {

    //sum (... +t4-t3 + t2-t1)
    double sum_deltatau = 0;
    for (auto it = _vertices.begin(); it != _vertices.end();)
    {
        sum_deltatau -= *it;  //-t1
        ++it;
        sum_deltatau += *it;  //+t2
        ++it;
    }      

    //attempt update, flipping spins of all diagram if accepted (and returning true); doing nothing (and returning false) if rejected         
    if (RNacc < acceptance_rate_flip(sum_deltatau))
    {
        _s0 *= -1;
        return true;
    }
    return false;
}
//END Diagram_core class definition
//--------------------------------------------------------------------------------------------------





//Methods definitions for class Diagram ------------------------------------------------------------
Diagram::Diagram(double beta, int s0, double H, double GAMMA, 
    std::list<double> vertices, 
    unsigned int seed)
    : Diagram_core(beta, s0, H, GAMMA, vertices) , _uniform_dist(0,1), _mt_generator(seed) {}


//getters
double Diagram_core::get_beta() const {
    return _beta; 
}

int Diagram_core::get_s0() const {
    return _s0;
}

double Diagram_core::get_H() const {
    return _H;
}

double Diagram_core::get_GAMMA() const {
    return _GAMMA;
}

std::list<double> Diagram_core::get_vertices() const {
    return _vertices;
}


//update functions
bool Diagram::attempt_add_segment() {
    return Diagram_core::attempt_add_segment(RNG, RNG, RNG);
}

bool Diagram::attempt_remove_segment() {
    return Diagram_core::attempt_remove_segment(RNG, RNG);
}

bool Diagram::attempt_spin_flip() {
    return Diagram_core::attempt_spin_flip(RNG);
}

void Diagram::reset_diagram(double beta, int s0, double H, double GAMMA, std::list<double> vertices, unsigned int seed) {


    if(! (beta > 0)) 
    {
        throw std::invalid_argument( 
            std::string("_beta must be > 0, but ") 
            + std::to_string(beta) + std::string(" was provided.") 
            );
    }    
    
    if(s0 != 1 && s0 != -1) 
    {
        throw std::invalid_argument( 
            std::string("The spin can either be +1 or -1, but ") 
            + std::to_string(s0) + std::string(" was provided.") 
            );
    }
   
    if(std::abs(H) < std::numeric_limits<double>::epsilon() && std::abs(GAMMA) < std::numeric_limits<double>::epsilon() ) 
    {
        throw std::invalid_argument( 
            std::string("_H and _GAMMA cannot both be 0.")
            ); 
    }


    if(vertices.size() % 2 != 0)
    {
        throw std::invalid_argument( 
            std::string("The vertices list must contain an even number of elements.")
            );         
    }

    for(auto v : vertices)
    {
        if (v > beta) throw std::invalid_argument("The _vertices list contains values > _beta."); 
    }
    

    if(!std::is_sorted(vertices.begin(), vertices.end()))
    {
        throw std::invalid_argument("The list used to initialize the diagram was not sorted.");
    }    

    


    _beta     = beta,
    _s0       = s0,
    _H        = H,
    _GAMMA    = GAMMA,
    _vertices = vertices;
    _mt_generator.seed(seed);

}
//--------------------------------------------------------------------------------------------------


