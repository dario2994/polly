#ifndef MONOMIAL_HPP
#define MONOMIAL_HPP

#include "utility.hpp"
#include "MonomialOrder.hpp"
using namespace std;

template <class VarNames, 
		bool (*MonomialOrder)(map <VarNames, int> const &, 
													map <VarNames, int> const &) 
		= LexicographicalOrder >
class Monomial{
	private:
		template <class VarNamesX,
							bool (*MonomialOrderX)(map <VarNamesX, int> const &, 
																		 map <VarNamesX, int> const &) > 
		friend class Monomial;
		typedef Monomial <VarNames, MonomialOrder> Mon;

		// (variable name, exponent) Exponent should always be > 0
		map <VarNames, int> exponent;   
		
	public:
		// Begin constructors
		Monomial <VarNames, MonomialOrder>() {
			exponent=map <VarNames, int>();
		}
		
		Monomial <VarNames, MonomialOrder>(VarNames x) {
			exponent[x] = 1;
		}
		
		Monomial <VarNames, MonomialOrder>(string S) {
			stringstream Sstream(S);
			readStream(Sstream);
		}
		
		template <bool (*MonOrd2)(map <VarNames, int> const &, 
															map <VarNames, int> const &)
							=LexicographicalOrder>
		Monomial <VarNames, MonomialOrder>(const Monomial <VarNames, MonOrd2> &mon) {
			for ( auto variable : mon.exponent ) {
				exponent[variable.first] = variable.second;
			}
		}
		// End constructors
		
		bool empty() const{
			return exponent.empty();
		}
		
		bool containsVariable(VarNames X) const{
			return exponent.find(X) != exponent.end();
		}
		
		template <class K> //Field K
		K evaluate(map <VarNames, K> &val) const{
			K res = K(1);
			for ( auto variable : exponent ) {
				K v = val[variable.first];
				for (int i = 0; i < variable.second; i++) res *= v;
			}
			return res;
		}
		
		// Begin operators (or similar)
		// TODO: Dovrebbe essere impostabile (ordinamento Monomiale)
		bool operator < (Mon const &A) const{
			return MonomialOrder(exponent, A.exponent);
		}
		
		bool operator > (Mon const &A) const{
			return (*this != A) and !(*this < A) ;
		}
		
		// FIXME: Cancellare il commento
		friend bool PrintCmp(Mon const &A,
													Mon const &B) {
			return A > B;					
		}
		
		Mon operator * (Mon const &A) const{
			Mon res=*this;
			for ( auto variable : A.exponent ) {
				res.exponent[variable.first] += variable.second;
			}
			return res;
		}
		
		bool dominates(Mon const &A) const{
			for ( auto variable : A.exponent ) {
				auto ExponentIter = exponent.find(variable.first);
				if ( ExponentIter == exponent.end() or 
						 variable.second > (*ExponentIter).second ) {
					return false;
				}
			}
			return true;
		}
		
		Mon operator / (Mon const &A) const{
			assert( dominates(A) ); //Forse è da togliere l'assert
			Mon res;
			for ( auto variable : exponent ) {
				VarNames x = variable.first;
				auto ExponentIter = A.exponent.find(x);
				if ( ExponentIter == A.exponent.end() ) {
					res.exponent[x] = variable.second;
				}
				else if ( variable.second > (*ExponentIter).second ) {
					res.exponent[x] = variable.second - (*ExponentIter).second;
				}
			}
			return res;
		}
		
		friend Mon lcm(Mon const &A, Mon const &B) {
			Mon res = A;
			for ( auto variable : B.exponent ) {
				VarNames x = variable.first;
				res.exponent[x] = max(res.exponent[x], variable.second);
			}
			return res;
		}
		
		bool operator == (Mon const &A) const{
			return exponent == A.exponent;
		}
		
		bool operator != (Mon const &A) const{
			return exponent != A.exponent;
		}
		//End operators
		
		// Begin input output
		bool readStream(istream &in) {
			VarNames c;
			while ( c.readStream( in ) ) {
				int e=1;
				char d;
				if ( in.get(d) ) {
					if ( d == '^' ) {
						if ( !( in >> e ) ) {
							in.unget();
						}
					}
					else if ( d == "²"[0] ) {
						if ( in.get(d) ) {
							if ( d == "²"[1] ) e = 2;
							else if ( d == "³"[1] ) e = 3;
							else {
								in.unget();
								in.unget();
							}
						}
						else {
							in.unget();
						}
					}
					else in.unget();
				}
				if ( e != 0 ) exponent[c] += e;
			}
			return true;
		}
		
		void writeStream(ostream &out, bool ImplicitOne) const{
			if ( empty() ) {
				if ( !ImplicitOne ) out << '1';
				return;
			}
			
			for( auto variable : exponent ) {
				int e = variable.second;
				if (e == 0) continue;
				
				variable.first.writeStream( out );
				
				if (e == 2) out << "²";
				if (e == 3) out << "³";
				else if ( e > 3 ) out << "^" << e;
			}
		}
		
		friend istream & operator >> (istream &in, Mon &A) {
			A.readStream(in);
			return in;
		}
		
		friend ostream&  operator << (ostream &out, Mon const &A) {
			A.writeStream(out, false);
			return out;
		}
		// End input output
};

#endif // MONOMIAL_HPP
