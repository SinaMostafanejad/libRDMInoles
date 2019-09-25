#include <stdio.h>
#include <armadillo>
#include <iostream>
#include <fstream>
#include "mcpdft.h"
#include "energy.h"
#include "memRW.h"

using namespace mcpdft;

int main(int argc, char *argv[]) {
    if ( argc < 2 ) { 

       std::cout << "An argument is needed!" << std::endl;
       printf("Usage: %s <test_case>\n", argv[0]);
       return 1;
    }
  
    MemRW *memrw;
    memrw = new MemRW();

    // MCPDFT* mc = new MCPDFT(test_case);
    MCPDFT *mc;
    mc = new MCPDFT("h2_svwn_sto3g");

    // fetching the number of basis functions
    int nbfs;
    nbfs = mc->get_nbfs();

    /* building alpha and beta 1-electron reduced
     * density matrices (1RDMs)
     */
    // mc->build_opdm();
    arma::mat D1a(mc->get_D1a());
    arma::mat D1b(mc->get_D1b());

    /* building the alpha-beta block of the 2-electron reduced
     * density matrix (2RDM)
     */
    mc->build_tpdm();
    arma::mat D2ab(mc->get_D2ab());

    // Calculating the amount of available and required  memory
    memrw->calculate_memory(D1a, D2ab);

    // building the one electron densities rho_a(r) and rho_b(r)
    mc->build_rho();

    // building the on-top pair density pi(r,r)
    mc->build_pi(D2ab);

    // building the R(r) factor for density translation
    mc->build_R();

    // translate the one-electron densities
    mc->translate();

    // calculating the MCPDFT energy correction
    double e = mcpdft_energy(mc,D1a,D1b,D2ab);

    // getting the value of the reference energy
    double eref = mc->get_eref();
    // printf("eref = %-20.15lf\n",eref);

    printf("=================================================\n");
    printf("   Reference energy      =  %-20.12lf\n",  eref);
    printf("   MCPDFT energy         =  %-20.12lf\n",     e);
    printf("   E(MCPDFT) - E(Ref)    =  %-20.2le\n", e-eref);
    printf("=================================================\n");

    delete memrw;
    delete mc;

    return 0;
}
