// CSE232-TeamProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

//VARs
bool is_Circular = false;
json materials_json;
int material_id = 0;
//

//read json file and store it in a json object
void readjson(json& j, const string& filename)
{
	ifstream file(filename);
	if (!file.is_open()) {
		cerr << "Could not open the file!" << endl;
	}
	file >> j;
	file.close();
}

//Save any json object to a file
void savejson(json& j,const string& filename) {
	std::ofstream writeFile(filename);
	writeFile << std::setw(4) << j << std::endl;
	writeFile.close();
}


void NewMaterial(json& j, const string& mname, const double& yield, const double& density)
{
	
	json newMaterial = {
		{"id",(j["materials"].size() + 1)},
		{"name", mname},
		{"sigma_yield", yield},
		{"density", density}
	};

	// 3. Push it into the "materials" array
	j["materials"].push_back(newMaterial);
	savejson(j, "materials.json");
}
///////////////////////////////////////////////WILL BE EDITED TO BE ABLE TO DISPLAY OTHER TABLES
void CreateTable() {
	// Define column widths
	const int idWidth = 4;
	const int nameWidth = 20;
	const int yieldWidth = 15;
	const int densityWidth = 10;

	// Print Header 
	cout << " " << left << setw(idWidth) << "ID"
		<< "| " << setw(nameWidth) << "Material"
		<< "| " << setw(yieldWidth) << "Yield (Mpa)"
		<< "| " << setw(densityWidth) << "Density(gm/cm^3)" << endl;

	cout << " " << string(idWidth + nameWidth + yieldWidth + densityWidth + 6, '-') << endl;

	for (const auto& m : materials_json["materials"]) {
		int id = m["id"];
		string name = m["name"];
		double yield = m["sigma_yield"];
		double density = m["density"];

		cout << " " << left << setw(idWidth) << id
			<< "| " << setw(nameWidth) << name
			<< "| " << setw(yieldWidth) << yield
			<< "| " << setw(densityWidth) << density << endl;
	}
}

double LinkMass(double density, double L, double b, double h, double r, bool is_Circular)
{
	if (is_Circular)
	{return density * (M_PI * pow(r, 2) * L);}
	else
	{return density * (b * h * L);}
}

double BendingMoment(double ml, double mp, double L, double alphaMax)
{
	double g = 9.81; 
	double M = (ml * g * (L / 2.0)) + (mp * g * L) + (ml * pow((L / 2.0), 2) * alphaMax) + (mp * pow(L, 2) * alphaMax);
	return M;
}

double MomentofInertia(double b, double h, double r, bool is_Circular)
{
	if (is_Circular)
	{return (M_PI * pow(r, 4)) / 4.0;}
	else 
	{return (b * pow(h, 3)) / 12.0;}
}

double MaxStress(double M, double h, double r, double I, bool is_Circular) 
{
	if (is_Circular)
	{return (M * r) / I;}
	else
	{return (M * (h / 2.0)) / I;}
}

int main()
{
	cout << R"(
			 _____     ____   ____    ____   _______ 
			|  __ \   / __ \ |  _ \  / __ \ |__   __|
			| |__) | | |  | || |_) || |  | |   | |   
			|  _  /  | |  | ||  _ < | |  | |   | |   
			| | \ \  | |__| || |_) || |__| |   | |   
			|_|  \_\  \____/ |____/  \____/    |_|   
		    _____   ______   _____   _____    _____   _   _  
		   |  __ \ |  ____| / ____| |_   _|  / ____| | \ | | 
		   | |  | || |__   | (___     | |   | |  __  |  \| | 
		   | |  | ||  __|   \___ \    | |   | | |_ | | |\  | 
		   | |__| || |____  ____) |  _| |_  | |__| | | | \ | 
		   |_____/ |______||_____/  |_____|  \_____| |_| \_|
--------------------------------------------------------------------------------------------------------)";
    cout << "\n \n Please choose the Cross-section type for the link\n for Circular (c) for Recatngular (r)....  ";

	if (cin.get() == 'c') is_Circular = true;
	
	cout << "\n Please choose a material from this catalog.... \n";
	//read the json file and print the materials in a table format
	readjson(materials_json, "materials.json");

	CreateTable();

	string input;
	cout << "\nType the material ID to choose, or to add a new press (n): ";

	while (true)
	{
		cin >> input;

		// 1. Check for the 'new' command first
		if (input == "n" || input == "N")
		{
			string name;
			double sigma_yield, density;

			cout << "\n Please enter the material name..."; cin >> name;
			cout << "\n material yield strength..."; cin >> sigma_yield;
			cout << "\n material density..."; cin >> density;
			NewMaterial(materials_json, name, sigma_yield, density);
			CreateTable();
		}
		else
		{
			// 2. If it's not 'n', try to parse it as an ID
			try
			{
				int id = stoi(input);


				if (id > 0 && id <= materials_json["materials"].size())
				{
					material_id = id;
					break;
				}
				else
				{
					cout << "\nID out of range. Please try again.\n";
				}
			}
			// 3. Catch errors if they type random letters like "abc"
			catch (const invalid_argument&)
			{
				cout << "\nInvalid input. Please enter a valid number or 'n'.\n";
			}
		}
	}

	cout << "\nYou have selected material: " << materials_json["materials"][material_id - 1]["name"] << endl;
	
	
	double b=0, h=0, r=0, L, mp, alphaMax ;
	cout << "Enter link length, Payload mass and Max angular acceleration respectively" << endl;
	cin >> L>> mp>> alphaMax;
	
	if (is_Circular)
	{
		cout << "Enter initial radius" << endl;
		cin  >> r;
	}
	else
	{
		cout << "Enter initial width(b) & hight(h)" << endl;
		cin  >> b >> h;
	}

	////starting optimization
	double ml, M, I, CurrentStress;
	bool is_optimized = false;
	
	
	return 0;
}

