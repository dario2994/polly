#ifndef SIMPLEEXTENSION_HPP
#define SIMPLEEXTENSION_HPP

#include <bits/stdc++.h>
#include "Polynomial.hpp"
using namespace std;


// TODO: Decidere se ha senso con anelli, o conviene campi
template <Polynomial <class Ring, class VarNames> MinPol>
class SimpleExtension {
	private:
		typedef Polynomial <Ring, VarNames> Pol;
		typedef SimpleExtension <MinPol> Extension;
		Pol value;
	
	public:
	// Begin constructors
		SimpleExtension <Ring, VarNames>(Pol P) {
			MinPol = P;
			value = Pol(0)
		}
	// End constructors
	
	// Begin operators
		Extension operator + (Extension const &A) const{
			
		}
		
		Extension operator * (Extension const &A) const{
			
		}
		
		bool operator == (Extension const &A) const{
			
		}
		
		bool operator != (Extension const &A) const{
			
		}
	// End operators
	
};

#endif // SIMPLEEXTENSION_HPP
