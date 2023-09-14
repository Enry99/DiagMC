# Diagrammatic Monte Carlo simulation of a 2-level spin system in an external magnetic field

![example workflow](https://github.com/Enry99/DiagMC/actions/workflows/cmake-multi-platform.yml/badge.svg)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This program implements a Markov Chain Monte Carlo algorithm, based on [Metropolis-Hastings](https://en.wikipedia.org/wiki/Metropolis%E2%80%93Hastings_algorithm), to sample Feynman diagrams
associated to a series expansion of the partition function for a 2-level spin system in an external magnetic field, with longitudinal and transversal components.
This method allows to calculate the magnetization along z and x, that can be compared to the theoretical values obtained by the exact diagonalization of the Hamiltonian.
Its purpose is to use a simple and analytically-solvable system to become familiar with the implementation of a Diagrammatic Monte Carlo algorithm, which can be applied effectively in cases where no analytical solution exists,
such as the study of complex electron-phonon interactions.

## Installation using CMake
The installation procedure relies on [CMake](https://cmake.org/), which is cross-platform and can be used on many operative systems, like Linux and Windows.\
Note that on Windows you need to have an available C++ compiler, such as [MSVC](https://docs.microsoft.com/en-us/cpp/), which is provided with [Visual Studio](https://visualstudio.microsoft.com/).\
A viable alternative on Windows is to use [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/).

First, **clone the repository** in your local machine using [git](https://git-scm.com/):
```sh
$ git clone https://github.com/Enry99/DiagMC.git
```
This will create a folder called ```DiagMC```. Go inside this folder with:
```sh
$ cd DiagMC
```
Now **build and install** the program using **CMake**:
```sh
$ mkdir build
$ cd build
$ cmake .. -DCMAKE_BUILD_TYPE Release
$ cmake --build . --target install --config Release
```
**The program executable can be found in ```DiagMC/bin```**.

### Dependencies
The program depends on the [json](https://github.com/nlohmann/json.git) library for reading the settings from file, and on [googletest](https://github.com/google/googletest.git) for unit testing.
Both dependencies are automatically downloaded and included by CMake during the build process, so an Internet connection is required during the installation.

It is possible to exclude the googletest dependency by building without tests:
```sh
$ cmake .. -DBUILD_TESTING=OFF
```
However, testing is recommended before using the program.


### Execute unit tests
In order to execute the tests, go to the  ```build``` directory and run:
```sh
$ ctest
```

## Utilization guide

The program reads the parameters from a json settings file.
The path of the file can be passed as command line argument upon execution. If nothing is specified, the programs looks for 'settings.json'.

To run the program, go to the ```bin``` directory and execute it with:
```sh
$ ./2levelDiagMC [settings_filename]
```
where you can replace ```settings_filename``` with the proper name of the desired json settings file.

The parameters for the settings file are described below.

Three types of calculations are possible, and need to be specified in the flag ```CALC_TYPE```:
1. **"single"**, which performs a single run of the algorithm for the given parameters, and prints a summary of the results. An example of settings file for this type of calculation is [settings_singlerun.json](https://github.com/Enry99/DiagMC/blob/main/examples/settings_singlerun.json)
2. **"sweep"**, which runs the algorithm for different values of ```H```, ```GAMMA``` and  ```beta``` in the given range, for all the combinations, and writes the results to a csv file. An example of settings file for this type of calculation is [settings_sweep.json](https://github.com/Enry99/DiagMC/blob/main/examples/settings_sweep.json)
3. **"convergence-test"**, which runs the program multiple times for a fixed set of physical parameters and the same seed, varying the number of steps of the simulation, ```N_total_steps```, and optionally also ```N_thermalization_steps```. An example of settings file for this type of calculation is [settings_conv_test.json](https://github.com/Enry99/DiagMC/blob/main/examples/settings_conv_test.json)


The results for "sweep" and "convergence-test" modes are written to a csv file, which must be specified as ```output_file```, with columns corresponding to variables and lines corresponding to each run.
The reported values include all the input parameters, the results for the two magnetizations, the statistics of acceptance for the updates, the maximum and average diagram order, the two seeds for each run, and the runtime of the Metropolis-Hastings loop (in nanoseconds) in the column "run_time".


The settings parameters for a single run are:
- ```beta```:	Inverse temperature (equal to legnth of the diagram). Must be > 0.
- ```initial_s0``` (optional):	Spin of the 0-th segment of the diagram (0-----t1) at the beginning of the simulation. Must be +1 or -1. Defaults to +1 if not specified.
- ```H```: Value of the longitudinal component of magnetic field.
- ```GAMMA```: Value of the transversal component of magnetic field. Must be $\neq$ 0. Values much higher than one could cause severe slowdown due to creation of a huge number of vertices.
- ```N_total_steps```: Total number of steps of the MCMC algorithm. For this system, values above 10 million give very accurate results.
- ```N_thermalization_steps``` (optional):	Number of initial steps for which statistics is not collected. For the suggested value of ```N_total_steps``` can be safely set to 0. Defaults to 0 if not specified.
- ```update_choice_seed``` (optional): Seed for the Mersenne-Twister random number generator to choose *which* update to attempt. Must be a non-negative integer.
- ```diagram_seed``` (optional): Seed for the diagram, used *inside* the updates.  Must be a non-negative integer.

In "sweep" mode, one or more parameters between ```H```, ```GAMMA``` and  ```beta``` can be substituted by a parameter range and a step, with the variable name and the suffix ```_min```, ```_max``` and ```_step```, e.g.
- ```H_min```= -1,
- ```H_max```= 1,
- ```H_step```= 0.2

In this mode it is not possible to set the seeds, which are assigned automatically in a unique way based on system clock.
  

In "convergence-test" mode, one or more parameters between ```N_total_steps``` and ```N_thermalization_steps``` can be substituted by a parameter range and the number of points per decade (the step is linear in logscale), with the variable name and the suffix ```_min```, ```_max``` and ```_points_per_decade```, e.g.

- ```N_total_steps_min``` = 1e3,
- ```N_total_steps_max``` = 1e8,
- ```N_total_steps_points_per_decade``` = 4


## Examples

The results of the sweep and convergence tests, produced with the two corresponding files in the [examples](https://github.com/Enry99/DiagMC/blob/main/examples) folder, are reported in these images,
showing a perfect match with the analytical results.

<img src="/examples/sweep_beta=10.0.png" width="800">
<img src="/examples/convergence_test.png" width="800">

These plots were generated using two Python scripts, provided in the [examples](https://github.com/Enry99/DiagMC/tree/main/examples) folder.
They can be run by passing as a command line argument the name of the csv with the results, e.g.

```sh
$ python3 plot_h_gamma_sweep.py results_sweep.csv
```

Note that this scripts are provided just as examples and do not cover every plot possiblity from the results file.

## Structure of the project

The project is structured as follows:
1. the [include/diagmc](https://github.com/Enry99/DiagMC/blob/main/include/diagmc) and [src](https://github.com/Enry99/DiagMC/blob/main/src) folders, which contain the declarations and definitions of the classes and functions.\
   The code is composed by the following units: 
    - [diagram.h](https://github.com/Enry99/DiagMC/blob/main/include/diagmc/diagram.h) / [diagram.cpp](https://github.com/Enry99/DiagMC/blob/main/src/diagram.cpp) implement the Diagram_core and Diagram classes,
      which contain the variables defining a Feynman diagram, and the methods to attempt and perform the Monte Carlo updates, modifying the variables inside the object.\
      In particular, Diagram_core contains all the main functionalities of the diagram object, involving the **fully deterministic** part of the code, while Diagram is a derived class of Diagram_core,
      adding the random behaviour by including the ([Mersenne-Twister](https://en.wikipedia.org/wiki/Mersenne_Twister)) random number generator, which allows to randomly perform updates within the object, without needing to pass values to the methods.\
      A Diagram object, once initalized, is fully self-sufficient to perform the desired sequence of updates.
    - [simulation.h](https://github.com/Enry99/DiagMC/blob/main/include/diagmc/simulation.h) / [simulation.cpp](https://github.com/Enry99/DiagMC/blob/main/src/simulation.cpp) implement the core function of the algorithm, run_simulation,
      which executes the Metropolis Hastings algorithm loop, attempting updates at each iteration, and collecting statistics.\
      Moreover, in these files the class SingleRunResults is implemented, which collects all the data pertaining to a run of the DMC loop (i.e. the input parameters, the results and the statisics).\
      The SingleRunResults class has a method for printing a summary of the results to standard output, and a method to write the data to file in csv format. An object of this class is returned by the run_simulation function.
    - [setup.h](https://github.com/Enry99/DiagMC/blob/main/include/diagmc/setup.h) / [setup.cpp](https://github.com/Enry99/DiagMC/blob/main/src/setup.cpp) implement the functions to read the settings from file, setup the proper parameters and run the
      selected type of calculations. In particular, the functions contain the loops to sweep over a range of the parameters and save the results of all the combination of parameters as rows of a csv file.
    - [main.cpp](https://github.com/Enry99/DiagMC/blob/main/src/main.cpp) is the main function of the executable, which calls the function setup function, with the possiblity to pass the name of the settings file as a command line argument.
3. the [tests](https://github.com/Enry99/DiagMC/blob/main/tests) folder, which contains the [tests.cpp](https://github.com/Enry99/DiagMC/blob/main/test/tests.cpp) source file, with all the unit tests for the program.
   The test involve all methods of the Diagram_core class, and the Metropolis-Hastings loop function, checking that the results coincide with the expected values
5. the [examples](https://github.com/Enry99/DiagMC/blob/main/examples) folder, which contains three examples of settings files, two csv files with the results of the calculations, two python scripts to plot the results, and the plotted images.
   More on this in the Examples section.

The documentation of all classes and functions (generated with [Doxygen](https://www.doxygen.nl/)) can be found [here](https://enry99.github.io/DiagMC/) under the 'Classes' and 'Files' menus.
  
## Theory
Diagrammatic Monte Carlo (DMC) is a very powerful method that allows to calculate quantities that can be expressed in terms of diagrammatic expansions of the form
    $$Q({y}) = \sum_{n=0}^\infty \sum_{\xi_n}\int dx_1 ... dx_n D_n^{\xi_n}(\{y\}; x_1,...,x_n)$$
by stochastically sampling the diagrams using a Markov-Chain process [[1](https://doi.org/10.1016/j.phpro.2010.09.034)]. 
$Q$ is usually a Green's function, with $y$ external parameters, and $D_n^{\xi_n}(\{y\}; x_1,...,x_n)$ are the Feynman diagrams of order n, depending on the internal (integration) variables $x_1,...,x_n$, where $\xi_n$ distinguishes different diagram structures of same order.

This program simulates a very simple quantum system: a single spin in an external magnetic field, which can be solved analytically, allowing direct comparison of the results with the exact theoretical values.

### Analytical treatment
The Hamiltonian of the single spin system in an external magnetic field is
$$\hat{H} = \hat{H_0} + \hat{H_1} = h \sigma_z + \Gamma \sigma_x$$
where $h$ and $\Gamma$ are the field strength in the z and x direction, respectively, while  $\sigma_x$ and $\sigma_z$ are the Pauli matrices.
The basis states are $\ket{\uparrow} = (1,0)$ and $\ket{\downarrow} = (0,1)$, which are eigenstates of $\sigma_z$ with eigenvalues $\pm 1$.

This Hamiltonian can be exactly diagonalized, with eigenvalues $\pm E := \pm\sqrt{h^2+\Gamma^2}$ and eigenvectors $\ket{\Psi_+}$, $\ket{\Psi_-}$

In the diagonalized basis, the partition function $Z$ of the system is simply given by

$$Z = Tr( e^{-\beta \hat{H}} ) =  \bra{\Psi_+} e^{-\beta \hat{H}} \ket{\Psi_+} + \bra{\Psi_-} e^{-\beta \hat{H}} \ket{\Psi_-} = e^{-\beta E} + e^{\beta E}$$
where $\beta = \frac{1}{T}$ is the inverse temperature.

In the same way, just with a bit more algebra, it's possible to calculate the **exact expressions for the magnetization in the z and x directions**:

$$ \langle \sigma_z \rangle = Z^{-1} Tr(\sigma_z e^{-\beta \hat{H}}) = -\frac{h}{E} \tanh(\beta E) $$

$$ \langle \sigma_x \rangle = Z^{-1} Tr(\sigma_x e^{-\beta \hat{H}}) = -\frac{\Gamma}{E} \tanh(\beta E) $$

### Computational approach with Diagrammatic Monte Carlo

To study this problem using DMC we will use a path integral representation in imaginary time of the
partition function. In the Matsubara formalism, in fact, it's possible to replace the expectation values of operators in a canonical ensemble with the expectation values in ordinary QFT (so involving Feynman diagrams) by considering time evolution in imaginary time $\tau = it$ (with $0\leq t \leq \beta$).
So the partition functions in this representation (for $\tau_1 < \tau_2 < ... < \tau_n$) reads as:

$$   Z = \sum_{n=0}^\infty (-1)^n \sum_ {s = \downarrow, \uparrow} \int_0^\beta d\tau_1 \cdots \int_{\tau_{n-1}}^\beta d\tau_n \bra{s} e^{-\beta \hat{H_0}} \hat{H_1}(\tau_n) \cdots \hat{H_1}(\tau_1) \ket{s} $$

in which the operators $\hat{H_1}(\tau_k)$ are in the interaction picture, and are given by:

$$   \hat{H_1}(\tau_k) = e^{\hat{H_0} \tau_k} \hat{H_1} e^{-\hat{H_0} \tau_k} =  e^{\hat{H_0} \tau_k} \Gamma \sigma_x e^{-\hat{H_0} \tau_k} $$

The **diagram weight** for a diagram of order $n$, spin of the first segment $s$, and $\tau_1, ... ,\tau_n$ vertices is:

$$ D_n^s(\tau_1, ... ,\tau_n) = \Gamma^n \prod_{i=0}^{n} \exp(-h s(i) (\tau_{i+1} - \tau_i)) $$

where $s(i) = s(-1)^{i}$ is the spin of the i-th segment, while $\tau_0 = 0$ and $\tau_{n+1} = \beta$. 

### DMC Updates
Since we want (in principle) to be able to explore the diagrams of any possible order, first of all we need an update ADD-SEGMENT, which adds two new vertices inside a specified segment, thus increasing the order of the diagram by 2.
The Markov-Chain process used in this work is based on the **Metropolis-Hastings algorithm**, so to satisfy the detailed balance we need also the inverse update, REMOVE-SEGMENT (which decreases the order by 2).

For the ADD-SEGMENT update, we first extract uniformly the first time $\tau_j$, with $p(\tau_j) = U(0, \beta) = 1/\beta$, which identifies the segment (of extremes $[\tau_{j-1},\tau_{j+2}]$), then we extract the second time $\tau_{j+1}$ inside the same segment, after $\tau_j$ but before the end of the segment, with $p(\tau_{j+1}) = U(\tau_j, \tau_{j+2}) = 1/(\tau_{j+2} - \tau_j)$.

For the REMOVE-SEGMENT update, instead, we simply draw uniformly the first vertex $N_{j}$ to be removed, and for the second one we simply take the next, so the probability is p($N_{j}) = \frac{1}{n - 1}$. Note that we can only choose $n-1$ vertices, since we cannot pick as first vertex the last one.
Of course, the REMOVE-SEGMENT update is only possible when there is a non-zero number of vertices, so in case of a 0-th order diagram the update is rejected right away.

Moreover, we can consider another update, which allows us to sample at the same times both spin components, which is the SPIN-FLIP update. This update simply flips the spin of all the segments in the diagram, so no new variables have to be randomly extracted. The spin-flip is already the inverse of itself, so we don't need another different update.

With these considerations, the **acceptance rates of the three updates** are:

- ADD_SEGMENT:
  
$$ A_{n\rightarrow n+2} = 
    \min \left( 1,  \frac{D_{n+2}^s(\tau_1,...,\tau_j,\tau_{j+1},...,\tau_n)}{D_{n}^s(\tau_1,...,\tau_n)}\frac{U(0,n+1)}{ U(0, \beta) U(\tau_j, \tau_{j+2})} \right) = 
    \min \left(1, \Gamma^2 e^{-2h s(j) (\tau_{j+1}- \tau_j)} \frac{\beta (\tau_{j+2} - \tau_j)}{n+1} \right) $$

- REMOVE_SEGMENT:

$$ A_{n\rightarrow n-2} = 
   \min \left(1,  \frac{D_{n-2}^s(\tau_1,...,\tau_n)}{D_{n}^s(\tau_1,...\tau_j,\tau_{j+1},...\tau_n)}\frac{U(0, \beta)U(\tau_j, \tau_{j+2})}{U(0,n-1)  } \right) = 
  \min \left(1, \Gamma^{-2} e^{2h s(j) (\tau_{j+1}- \tau_j)} \frac{n-1}{\beta (\tau_{j+2} - \tau_j)} \right) $$
  
- SPIN_FLIP:

$$ A_{s\rightarrow -s} 
      = \min \left(1, \frac{D_{n}^{-s}(\tau_1,...,\tau_n)}{D_{n}^{s}(\tau_1,...,\tau_n)}\right) \\
      = \min \left(1, \prod_{i=0}^{n} e^{2h s(i) (\tau_{i+1} - \tau_i)} \right) $$


To extract the the two magnetizations along x and z, we use the following **Monte Carlo estimators**:

$$ \langle \sigma_x \rangle _{MC} = -\frac{\langle n \rangle}{\beta \Gamma}  $$

$$ \langle \sigma_z \rangle _{MC} = \langle m_z(D) \rangle $$ 

where $m_z(D)$ is the magnetization of each diagram:

$$ m_z(D) = \frac{1}{\beta}\int_0^\beta d\tau \sigma_z(\tau) = \frac{s}{\beta}(\beta -  2 \sum_{k=1}^{n/2} (\tau_{2k} - \tau_{2k-1})) $$

### Metropolis-Hastings algorithm

The algorithm used to draw samples from the unknown diagram distribution is a Markov-Chain Monte Carlo method, based on the Metropolis-Hastings algorithm. The steps are as follows:
1.  Initialize external variables: $h$, $\Gamma$, $\beta$.
2.  Start with a 0-th order diagram, initialized with spin up or down
3.  Thermalization steps to allow the Markov chain to reach the stationary desired distribution, before starting to collect samples.
4.  Main loop (repeat until the desired number of samples has been collected):

    -   Randomly choose one of the three updates (in this case
        uniformly) and propose a new diagram according to the selected
        update

    -   Calculate the acceptance rate $A$ for the update

    -   Extract a random number $r \sim U(0,1)$, if $r &lt; A$ accept the
        update, setting the proposed diagram as the current one,
        otherwise reject the update.

    -   Collect desired samples (in this case the diagram magnetization
        and the order)

5.  Calculate final quantities (in this case the magnetization) by
    averaging over the samples.



## License
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

This program is licensed under the [GNU General Public License Version 3][GPLv3].

[GPLv3]: https://www.gnu.org/licenses/gpl-3.0
