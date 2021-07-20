#ifndef PROVER_TOOM_H_
#define PROVER_TOOM_H_

#include "../global.h"
#include "func_pro.h"
#include "func_ver.h"
#include "Functions.h"
NTL_CLIENT

class Prover_toom
{
private:
	int n, m;		 //Userinput, defines the dimensions of the vectors used, N = n*m
	int omega_sw;	 //windowsize for sliding-window technique
	int omega_LL;	 //windowsize for multi-expo technique by Lim and Lee
	int omega_mulex; //windowsize for multi-expo technique
	int mu;
	int m_r;
	int mu_h;
	double time_di;	 //time to calculate the Di's
	ofstream ost;
	ZZ ord;
	array<string, 6> hashStr;
	stringstream pk_ss;
	SHA256 sha;

	vector<vector<ZZ>*>* A;			   //Matrix containing the numbers 1 to N after the permutation
	vector<vector<vector<int>*>*>* pi;  // Matrix containing the  permutation
	vector<vector<Cipher_elg>*>* C;	   //Contains the reencryptetd ciphers
	vector<vector<Cipher_elg>*>* C_small; //smaller matrix of ciphertexts constructed for interaction
	vector<vector<ZZ>*>* R;			   //Random elements from the reencryption
	vector<vector<ZZ>*>* R_small;		   //matrix constructed for interaction

	vector<vector<ZZ>*>* B;				   //Matrix of permuted Vandermond challenges, generated out of challenges x2;
	vector<vector<ZZ>*>* B_small;			   //matrix constructed for interaction
	vector<vector<vector<long>*>*>* basis_B; //Matrix containing the basis_vec for multi-expo.
	ZZ chal_y4;								   //random challenge from round 4
	ZZ chal_z4;								   //random challenge element from round 4,
	vector<ZZ>* chal_x6;					   //Vector of challenges, output of round 6
	vector<ZZ>* chal_y6;					   //Vector of challenges, output of round 6
	vector<ZZ>* chal_x8;					   //Vector of Vandermonde challenges, output of round 8
	vector<ZZ>* x;							   //challenges for reduction m=64

	vector<ZZ>* r_A;				  //random elements to generate the commitments for A
	ZZ r_D0;						  //random element to generate the commitment to first row in D
	vector<ZZ>* B_0;				  //Vector containing random exponents B_0i
	vector<vector<long>*>* basis_B0; // contains the basis_vec for multi-expo
	vector<ZZ>* r_B;				  //Random elements to commit to rows in B
	vector<ZZ>* r_B_small;			  //Random elements to commit to rows in T_small
	ZZ r_B0;						  //random element to commit to B_0
	vector<Mod_p>* c_B;				  //Vector containing the commitments B_i to the values in B
	Mod_p c_B0;						  //commitment to B_0
	vector<Mod_p>* c_a;				  //Vector containing the commitments to values used for the reencryption in 5
	vector<ZZ>* r_a;				  //Vector containing the random elements used for a
	vector<ZZ>* a;					  //vector containing exponents for reencryption
	vector<Mod_p>* c_A;				  //Commitments from round 1 to the values of A
	Mod_p c_D0;						  //commitment to D_0
	vector<Cipher_elg>* E;			  //vector of the products of the diogonals of Y^T generated in round 5
	ZZ R_b;							  //contains the negative sum of B_ij*R_ij
	vector<ZZ>* rho_a;				  //contains random elements used for the reencryption in 5

	vector<Cipher_elg>* C_c; //Ciphertexts to prove correctness of reduction
	vector<Mod_p>* c_a_c;	 //vector containing the commitments to value used for the reencryption of C_c
	vector<ZZ>* a_c;		 //vector containing the exponents
	vector<ZZ>* r_c;		 //vector of random elements to commit to a_c
	vector<ZZ>* rho_c;		 //contains random elements used for the reencryption

	vector<vector<ZZ>*>* D;   //Matrix containing the values y*A_ij +Bij - z
	vector<vector<ZZ>*>* D_h; //Vector of the Hadamar products D_h_i = A_1¡...¡A_i of the rows of (A_ij-z)
	vector<vector<ZZ>*>* D_s; //Vector of the shifted Hadamar proucts in D_h
	vector<ZZ>* d;			   //containing random elements to proof product of D_hm
	vector<ZZ>* Delta;		   //containing random elements to proof product of D_hm
	vector<ZZ>* d_h;		   //containing random products w_j=prod_i=1^j D_hm[i]

	ZZ r_z;			   // random element to commit to vector z
	vector<ZZ>* r_D_h; // vector of random elements for commitments to D_h;
	ZZ r_Dm;		   //random element for commitment to last row in D;
	ZZ r_d;			   //random element for commitment to d
	ZZ r_Delta;		   //random element for commitment to Delta
	ZZ r_d_h;		   //random element for commitment to d_h

	Mod_p c_z;			  // commitment to vector of z's
	vector<Mod_p>* c_D_h; //  commitments to D_h;
	Mod_p c_Dm;			  // commitment to last row in D, D=D_h*t_1;
	Mod_p c_d;			  //commitment to vector d
	Mod_p c_Delta;		  //commitment to vector Delta
	Mod_p c_d_h;		  // commitment to vector d_h

	vector<ZZ>* Dl;		 //vector containing the sums of bilinear maps of rows of Y and U
	vector<ZZ>* r_Dl;	 // vector of random elements  for the commitments  to D_l;
	vector<Mod_p>* c_Dl; // commitments to the values D_l

	vector<ZZ>* D_h_bar;   //Sum over the row in D_h multiplied by chal^i
	ZZ r_Dh_bar;		   // sum over the random elements used for commitments to D_h
	vector<ZZ>* d_bar;	   // chal_x8*D_h(m-1) +d
	vector<ZZ>* Delta_bar; //chal_x8*d_h+Delta
	ZZ r_d_bar;			   //chal_x8*r_Dh(m-1)+r_d
	ZZ r_Delta_bar;		   //chal_x8*r_dh +r_Delta

	ZZ a_c_bar;		   //sum over elements to reencrypt E_low_up
	ZZ r_ac_bar;	   // sum over random elements
	vector<ZZ>* B_bar; // sum over the rows in B multiplied by chal^i
	ZZ a_bar;		   //sum over the elements in a times chal^i
	ZZ r_a_bar;		   // sum over random elements used for commitments to a
	ZZ r_B_bar;		   //sum over the random elements used for commitments to B
	ZZ rho_bar;		   //sum over random elements rho_a
	ZZ rho_c_bar;	   //sum over random elements rho_c

	vector<ZZ>* A_bar;	 //sum over the row in A times the challenges
	vector<ZZ>* D_s_bar; //sum over the rows in D_S_bar times the challenges

	ZZ r_A_bar;	 //sum over the random elements in r_A times the challenges
	ZZ r_Ds_bar; //sum over the random elements in r_DS times the challenges
	ZZ r_Dl_bar; //sum over the random elements in r_Dl times the challenges

	ZZ Sigma_C; //sum over the elements C times the challenges

	long omega;							   //window size for multi-exponentiation technique
	vector<vector<long>*>* basis_chal_x8; //Vector of basis_vec for multi-expo
	vector<ZZ>* mul_chal_x8;			   //Vector of basis_vec for multi-expo

public:
	Prover_toom();
	Prover_toom(vector<vector<Cipher_elg>*>* E, vector<vector<ZZ>*>* R, vector<vector<vector<int>*>*>* pi, vector<int> num, string code);
	virtual ~Prover_toom();

	//round_1 calculates and returns the commitment to the row in Y
	void round_1();
	//round_3 calculates and returns the commitment to permuted exponents s_1(i)*s_2(j)
	void round_3();
	//round_5a calculates the commitments to the vectors h, W, and C,c and returns them
	void round_5a();
	//round_5, combines the round 5a and 5b
	void round_5();
	//calculates the first set of extra Elements for the reduction loop
	// string round_5_red(string name);
	//last reduction from m=16 to m=4, after reduction loop, calls also 5a
	// string round_5_red1(string name);
	//round_5_opt_red, combines the round 5a and 5b_red
	//	string round_5_red2(string name );
	//round_7a calculates the commitments to the vectors C and c
	void round_7a();
	// void round_7c_red();
	//round_7 reads the values in and writes them, and combines 7a-7c
	void round_7();
	// string round_7_red();
	void round_9a();
	void round_9();
	int prove(array<string, 2> codes, string fileName);

	void commit_ac();
	void calculate_Cc(vector<vector<Cipher_elg>*>* C, vector<vector<vector<long>*>*>* B);
	void calculate_Cc(vector<vector<Cipher_elg>*>* C, vector<vector<ZZ>*>* B);
	void calculate_ac_bar(vector<ZZ>* x);
	void calculate_r_ac_bar(vector<ZZ>* x);
	void reduce_C(vector<vector<Cipher_elg>*>* C, vector<vector<ZZ>*>* B, vector<ZZ>* r_B, vector<ZZ>* x, long length);
	void set_Rb1(vector<ZZ>* x);

	vector<Cipher_elg>* calculate_e();
	void calculate_E(vector<Cipher_elg>* d);

	vector<vector<Cipher_elg>*>* copy_C();
	vector<vector<ZZ>*>* copy_B();
	vector<ZZ>* copy_r_B();

	static vector<vector<ZZ>*>* evulation(vector<vector<ZZ>*>* p);
	static vector<vector<vector<ZZ>*>*>* evulation_pow(vector<vector<Cipher_elg>*>* p);
	static vector<vector<vector<ZZ>*>*>* point_pow(vector<vector<vector<ZZ>*>*>* p, vector<vector<ZZ>*>* q);
	static vector<vector<ZZ>*>* mult_points(vector<vector<vector<ZZ>*>*>* points);
	static vector<Cipher_elg>* toom4_pow(vector<vector<Cipher_elg>*>* p, vector<vector<ZZ>*>* q);
	static vector<ZZ>* interpolation_pow(vector<ZZ>* points);
};

#endif /* PROVER_TOOM_H_ */
