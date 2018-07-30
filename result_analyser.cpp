// result_analyser.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

int main()
{
	std::string infile_name =  "../results_p10_b8_t900.dat";
	std::string outfile_name = "coarse_analysis_p10_b8_t900.dat";
	std::string outfile2_name = "fine_analysis_p10_b8_t900.dat";
	std::string outfile_3 = "rho_results";

	std::ifstream infile(infile_name, std::ifstream::in);

	if (!infile.is_open())
	{
		std::cout << "Failed to open input file " << infile_name << std::endl;
		return -1;
	}

	// Read in the data
	std::string line;
	std::vector<int> number_of_eignestates;
	while (std::getline(infile, line))
	{
		std::stringstream linestream(line);
		int energy, eigenstates;
		linestream >> energy >> eigenstates;
		number_of_eignestates.push_back(eigenstates);
	}
	std::cout << "Read statistics for " << number_of_eignestates.size() << " states" << std::endl;

	// Coarse data analysis
	std::ofstream outfile(outfile_name, std::ofstream::out);
	std::vector<double> temperatures = { 11, 22, 33, 44 };
	std::vector<double> partition_function = {0.0, 0.0, 0.0, 0.0};
	int energy = 1;


	for (const auto& states : number_of_eignestates)
	{
		//outfile << std::setw(6) << energy;
		for (unsigned int i = 0; i < temperatures.size(); ++i)
		{
			double probability = 0.0;
		    if (states > 0)
		    {
		    	probability += exp(-1.0 * static_cast<double>(energy) / temperatures[i]);
		    	probability *= states;
		    }
		    partition_function[i] += probability;
			//outfile << std::setw(16) << std::setprecision(4) << probability;
			//outfile << std::setw(16) << std::setprecision(4) << partition_function[i];
	    }
		//outfile << std::endl;
	    energy++;
	}

	energy = 1;
	auto partition_function_pre = partition_function;
	partition_function = { 0.0, 0.0, 0.0, 0.0 };

	for (const auto& states : number_of_eignestates)
	{
		outfile << std::setw(6) << energy;
		for (unsigned int i = 0; i < temperatures.size(); ++i)
		{
			double probability = 0.0;
			if (states > 0)
			{
				probability += exp(-1.0 * static_cast<double>(energy) / temperatures[i]);
				probability *= states;
			}
			partition_function[i] += probability;
			outfile << std::setw(16) << std::setprecision(4) << (probability / partition_function_pre[i]);
			outfile << std::setw(16) << std::setprecision(4) << partition_function[i];
		}
		outfile << std::endl;
		energy++;
	}



	outfile.close();

	// Fine data analysis
	
	temperatures.clear();
	partition_function.clear();

	for (int i = 1; i < 33; ++i)
	{
		temperatures.push_back(i);
		partition_function.push_back(0.0);
	}

	energy = 1;
	for (const auto& states : number_of_eignestates)
	{
		for (unsigned int i = 0; i < temperatures.size(); ++i)
		{
			double probability = 0.0;
			if (states > 0)
			{
				probability += exp(-1.0 * static_cast<double>(energy) / temperatures[i]);
				probability *= states;
			}
			partition_function[i] += probability;

		}
		outfile << std::endl;
		energy++;
	}

	outfile.open(outfile2_name, std::ofstream::out);
	int i = 1;
	for (const auto& pf : partition_function)
	{
		std::cout << std::setw(5) << i << std::setw(16) << std::setprecision(4) << pf << std::endl;
		outfile << std::setw(5) << i << std::setw(16) << std::setprecision(4) << pf << std::endl;
		i++;
	}
	outfile.close();

    return 0;
}

