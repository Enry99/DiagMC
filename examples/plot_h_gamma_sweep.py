#import necessary libraries
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys



def plot_sweep(results_filename : str = 'results_sweep.csv'):
    '''
        This function plots the results of the sweep varying both H and GAMMA (fixed beta), plotting magnetizations as a
        function of H for different GAMMA values.

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

    #set the figure for the two subplots
    fig, (ax1, ax2) = plt.subplots(1,2, figsize=(14,5))
    fig.suptitle(r'z and x Magnetization ($\beta$ = '+str(beta)+')')

    #enhanced lines to better see x and y axis
    ax1.axhline(0, color='dimgray', linewidth = 1)
    ax1.axvline(0, color='dimgray', linewidth = 1)
    ax2.axhline(0, color='dimgray', linewidth = 1)
    ax2.axvline(0, color='dimgray', linewidth = 1)

    #set colormap for the different gamma values
    colors = plt.cm.gnuplot(np.linspace(0, 1, len(results["GAMMA"].unique()) +2 ))

    #x values for plotting the continous theoretical function
    x = np.linspace(-1,1, 100)

    for i, gamma in enumerate(results["GAMMA"].unique()):
        E = np.sqrt(x**2 + gamma**2)

        #plot theoretical function
        ax1.plot(x, -x/E*np.tanh(beta*E), color=colors[i])
        ax2.plot(x, -gamma/E*np.tanh(beta*E), color=colors[i])

        #plot diagMC values
        ax1.plot(results[results["GAMMA"] == gamma]['H'], results[results["GAMMA"] == gamma]['measured_sigmaz'], 'o', color=colors[i], label = '$\Gamma$={0}'.format(round(gamma,1)))
        ax2.plot(results[results["GAMMA"] == gamma]['H'], results[results["GAMMA"] == gamma]['measured_sigmax'], 'o', color=colors[i], label = '$\Gamma$={0}'.format(round(gamma,1)))

    ax1.set_title('$m_z$')
    ax1.set_ylabel('$m_z$')
    ax1.set_xlabel('h')
    ax1.set_xlim(-1,1)
    ax1.grid(linestyle='dotted')
    ax1.legend()

    ax2.set_title('$m_x$')
    ax2.set_ylabel('$m_x$')
    ax2.set_xlabel('h')
    ax1.set_xlim(-1,1)
    ax2.grid(linestyle='dotted')
    ax2.legend()

    #save plot to file
    fig.savefig('sweep_beta={:.1f}'.format(beta)+'.png', dpi=300, bbox_inches='tight')



#execute plot function
if len(sys.argv) > 1:
    plot_sweep(sys.argv[1])  #use command line argument for filename if passed
else: plot_sweep()           #otherwise use default name