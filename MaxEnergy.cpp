// MaxEnergy.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main()
{

	int N = 10;
	

	for (int N = 5; N < 20; N++)
	{
		for (int buckets = 5; buckets < 11; buckets++)
		{
			int levels = buckets * 3;
			int n = N;
			int i = 1;
			int energy = 0;

			while (n > 0)
			{
				if (n >= 2)
				{
					n -= 2;
					energy += 2 * (i*i);
				}
				else
				{
					n -= 1;
					energy += (i*i);
				}
				i++;
			}

			printf("Fermi energy  for %d particles is %d\n", N, energy);

			// Now remove the top particle in the fermi state
			int top_fermi_level_energy = (N / 2) + (N % 2);
			top_fermi_level_energy *= top_fermi_level_energy;
			energy -= top_fermi_level_energy;

			//printf("Top energy is %d\n", top_fermi_level_energy);

			// Promote it to the first levels above the highest level
			// considered

			//printf("First ignore levels is %d at energy %d\n", levels + 1, ((levels + 1)*(levels + 1)));

			energy += ((levels + 1)*(levels + 1));

			printf("For %2d particles with %2d levels the max energy is %d \n", N, levels, energy);
		}

	}
	return 0;


}

