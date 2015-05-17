// Stile:
// Spazi ovunque, tranne che nelle chiamate a funzione. 
// Un'eccezione è: 'if ( true ) { ...' può diventare 'if (true) { ...'
// Le cose da fare sono segnate con TODO (o FIXME)

// #define POLLY_DEBUG 1

#include "utility.hpp"

#include "VarNames.hpp"
#include "Rings.hpp"
#include "Fields.hpp"

#include "Grobner.hpp"
using namespace std;

int main() {
	typedef SingleChar SC;
	typedef Monomial <SC, LexicographicalOrder> Mon;
	typedef Polynomial <Fraction <Integer>, SC, LexicographicalOrder> Pol;
	typedef PolynomialIdeal <Fraction <Integer>, SC, LexicographicalOrder> Ideal;
	
	
	Ideal I = Ideal({"xy+z+3w", "yz+2x-3w²", "zx-y+4w"});
	
	// Ideal I = Ideal({"xyz^6+4x³y³", "7/2x²+1/3z^5y", "xyz+x²y²z²", "x^8yz³-11x³"});
	I.reduceGrobnerBasis();
	I.normalizeBasis();
	cout << I << "\n";
}
