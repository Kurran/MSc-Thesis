// Insertion of Partition.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>


std::vector<double> calculate_partition_function(std::string in_filename)
{

	std::ifstream infile(in_filename, std::ifstream::in);
	if (!infile.is_open())
	{
		std::cout << "Failed to open input file " << in_filename << std::endl;
		return std::vector<double>();
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

	std::vector<double> temperatures;
	std::vector<double> partition_function;



	// Fine data analysis
	temperatures.clear();
	partition_function.clear();

	for (int i = 1; i < 33; ++i)
	{
		temperatures.push_back(i);
		partition_function.push_back(0.0);
	}

	int energy = 1;
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
		energy++;
	}

	
	return partition_function;

}

void print_partition_function_to_file(std::vector<double> partition_function, std::string outfile_name) {


	std::ofstream outfile(outfile_name, std::ofstream::out);

	if (!outfile.is_open())
	{
		std::cout << "Failed to open output file " << outfile_name << std::endl;
		return;
	}

	int i = 1;
	for (const auto& part : partition_function)
	{
		std::cout << std::setw(5) << i << std::setw(16) << std::setprecision(4) << part << std::endl;
		outfile << std::setw(5) << i << std::setw(16) << std::setprecision(4) << part << std::endl;
		i++;
	}

	outfile.close();


}


double z(int N, double L, double T) 
{

	if (N == 0) {
		return 0;
	}

	std::vector<std::string> infile_names = {
		"results_p1_b8_t8000.dat",
		"results_p2_b8_t900.dat",
		"results_p3_b8_t900.dat",
		"results_p4_b8_t900.dat",
		"results_p5_b8_t900.dat",
		"results_p6_b8_t8000.dat",
		"results_p7_b8_t5000.dat",
		"results_p8_b8_t5000.dat",
		"results_p9_b8_t1000.dat",
		"results_p10_b8_t1000.dat" };

	std::string infile_name = infile_names[N-1];

	std::ifstream infile(infile_name, std::ifstream::in);

	if (!infile.is_open())
	{
		std::cout << "Failed to open input file " << infile_name << std::endl;
		return 0;
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
	//std::cout << "Read statistics for " << number_of_eignestates.size() << " states" << std::endl;

	double partition_function = 0.0;


	int energy = 1;
	

	for (const auto& states : number_of_eignestates)
	{
		    double modified_energy = static_cast<double>(energy) / pow(L, 2.0);
			double probability = 0.0;
			if (states > 0)
			{
				probability += exp((-1.0 * modified_energy) / T);
				probability *= states;
			}
			partition_function += probability;
		    energy++;
	}

	return partition_function;	
}

double find_force(int number_of_particles, double l, double temperature) {

	if (number_of_particles == 0) {
		return 0;
	}

	z(number_of_particles, l, temperature);

	std::vector<std::string> infile_names = {
	"results_p1_b8_t8000.dat",
	"results_p2_b8_t900.dat",
	"results_p3_b8_t900.dat",
	"results_p4_b8_t900.dat",
	"results_p5_b8_t900.dat",
	"results_p6_b8_t8000.dat",
	"results_p7_b8_t5000.dat",
	"results_p8_b8_t5000.dat",
	"results_p9_b8_t1000.dat",
	"results_p10_b8_t1000.dat" };

	std::string infile_name = infile_names[number_of_particles - 1];

	std::ifstream infile(infile_name, std::ifstream::in);

	if (!infile.is_open())
	{
		std::cout << "Failed to open input file " << infile_name << std::endl;
		return 0;
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
	//std::cout << "Read statistics for " << number_of_eignestates.size() << " states" << std::endl;

	double generalised_force = 0.0;
	int energy = 1;
	for (const auto& states : number_of_eignestates)
	{
		double probability = 0.0;
		double energy_modified = (static_cast<double>(energy)) / (pow(l, 2.0));
		
		if (states > 0)
		{
			probability += exp(-1.0 * energy_modified / temperature);
			probability *= states;
		}
		
		generalised_force += (probability * (energy_modified/l));
		
		energy++;
	}

	return generalised_force;


}

double find_equilibrium_position() {
	
	for (double l = 0.01; l <= 0.999; l += 0.01) {
		double f1 = find_force(5, l, 10.0);
		double f2 = find_force(5, 1.0-l, 10.0);
		std::cout << l << "			 " << f1 << "			 "  << "\n";
	}
	return 0;
}

int main()
{
	std::vector<std::string> infile_name = {
		"results_p1_b8_t8000.dat",
		"results_p2_b8_t900.dat",
		"results_p3_b8_t900.dat",
		"results_p4_b8_t900.dat",
		"results_p5_b8_t900.dat",
		"results_p6_b8_t8000.dat",
		"results_p7_b8_t5000.dat",
		"results_p8_b8_t5000.dat",
		"results_p9_b8_t1000.dat",
		"results_p10_b8_t1000.dat" };


	//output individual plots
	/*for (const auto& if_name : infile_name) {

		auto part = calculate_partition_function(if_name);
		std::string outfile_name = "partition_function_" + if_name;
		print_partition_function_to_file(part, outfile_name);
	}
	*/

	// Szilard Engine analysis

	double temperature = 5.0;
	double l = 0.5;
	double z_initial = z(10.0, 1.0, temperature);
	double z_final = 0.0;
	int N = 10;
	for (int m = 0; m <= N; m++) {

		double z2 = z(m, l, temperature);
		double z3 = z(N- m, 1.0 - l, temperature);
		z_final += (z2*z3); 
	}

	double w_ins = z_final - z_initial;

	//for (int m = 0; m <= N; m++) {
	 
	temperature = 8.0;

	std::vector<double> z_totals;

	std::cout << "Calculating partition functions" << std::endl;

	for (double l = 0.01; l <= 0.99; l += 0.02)
	{
		double z_total = 0.0;

		for (int m = 0; m <= N; m++)
		{
			double z2 = z(m, l, temperature);
			double z3 = z(N - m, 1.0 - l, temperature);
			if (m == 0)
			{
				z_total += z3;
			}
			else if (m == N)
			{
				z_total += z2;
			}
			else
			{
				z_total += (z2*z3);
			}
		}
		z_totals.push_back(z_total);
	}

	l = 0.01;

	std::cout << "Calculating probabilities" << std::endl;

	for (const auto& zt : z_totals)
	{
		
		for (int m = 0; m <= N; m++)
		{
			std::stringstream outfile_name;
			
			outfile_name << "probability_" << m << ".dat";


			double z2 = z(m, l, temperature);
			double z3 = z(N - m, 1.0 - l, temperature);
			double f = 0.0;

			if (m == 0)
			{
				f = z3 / zt;
			}
			else if (m == N)
			{
				f = z2 / zt;
			}
			else
			{
				f = (z2*z3) / zt;
			}

			std::ofstream ofs(outfile_name.str(), std::ofstream::app);
			ofs << std::endl;
			ofs << l << "         " << f;
			std::cout << l << "         " << f << std::endl;
			ofs.close();
		}

		l += 0.02;
	}

	//for (double l = 0.01; l <= 0.99; l += 0.02)
	//{
	//	for (int m = 0; m <= N; m++)
	//	{
	//		double z2 = z(m, l, temperature);
	//		double z3 = z(N - m, 1.0 - l, temperature);
	//		double f = 0.0;
	//
	//		if (m == 0)
	//		{
	//			f = z3 / z_total;
	//		}
	//		else if (m == N)
	//		{
	//			f = z2 / z_total;
	//		}
	//		else
	//		{
	//			f = (z2*z3) / z_total;
	//		}
	//
	//		std::cout << m << "			" << l << "			" << f << "\n";
	//	}
	//}



	//for (double l = 0.01; l <= 0.5; l += 0.02) 
	//{
	//	double z_total = 0.0;
	//	
	//	for (int m = 0; m <= N; m++) 
	//	{
	//		double z2 = z(m, l, temperature);
	//		double z3 = z(N - m, 1.0 - l, temperature);
	//		if (m == 0)
	//		{
	//			z_total += z3;
	//		}
	//		else if (m == N)
	//		{
	//			z_total += z2;
	//		}
	//		else
	//		{
	//			z_total += (z2*z3);
	//		}
	//	}
	//
	//	for (int m = 0; m <= N; m++)
	//	{
	//		double z2 = z(m, l, temperature);
	//		double z3 = z(N - m, 1.0 - l, temperature);
	//		double f = 0.0;
	//
	//		if (m == 0)
	//		{
	//			f  = z3 / z_total;
	//		}
	//		else if (m == N)
	//		{
	//			f = z2 / z_total;
	//		}
	//		else
	//		{
	//			f = (z2*z3) / z_total;
	//		}
	//
	//		std::cout << m << "			" << l << "			" << f << "\n";
	//	}
	//
	//	//std::cout << m << "			" << l << "			" << z(m, l, 8.0) << "\n";
	//	// std::cout << m << "			" << l << "			" << f << "\n";
	//}
	//
	////}
	//
	////find_equilibrium_position();
	//
	//
	//
	//
	//
	return 0;
}