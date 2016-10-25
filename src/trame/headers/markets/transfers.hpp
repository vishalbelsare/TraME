/*################################################################################
  ##
  ##   Copyright (C) 2015 - 2016 the TraME Team:
  ##      Alfred Galichon
  ##      Keith O'Hara
  ##      Simon Weber
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
 * transfers class
 *
 * Keith O'Hara
 * 08/16/2016
 *
 * This version:
 * 10/23/2016
 */

class transfers
{
    public:
        // build objects
        bool ETU = false;
        bool LTU = false;
        bool NTU = false;
        bool TU  = false;

        int transfers_type;

        int nbX;
        int nbY;
        int nbParams;

        arma::mat alpha;
        arma::mat gamma;
        arma::mat lambda;
        arma::mat tau;

        arma::mat phi;

        // member functions
        void build_ETU(const arma::mat& alpha_ETU, const arma::mat& gamma_ETU, const arma::mat& tau_ETU);
        void build_LTU(const arma::mat& lambda_LTU, const arma::mat& phi_LTU);
        void build_NTU(const arma::mat& alpha_NTU, const arma::mat& gamma_NTU);
        void build_TU(const arma::mat& phi_TU);

        void trans();

        // Psi functuons are const restricted because of calls to const market pointers in solvers
        arma::mat Psi(const arma::mat& U, const arma::mat& V) const;
        arma::mat Psi(const arma::mat& U, const arma::mat& V, arma::uvec* xs, arma::uvec* ys) const;
        arma::mat Psi(double U, const arma::mat& V, arma::uvec* xs, arma::uvec* ys) const;
        arma::mat Psi(const arma::mat& U, double V, arma::uvec* xs, arma::uvec* ys) const;
        double Psi(double U, double V, int x_ind, int y_ind) const;

        arma::mat du_Psi(const arma::mat& U, const arma::mat& V, arma::uvec* xs, arma::uvec* ys);
        arma::mat du_Psi(double U, const arma::mat& V, arma::uvec* xs, arma::uvec* ys);
        arma::mat du_Psi(const arma::mat& U, double V, arma::uvec* xs, arma::uvec* ys);

        arma::mat dtheta_Psi(const arma::mat& U, const arma::mat& V, arma::mat* dtheta);

        arma::mat Ucal(const arma::mat& vs, arma::uvec* xs, arma::uvec* ys);
        double Ucal(double vs, int xs, int ys);
        arma::mat Vcal(const arma::mat& us, arma::uvec* xs, arma::uvec* ys);
        double Vcal(double us, int xs, int ys);

        arma::mat UW(const arma::mat& Ws);
        arma::mat UW(const arma::mat& Ws, arma::uvec* xs, arma::uvec* ys);
        double UW(double Ws, int x_ind, int y_ind);
        arma::mat VW(const arma::mat& Ws);
        arma::mat VW(const arma::mat& Ws, arma::uvec* xs, arma::uvec* ys);
        double VW(double Ws, int x_ind, int y_ind);

        arma::mat du_UW(const arma::mat& Ws, arma::uvec* xs, arma::uvec* ys);
        arma::mat du_VW(const arma::mat& Ws, arma::uvec* xs, arma::uvec* ys);

        arma::mat WU(const arma::mat& Us);
        arma::mat WU(const arma::mat& Us, arma::uvec* xs, arma::uvec* ys);
        arma::mat WV(const arma::mat& Vs);
        arma::mat WV(const arma::mat& Vs, arma::uvec* xs, arma::uvec* ys);

    private:
        arma::mat aux_exp_alphaovertau;
        arma::mat aux_exp_gammaovertau;

        arma::mat aux_zeta;
};
