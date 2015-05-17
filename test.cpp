// Testing suite

#define POLLY_DEBUG 1

#include "utility.hpp"

#include "VarNames.hpp"
#include "Rings.hpp"
#include "Fields.hpp"

#include "Grobner.hpp"
using namespace std;

typedef Polynomial <Fraction <Integer>, SingleChar> Pol;
typedef Monomial <SingleChar> Mon;
typedef PolynomialIdeal <Fraction <Integer>, SingleChar> Ideal;
typedef vector <string> VC;

void VerifyAnswer(vector <Pol> res, vector <Pol> correct, string TestName) {
	sort(res.begin(), res.end());
	sort(correct.begin(), correct.end());
	if (res != correct) {
		cerr << "\nres:\n";
		for( auto P : res ) cerr << P << "\n";
		cerr << "\n";
		cerr << "correct:\n";
		for( auto P : correct ) cerr << P << "\n";
		throw TestName + " failed.";
	}
}

vector <Pol> StringToPol(VC S) {
	vector <Pol> res;
	for ( string s : S ) res.push_back( Pol(s) );
	return res;
}

void TestGrobnerLex(VC gen, VC ans) {
	
	Ideal I = Ideal(gen);
	I.reduceGrobnerBasis();
	I.normalizeBasis();
	
	VerifyAnswer(I.idealGenerators(), StringToPol(ans), "GrobnerLex");
}

void TestIntersectionLex(VC I, VC J, VC ans) {
	Ideal II = Ideal(I);
	Ideal JJ = Ideal(J);
	Ideal KK = IdealIntersection(II, JJ);
	KK.reduceGrobnerBasis();
	KK.normalizeBasis();
	
	VerifyAnswer(KK.idealGenerators(), StringToPol(ans), "IntersectionLex");
}

void TestZerosEquality() {
	vector <Pol> pol0 = {Pol(0), Pol("0"), Pol("0xyz")};
	for (unsigned int i = 0; i < pol0.size(); i++) {
		for (unsigned int j = 0; j < pol0.size(); j++) {
			Pol P =pol0[i];
			Pol Q =pol0[j];
			if ( P != Q ) {
				cerr << P << "\n";
				cerr << Q << "\n";
				throw "ZerosEquality failed";
			}
		}
	}
}

vector <vector <string>> TestCasesLex = {
	{"x²+y²", "x³y³+y^4"},
	{"y^8+y^4", "xy^4+y^7", "x²+y²"},
	
	{"xy+z", "yz+x", "zx+y"},
	{"x+yz", "y²-z²", "yz²-y", "z³-z"},
	
	{"xy+z", "yz+x", "zx+y", "x²+y²+z²"},
	{"x", "y", "z"},
	
	{"xyz^6+4x³y³", "7/2x²+1/3z^5y", "xyz+x²y²z²"},
	{"y²z^14+4y²z^6", "y^5z^6-21/8y²z^7", "xyz-2/21y³z^7", "x²+2/21yz^5"}
};

vector <vector <string>> TestCasesIntersection = {
	{"ac", "ad", "bc", "bd"},
	{"a-c", "b-d"},
	{"b²d-bd²", "b²c-bcd", "ad-bc", "abc-bc²", "a²c-ac²"}
};



int main() {
	try {
		for (int i = 0; i < (int)TestCasesLex.size(); i+=2) {
			auto test = TestCasesLex[i];
			auto ans  = TestCasesLex[i+1];
			TestGrobnerLex(test, ans);
		}
		
		for (int i = 0; i < (int)TestCasesIntersection.size(); i+=3) {
			auto testI = TestCasesIntersection[i];
			auto testJ = TestCasesIntersection[i+1];
			auto ans = TestCasesIntersection[i+2];
			TestIntersectionLex(testI, testJ, ans);
		}
		
		TestZerosEquality();
	}
	catch (string ErrorMessage){
		cerr << "\n\n" << ErrorMessage << "\n";
	}
	
	cout << "All tests passed\n";
}
