// TODO: Full latex compatibility in input-output

#ifndef VARNAMES_HPP
#define VARNAMES_HPP

#include "utility.hpp"
using namespace std;

class SingleChar {
	private:
		char c;
	public:
		static const char EliminationVariable = '@';
		
		// Begin constructors
		SingleChar() {}
		
		SingleChar(char x) {
			c = x;
		}
		// End constructors
		
		// Begin operators
		bool const operator < (SingleChar X) const{
			return c < X.c;
		}
		
		bool const operator > (SingleChar X) const{
			return c > X.c;
		}
		
		bool const operator == (SingleChar X) const{
			return c == X.c;
		}
		
		bool const operator != (SingleChar X) const{
			return c != X.c;
		}
		// End operators
		
		// Begin input output
		bool readStream(istream &in) {
			if( !in.get( c ) ) return false;
			
			if ( ( c < 'a' or 'z' < c ) and ( c < 'A' or 'Z' < c ) ) {
				in.unget();
				return false;
			}
			
			return true;
		}
		
		void writeStream(ostream &out) const{
			out << c;
		}
		// End input output
};


struct NumberedX {
	private:
		int n;
	public:
		static const int EliminationVariable = -17;
	
		// Begin constructors
		NumberedX() {}
		
		NumberedX(int m) {
			n = m;
		}
		// End constructors
		
		// Begin operators
	
	bool const operator < (NumberedX X) const{
		return n < X.n;
	}
	
	bool const operator > (NumberedX X) const{
		return n > X.n;
	}
	
	bool const operator == (NumberedX X) const{
		return n == X.n;
	}
	
	bool const operator != (NumberedX X) const{
		return n != X.n;
	}
	// End operators
		
	// Begin input output
	bool readStream(istream &in) {
		char c;
		if( !in.get( c ) ) return false;
		if ( c != 'x' ) {
			in.unget();
			return false;
		}
		if ( !in.get( c ) ) return false;
		if ( c!= '_' ) {
			in.unget();
			in.unget();
			return false;
		}
		if ( !(in >> n) ) return false;
		
		return true;
	}
	
	void writeStream(ostream &out) const{
		out << "x_" << n;
	}
	// End input output
};
#endif // VARNAMES_HPP
