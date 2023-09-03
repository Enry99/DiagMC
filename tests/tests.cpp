/**
 * @file tests.cpp
 * @brief File with tests for Diagram and Diagram_core most relevant functions
 * @author Enrico Pedretti
 * @date 2023-09-01
 */

#include <gtest/gtest.h>
#include <diagmc/diagram.h>



/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * the beta parameter is < 0.
 * 
 * GIVEN: valid s0, H, gamma parameters, but beta parameter < 0
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_invalid_beta)
{
    EXPECT_THROW( Diagram_core(-10, 1, 1, 1) , std::invalid_argument );
}


/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * the spin parameter is different from +1 or -1
 * 
 * GIVEN: valid beta, H, gamma parameters, but s0 parameter != +1/-1
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_invalid_spin)
{
    EXPECT_THROW( Diagram_core(1, -7, 1, 1) , std::invalid_argument );
    EXPECT_THROW( Diagram_core(1, 0, 1, 1) , std::invalid_argument );
    EXPECT_THROW( Diagram_core(1, 10, 1, 1) , std::invalid_argument );
}


/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * both H and GAMMA parameters are 0
 * 
 * GIVEN: valid beta, s0 parameters, but H=0 and GAMMA=0
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_invalid_H_GAMMA_bothzero)
{
    EXPECT_THROW( Diagram_core(1, 1, 0, 0) , std::invalid_argument );
}


/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * the (optional) parameter 'vertices' contains a non-even number of elements
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter is a list with an odd number of elements
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_non_even_number_vertices)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3}) , std::invalid_argument );
}


/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * the (optional) parameter 'vertices' contains a value greater than beta
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter contains a value > beta
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_vertices_greaterthan_beta)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3, 100}) , std::invalid_argument );
}


/**
 * @brief This test checks that the Diagram_core class constructor throws an exception if 
 * the (optional) parameter 'vertices' contains a non-sorted list
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter contains a non-sorted list of numbers
 * WHEN: they are provided as parameters to Diagram_core's class constructor
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram_core, constructor_throws_for_vertices_notsorted)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 9, 8}) , std::invalid_argument );
}




/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if 
 * the beta parameter is < 0.
 * 
 * GIVEN: valid s0, H, gamma parameters, but beta parameter < 0
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_invalid_beta_argument)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(-10, 1, 1, 1) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if 
 * the spin parameter is different from +1 or -1
 * 
 * GIVEN: valid beta, H, gamma parameters, but s0 parameter != +1/-1
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_invalid_spin_argument)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(1, -7, 1, 1) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if  
 * both H and GAMMA parameters are 0
 * 
 * GIVEN: valid beta, s0 parameters, but H=0 and GAMMA=0
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_invalid_H_GAMMA_bothzero)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(1, 1, 0, 0) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if
 * the (optional) parameter 'vertices' contains a non-even number of elements
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter is a list with an odd number of elements
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_non_even_number_vertices)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(10, 1, 1, 1, {1, 2, 3}) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if
 * the (optional) parameter 'vertices' contains a value greater than beta
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter contains a value > beta
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_vertices_argument_greaterthan_beta)
{
    EXPECT_THROW( Diagram_core(10, 1, 1, 1, {1, 2, 3, 100}) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class throws an exception if
 * the (optional) parameter 'vertices' contains a non-sorted list
 * 
 * GIVEN: valid beta, s0 , H, GAMMA parameters, but the vertices parameter contains a non-sorted list of numbers
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: a std::invalid_argument exception is thrown
 */
TEST(TestDiagram, reset_diagram_throws_for_vertices_argument_notsorted)
{
    Diagram diag(1,1,1,1);
    EXPECT_THROW( diag.reset_diagram(10, 1, 1, 1, {1, 2, 9, 8}) , std::invalid_argument );
}


/**
 * @brief This test checks that the reset_diagram method of Diagram class stores correctly
 * the new parameters in the class variables
 * 
 * GIVEN: valid beta, s0 , H, GAMMA, vertices, seed parameters
 * WHEN: they are provided as parameters to the reset_diagram method of Diagram class
 * THEN: the object whose reset_diagram method is equal to a newly constructed object with the same parameters
 */
TEST(TestDiagram, reset_diagram_sets_correct_values)
{

    double beta = 10;
    int s0 = -1;
    double H    = -2;
    double GAMMA = 0.5;
    std::list<double> vertices {1,2,3,5,9,9.9};

    Diagram diag_expected(beta, s0, H, GAMMA, vertices);
    Diagram diag_test(1,1,1,1);
    
    
    diag_test.reset_diagram(beta, s0, H, GAMMA, vertices);

    EXPECT_EQ(diag_test, diag_expected);
   
}



//#########################################################################################

//Series of tests that check wether the diagram is correctly modified when update is accepted.
//The vertices to add/remove are chosen deterministically, using the inverse of
//espression that produces a time value from the random number in the addempt_* methods

/**
 * @brief This test checks that when the ADD_SEGMENT update is accepted in the determinitic
 * attempt_add_segment method, the diagram is correctly modified with the new segment
 * 
 * GIVEN: a diagram with 4 vertices ([1,2,7,9]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=5, tau2=5.5] of the added segment, and the parameter RNacc = -1, which ensures
 * that the update is accepted, since the condition is RNacc < acceptance_rate, and acceptance_rate is >= 0
 * 
 * WHEN: RN1, RN2 and RNacc are passed as parameters to the attempt_add_segment of the diagram object with 4 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 6 vertices, containing the same 4 vertices of the
 * test diagram, plus the [tau1, tau2] segment
 */
TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram)
{

    double beta = 10;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = 7;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {1,2, tau1, tau2, tau3, 9});
    Diagram_core diag_test(beta, 1, 1, 1,     {1,2,             tau3, 9});
    
    
    diag_test.attempt_add_segment(RN1, RN2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is accepted in the determinitic
 * attempt_add_segment method, the diagram is correctly modified with the new segment, in the specific
 * case where the segment is added at the beginning. This can be useful to spot problems in the 
 * boundary cases of the algorithm that sweeps the vertices list.
 * 
 * GIVEN: a diagram with 6 vertices ([4,5,6,7,8,9]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=0.1, tau2=3] of the added segment, that should be inserted at the beginning of the list, 
 * and the parameter RNacc = -1, which ensures that the update is accepted, since the condition is RNacc < acceptance_rate, 
 * and acceptance_rate is >= 0
 * 
 * WHEN: RN1, RN2 and RNacc are passed as parameters to the attempt_add_segment of the diagram object with 4 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 8 vertices, containing the same 6 vertices of the
 * test diagram, plus the [tau1, tau2] segment in front
 */
TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_beginning)
{

    double beta = 10;
    double tau1 = 0.1;
    double tau2 = 3;
    double tau3 = 4;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {tau1, tau2, tau3, 5,6,7,8,9});
    Diagram_core diag_test(beta, 1, 1, 1, {              tau3, 5,6,7,8,9});
    

    diag_test.attempt_add_segment(RN1, RN2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is accepted in the determinitic
 * attempt_add_segment method, the diagram is correctly modified with the new segment, in the specific
 * case where the segment is added at the end. This can be useful to spot problems in the 
 * boundary cases of the algorithm that sweeps the vertices list.
 * 
 * GIVEN: a diagram with 6 vertices ([1,2,3,4,5,6]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=8.8, tau2=9.4] of the added segment, that should be inserted at the end of the list, 
 * and the parameter RNacc = -1, which ensures that the update is accepted, since the condition is RNacc < acceptance_rate, 
 * and acceptance_rate is >= 0
 * 
 * WHEN: RN1, RN2 and RNacc are passed as parameters to the attempt_add_segment of the diagram object with 6 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 8 vertices, containing the same 6 vertices of the
 * test diagram, plus the [tau1, tau2] segment at the end
 */
TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_end)
{

    double beta = 10;
    double tau1 = 8.8;
    double tau2 = 9.4;
    double tau3 = beta;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {1,2,3,4,5,6, tau1, tau2});
    Diagram_core diag_test(beta, 1, 1, 1, {  1,2,3,4,5,6,           });
    
    
    diag_test.attempt_add_segment(RN1, RN2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is accepted in the determinitic
 * attempt_add_segment method, the diagram is correctly modified with the new segment, in the specific
 * case where the initial diagram is of zero order (no vertices). This can be useful to spot problems in the 
 * boundary cases of the algorithm that sweeps the vertices list.
 * 
 * GIVEN: a diagram with 0 vertices, two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=5, tau2=5.5] of the added segment, and the parameter RNacc = -1, 
 * which ensures that the update is accepted, since the condition is RNacc < acceptance_rate, 
 * and acceptance_rate is >= 0
 * 
 * WHEN: RN1, RN2 and RNacc are passed as parameters to the attempt_add_segment of the diagram object with no vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 2 vertices, containing the [tau1, tau2] segment
 */
TEST(TestDiagram_core, attempt_add_segment_creates_correct_diagram_zeroorder)
{

    double beta = 10;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = beta;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_expected(beta, 1, 1, 1, {tau1, tau2});
    Diagram_core diag_test(beta, 1, 1, 1,    {           });
    
    
    diag_test.attempt_add_segment(RN1, RN2, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is accepted in the determinitic
 * attempt_remove_segment method, the diagram is correctly modified with the new segment
 * 
 * GIVEN: a diagram with 6 vertices ([1, 2,  5, 5.5,  8, 9]), a "fake random number" RN1 that should result
 * in the removal of the third segment (with index=2 starting from 0) [tau1=5, tau2=5.5], and the parameter RNacc = -1, which ensures
 * that the update is accepted, since the condition is RNacc < acceptance_rate, and acceptance_rate is >= 0
 * 
 * WHEN: RN1 and RNacc are passed as parameters to the attempt_remove_segment of the diagram object with 6 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 4 vertices, containing the same vertices of the
 * test diagram, minus the removed one
 */
TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram)
{

    int remove_index = 2;
    double RN1 = (double) remove_index / (6 - 1);

    Diagram_core diag_expected(10, 1, 1, 1, {1, 2,           8, 9});
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2,  5, 5.5,  8, 9});

    
    
    diag_test.attempt_remove_segment(RN1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is accepted in the determinitic
 * attempt_remove_segment method, the diagram is correctly modified with the new segment,
 * in the specific case where the removed segment is at the front of the list. This can be useful 
 * to spot problems in the boundary cases of the algorithm that sweeps the vertices list.
 * 
 * GIVEN: a diagram with 6 vertices ([1, 2,  5, 5.5,  8, 9]), a "fake random number" RN1 that should result
 * in the removal of the first segment (with index=0 starting from 0) [tau1=1, tau2=2], and the parameter RNacc = -1, which ensures
 * that the update is accepted, since the condition is RNacc < acceptance_rate, and acceptance_rate is >= 0
 * 
 * WHEN: RN1 and RNacc are passed as parameters to the attempt_remove_segment of the diagram object with 6 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 4 vertices, containing the same vertices of the
 * test diagram, minus the removed one
 */
TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram_beginning)
{

    int remove_index = 0;
    double RN1 = (double) remove_index / (6 - 2 + 1);

    Diagram_core diag_expected(10, 1, 1, 1, {      5, 5.5, 8, 9});
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2, 5, 5.5, 8, 9});

    
    
    diag_test.attempt_remove_segment(RN1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is accepted in the determinitic
 * attempt_remove_segment method, the diagram is correctly modified with the new segment,
 * in the specific case where the removed segment is at the end of the list. This can be useful 
 * to spot problems in the boundary cases of the algorithm that sweeps the vertices list.
 * 
 * GIVEN: a diagram with 6 vertices ([1, 2,  5, 5.5,  8, 9]), a "fake random number" RN1 that should result
 * in the removal of the last segment (with index=4 starting from 0) [tau1=8, tau2=9], and the parameter RNacc = -1, which ensures
 * that the update is accepted, since the condition is RNacc < acceptance_rate, and acceptance_rate is >= 0
 * 
 * WHEN: RN1 and RNacc are passed as parameters to the attempt_remove_segment of the diagram object with 6 vertices
 * 
 * THEN: the diagram under test becomes equal to another diagram with 4 vertices, containing the same vertices of the
 * test diagram, minus the removed one
 */
TEST(TestDiagram_core, attempt_remove_segment_creates_correct_diagram_end)
{

    int remove_index = 4;
    double RN1 = (double) remove_index / (6 - 2 + 1);

    Diagram_core diag_expected(10, 1, 1, 1, {1, 2, 5, 5.5,       });
    Diagram_core diag_test(10, 1, 1, 1,     {1, 2, 5, 5.5,  8, 9});

    
    
    diag_test.attempt_remove_segment(RN1, -1); //-1: force acceptance

    EXPECT_EQ(diag_test, diag_expected);
   
}


/**
 * @brief This test checks that when the SPIN_FLIP update is accepted in the determinitic
 * attempt_spin_flip method, the diagram is correctly modified with the new spin value, and
 * also that a double flip results in the initial diagram.
 * 
 * GIVEN: a diagram starting with spin up (s0 = +1), and the parameter RNacc = -1, which ensures
 * that the update is accepted, since the condition is RNacc < acceptance_rate, and acceptance_rate is >= 0
 * 
 * WHEN: RNacc is passed twice as parameters to the attempt_remove_segment of the tested diagram object
 * 
 * THEN: the diagram under test, intially with spin up(s0=+1), becomes equal to another diagram with spin down(s0=-1)
 * when attempt_spin_flip is executed for the first time. Then, executing again attempt_spin_flip the test diagram
 * becomes equal to the diagram with spin up, and so also to its initial value before the double flip.
 */
TEST(TestDiagram_core, attempt_spin_flip_creates_correct_diagram)
{
    Diagram_core diag_up(10, 1, 1, 1, {1,2, 7,9});
    Diagram_core diag_dw(10, -1, 1, 1, {1,2, 7,9});

    Diagram_core diag_test = diag_up;

    diag_test.attempt_spin_flip(-1); //force acceptance

    EXPECT_EQ(diag_test, diag_dw);

    diag_test.attempt_spin_flip(-1); //force acceptance

    EXPECT_EQ(diag_test, diag_up);        

}


//#############################################################################################

//Series of tests that check wether the attempted updates are accepted/rejected with the correct rate.
//The vertices to add/remove are chosen deterministically, using the inverse of espression 
//that produces a time value from the random number in the addempt_* methods


/**
 * @brief This test checks that when the ADD_SEGMENT update is attempted through the 
 * attempt_add_segment method, it is accepted with the correct rate. This test is useful
 * to check that the vertices and spin of the new segment are calculated correctly by the method.
 * 
 * GIVEN: a diagram with 4 vertices ([1,2,8,9]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=5, tau2=5.5] of the added segment, and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1, RN2 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_add_segment_correct_rate)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 5;
    double tau2 = 5.5;
    double tau3 = 8;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,      {1,2, tau1, tau2, tau3, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {1,2,             tau3, 9});


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(RN1, RN2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(RN1, RN2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is attempted through the 
 * attempt_add_segment method, it is accepted with the correct rate, in the specific case
 * where the segment is added in front of the list. 
 * This test is useful to check that the vertices and spin of the new segment are calculated correctly by the method,
 * in the boundary cases for the algorithm that sweeps the list of vertices.
 * 
 * GIVEN: a diagram with 6 vertices ([2,5,6,7,8,9]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=1, tau2=1.5] of the added segment, and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1, RN2 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_add_segment_correct_rate_beginning)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 1;
    double tau2 = 1.5;
    double tau3 = 2;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,      {tau1, tau2, tau3, 5,6,7,8,9});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {            tau3, 5,6,7,8,9});

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(RN1, RN2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(RN1, RN2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is attempted through the 
 * attempt_add_segment method, it is accepted with the correct rate, in the specific case
 * where the segment is added at the end of the list. 
 * This test is useful to check that the vertices and spin of the new segment are calculated correctly by the method,
 * in the boundary cases for the algorithm that sweeps the list of vertices.
 * 
 * GIVEN: a diagram with 6 vertices ([1,2,3,4,5,6]), two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=8, tau2=9] of the added segment, and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1, RN2 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_add_segment_correct_rate_end)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 8;
    double tau2 = 9;
    double tau3 = beta;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,  {1,2,3,4,5,6, tau1, tau2});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {1,2,3,4,5,6,           });

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(RN1, RN2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(RN1, RN2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}


/**
 * @brief This test checks that when the ADD_SEGMENT update is attempted through the 
 * attempt_add_segment method, it is accepted with the correct rate, in the specific case
 * where the diagram is of zero order (no vertices - empty list). 
 * This test is useful to check that the vertices and spin of the new segment are calculated correctly by the method,
 * in the boundary cases for the algorithm that sweeps the list of vertices.
 * 
 * GIVEN: a diagram with no vertices, two "fake random numbers" RN1 and RN2 that should result
 * in the new vertices [tau1=1, tau2=9] of the added segment, and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1, RN2 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_add_segment_correct_rate_zeroorder)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1 = 1;
    double tau2 = 9;
    double tau3 = beta;

    double RN1 = tau1 / beta;
    double RN2 = (tau2 - tau1) / (tau3 - tau1);

    Diagram_core diag_new(10, 1, 1, GAMMA,  {tau1, tau2});
    Diagram_core diag_current(10, 1, 1, GAMMA,  {          });


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * GAMMA*GAMMA * beta * (tau3 - tau1) / (diag_current.order() + 1);

    EXPECT_TRUE(diag_test1.attempt_add_segment(RN1, RN2, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_add_segment(RN1, RN2, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
    
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is attempted through the 
 * attempt_remove_segment method, it is accepted with the correct rate.
 * This test is useful to check that the vertices and spin of the segment to be removed are calculated correctly by the method.
 * 
 * GIVEN: a diagram with 6 vertices ([1,2, 5, 5.5, 8,9]), a "fake random number" RN1 that should result
 * in the removal of the third segment (with index=2 starting from 0) [tau1=5, tau2=5.5], and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_remove_segment_correct_rate)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 5; int remove_index = 2;
    double tau3  = 8;

    double RN1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {1, 2,              tau3, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2,  tau1, 5.5,  tau3, 9});
 
    
    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(RN1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(RN1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is attempted through the 
 * attempt_remove_segment method, it is accepted with the correct rate, in the specific case where the segment
 * to be removed is at the front of the diagram.
 * This test is useful to check that the vertices and spin of the segment to be removed are calculated correctly by the method.
 * 
 * GIVEN: a diagram with 6 vertices ([1,2, 5, 5.5, 8,9]), a "fake random number" RN1 that should result
 * in the removal of the first segment (with index=0 starting from 0) [tau1=1, tau2=2], and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_remove_segment_correct_rate_beginning)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 1; int remove_index = 0;
    double tau3  = 5;

    double RN1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {      5, 5.5, 8, 9});
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2, 5, 5.5, 8, 9});


    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(RN1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(RN1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}


/**
 * @brief This test checks that when the REMOVE_SEGMENT update is attempted through the 
 * attempt_remove_segment method, it is accepted with the correct rate, in the specific case where the segment
 * to be removed is at the end of the diagram.
 * This test is useful to check that the vertices and spin of the segment to be removed are calculated correctly by the method.
 * 
 * GIVEN: a diagram with 6 vertices ([1,2, 5, 5.5, 8,9]), a "fake random number" RN1 that should result
 * in the removal of the last segment (with index=4 starting from 0) [tau1=8, tau2=9], and the expected_acceptance_rate,
 * calculated using the ratio of the WEIGTHS (values) of the new and current diagram, instead of the 
 * faster expression inside the method. 
 * 
 * WHEN: RN1 and RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 are passed as parameters to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
TEST(TestDiagram_core, attempt_remove_segment_correct_rate_end)
{

    double beta = 10;
    double GAMMA = 1;
    double tau1  = 8; int remove_index = 4;
    double tau3  = beta;

    double RN1 = (double) remove_index / (6 - 1);

    Diagram_core diag_new(10, 1, 1, GAMMA,     {1, 2, 5, 5.5,       });
    Diagram_core diag_current(10, 1, 1, GAMMA, {1, 2, 5, 5.5,  8, 9});

    Diagram_core diag_test1 = diag_current;
    Diagram_core diag_test2 = diag_current;

    double expected_acceptance_rate = diag_new / diag_current * (diag_current.order() - 1) / ( GAMMA*GAMMA * beta * (tau3-tau1) ) ;

    EXPECT_TRUE(diag_test1.attempt_remove_segment(RN1, expected_acceptance_rate - 0.00001));   //acctepted if RNG < acc
    EXPECT_FALSE(diag_test2.attempt_remove_segment(RN1, expected_acceptance_rate + 0.00001));  //recjected if RNG > acc
   
}


/**
 * @brief This test checks that when the SPIN_FLIP update is attempted through the 
 * attempt_spin_flip method, it is accepted with the correct rate.
 * 
 * GIVEN: a diagram with spin up (s0=+1) and the expected_acceptance_rate, calculated using the ratio 
 * of the WEIGTHS (values) of the new and current diagram, instead of the faster expression inside the method. 
 * 
 * WHEN: RNacc = expected_acceptance_rate + 0.00001 and - 0.00001 is passed as parameter to 
 * two copies of the same test diagram
 * 
 * THEN: the update is accepted if RNacc < expected_acceptance_rate, rejected if RNacc > expected_acceptance_rate
 */
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


/**
 * @brief This test checks that the REMOVE_SEGMENT update, attempted through the 
 * attempt_remove_segment method, is always rejected immediately if the diagram is of zero order.
 * 
 * GIVEN: a diagram of 0 order (no vertices - empty list)
 * 
 * WHEN: RNacc = -1 is passed to attempt_remove_segment, so that in principle the update should be 
 * always accepted 
 * 
 * THEN: the update is rejected
 */
TEST(TestDiagram_core, attempt_remove_segment_always_rejects_for_zero_order)
{

    Diagram_core diag_test(1, 1, 1, 1, {});

    EXPECT_FALSE(diag_test.attempt_remove_segment(0.5, -1)); //-1 would be certain acceptance, regardless of tau1

   
}






/*

TEST: 
-Controllo sia su value() che sui 3 acceptance_rate() con valori numerici calcolati "a mano"
-Test per sum_deltatau
-Insead of checking for equality of the diagrams, should I check float equality of their values?

*/