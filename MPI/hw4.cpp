#include "helper.h"
#include <mpi.h>
using namespace std;
int main (int argc, char * argv[])
{
   
	if (argc < 3)
	{
		cout << "Usage: " << argv[0] << " <input_directory>  <output_file>" << endl;
		return 1;
	}
  MPI_Init(&argc, &argv);
  invertFiles(argv[1],argv[2]);
  MPI_Finalize();
	return 0;
}
