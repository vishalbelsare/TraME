/*################################################################################
  ##
  ##   Copyright (C) 2015-2018 the TraME Team:
  ##      Alfred Galichon
  ##      Keith O'Hara
  ##
  ##   This file is part of TraME.
  ##
  ##   TraME is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   TraME is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ##   You should have received a copy of the GNU General Public License
  ##   along with TraME. If not, see <http://www.gnu.org/licenses/>.
  ##
  ################################################################################*/

/*
 * arc_newton test
 *
 * Keith O'Hara
 * 02/07/2017
 *
 * This version:
 * 08/18/2017
 */

#include "trame.hpp"

int main()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    //
    // inputs:

    int nbX = 10;
    int nbY = 8;

    arma::vec n = arma::ones(nbX,1);
    arma::vec m = arma::ones(nbY,1);

    arma::mat phi = arma::randu(nbX,nbY);

    //
    // results

    printf("\n*===================   Start of arc_newton Test   ===================*\n");
    printf("\n");

    //
    // build

    trame::arums::logit logit_1(nbX,nbY), logit_2(nbY,nbX);
    
    trame::dse<trame::arums::logit,trame::arums::logit,trame::transfers::tu> dse_obj_TU;

    dse_obj_TU.build(n,m,phi,logit_1,logit_2,false);
    
    //

    double tol = 1E-06;
    int max_iter = 5000;

    arma::mat mu_TU;

    trame::arc_newton(dse_obj_TU,mu_TU);
    trame::arc_newton(dse_obj_TU,mu_TU,tol,max_iter);

    arma::cout << "arc_newton solution:\n" << mu_TU << arma::endl;

    //

    double val_out;
    arma::vec mu_x0_out, mu_0y_out;
    arma::mat U_out, V_out;

    trame::arc_newton(dse_obj_TU,mu_TU,U_out,V_out);
    trame::arc_newton(dse_obj_TU,mu_TU,mu_x0_out,mu_0y_out,U_out,V_out,val_out,tol,max_iter);

    //
    printf("\n*===================    End of arc_newton Test    ===================*\n");
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
