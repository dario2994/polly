#ifndef FIELDS_HPP
#define FIELDS_HPP

#include "utility.hpp"
using namespace std;

template <class Ring>
class Fraction{
	private:
		typedef Fraction <Ring> Fr;
	
		Ring num, den; //If possible, in canonical form
		
		void reduceFraction() {
			Simplify(num, den);
		}
	public:
		// Begin constructors
		Fraction <Ring>() {
			num = Ring(0);
			den = Ring(1);
		}
		
		Fraction <Ring>(Ring nnum, Ring dden) {
			num = nnum;
			den = dden;
			reduceFraction();
		}
		
		Fraction <Ring>(Ring nnum) : Fraction <Ring>(nnum, Ring(1)) {}
		
		Fraction <Ring>(long long int n) : Fraction <Ring>(Ring(n)) {}
		
		Fraction <Ring>(string S) {
			stringstream Sstream(S);
			bool ImplicitOne;
			assert( readStream(Sstream, true, ImplicitOne) );
		}
		// End constructors
		
		// Begin operators
		Fr operator + (Fr const &A) const{
			return Fr((num * A.den)+(A.val * den), den * A.den);
		}
		
		Fr operator - (Fr const &A) const{
			return Fr((num * A.den)-(A.val * den), den * A.den);
		}
		
		Fr operator * (Fr const &A) const{
			return Fr(num * A.num, den * A.den);
		}
		
		Fr operator / (Fr const &A) const{
			return Fr(num * A.den, den * A.num);
		}
		
		void operator += (Fr const &A) {
			num = (num * A.den) + (A.num * den) ;
			den = den * A.den;
			reduceFraction();
		}
		
		void operator -= (Fr const &A) {
			num = (num * A.den) - (A.num * den) ;
			den = den * A.den;
			reduceFraction();
		}
		
		void operator *= (Fr const &A) {
			num = num * A.num;
			den = den * A.den;
			reduceFraction();
		}
		
		void operator /= (Fr const &A) {
			num = num * A.den;
			den = den * A.num;
			reduceFraction();
		}
		
		bool operator == (Fr const &A) const{
			return num * A.den == A.num * den;
		}
		
		bool operator != (Fr const &A) const{
			return num * A.den != A.num * den;
		}
		// End operators
		
		// Begin input output
		bool readStream(istream &in, bool ImplicitSign, bool &ImplicitOne) {
			long long int sign = 1;
			if ( !ImplicitSign ) {
				if ( !SignReadStream(in, sign) ) {
					return false;
				}
			}
			
			char c;
			bool Parenthesis = false;
			if ( !in.get(c) ) return false;
			if ( c == '(' ) {
				Parenthesis = true;
				ImplicitOne = false;
			}
			else in.unget();
			
			if ( !num.readStream(in, true, ImplicitOne) ) return false;
			num *= Ring(sign);
			
			if ( Parenthesis and ImplicitOne ) return false;
			else if (ImplicitOne ) {
				den = Ring(1);
				return true;
			}
			else if ( Parenthesis ) {
				if ( !in.get(c) ) return false;
				else if ( c != ')' ) return false;
			}
			
			if ( !in.get(c) ) {
				den = Ring(1);
				return true;
			}
			else if ( c == '/' ) {
				Parenthesis = false;
				if ( !in.get(c) ) return false;
				else if ( c == '(' ) Parenthesis = true;
				else in.unget();
			
				if ( !den.readStream(in, true, ImplicitOne) ) return false;
				if ( ImplicitOne ) return false;
				
				if ( Parenthesis ) {
					if ( !in.get(c) ) return false;
					else if ( c != ')' ) return false;
				}
				
				reduceFraction();
				return true;
			}
			else {
				in.unget();
				den = Ring(1);
				return true;
			}
		}
		
		void writeStream(ostream &out, bool ImplicitSign, bool ImplicitOne) const{
			if ( den != Ring(1) ) {
				num.writeStream(out, ImplicitSign, false);
				out << '/';
				den.writeStream(out, true, false);
			}
			else num.writeStream(out, ImplicitSign, ImplicitOne);
		}
		
		friend istream & operator >> (istream &in , Fr &A) {
			bool ImplicitOne;
			A.readStream(in, true, ImplicitOne);
			return in;
		}
		
		friend ostream & operator << (ostream &out , Fr const &A) {
			A.writeStream(out, true, false);
			return out;
		}
		//End input output
};

#endif // FIELDS_HPP
