#ifndef POLYNOMIAL_HPP
#define POLYNOMIAL_HPP

#include "utility.hpp"
#include "Monomial.hpp"
using namespace std;

// K is the field
template <class K, class VarNames, 
				bool (*MonomialOrder)(map <VarNames, int> const &, 
															map <VarNames, int> const &) 
				= LexicographicalOrder >
class Polynomial{
	private:
		template <class KX, class VarNamesX,
							bool (*MonomialOrderX)(map <VarNamesX, int> const &, 
																		 map <VarNamesX, int> const &) > 
		friend class Polynomial;
		
		typedef Polynomial <K, VarNames, MonomialOrder> Pol;
		typedef Monomial <VarNames, MonomialOrder> Mon;
		
		map <Mon, K> coef;
		
		void removeNullCoefficients() {
			auto it = coef.begin();
			while ( it != coef.end() ) {
				if ( (*it).second == K(0) ) coef.erase( it++ );
				else it++;
			}
		}
		
	public:
		// Begin constructors
		Polynomial <K, VarNames, MonomialOrder>() {
			coef = map <Mon, K>();
		}
		
		Polynomial <K, VarNames, MonomialOrder>(Mon M) {
			coef = map <Mon, K>();
			coef[M] = 1;
		}
		
		Polynomial <K, VarNames, MonomialOrder>(K c) {
			coef = map <Mon, K>();
			if ( c != K(0) ) coef[Mon()] = c;
		}
		
		Polynomial <K, VarNames, MonomialOrder>(long long int c) : 
			Polynomial <K, VarNames, MonomialOrder>(K(c)) {}
		
		Polynomial <K, VarNames, MonomialOrder>(string S) {
			coef = map <Mon, K>();
			stringstream Sstream(S);
			assert( readStream(Sstream) );
		}
		
		template <bool (*MonOrd2)(map <VarNames, int> const &, 
															map <VarNames, int> const &)
							=LexicographicalOrder>
		Polynomial <K, VarNames, MonomialOrder>(const Polynomial <K, VarNames, MonOrd2> &P) {
			for ( auto mon : P.coef ) {
				coef[Mon(mon.first)] = mon.second;
			}
		}
		// End constructors
		
		bool empty() const{
			return coef.empty();
		}
		
		bool containsVariable(VarNames X) const{
			for ( auto mon : coef ) {
				if ( mon.first.containsVariable(X) ) return true;
			}
			return false;
		}
		
		// TODO: Potrebbe restituire un polinomio (valutando solo alcune
		// delle variabili e non tutte)
		K evaluate(map <VarNames,K> &val) const{
			K res=0;
			for( auto mon : coef ) {
				res += mon.second * mon.first.evaluate(val);
			}
			return res;
		}
		
		// Begin operators
		Pol operator + (Pol const &A) const{
			Pol res = *this;
			for ( auto mon : A.coef ) res.coef[mon.first] += mon.second; 
			
			res.removeNullCoefficients();
			return res;
		}
		
		Pol operator - (Pol const &A) const{
			Pol res = *this;
			for ( auto mon : A.coef ) res.coef[mon.first] -= mon.second; 
			
			res.removeNullCoefficients();
			return res;
		}
		
		Pol operator * (Pol const &A) const{
			Pol res;
			for ( auto mon1 : coef ) {
				for ( auto mon2 : A.coef ) {
					res.coef[mon1.first * mon2.first] += mon1.second * mon2.second;
				}
			}
			
			res.removeNullCoefficients();
			return res;
		}
		
		void operator += (Pol const &A) {
			(*this) = (*this) + A;
		}
		
		void operator -= (Pol const &A) {
			(*this) = (*this) - A;
		}
		
		void operator *= (Pol const &A) {
			(*this) = (*this) * A;
		}
		
		Pol operator * (K c) const{
			if ( c == K(0) ) return Pol();
			Pol res = *this;
			for (auto it = res.coef.begin(); it!=res.coef.end(); it++) {
				(*it).second *= c;
			}
			return res;
		}
		
		Pol operator / (K c) const{
			assert( c != K(0) );
			Pol res = *this;
			for (auto it = res.coef.begin(); it!=res.coef.end(); it++) {
				(*it).second /= c;
			}
			return res;
		}
		
		void operator *= (K c) {
			(*this) = (*this) * c;
		}
		
		void operator /= (K c) {
			(*this) = (*this) / c;
		}
		
		bool operator == (Pol const &A) const{
			return coef == A.coef;
		}
		
		bool operator != (Pol const &A) const{
			return coef != A.coef;
		}
		
		bool operator < (Pol const &A) const{
			return leadingMonomial() < A.leadingMonomial();
		}
		
		bool operator > (Pol const &A) const{
			return leadingMonomial() > A.leadingMonomial();
		}
		// End operators
		
		
		// Begin leading...
		K leadingCoefficient() const{
			if ( coef.empty() ) return K(0);
			return ( *coef.rbegin() ).second;
		}
		
		Mon leadingMonomial() const{
			if ( coef.empty() ) return Mon();
			return ( *coef.rbegin() ).first;
		}
		
		Pol leadingTerm() const{
			return Pol(leadingMonomial()) * leadingCoefficient();
		}
		// End leading...
		
		// Begin input output
		bool readStream(istream &in) {
			bool first=true;
			K c;
			bool ImplicitOne;
			
			while ( c.readStream(in , first, ImplicitOne) ) {
				first = false;
				Mon mon;
				in >> mon; // Potrebbe essere vuoto
				
				if( ImplicitOne and mon.empty() ) {
					// Vuol dire che ha praticamente letto solo il segno
					break;
				}
				
				coef[mon] += c;
			}
			
			removeNullCoefficients();
			return true;
		}
		
		void writeStream(ostream &out) const{
			bool empty = true;
			bool first = true;
			
			vector < pair <Mon, K> > PrintOrder(coef.begin(), coef.end());
			sort(PrintOrder.begin(), PrintOrder.end(), 
			[](pair <Mon, K> const &left, 
				 pair <Mon, K> const &right) {
				return PrintCmp(left.first, right.first);
			});
			
			for ( auto mon : PrintOrder ) {
				
				auto c = mon.second;
				auto U = mon.first;
				
				if ( c == K(0) ) continue;
				empty = false;
				
				if ( U.empty() ) {
					c.writeStream(out, first, false);
				}
				else {
					c.writeStream(out, first, true);
					U.writeStream(out, true); //here U is always non empty
				}
				
				first = false;
			}
			
			if ( empty ) out << "0";
		}
		
		friend istream & operator >> (istream &in, Pol &A) {
			A.readStream(in);
			return in;
		}
		
		friend ostream & operator << (ostream &out, Pol const &A) {
			A.writeStream(out);
			return out;
		}
		// End input output
		
};

template <class K, class VarNames, 
					bool (*MonomialOrder)(map <VarNames, int> const &, 
																map <VarNames, int> const &)>
Polynomial <K, VarNames, MonomialOrder> pow(Polynomial <K, VarNames, MonomialOrder> P, 
															unsigned int e) {
	if ( e == 0 ) return Polynomial <K, VarNames, MonomialOrder>("1");
	if ( e == 1 ) return P;
	
	Polynomial <K, VarNames, MonomialOrder> Q = pow(P, e / 2);
	if ( e % 2 ) return P * Q * Q;
	return Q * Q;
}

// S-polynomial
template <class K, class VarNames, 
					bool (*MonomialOrder)(map <VarNames, int> const &, 
																map <VarNames, int> const &)>
Polynomial <K, VarNames, MonomialOrder> SPolynomial(Polynomial <K, VarNames, MonomialOrder> const &P, 
																										Polynomial <K, VarNames, MonomialOrder> const &Q) {
	typedef Polynomial <K, VarNames, MonomialOrder> Pol;
	typedef Monomial <VarNames, MonomialOrder> Mon;
	Mon leadingLcm = lcm(P.leadingMonomial(), Q.leadingMonomial());
	Mon coefP = leadingLcm / P.leadingMonomial();
	Mon coefQ = leadingLcm / Q.leadingMonomial();
	
	Pol coefP2 = Pol(coefP) / P.leadingCoefficient();
	Pol coefQ2 = Pol(coefQ) / Q.leadingCoefficient();
	
	return coefP2*P-coefQ2*Q;
}

#endif // POLYNOMIAL_HPP
