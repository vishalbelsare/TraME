/*################################################################################
  ##
  ##   Copyright (C) 2015 - 2016 Alfred Galichon and the TraME Team
  ##
  ##   This file is part of the R package TraME.
  ##
  ##   The R package TraME is free software: you can redistribute it and/or modify
  ##   it under the terms of the GNU General Public License as published by
  ##   the Free Software Foundation, either version 2 of the License, or
  ##   (at your option) any later version.
  ##
  ##   The R package TraME is distributed in the hope that it will be useful,
  ##   but WITHOUT ANY WARRANTY; without even the implied warranty of
  ##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  ##   GNU General Public License for more details.
  ##
  ##   You should have received a copy of the GNU General Public License
  ##   along with TraME. If not, see <http://www.gnu.org/licenses/>.
  ##
  ################################################################################*/

// MMF class

class MMF
{
    public:
        // build objects
        bool ETU = false;
        bool LTU = false;
        bool NTU = false;
        bool TU  = false;

        bool need_norm = false;

        arma::vec n;
        arma::vec m;

        double kappa;
        double lambda;

        arma::mat A;
        arma::mat B;
        arma::mat C;
        arma::mat D;
        arma::mat K;

        // member functions

    private:
        arma::mat aux_log_C;
        arma::mat aux_log_D;
}

void MMF::build(int n_ETU, int m_ETU, arma::mat C_ETU, arma::mat D_ETU, double kappa_ETU, bool need_norm_ETU)
{
    n = n_ETU;
    m = m_ETU;

    C = C_ETU;
    D = D_ETU;

    aux_log_C = arma::log(C);
    aux_log_D = arma::log(D);

    kappa = kappa_ETU;
    
    need_norm = need_norm_ETU;

    ETU = true;
}

void MMF::build(int n_LTU, int m_LTU, arma::mat K_LTU, double lambda_LTU, bool need_norm_LTU)
{
    n = n_LTU;
    m = m_LTU;
    K = K_LTU;

    lambda = lambda_LTU;
    
    need_norm = need_norm_LTU;

    LTU = true;
}

void MMF::build(int n_NTU, int m_NTU, arma::mat A_NTU, arma::mat B_NTU, bool need_norm_NTU)
{
    n = n_NTU;
    m = m_NTU;
    
    A = A_NTU;
    B = B_NTU;
    
    need_norm = need_norm_NTU;

    NTU = true;
}

void MMF::build(int n_TU, int m_TU, arma::mat K_TU, bool need_norm_TU)
{
    n = n_TU;
    m = m_TU;
    K = K_TU;

    need_norm = need_norm_TU;

    TU = true;
}

arma::mat MMF::M(arma::mat a_xs, arma::mat b_ys, arma::uvec* xs, arma::uvec* ys)
{
    if (!xs) {
        *xs = uvec_linspace(0,n.n_elem);
    }
    if (!ys) {
        *ys = uvec_linspace(0,m.n_elem);
    }
    //
    if (ETU) {
        arma::mat term_1 = arma::exp(aux_log_C(xs,ys) + kappa(xs,ys) % arma::log(a_xs));
        arma::mat term_2 = arma::exp(aux_log_D(xs,ys) + arma::trans(arma::trans(kappa(xs,ys)) % arma::log(b_ys)));

        arma::mat ret = arma::exp((1/kappa(xs,ys)) % arma::log((term_1+term_2)/2)); 
    }
    //
    if (LTU) {
        arma::mat term_1 = arma::exp(lambda(xs,ys) % arma::log(a_xs));
        arma::mat term_2 = arma::exp(arma::trans(aux_zeta(xs,ys)) % arma::log(b_ys));
        arma::mat term_3 = K(xs,ys);

        arma::mat ret = term_1 % term_2 % term_3;
    }
    //
    if (NTU) {
        arma::mat term_1 = a_xs % A(xs,ys);
        arma::mat term_2 = arma::trans(b_ys % arma::trans(A(xs,ys)));

        arma::mat ret = arma::min(term_1, term_2);
    }
    //
    if (TU) {
        arma::mat term_1 = K(xs,ys);
        arma::mat term_2 = arma::sqrt(a_xs * b_ys.t());

        arma::mat ret = term_1 % term_2;
    }
    //
    return ret;
}

arma::mat MMF::M(double a_xs, arma::mat b_ys, arma::uvec* xs, arma::uvec* ys)
{
    if (!xs) {
        *xs = uvec_linspace(0,n.n_elem);
    }
    if (!ys) {
        *ys = uvec_linspace(0,m.n_elem);
    }
    //
    if (ETU) {
        arma::mat term_1 = arma::exp(aux_log_C(xs,ys) + kappa(xs,ys) * std::log(a_xs));
        arma::mat term_2 = arma::exp(aux_log_D(xs,ys) + arma::trans(arma::trans(kappa(xs,ys)) % arma::log(b_ys)));

        arma::mat ret = arma::exp((1/kappa(xs,ys)) % arma::log((term_1+term_2)/2)); 
    }
    //
    if (LTU) {
        arma::mat term_1 = arma::exp(lambda(xs,ys) * std::log(a_xs));
        arma::mat term_2 = arma::exp(arma::trans(aux_zeta(xs,ys)) % arma::log(b_ys));
        arma::mat term_3 = K(xs,ys);

        arma::mat ret = term_1 % term_2 % term_3;
    }
    //
    if (NTU) {
        arma::mat term_1 = a_xs * A(xs,ys);
        arma::mat term_2 = arma::trans(b_ys % arma::trans(A(xs,ys)));

        arma::mat ret = arma::min(term_1, term_2);
    }
    //
    if (TU) {
        arma::mat term_1 = K(xs,ys);
        arma::mat term_2 = arma::sqrt(a_xs * b_ys.t());

        arma::mat ret = term_1 % term_2;
    }
    //
    return ret;
}

arma::mat MMF::M(arma::mat a_xs, double b_ys, arma::uvec* xs, arma::uvec* ys)
{
    if (!xs) {
        *xs = uvec_linspace(0,n.n_elem);
    }
    if (!ys) {
        *ys = uvec_linspace(0,m.n_elem);
    }
    //
    if (ETU) {
        arma::mat term_1 = arma::exp(aux_log_C(xs,ys) + kappa(xs,ys) % arma::log(a_xs));
        arma::mat term_2 = arma::exp(aux_log_D(xs,ys) + arma::trans(arma::trans(kappa(xs,ys)) % arma::log(b_ys)));

        arma::mat ret = arma::exp((1/kappa(xs,ys)) % arma::log((term_1+term_2)/2)); 
    }
    //
    if (LTU) {
        arma::mat term_1 = arma::exp(lambda(xs,ys) % arma::log(a_xs));
        arma::mat term_2 = arma::exp(arma::trans(aux_zeta(xs,ys)) % std::log(b_ys));
        arma::mat term_3 = K(xs,ys);

        arma::mat ret = term_1 % term_2 % term_3;
    }
    //
    if (NTU) {
        arma::mat term_1 = a_xs % A(xs,ys);
        arma::mat term_2 = arma::trans(b_ys * arma::trans(A(xs,ys)));

        arma::mat ret = arma::min(term_1, term_2);
    }
    //
    if (TU) {
        arma::mat term_1 = K(xs,ys);
        arma::mat term_2 = arma::sqrt(a_xs * b_ys);

        arma::mat ret = term_1 % term_2;
    }
    //
    return ret;
}

arma::vec MMF::marg_x_inv(arma::uvec* xs, arma::mat B_ys)
{
    if (!xs) {
        *xs = uvec_linspace(0,n.n_elem);
    }
    //
    arma::vec the_a_xs = invPWA(n.elem(xs),arma::trans(arma::trans(B.rows(xs)/A.rows(xs)) % B_ys),A.rows(xs),1);
    //
    return the_a_xs;
}

double MMF::marg_y_inv_fn(double z, const trame_zeroin_data& opt_data)
{
    if (coeff) {
        term_1 = z;
    } else {
        term_1 = 0;
    }

    double ret = term_1 + m_y + arma::accu(M(A_xs,z,));
    //
    return ret;
}

arma::vec MMF::marg_y_inv(arma::uvec* ys, arma::mat A_xs)
{
    if (NTU) {
        if (!ys) {
            *ys = uvec_linspace(0,m.n_elem);
        }
        //
        arma::vec the_a_xs = invPWA(m.elem(ys),arma::trans(arma::trans(A.cols(ys)/B.cols(ys)) % A_xs),B.cols(ys),1);
        //
        return the_a_xs;
    } else {
        int nb_Y = m.n_elem;

        double coeff;
        arma::vec ubs(nb_Y);

        if (need_norm) {
            coeff = 0.0;
            ubs.fill(1E10);
        } else {
            coeff = 1.0;
            ubs = m;
        }
        //
        if (!ys) {
            *ys = uvec_linspace(0,m.n_elem);
        }

        the_b_ys(ys.n_elem);
        //
        int j, y;
        for (j=0; j < ys.n_elem; j++) {
            y = ys(j);
            the_b_ys(j) = zeroin(0.0, ubs(y), marg_y_inv_fn, root_data, tol_zero, max_iter);
        }
    }
}

double MMF::zeroin(double ax, double bx, double (*f)(double x, const trame_zeroin_data& opt_data), const trame_zeroin_data& zeroin_data, double tol, int max_iter)
{
	double a,b,c;
	double fa;
	double fb;
	double fc;
		
	a = ax;  b = bx;  fa = (*f)(a,zeroin_data);  fb = (*f)(b,zeroin_data);
	c = a;   fc = fa;
	
	// check endpoints
	if(fa == 0.0){
		return b;
	}
	if(fb == 0.0){
		return b;
	}
	
	// otherwise begin iterations
	int iter = 0;
	
	double eps_temp = std::numeric_limits<double>::epsilon();
	double tol_act = 2*eps_temp*fabs(b) + tol/2;

	double p, q, prev_step, new_step;
	//register double t1,cb,t2;
	double t1,cb,t2; // Keith: register is deprecated as of C++-11
	
	new_step = (c - b)/2;
	
	while(fabs(new_step) > tol_act && iter < max_iter){
		iter++;
		prev_step = b-a;
			
		if( fabs(fc) < fabs(fb) ){
			a = b;  b = c;  c = a;
			fa=fb;  fb=fc;  fc=fa;
		}
		
		new_step = (c-b)/2;

		if( fabs(prev_step) >= tol_act	&& fabs(fa) > fabs(fb) ){
			
			cb = c - b;
						
			if( a==c ){
				t1 = fb/fa;
				p = cb*t1;
				q = 1.0 - t1;
			}else{
				q = fa/fc;  t1 = fb/fc;  t2 = fb/fa;
				p = t2 * ( cb*q*(q-t1) - (b-a)*(t1-1.0) );
				q = (q-1.0) * (t1-1.0) * (t2-1.0);
			}
						
			if( p > 0.0 ){
				q = -q;
			}else{
				p = -p;
			}

			if( p < (0.75*cb*q-fabs(tol_act*q)/2) && p < fabs(prev_step*q/2) ){
				new_step = p/q;
			}
		}
				
		if( fabs(new_step) < tol_act ){
			if( new_step > 0.0 ){
				new_step = tol_act;
			}else{
				new_step = -tol_act;
			}
		}
						
		a = b;  fa = fb;
		b += new_step;  fb = (*f)(b,zeroin_data);
		if( (fb > 0.0 && fc > 0.0) || (fb < 0.0 && fc < 0.0) ){
			c = a;  fc = fa;
		}
	}
	//
	return b;
}
