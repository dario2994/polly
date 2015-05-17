#ifndef MONOMIALORDER_HPP
#define MONOMIALORDER_HPP

#include "utility.hpp"
using namespace std;

template <class VarNames>
bool LexicographicalOrder(map <VarNames, int> const &A, 
													map <VarNames, int> const &B) {
	auto it1 = A.begin();
	auto end1 = A.end();
	auto it2 = B.begin();
	auto end2 = B.end();
	
	while ( it1 != end1 and it2 != end2 and (*it1) == (*it2) ) {
		it1++;
		it2++;
	}
	
	if ( it1 == end1 and it2 == end2 ) return false; //equality
	else if ( it1 == end1 ) return true;
	else if ( it2 == end2 ) return false;
	else if ( (*it1).first != (*it2).first ) {
		// 'x' is more important than 'y'
		return (*it1).first > (*it2).first;
	}
	else {
		return (*it1).second < (*it2).second;
	}
}
		
template <class VarNames>
bool GradedReverse(map <VarNames, int> const &A, 
									 map <VarNames, int> const &B) {
	// Total Degree comparison
	int TotalDegree1 = 0;
	int TotalDegree2 = 0;
	for ( auto var : A ) TotalDegree1 += var.second;
	for ( auto var : B ) TotalDegree2 += var.second;
	if ( TotalDegree1 != TotalDegree2 ) return TotalDegree1 < TotalDegree2;
	
	// Inverted lex
	auto it1 = A.rbegin();
	auto end1 = A.rend();
	auto it2 = B.rbegin();
	auto end2 = B.rend();
	
	while ( it1 != end1 and it2 != end2 and (*it1) == (*it2) ) {
		it1++;
		it2++;
	}
	
	if ( it1 == end1 and it2 == end2 ) return false; //equality
	else if ( (*it1).first != (*it2).first ) {
		return (*it1).first > (*it2).first;
	}
	else {
		return (*it1).second > (*it2).second;
	}
}

template <class VarNames>
bool EliminationOrder(map <VarNames, int> const &A, 
											map <VarNames, int> const &B) {
	VarNames EliminationVariable = VarNames(VarNames::EliminationVariable);
	bool member1 = A.find(EliminationVariable) != A.end();
	bool member2 = B.find(EliminationVariable) != B.end();
	
	if ( member1 and member2 ) {
		int exponent1 = A.at(EliminationVariable);
		int exponent2 = B.at(EliminationVariable);
		if ( exponent1 != exponent2 ) {
			return exponent1 < exponent2;
		}
	}
	else if( member1 and !member2 ) return false;
	else if( !member1 and member2 ) return true;
	
	return GradedReverse(A, B);
}

#endif // MONOMIALORDER_HPP
