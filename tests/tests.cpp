/**
 * @file tests.cpp
 * @brief File with tests for Diagram and Diagram_core most relevant functions
 * @author Enrico Pedretti
 * @date 2023-09-01
 */

#include <gtest/gtest.h>
#include <diagmc/diagram.h>

/**
 * @brief Construct a new TEST object
 * GIVEN: what is the starting state of the system its testing
 * WHEN: what happens, what operation is going to perform
 * THEN: the expected effect of the operation after the operation
 */
TEST(TestDiagram_core, constructor_throws_for_invalid_beta_argument)
{
    EXPECT_THROW( Diagram_core(-10, 1, 1, 1) , std::invalid_argument );
}

TEST(TestDiagram_core, constructor_throws_for_invalid_spin_argument)
{
    EXPECT_THROW( Diagram_core(1, -7, 1, 1) , std::invalid_argument );
}

TEST(TestDiagram_core, constructor_throws_for_invalid_H_GAMMA_bothzero)
{
    EXPECT_THROW( Diagram_core(1, 1, 0, 0) , std::invalid_argument );
}

TEST(TestDiagram_core, constructor_throws_for_non_even_number_vertices)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3}) , std::invalid_argument );
}

TEST(TestDiagram_core, constructor_throws_for_vertices_argument_greaterthan_beta)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3, 100}) , std::invalid_argument );
}

TEST(TestDiagram_core, constructor_throws_for_vertices_argument_notsorted)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 9, 8}) , std::invalid_argument );
}


TEST(TestDiagram, reset_diagram_throws_for_invalid_beta_argument)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(-10, 1, 1, 1) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_throws_for_invalid_spin_argument)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(1, -7, 1, 1) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_throws_for_invalid_H_GAMMA_bothzero)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(1, 1, 0, 0) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_throws_for_non_even_number_vertices)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(10, 1, 1, 1, {1, 2, 3}) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_throws_for_vertices_argument_greaterthan_beta)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3, 100}) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_throws_for_vertices_argument_notsorted)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(10, 1, 1, 1, {1, 2, 9, 8}) , std::invalid_argument );
}

TEST(TestDiagram, reset_diagram_sets_correct_values)
{

    double beta = 10;
    int s0 = -1;
    double H    = -2;
    double GAMMA = 0.5;
    std::list<double> vertices {1,2,3,5,9,9.9};
    unsigned int seed = 123456789;

    Diagram diag_expected(beta, s0, H, GAMMA, vertices, seed);
    Diagram diag_test(1,1,1,1);
    
    
    diag_test.reset_diagram(beta, s0, H, GAMMA, vertices, seed);

    EXPECT_EQ(diag_test, diag_expected);
   
}



//Check that IF updated is accepted, the new diagram is correct
TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram)
{

    double beta = 10;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = 7;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {1,2, tau1, tau2, tau3, 9});
    Diagram_core diag_test(beta, 1, 1, 1,     {1,2,             tau3, 9});
    
    
    diag_test.attempt_add_segment(rn1, rn2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_beginning)
{

    double beta = 10;
    double tau1 = 0.1;
    double tau2 = 3;
    double tau3 = 4;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {tau1, tau2, tau3, 5,6,7,8,9});
    Diagram_core diag_test(beta, 1, 1, 1, {              tau3, 5,6,7,8,9});
    

    diag_test.attempt_add_segment(rn1, rn2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_end)
{

    double beta = 10;
    double tau1 = 8.8;
    double tau2 = 9.4;
    double tau3 = beta;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {1,2,3,4,5,6, tau1, tau2});
    Diagram_core diag_test(beta, 1, 1, 1, {  1,2,3,4,5,6,           });
    
    
    diag_test.attempt_add_segment(rn1, rn2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_zeroorder)
{

    double beta = 10;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = beta;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {tau1, tau2});
    Diagram_core diag_test(beta, 1, 1, 1,    {           });
    
    
    diag_test.attempt_add_segment(rn1, rn2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram)
{

    int remove_index = 2;
    double rn1 = (double) remove_index / (6 - 1);

    Diagram_core diag_expected(10, 1, 1, 1, {1, 2,           8, 9});
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2,  5, 5.5,  8, 9});

    
    
    diag_test.attempt_remove_segment(rn1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram_beginning)
{

    int remove_index = 0;
    double rn1 = (double) remove_index / (6 - 2 + 1);

    Diagram_core diag_expected(10, 1, 1, 1, {      5, 5.5, 8, 9});
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2, 5, 5.5, 8, 9});

    
    
    diag_test.attempt_remove_segment(rn1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram_end)
{

    int remove_index = 4;
    double rn1 = (double) remove_index / (6 - 2 + 1);

    Diagram_core diag_expected(10, 1, 1, 1, {1, 2, 5, 5.5,       });
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2, 5, 5.5,  8, 9});

    
    
    diag_test.attempt_remove_segment(rn1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}

TEST(TestDiagram_core, attempt_spin_flip_creates_correct_diagram)
{
    Diagram_core diag_up(10, 1, 1, 1, {1,2, 7,9});
    Diagram_core diag_dw(10, -1, 1, 1, {1,2, 7,9});

    Diagram_core diag_test(10, 1, 1, 1, {1,2, 7,9});

    diag_test.attempt_spin_flip(-1); //force acceptance

    EXPECT_EQ(diag_test, diag_dw);

    diag_test.attempt_spin_flip(-1); //force acceptance

    EXPECT_EQ(diag_test, diag_up);        

}






TEST(TestDiagram_core, attempt_add_segment_correct_rate)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = 8;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,      {1,2, tau1, tau2, tau3, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {1,2,             tau3, 9});


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(rn1, rn2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(rn1, rn2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}

TEST(TestDiagram_core, attempt_add_segment_correct_rate_beginning)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 1;
    double tau2 = 1.5;
    double tau3 = 2;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,      {tau1, tau2, tau3, 5,6,7,8,9});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {            tau3, 5,6,7,8,9});

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(rn1, rn2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(rn1, rn2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}

TEST(TestDiagram_core, attempt_add_segment_correct_rate_end)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 8;
    double tau2 = 9;
    double tau3 = beta;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,  {1,2,3,4,5,6, tau1, tau2});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {1,2,3,4,5,6,           });

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(rn1, rn2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(rn1, rn2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}

TEST(TestDiagram_core, attempt_add_segment_correct_rate_zeroorder)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 1;
    double tau2 = 9;
    double tau3 = beta;

    double rn1 = tau1 / beta;
    double rn2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,  {tau1, tau2});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {          });


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(rn1, rn2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(rn1, rn2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}

TEST(TestDiagram_core, attempt_remove_segment_correct_rate)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 5; int remove_index = 2;
    double tau3  = 8;

    double rn1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {1, 2,              tau3, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2,  tau1, 5.5,  tau3, 9});
 
    
    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(rn1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(rn1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}

TEST(TestDiagram_core, attempt_remove_segment_correct_rate_beginning)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 1; int remove_index = 0;
    double tau3  = 5;

    double rn1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {      5, 5.5, 8, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2, 5, 5.5, 8, 9});


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(rn1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(rn1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}

TEST(TestDiagram_core, attempt_remove_segment_correct_rate_end)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 8; int remove_index = 4;
    double tau3  = beta;

    double rn1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {1, 2, 5, 5.5,       });
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2, 5, 5.5,  8, 9});

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(rn1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(rn1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}

TEST(TestDiagram_core, attempt_spin_flip_correct_rate)
{
    Diagram_core diag_new(10,-1, 1, 1, {1,2, 7,9});
    Diagram_core diag_current(10, 1, 1, 1, {1,2, 7,9});

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current;

    EXPECT_TRUE(diag_test1.attempt_spin_flip(expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_spin_flip(expected_acceptance_rate + 0.00001));  //recjected if RNG > acc    
}

TEST(TestDiagram_core, attempt_remove_segment_always_rejects_for_zero_order)
{

    Diagram_core diag_test(1, 1, 1, 1, {});

    EXPECT_FALSE(diag_test.attempt_remove_segment(0.5, -1)); //-1 would be certain acceptance, regardless of tau1

   
}






/*

TEST: 

-Controllo sia su value() che su acceptance_rate() con valori numerici calcolati "a mano"

TEST of type 1) checks that the diagram is correctly modified when update is accepted
TEST of type 2) checks that the diagram is accepted with the correct acceptance rate


*/