#include "mcpdft.h"
#include "energy.h"
#include <stdio.h>
#include <armadillo>
#include <iostream>
#include <fstream>

using namespace mcpdft;

int main() {
  
    MCPDFT* mc = new MCPDFT;

    // fetching the number of basis functions
    int nbfs;
    nbfs = mc->get_nbfs();

    mc->build_opdm();

    arma::mat D1a(mc->get_D1a());
    arma::mat D1b(mc->get_D1b());

    /* building the alpha-beta block of the 2-electron reduced
     * density matrix (2RDM)
     */
    arma::mat D2ab(nbfs*nbfs, nbfs*nbfs, arma::fill::zeros);
    D2ab = arma::kron(D1a,D1b);
    // D2ab.print("D2ab = ");


    double e = mcpdft_energy(mc,D1a,D1b,D2ab);


    printf("==========================================\n");
    printf("   MCPDFT energy =  %-20.15lf\n",   e);
    printf("==========================================\n");

    delete mc;

    return 0;
}
