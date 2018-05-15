#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <chrono>

#include <boost/numeric/ublas/io.hpp>

#include "../src/FloatMatrix.hpp"
#include "../src/MatrixMultiply.hpp"

void initRandomMatrix(scottgs::FloatMatrix& m);

int main(int argc, char * argv[])
{
	
	// ---------------------------------------------
	// BEGIN: Timing Analysis
	// ---------------------------------------------
	std::cout << "Running Timing Analysis" << std::endl
		  << "-----------------------" << std::endl;
	srand(123456);	// use a constant number to seed the pseudo random number generator
			// this way your results at least have the same input on a given system
	
	const unsigned int ITR=100;
	
	// ---------------------------------------------
	// Build up a set of test matrix-pair sizes
	// ---------------------------------------------
	std::vector<std::pair<std::pair<unsigned short,unsigned short>, std::pair<unsigned short,unsigned short> > > testList;
	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(50,40),
					   std::pair<unsigned short,unsigned short>(40,60)) );


	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(60,50),
					   std::pair<unsigned short,unsigned short>(50,70)) );

	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(70,60),
					   std::pair<unsigned short,unsigned short>(60,80)) );

	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(80,70),
					   std::pair<unsigned short,unsigned short>(70,90)) );

// Change this to 1 to enable larger matrices, set to 0 for just the smalls
#if 1
	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(90,80),
					   std::pair<unsigned short,unsigned short>(80,100)) );

	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(100,90),
					   std::pair<unsigned short,unsigned short>(90,110)) );
					   
	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(200,90),
					   std::pair<unsigned short,unsigned short>(90,220)) );

	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(200,180),
					   std::pair<unsigned short,unsigned short>(180,220)) );

	testList.push_back( std::make_pair(std::pair<unsigned short,unsigned short>(400,360),
					   std::pair<unsigned short,unsigned short>(360,240)) );

#endif
	// ***********************************
	// Test functor : Your implementation
	// ***********************************

	scottgs::MatrixMultiply mm;

	for (std::vector<std::pair<std::pair<unsigned short,unsigned short>, std::pair<unsigned short,unsigned short> > >::const_iterator t=testList.begin();
		t!=testList.end();++t)
	{
		// Get matrix size pairs from iterator
		// instantiate matrices, then randomize
		std::pair<unsigned short,unsigned short> lp = t->first;
		std::pair<unsigned short,unsigned short> rp = t->second;
		scottgs::FloatMatrix l(lp.first,lp.second);
		scottgs::FloatMatrix r(rp.first,rp.second);
		initRandomMatrix(l);
		initRandomMatrix(r);

		// Run Timing Experiment
                std::chrono::high_resolution_clock c;
                std::chrono::high_resolution_clock::time_point start = c.now();
		for (unsigned int i = 0; i < ITR; ++i)
		{
			// This is an assignemnt of a call to a object functor;
			scottgs::FloatMatrix p = mm(l,r);
		}
                std::chrono::high_resolution_clock::time_point stop = c.now();
		double avgMs = (double) std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / (1000000 * ITR);

		// Compute Ops and Elements
		// Log timing statistics
		const unsigned long opsMaybe = l.size1() * r.size2() * l.size2() + l.size1() + r.size2();
		const unsigned long elements = l.size1() * r.size2();
		std::cout << "------------------------------------------------------------------" << std::endl
			  << ITR << " iterations of matrix multiplication (functor) ran using ("
			  << l.size1() <<","<< l.size2() <<")*("
			  << r.size1() <<","<< r.size2() <<") = ("
			  << l.size1() <<","<< r.size2() <<")" << std::endl
			  << "      :Method:Average Time (s):approximate ops:computed elements" << std::endl
			  << "Data  Point:f:" << avgMs << ":" << opsMaybe<< ":" << elements << std::endl;
	}


#if 1
	// ***********************************
	// Test built-in Boost prod()
	// ***********************************
	
	for (std::vector<std::pair<std::pair<unsigned short,unsigned short>, std::pair<unsigned short,unsigned short> > >::const_iterator t=testList.begin();
		t!=testList.end();++t)
	{
		// Get matrix size pairs from iterator
		// instantiate matrices, then randomize
		std::pair<unsigned short,unsigned short> lp = t->first;
		std::pair<unsigned short,unsigned short> rp = t->second;
		scottgs::FloatMatrix l(lp.first,lp.second);
		scottgs::FloatMatrix r(rp.first,rp.second);
		initRandomMatrix(l);
		initRandomMatrix(r);

		// Run Timing Experiment
                std::chrono::high_resolution_clock c;
                std::chrono::high_resolution_clock::time_point start = c.now();
                for (unsigned int i = 0; i < ITR; ++i)
		{
			scottgs::FloatMatrix p = mm.multiply(l,r);
		}
                std::chrono::high_resolution_clock::time_point stop = c.now();
		double avgMs = (double) std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() / (1000000 * ITR);

		// Compute Ops and Elements
		// Log timing statistics
		const unsigned long opsMaybe = l.size1() * r.size2() * l.size2() + l.size1() + r.size2();
		const unsigned long elements = l.size1() * r.size2();
		std::cout << "------------------------------------------------------------------" << std::endl
			  << ITR << " iterations of matrix multiplication (method) ran using ("
			  << l.size1() <<","<< l.size2() <<")*("
			  << r.size1() <<","<< r.size2() <<") = ("
			  << l.size1() <<","<< r.size2() <<")" << std::endl
			  << "      :Method:Average Time (s):approximate ops:computed elements" << std::endl
			  << "Data Point:m1:" << avgMs << ":" << opsMaybe<< ":" << elements << std::endl;

	}
#endif



	std::cout << "Timing Analysis Completed" << std::endl
		  << "=========================" << std::endl;
	// ---------------------------------------------
	// END: Timing Analysis
	// ---------------------------------------------
}

void initRandomMatrix(scottgs::FloatMatrix& m)
{
	// Initialize each element.
	// See discussion board for better way, 
	// this was originally posted to be a 
	// simple example of per-element access into the matric
	for (unsigned i = 0; i < m.size1(); ++ i)
	        for (unsigned j = 0; j < m.size2(); ++ j)
		            m (i, j) = (static_cast<float>(rand()) / RAND_MAX) * 100.0;
}

