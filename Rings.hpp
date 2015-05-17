#ifndef RINGS_HPP
#define RINGS_HPP

#include "utility.hpp"
using namespace std;

class Integer {
	private:
		long long int val;
	
	public:
		// Begin constructors
		Integer() {
			val = 0;
		}
		
		Integer(long long int n) {
			val = n;
		}
		
		Integer(string S) {
			bool ImplicitOne;
			stringstream Sstream(S);
			assert( readStream(Sstream, true, ImplicitOne) );
		}
		// End constructors
		
		long long int value() const{
			return val;
		}
		
		// Begin operators
		Integer operator + (Integer const &A) const{
			return Integer(val + A.val);
		}
		
		Integer operator - (Integer const &A) const{
			return Integer(val - A.val);
		}
		
		Integer operator * (Integer const &A) const{
			return Integer(val * A.val);
		}
		
		void operator += (Integer const &A) {
			val += A.val;
		}
		
		void operator -= (Integer const &A) {
			val -= A.val;
		}
		
		void operator *= (Integer const &A) {
			val *= A.val;
		}
		
		bool operator == (Integer const &A) const{
			return val == A.val;
		}
		
		bool operator != (Integer const &A) const{
			return val != A.val;
		}
		// End operators
		
		friend void Simplify(Integer &A, Integer &B) {
			if ( B.val < 0 ) {
				B.val = -B.val;
				A.val = -A.val;
			}
			
			long long int gcdAB = IntegerGcd(A.val, B.val);
			A.val /= gcdAB;
			B.val /= gcdAB;
		}
		
		// Begin input output
		bool readStream(istream &in, bool ImplicitSign, bool &ImplicitOne) {
			long long int sign = 1;
			if( !ImplicitSign ) {
				if ( !SignReadStream(in, sign) ) {
					return false;
				}
			}
			
			long long int NoSign = 1;
			char c;
			
			if ( !in.get(c) ) return false; 
			in.unget();
			
			if ( ImplicitSign and (c == '-' or c == '+') ) {
				sign = (c == '-')?-1:1;
				in.get(c);
				if ( !in.get(c) ) return false;
				in.unget();
			}
			
			if ( '0' <= c and c <= '9' ) {
				ImplicitOne = false;
				in >> NoSign; 
			}
			else {
				ImplicitOne = true;
			}
			
			val = sign * NoSign;
			return true;
		}
		
		void writeStream(ostream &out, bool ImplicitSign, bool ImplicitOne) const{
			if ( !ImplicitSign and val >= 0 ) out << '+';
			if ( ImplicitOne ) {
				if ( val == -1 ) out << '-';
				else if ( val != 1 ) out << val;
			}
			else out << val;
		}
		
		friend istream & operator >> (istream &in , Integer &A) {
			bool ImplicitOne;
			A.readStream(in, true, ImplicitOne);
			return in;
		}
		
		friend ostream & operator << (ostream &out , Integer const &A) {
			A.writeStream(out, true, false);
			return out;
		}
		// End input output
};

template <unsigned long long int mod>
class IntegerMod {
	private:
		typedef IntegerMod <mod> Int;
	
		long long int val; //Should always be a non negative number
	
	public:
		// Begin constructors
		IntegerMod <mod>(){
			val = 0;
		}
		
		IntegerMod <mod>(long long int n) {
			n = n % mod;
			val = ( n >= 0 ) ? n : n+mod;
		}
		
		IntegerMod <mod>(Integer n) {
			IntegerMod <mod>( n.value() );
		}
		
		IntegerMod <mod>(string S) {
			bool ImplicitOne;
			stringstream Sstream(S);
			assert( readStream(Sstream, true, ImplicitOne) );
		}
		// End constructors
		
		// Begin operators
		Int operator + (Int const &A) const{
			return Int(val + A.val);
		}
		
		Int operator - (Int const &A) const{
			return Int(val - A.val);
		}
		
		Int operator * (Int const &A) const{
			return Int(val * A.val);
		}
		
		void operator += (Int const &A) {
			val = (val + A.val) % mod;
		}
		
		void operator -= (Int const &A) {
			val = (val - A.val + mod) % mod;
		}
		
		void operator *= (Int const &A) {
			val = (val * A.val) % mod;
		}
		
		bool operator == (Int const &A) const{
			return val == A.val;
		}
		
		bool operator != (Int const &A) const{
			return val != A.val;
		}
		// End operators
		
		friend void Simplify(Int &A, Int &B) {
			return;
		}
		
		// Begin input output
		bool readStream(istream &in, bool ImplicitSign, bool &ImplicitOne) {
			long long int sign = 1;
			
			if( !ImplicitSign ) {
				if ( !SignReadStream(in, sign) ) {
					return false;
				}
			}
			
			long long int NoSign = 1;
			char c;
			
			if ( !in.get(c) ) return false; // Forse dovrei ungettare?
			in.unget();
			
			if ( ('0' <= c and c <= '9') or 
					 ( ImplicitSign and (c == '-' or c == '+') ) ) {
				ImplicitOne = false;
				in >> NoSign; 
			}
			else {
				ImplicitOne = true;
			}
			
			val = ( sign * NoSign ) % mod;
			if ( val < 0 ) val += mod;
			return true;
		}
		
		void writeStream(ostream &out, bool ImplicitSign, bool ImplicitOne) const{
			if ( !ImplicitSign and val >= 0 ) out << '+';
			if ( ImplicitOne ) {
				if ( val == -1 ) out << '-';
				else if ( val != 1 ) out << val;
			}
			else out << val;
		}
		
		friend istream & operator >> (istream &in , Int &A) {
			bool ImplicitOne;
			A.readStream(in, true, ImplicitOne);
			return in;
		}
		
		friend ostream & operator << (ostream &out , Int const &A) {
			A.writeStream(out, true, false);
			return out;
		}
		
		// End input output
};


// TODO: Finire (iniziare?) i BigInt
class BigInteger{
	private:
		typedef unsigned long long int ULL;
		ULL sign;
		vector <ULL> num; //should be less than 1<<62
		static const ULL MaxNum = 1ull<<62;
		
		void canonicalForm() {
			for (unsigned int i = 0; i < num.size(); i++) {
				if ( num[i] & MaxNum ) {
					num[i] ^= MaxNum;
					if ( i+1 == num.size() ) num.push_back(1);
					else num[i+1]++;
				}
			}
			
			for (unsigned int i = num.size()-1; i > 0; i--) {
				if ( num[i] == 0 ) num.pop_back();
			}
		}
	public:
		// Begin constructors
			BigInteger(){}
			BigInteger(int n) {
				sign = (n>=0)?1:-1;
				if (n<0) n = -n;
				num.push_back(n);
			}
			BigInteger(long long int n) {
				sign = (n>=0)?1:-1;
				if (n<0) n = -n;
				num.push_back(n);
			}
		// End constructors
		
		// Begin operators
			BigInteger operator +(BigInteger const &A) const{
				//tocca decidere se i segni sono concordi
				if (A.num.size() < num.size() ) {
					
				}
				return A;
			}
		// End operators
};

#endif // RINGS_HPP
