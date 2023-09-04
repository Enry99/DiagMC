#import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys



def plot_convergence_test(results_filename : str = 'results_conv_test.csv'):
    '''
        This function plots the results of the convergence test varying N_total_steps.

        Parameters:
            results_filename : name (path) of the csv file containing the results produced by the program
    '''

    #increase default plot sizes for better visiblity
    plt.rcParams['axes.labelsize'] = 14
    plt.rcParams['axes.titlesize'] = 15


    #read results data from file, store it inside a Pandas Dataframe
    results = pd.read_csv(results_filename)


    #extract value of the (constant during the convergence sweep) physical parameters
    beta = results["beta"][0]
    h = results["H"][0]
    gamma = results["GAMMA"][0]


    #calculate the theoretical (expected) value of the magnetizations along z and x
    E = np.sqrt(h**2 + gamma**2)
    mz_theor = -h/E*np.tanh(beta*E)
    mx_theor = -gamma/E*np.tanh(beta*E)


    #set the figure for the two subplots
    fig, (ax1, ax2) = plt.subplots(1,2, figsize=(14,5))
    fig.suptitle('Convergence test - Magnetization vs N_measures (fixed seed)')

    #plot mz
    ax1.axhline(mz_theor , label = '$m_z$ theor.', linestyle='--', color='dimgrey')
    ax1.semilogx(results['N_measures'], results['measured_sigmaz'], '-o', color='tab:blue', label = '$m_z$ diagMC')
    ax1.set_title('$m_z$')
    ax1.set_ylabel('$m_z$')
    ax1.set_xlabel('N_measures')
    ax1.grid(linestyle='dotted')
    ax1.legend()



    #plot mx
    ax2.axhline(mx_theor , label = '$m_x$ theor.', linestyle='--', color='dimgrey')
    ax2.semilogx(results['N_measures'], results['measured_sigmax'], '-o', color='tab:red', label = '$m_x$ diagMC')
    ax2.set_title('$m_x$')
    ax2.set_ylabel('$m_x$')
    ax2.set_xlabel('N_measures')
    ax2.grid(linestyle='dotted')
    ax2.legend()

    #save plot to file
    fig.savefig('convergence_test.png', dpi=300, bbox_inches='tight')



#execute plot function
if len(sys.argv) > 1:
    plot_convergence_test(sys.argv[1])  #use command line argument for filename if passed
else: plot_convergence_test()           #otherwise use default name
