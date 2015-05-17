#ifndef GROBNER_HPP
#define GROBNER_HPP

#include "utility.hpp"
#include "Polynomial.hpp"
using namespace std;

template <class K, class VarNames,
		bool (*MonomialOrder)(map <VarNames, int> const &, 
													map <VarNames, int> const &) 
		= LexicographicalOrder >
class PolynomialIdeal{
	private:
		typedef Monomial <VarNames, MonomialOrder> Mon;
		typedef Polynomial <K, VarNames, MonomialOrder> Pol;
		typedef PolynomialIdeal <K, VarNames, MonomialOrder> Ideal;
		
		bool IsGrobnerBasis, IsMinimized, IsReduced;
		vector <Pol> generators;
	public:
		// Begin constructors
		PolynomialIdeal <K, VarNames, MonomialOrder>() {
			IsGrobnerBasis = IsMinimized = IsReduced = true;
			generators = vector <Pol>();
		}
		
		PolynomialIdeal <K, VarNames, MonomialOrder>(vector <Pol> gen) {
			IsGrobnerBasis = IsMinimized = IsReduced = false;
			generators = gen;
		}
		
		PolynomialIdeal <K, VarNames, MonomialOrder>(vector <string> gen) {
			IsGrobnerBasis = IsMinimized = IsReduced = false;
			for ( string S : gen ) generators.push_back(Pol(S));
		}
		// End Constructors
		
		bool empty() const{
			return generators.empty();
		}
		
		vector <Pol> idealGenerators() {
			return generators;
		}
		
		// Begin basis modificators
		void removeNullPolynomials() {
			generators.erase( 
				remove( generators.begin(), generators.end(), Pol(0) ),
				generators.end() 
			);
		}
		
		void normalizeBasis() {
			for (unsigned int i = 0; i < generators.size(); i++) {
				generators[i] /= generators[i].leadingCoefficient();
			}
			sort(generators.begin(), generators.end());
		}
		// End basis modificators
		
		
		// Begin Grobner
		// Multivariate division for the generators
		Pol divisionRemainder(Pol P) const{
			MP_WRITE("divisionRemainder");
			Pol remainder = Pol(0);
			MP_WRITE("Polynomial = " << P << "\nStarting division:");
			while ( !P.empty() ) {
				MP_WRITE("\t" << P);
				bool divided = false;
				Mon PleadMon = P.leadingMonomial();
				K PleadCoef = P.leadingCoefficient();
				
				for ( Pol Q : generators ) {
					Mon QleadMon = Q.leadingMonomial();
					K QleadCoef = Q.leadingCoefficient();
					
					if ( PleadMon.dominates(QleadMon) ) {
						Mon DivisionMon = PleadMon / QleadMon;
						K DivisionCoef = PleadCoef / QleadCoef;
						P -= Q * Pol(DivisionMon) * DivisionCoef;
						divided = true;
						break;
					}
					
				}
				if ( divided ) continue;
				
				Pol NewRemainder = P.leadingTerm();
				remainder += NewRemainder;
				P -= NewRemainder;
			}
			MP_WRITE("Remainder = " << remainder << "\n");
			return remainder;
		}
		
		void BuchbergerAlgorithm(unsigned int i0) {
			MP_WRITE("Generators:");
			for ( auto P : generators ) MP_WRITE("\t" << P);
			
			MP_WRITE("S-Polynomials:");
			for (unsigned int i = i0; i < generators.size(); i++) {
				for (unsigned int j = 0; j < i; j++) {
					Mon LeadMonP = generators[i].leadingMonomial();
					Mon LeadMonQ = generators[j].leadingMonomial();
					
					// If leading monomial are coprime, than it is not necessary
					// to check the S-polynomial
					if (LeadMonP * LeadMonQ == lcm(LeadMonP, LeadMonQ)) continue;
					
					Pol S = SPolynomial(generators[i], generators[j]);
					
					MP_WRITE("\t" << S);
					Pol remainder = divisionRemainder(S);
					if ( !remainder.empty() ) {
						MP_WRITE("Adding to generators: " << remainder << "\n");
						generators.push_back(remainder);
						
						BuchbergerAlgorithm(i);
						return;
					}
				}
			}
		}
		
		void calculateGrobnerBasis() {
			removeNullPolynomials();
			
			if ( !IsGrobnerBasis ) {
				BuchbergerAlgorithm(0);
				IsGrobnerBasis = true;
			}
		}
		
		void minimizeGrobnerBasis() {
			if ( !IsGrobnerBasis ) calculateGrobnerBasis();
			if ( IsMinimized ) return;
			
			vector <Pol> res = generators;
			for ( unsigned int i = 0; i < generators.size(); i++) {
				for ( unsigned int j = i+1; j < generators.size(); j++) {
					Mon IleadMon = generators[i].leadingMonomial(); 
					Mon JleadMon = generators[j].leadingMonomial();
					
					if ( JleadMon.dominates(IleadMon) ) res[j] = 0;
					else if ( IleadMon.dominates(JleadMon) ) res[i] = 0;
				}
			}
			
			generators = res;
			removeNullPolynomials();
			
			IsMinimized = true;
		}
		
		void reduceGrobnerBasis() {
			if ( !IsMinimized ) minimizeGrobnerBasis();
			if ( IsReduced ) return;
			
			bool changed = true;
			while (changed) {
				changed = false;
				vector <Pol> NewGenerators;
				for (unsigned int i = 0; i < generators.size(); i++) {
					vector <Pol> GeneratorsCopy = generators;
					GeneratorsCopy.erase(GeneratorsCopy.begin()+i);
					Ideal ReducedIdeal = Ideal(GeneratorsCopy);
					Pol NewGenerator = ReducedIdeal.divisionRemainder(generators[i]);
					if ( NewGenerator != generators[i] ) changed = true;
					NewGenerators.push_back(NewGenerator);
				}
				generators = NewGenerators;
				removeNullPolynomials();
			}
			
			IsReduced = true;
		}
		// End Grobner
		
		// Begin operators
		bool contains(Pol const &P) {
			if ( !IsGrobnerBasis ) calculateGrobnerBasis();
			return divisionRemainder(P).empty();
		}
		
		bool operator == (Ideal &I ) {
			if ( !IsReduced ) reduceGrobnerBasis();
			if ( !I.IsReduced ) I.reduceGrobnerBasis();
			normalizeBasis();
			I.normalizeBasis();
			
			return generators == I.generators;
		}
		
		Ideal operator + (Ideal const &I) const{
			vector <Pol> res( generators.size() + I.generators.size() );
			res.insert(res.end(), generators.begin(), generators.end());
			res.insert(res.end(), I.generators.begin(), I.generators.end());
			return Ideal(res);
		}
		
		Ideal operator * (Ideal const &I) const{
			vector <Pol> res;
			for ( Pol P : generators ) {
				for ( Pol Q : I.generators ) {
					res.push_back(P * Q);
				}
			}
			return res;
		}
		
		friend Ideal IdealIntersection (Ideal &I, Ideal &J) {
			typedef Monomial <VarNames, EliminationOrder> MonX;
			typedef Polynomial <K, VarNames, EliminationOrder> PolX;
			typedef PolynomialIdeal <K, VarNames, EliminationOrder> IdealX;
			
			PolX t = PolX(MonX(VarNames(VarNames::EliminationVariable)));
			PolX t1 = PolX(K(1)) - t;
			
			vector <PolX> NewGenerators;
			
			for ( Pol P : I.generators ) NewGenerators.push_back( t * PolX(P) );
			for ( Pol P : J.generators ) NewGenerators.push_back( t1 * PolX(P) );
			
			IdealX NewIdeal = IdealX(NewGenerators);
			
			NewIdeal.reduceGrobnerBasis();
			
			vector <Pol> IntersectionGenerators;
			
			// Come riesce ad accedere a generators? Visto che la specializzazione
			// è diversa (e generators è private)
			for ( PolX P : NewIdeal.generators ) { 
				if ( !P.containsVariable(VarNames(VarNames::EliminationVariable)) ) {
					IntersectionGenerators.push_back(Pol(P));
				}
			}
			
			return Ideal(IntersectionGenerators);
		}
		
		// End operators
		
		// Begin input output
		friend ostream & operator << (ostream &out, Ideal const &A) {
			for ( auto P : A.generators ) cout << P << "\n";
			return out;
		}
		// End input output
};

#endif // GROBNER_HPP
