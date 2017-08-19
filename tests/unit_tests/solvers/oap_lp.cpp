/*
 * DSE class test
 *
 * Keith O'Hara
 * 08/01/2016
 * 
 * cd ~/Desktop/SCM/GitHub/TraME/src/trame/tests/markets
 *
 * g++-mp-5 -O2 -Wall -std=c++11 -I/opt/local/include -I./../../headers -I/usr/local/include oap_lp_test.cpp -o oap_lp.test -L/opt/local/lib -ltrame -framework Accelerate
 */

#include "trame.hpp"

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //
    // inputs:

    int nbX = 8;
    int nbY = 5;

    arma::vec n = arma::ones(nbX,1);
    arma::vec m = arma::ones(nbY,1);

    arma::mat alpha  = arma::randu(nbX,nbY);
    arma::mat gamma  = arma::randu(nbX,nbY);

    arma::mat phi = alpha + gamma;

    //
    // results

    printf("\n*===================   Start of oap_lp Test   ===================*\n");
    printf("\n");

    //
    // build

    trame::dse<trame::arums::none,trame::arums::none,trame::transfers::tu> dse_obj_TU;
    dse_obj_TU.build(n,m,phi,false);

    //

    bool x_first = true;

    arma::vec mux0, mu0y, u, v;
    arma::mat mu_TU;

    trame::oap_lp(dse_obj_TU,mu_TU);

    trame::oap_lp(dse_obj_TU,mu_TU,true);
    trame::oap_lp(dse_obj_TU,mu_TU,false);

    arma::mat resid_out;
    trame::oap_lp(dse_obj_TU,mu_TU,resid_out);

    trame::oap_lp(dse_obj_TU,mu_TU,true,resid_out);

    trame::oap_lp(dse_obj_TU,mu_TU,u,v);

    double val_out;

    trame::oap_lp(dse_obj_TU,mu_TU,x_first,mux0,mu0y,u,v,val_out,resid_out);

    //
    printf("\n*===================    End of oap_lp Test    ===================*\n");
    printf("\n");
    //
    
    end = std::chrono::system_clock::now();
        
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    //
    return 0;
}