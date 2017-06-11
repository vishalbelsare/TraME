/*################################################################################
  ##
  ##   Copyright (C) 2015 - 2017 the TraME Team:
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
 * Linearly Transferable Utility (LTU) transfers class
 *
 * Keith O'Hara
 * 08/15/2016
 *
 * This version:
 * 06/03/2017
 */

#ifndef _trame_transfers_ltu_HPP
#define _trame_transfers_ltu_HPP

// some functions are const restricted because of calls to const market pointers in equilibrium solvers

class ltu
{
    public:
        // build objects
        bool need_norm;

        int transfers_type = 1;
        int nbX;
        int nbY;
        int dim_params;

        arma::mat lambda;
        arma::mat phi;

        arma::mat aux_zeta; // could be 1 - lambda_inp;
        arma::mat aux_phi_exp; // aux_phi_exp = exp(phi); will probably end up as exp(phi / sigma) when using MFE; also labelled K

        // member functions
        ~ltu(){};
         ltu(){};

        void build(const arma::mat& lambda_inp, const arma::mat& phi_inp, bool need_norm_inp);

        void trans();

        void gen_mmf(mmfs::cd& mmf_obj) const; // generate an MMF object from transfers
        mmfs::cd gen_mmf() const;

        //
        // DSE-related functions
        arma::mat Psi(const arma::mat& U, const arma::mat& V) const;
        arma::mat Psi(const arma::mat& U, const arma::mat& V, const arma::uvec* xs, const arma::uvec* ys) const;
        arma::mat Psi(const double& U, const arma::mat& V, const arma::uvec* xs, const arma::uvec* ys) const;
        arma::mat Psi(const arma::mat& U, const double& V, const arma::uvec* xs, const arma::uvec* ys) const;
        double Psi(const double& U, const double& V, int x_ind, int y_ind) const;

        arma::mat du_Psi(const arma::mat& U, const arma::mat& V) const;
        arma::mat du_Psi(const arma::mat& U, const arma::mat& V, const arma::uvec* xs, const arma::uvec* ys) const;
        arma::mat du_Psi(const double& U, const arma::mat& V, const arma::uvec* xs, const arma::uvec* ys) const;
        arma::mat du_Psi(const arma::mat& U, const double& V, const arma::uvec* xs, const arma::uvec* ys) const;

        arma::mat dparams_Psi(const arma::mat& U, const arma::mat& V, const arma::mat& dparams) const;
        arma::mat dparams_Psi(const arma::mat& U, const arma::mat& V, const arma::mat* dparams) const;

        arma::mat Ucal(const arma::mat& vs) const;
        arma::mat Ucal(const arma::mat& vs, const arma::uvec* xs, const arma::uvec* ys) const;
        double Ucal(const double& vs, int xs, int ys) const;
        arma::mat Vcal(const arma::mat& us) const;
        arma::mat Vcal(const arma::mat& us, const arma::uvec* xs, const arma::uvec* ys) const;
        double Vcal(const double& us, int xs, int ys) const;

        arma::mat UW(const arma::mat& Ws) const;
        arma::mat UW(const arma::mat& Ws, const arma::uvec* xs, const arma::uvec* ys) const;
        double UW(const double& Ws, int x_ind, int y_ind) const;
        arma::mat VW(const arma::mat& Ws) const;
        arma::mat VW(const arma::mat& Ws, const arma::uvec* xs, const arma::uvec* ys) const;
        double VW(const double& Ws, int x_ind, int y_ind) const;

        arma::mat dw_UW(const arma::mat& Ws) const;
        arma::mat dw_UW(const arma::mat& Ws, const arma::uvec* xs, const arma::uvec* ys) const;
        arma::mat dw_VW(const arma::mat& Ws) const;
        arma::mat dw_VW(const arma::mat& Ws, const arma::uvec* xs, const arma::uvec* ys) const;

        arma::mat WU(const arma::mat& Us);
        arma::mat WU(const arma::mat& Us, const arma::uvec* xs, const arma::uvec* ys);
        arma::mat WV(const arma::mat& Vs);
        arma::mat WV(const arma::mat& Vs, const arma::uvec* xs, const arma::uvec* ys);
};

#endif
