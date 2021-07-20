#ifndef CIPHER_ELG_H_
#define CIPHER_ELG_H_
#include "base.h"
#include "Mod_p.h"
#include "G_q.h"
NTL_CLIENT

class Cipher_elg {
private:
	ZZ u, v; //u,v represent u=g^r, v = m¡Áy^r , where r is random, y is the public key and m the message
	ZZ mod; // modular values of the calculations in the cyclic group
public:
	//Constructors & Destructor
	Cipher_elg();
	Cipher_elg(long u_val, long v_val, long mod);
	Cipher_elg(ZZ u_val, long v_val, long mod);
	Cipher_elg(long u_val, ZZ v_val, long mod);
	Cipher_elg(ZZ u_val, ZZ v_val, long mod);
	Cipher_elg(long u_val, long v_val, ZZ mod);
	Cipher_elg(ZZ u_val, long v_val, ZZ mod);
	Cipher_elg(long u_val, ZZ v_val, ZZ mod);
	Cipher_elg(ZZ u_val, ZZ v_val, ZZ mod);
	Cipher_elg(Mod_p u_t, Mod_p v_t);
	virtual ~Cipher_elg();

	//Access to the parameters
	ZZ get_u() const;
	ZZ get_v() const;
	ZZ get_mod() const;

	//Operators functionality
	void operator =(const Cipher_elg& c);
	Cipher_elg operator *(const Cipher_elg& el)const;
	Cipher_elg operator *(const int el)const;
	bool operator ==(const Cipher_elg& b) const;

	static void mult(Cipher_elg& a, const Cipher_elg& b, const Cipher_elg& c);
	static Cipher_elg expo(const Cipher_elg& el, const ZZ expo);
	static Cipher_elg expo(const Cipher_elg& el, const int expo);
	static void expo(Cipher_elg& e, const Cipher_elg& el, const ZZ expo);
	static void expo(Cipher_elg& e, const Cipher_elg& el, const long expo);
	static Cipher_elg inverse(const Cipher_elg& el);
	static void inverse(Cipher_elg& e, const Cipher_elg& el);

	//Output and Input
	friend ostream& operator<<(ostream& os, const Cipher_elg el);
	friend istream& operator >>(istream& is, Cipher_elg& el);
	void toCipher(string s);

};

#endif /* CIPHER_ELG_H_ */
