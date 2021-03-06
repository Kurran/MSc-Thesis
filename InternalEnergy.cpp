#include "stdafx.h"


#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

std::vector<double> calculate_internal_energies(std::string in_filename)
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

	for (int i = 2; i < 33; ++i)
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

	energy = 1;
	std::vector<double> internal_energy;
	for (int i = 2; i < 33; ++i)
	{
		internal_energy.push_back(0.0);
	}
	for (const auto& states : number_of_eignestates)
	{
		for (unsigned int i = 0; i < temperatures.size(); ++i)
		{
			if (states > 0)
			{
				double probability = 0.0;
				probability += ((exp(-1.0 * static_cast<double>(energy) / temperatures[i])) / partition_function[i]);
				double nancheck = states*probability*static_cast<double>(energy);
				if (isnan(nancheck)) {
					nancheck = 0.0;
				}
				internal_energy [i] += nancheck;



			}
		}
		energy++;
	}
	return internal_energy;

}


void print_internal_energies_to_file(std::vector<double> internal_energy, std::string outfile_name) {


	std::ofstream outfile(outfile_name, std::ofstream::out);

	if (!outfile.is_open())
	{
		std::cout << "Failed to open output file " << outfile_name << std::endl;
		return;
	}

	int i = 1;
	for (const auto& int_ent : internal_energy)
	{
		std::cout << std::setw(5) << i << std::setw(16) << std::setprecision(4) << int_ent << std::endl;
		outfile << std::setw(5) << i << std::setw(16) << std::setprecision(4) << int_ent << std::endl;
		i++;
	}

	outfile.close();


}

void print_internal_energies_to_3D_file(std::vector<std::vector<double>> internal_energy, std::string outfile_name) {


	std::ofstream outfile(outfile_name, std::ofstream::out);

	if (!outfile.is_open())
	{
		std::cout << "Failed to open output file " << outfile_name << std::endl;
		return;
	}


	int particle = 1;
	for (const auto& int_en : internal_energy)

	{
		//std::cout << std::setw(5) << i << std::setw(16) << std::setprecision(4) << ent * -1.0 << std::endl;
		//outfile << std::setw(5) << i << std::setw(16) << std::setprecision(4) << ent * -1.0 << std::endl;

		int temperature = 1;
		for (const auto& e : int_en) {
			outfile << std::setw(4) << particle << "\t";
			outfile << std::setw(4) << temperature << "\t";
			temperature++;
			outfile << std::setw(16) << std::setprecision(4) << e << std::endl;
		}
		particle++;
	}


	outfile.close();


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
	for (const auto& if_name : infile_name) {

		auto internal_energies = calculate_internal_energies(if_name);
		std::string outfile_name = "internal_energies_" + if_name;
		print_internal_energies_to_file(internal_energies, outfile_name);
	}
	//output single 3d plot
	std::vector<std::vector<double>> internal_energies;

	for (const auto& if_name : infile_name) {

		auto entropy = calculate_internal_energies(if_name);
		internal_energies.push_back(entropy);

	}
	print_internal_energies_to_3D_file(internal_energies, "internal_energy.dat");


	return 0;
}