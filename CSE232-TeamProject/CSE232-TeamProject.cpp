// CSE232-TeamProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;

//Global json objects 
json materials_json;
json motors_json;
json gearboxes_json;

//Classes
class Material {
private:
	//Protected private props
	int id;
	string name;
	double sigma_yield;
	double density;
public:
	//default constr
	Material() {
		id = 0;
		name = "";
		sigma_yield = 0.0;
		density = 0.0;
	}
	//constr
	Material(int mat_id)
	{
		id = mat_id;
		name = materials_json["materials"][mat_id - 1]["name"];
		sigma_yield = materials_json["materials"][mat_id - 1]["sigma_yield"];
		density = materials_json["materials"][mat_id - 1]["density"];
	}

	//getters
	int getid() const { return id; }
	string getName() const { return name; }
	double getSigmaYield() const { return sigma_yield; }
	double getDensity() const { return density; }
};

class Motor { 
private:
	int id;
	string name, manufacturer;
	double no_load_speed, nominal_torque, nominal_voltage, stall_torque, efficiency, diameter, length, weight;
public:
	//default constr
	Motor() {
		id = 0;
		name = "";
		manufacturer = "";
		nominal_voltage = 0.0;
		no_load_speed = 0.0;
		nominal_torque = 0.0;
		stall_torque = 0.0;
		efficiency = 0.0;
		diameter = 0.0;
		length = 0.0;
		weight = 0.0;
	}
	//constr
	Motor(int motor_id)
	{
		id = motor_id;
		name = motors_json["motors"][motor_id - 1]["name"];
		manufacturer = motors_json["motors"][motor_id - 1]["manufacturer"];
		nominal_voltage = motors_json["motors"][motor_id - 1]["nominal_voltage"];
		no_load_speed = motors_json["motors"][motor_id - 1]["no_load_speed"];
		nominal_torque = motors_json["motors"][motor_id - 1]["nominal_torque"];
		stall_torque = motors_json["motors"][motor_id - 1]["stall_torque"];
		efficiency = motors_json["motors"][motor_id - 1]["efficiency"];
		diameter = motors_json["motors"][motor_id - 1]["diameter"];
		length = motors_json["motors"][motor_id - 1]["length"];
		weight = motors_json["motors"][motor_id - 1]["weight"];
	}
	//getters
	int getid() const { return id; }
	string getName() const { return name; }
	double getNoLoadSpeed() const { return no_load_speed; }
	double getNominalTorque() const { return nominal_torque; }
	double getStallTorque() const { return stall_torque; }
	double getEfficiency() const { return efficiency; }
	double getDiameter() const { return diameter; }
};

class Gearbox {
private:
	int id;
	string name, manufacturer;
	double gear_ratio, stages, max_torque, efficiency, diameter, length, weight;
public:
	//Props
	
	//default constr
	Gearbox() {
		id = 0;
		name = "";
		manufacturer = "";
		gear_ratio = 0.0;
		stages = 0.0;
		max_torque = 0.0;
		efficiency = 0.0;
		diameter = 0.0;
		length = 0.0;
		weight = 0.0;
	}
	//constr
	Gearbox(int gearbox_id)
	{
		id = gearbox_id;
		name = gearboxes_json["gears"][gearbox_id - 1]["name"];
		manufacturer = gearboxes_json["gears"][gearbox_id - 1]["manufacturer"];
		gear_ratio = gearboxes_json["gears"][gearbox_id - 1]["gear_ratio"];
		stages = gearboxes_json["gears"][gearbox_id - 1]["stages"];
		max_torque = gearboxes_json["gears"][gearbox_id - 1]["max_torque"];
		efficiency = gearboxes_json["gears"][gearbox_id - 1]["efficiency"];
		diameter = gearboxes_json["gears"][gearbox_id - 1]["diameter"];
		length = gearboxes_json["gears"][gearbox_id - 1]["length"];
		weight = gearboxes_json["gears"][gearbox_id - 1]["weight"];
	}
	//getters
	int getid() const { return id; }
	string getName() const { return name; }
	double getGearRatio() const { return gear_ratio; }
	double getMaxTorque() const { return max_torque; }
	double getEfficiency() const { return efficiency; }
	double getDiameter() const { return diameter; }
};

class RobotLink {
private:
	//Props
	Material material;
	Motor motor;
	Gearbox gearbox;
	double b = 0, h = 0, r = 0, L, m_payload, alphaMax,omega;
	double m_link, M_bending, I_xx, stress;
	double cost, torque_out;
	bool  is_optimized = false;
	bool  is_Circular = false;
public:
	//default constr
	RobotLink() {
		b = 0;
		h = 0;
		r = 0;
		L = 0;
		m_payload = 0;
		m_link = 0;
		M_bending = 0;
		I_xx = 0;
		stress = 0;
		alphaMax = 0;
		cost = 0;
		omega = 0;
		torque_out = 0;
	}
	//constr
	RobotLink(Material mat) {
		material = mat;
		b = 0;
		h = 0;
		r = 0;
		L = 0;
		m_payload = 0;
		m_link = 0;
		M_bending = 0;
		I_xx = 0;
		stress = 0;
		alphaMax = 0;
		cost = 0;
		omega = 0;
		torque_out = 0;
	}
	//getters
	Material getMaterial() const { return material; }
	Motor getMotor() const { return motor; }
	Gearbox getGearbox() const { return gearbox; }
	double getB() const { return b; }
	double getH() const { return h; }
	double getR() const { return r; }
	double getL() const { return L; }
	double getMPayload() const { return m_payload; }
	double getMLink() const { return m_link; }
	double getMBending() const { return M_bending; }
	double getIxx() const { return I_xx; }
	double getStress() const { return stress; }
	double getAlphaMax() const { return alphaMax; }
	bool getIsCircular() const { return is_Circular; }
	bool getIsOptimized() const { return is_optimized; }
	double getOmega() const { return omega; }
	double getTorqueOut() const { return torque_out; }
	double getCost() const { return cost; }
	//setters
	void setMaterial(Material mat) { material = mat; }
	void setMotor(int mot_id) { motor = Motor(mot_id); }
	void setGearbox(int gb_id) { gearbox = Gearbox(gb_id); }
	void setB(double width) { b = width; }
	void setH(double height) { h = height; }
	void setR(double radius) { r = radius; }
	void setL(double length) { L = length; }
	void setMPayload(double payload) { m_payload = payload; }
	void setMLink(double link) { m_link = link; }
	void setMBending(double bending) { M_bending = bending; }
	void setIxx(double ixx) { I_xx = ixx; }
	void setStress(double s) { stress = s; }
	void setAlphaMax(double alpha) { alphaMax = alpha; }
	void setIsOptimized(bool optimized) { is_optimized = optimized; }
	void setIsCircular(bool circular) { is_Circular = circular; }
	void setOmega(double inomega) { omega = inomega; }
	void setTorqueOut(double intorque) { torque_out = intorque; }
	void setCost(double c) { cost = c; }
};

//json functions
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
//

void newMaterial(json& j, const string& mname, const double& yield, const double& density)
{
	json newMaterial = {
		{"id",(j["materials"].size() + 1)},
		{"name", mname},
		{"sigma_yield", yield},
		{"density", density}
	};	
	j["materials"].push_back(newMaterial);
	savejson(j, "materials.json");
}

///////////////////////////////////////////////WILL BE EDITED TO BE ABLE TO DISPLAY OTHER TABLES
void displayMaterialsTable() {
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
void displayMotorsTable()
{
	cout << "\nAvailable Motors:\n";
	cout << " ID | Torque(Nm) | Speed(rpm) | Mass(kg) | Dia(mm) | Width(mm)\n";
	cout << "---------------------------------------------------------------\n";

	for (const auto& m : motors_json["motors"]) {
		cout << setw(3) << m["id"] << " | "
			<< setw(10) << m["nominal_torque"] << " | "
			<< setw(10) << m["no_load_speed"] << " | "
			<< setw(9) << m["weight"] << " | "
			<< setw(7) << m["diameter"] << " | "
			<< setw(9) << m["length"] << endl;
	}
}

void displayGearboxesTable()
{
	cout << "\nAvailable Gearboxes:\n";
	cout << " ID | Ratio | Eff | Mass(kg) | Dia(mm) | Width(mm)\n";
	cout << "--------------------------------------------------\n";

	for (const auto& g : gearboxes_json["gearboxes"]) {
		cout << setw(3) << g["id"] << " | "
			<< setw(5) << g["gear_ratio"] << " | "
			<< setw(4) << g["efficiency"] << " | "
			<< setw(9) << g["weight"] << " | "
			<< setw(7) << g["diameter"] << " | "
			<< setw(9) << g["length"] << endl;
	}
}

void createTableHeaders(vector<int> column_widths, vector<string> column_names) {
	int total_width = 0;
	cout << " ";
	for (int n : column_widths)
		total_width +=n;
	total_width += (column_widths.size() - 1) * 2 + 1; // account for "| " between columns and initial space

	cout << "" << left;

	for (int i=0; i<= column_widths.size()-1;i++)
	{
		if (i != 0)
			cout << "| ";
		cout << setw(column_widths[i]) << column_names[i];
	}
	cout << "\n";
	
	cout << " " << string(total_width, '-') << endl;
}

void fillTable(const vector<int>& column_widths, const json& j, string j_arrayname, const vector<string>& j_propnames) {

	for (const auto& m : j[j_arrayname]) {
		cout << " " << left;

		for (int i = 0; i < column_widths.size(); i++) {
			if (i != 0) cout << "| ";

			auto value = m[j_propnames[i]];

			
			if (value.is_string()) {
				cout << setw(column_widths[i]) << value.get<string>();
			}
			else if (value.is_number_integer()) {
				cout << setw(column_widths[i]) << value.get<int>();
			}
			else if (value.is_number_float()) {
				cout << setw(column_widths[i]) << value.get<double>();
			}
			else {
				
				cout << setw(column_widths[i]) << value;
			}
		}
		cout << "\n";
	}
}

void fillTable(vector<int> column_widths, vector<string> column_names, vector<vector<string>> data) {
	for (const auto& row : data) {
		cout << "" << left;
		for (int i=0; i < column_widths.size(); i++)
		{
			if (i != 0)
				cout << "| ";
			cout << setw(column_widths[i]) << row[i];
		}
		cout << "\n";
	}
}

//Calcualations functions
//Part1
double getLinkMass(double density, double L, double b, double h, double r, bool is_Circular)
{
	if (is_Circular)
	{return density * (M_PI * pow(r, 2) * L);}
	else
	{return density * (b * h * L);}
}

double getBendingMoment(double m_link, double m_payload, double L, double alphaMax)
{
	double g = 9.81; 
	double M_bending = (m_link * g * (L / 2.0)) + (m_payload * g * L) + (m_link * pow((L / 2.0), 2) * alphaMax) + (m_payload * pow(L, 2) * alphaMax);
	return M_bending;
}

double getMomentofInertia(double b, double h, double r, bool is_Circular)
{
	if (is_Circular)
	{return (M_PI * pow(r, 4)) / 4.0;}
	else 
	{return (b * pow(h, 3)) / 12.0;}
}

double getMaxStress(double M_bending, double h, double r, double I_xx, bool is_Circular) 
{
	if (is_Circular)
	{return (M_bending * r) / I_xx;}
	else
	{return (M_bending * (h / 2.0)) / I_xx;}
}
//

//Part2
double getRequiredTorque(double ml, double mp, double L, double amax)
{
	double g = 9.81;
	return (ml * g * (L / 2.0)) +
		(mp * g * L) +
		((ml * pow(L / 2.0, 2) + mp * pow(L, 2)) * amax);
}

double getOutputTorque(double Tmotor, double ratio, double eff)
{
	return Tmotor * ratio * (eff / 100.0);  // efficiency is stored as percentage (80-90)
}

double getOutputSpeed(double motor_rpm, double ratio)
{
	double omega_motor = motor_rpm * (2 * M_PI / 60.0); // rpm → rad/s
	return omega_motor / ratio;
}

//Input validators
double getValidateInput() {
	while (true)
	{
		string cin_char;
		cin >> cin_char;
		try {
			double value = stod(cin_char); // Try to convert to double
			return value;
			break;// If successful, return the double value
		}
		catch (const invalid_argument& e) {
			cout << "\n Please enter a valid number: ";
		}
	}
}

bool validateInput(char expectedchar, bool acceptbothcases)
{
	int capletter = 0;
	int smallletter = 0;

	while (true)
	{
		if (acceptbothcases)
		{
			if ((int)expectedchar < 91) {
				capletter = (int)expectedchar;
				smallletter = (int)expectedchar + 32;
			}
			char cin_char;
			cin >> cin_char;
			if ((int)cin_char == capletter || (int)cin_char == smallletter)
			{
				return true;
				break;
			}
		}
		else {
			int letter = (int)expectedchar;
			char cin_char;
			cin >> cin_char;
			if ((int)cin_char == letter)
			{
				return true;
				break;
			}
			else {
				cout << "\n Please enter a valid character: ";
			}
		}
		
	}
}

int main()
{
	RobotLink robot_link;
	
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
========================================================================================================)";
	this_thread::sleep_for(1s);
	cout << "\n \n ";
	while (true)
	{
		cout << "Please choose the Cross-section type for the link\n for Circular (c) for Recatngular (r)....  ";
		char cin_char;
		cin >> cin_char;
		cout << "\n";
		if (cin_char == 'c') { robot_link.setIsCircular(true); break; }
		else if (cin_char == 'r')
			break;
	}

	
	cout << "\n Please choose a material from this catalog.... \n";
	//read the json file and print the materials in a table format
	readjson(materials_json, "materials.json");

	cout << "\n";
	createTableHeaders({ 4,20,15,10 }, { "ID", "Material", "Yield (MPa)", "Density(gm/cm^3)" });
	fillTable({ 4,20,15,10 }, materials_json, "materials",{"id","name","sigma_yield","density"});
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
			newMaterial(materials_json, name, sigma_yield, density);
			displayMaterialsTable();
		}
		else
		{
			// 2. If it's not 'n', try to parse it as an ID
			try
			{
				int id = stoi(input);

				if (id > 0 && id <= materials_json["materials"].size())
				{
					robot_link.setMaterial(Material(id));
					break;
				}
				else
				{
					cout << "\nID out of range. Please type a valid material ID or (n)..\n";
				}
			}
			// 3. Catch errors if they type random letters like "abc"
			catch (const invalid_argument&)
			{
				cout << "\nInvalid input. Please enter a valid number or (n)...\n";
			}
		}
	}

	cout << "\nYou have selected material: " << robot_link.getMaterial().getName() << "\n\n";
	
	double b=0, h=0, r=0;

	cout << "Enter link length(m), Payload mass(kg) and Max angular acceleration(rad/s^2) respectively.." << endl;

	/*double length, payload, alpha;
	cin >> length >> payload >> alpha;*/
	robot_link.setL(getValidateInput());
	robot_link.setMPayload(getValidateInput());
	robot_link.setAlphaMax(getValidateInput());
	cout << endl;
	if (robot_link.getIsCircular())
	{
		cout << "Enter initial radius(m).." << endl;
		r = getValidateInput();
		cout << "\n";
	}
	else
	{
		cout << "Enter initial width(b) & hight(h) in meter.." << endl;
		b = getValidateInput();
		h = getValidateInput();
		cout << "\n";
	}


	double sigma_yield = static_cast<double>(robot_link.getMaterial().getSigmaYield()) * 1000000.0;
	double density     = static_cast<double>(robot_link.getMaterial().getDensity()) * 1000.0;
	
	////starting optimization
	double m_link =0, M_bending =0, I_xx =0, current_stress =0;
	

	while (!robot_link.getIsOptimized())
	{
		robot_link.setMLink(m_link = getLinkMass (density, robot_link.getL(), b, h, r, robot_link.getIsCircular()));
		robot_link.setMBending(M_bending = getBendingMoment (m_link, robot_link.getMPayload(), robot_link.getL(), robot_link.getAlphaMax()));
		robot_link.setIxx(I_xx = getMomentofInertia (b, h, r, robot_link.getIsCircular()));
		current_stress = getMaxStress(M_bending, h, r, I_xx, robot_link.getIsCircular());

		
		if (current_stress > sigma_yield)
		{
			if (robot_link.getIsCircular()) { r *= 1.01; }
			else { b *= 1.01; h *= 1.01; }
			cout << "Current stress (" << current_stress / 1000000.0 << " MPa) exceeds yield strength (" << sigma_yield / 1000000.0 << " MPa). Increasing dimensions..." << endl;
			this_thread::sleep_for(0.001s);
		}
		else if (current_stress < (sigma_yield * 0.97))
		{
			if (robot_link.getIsCircular()) { r *= 0.99; }
			else { b *= 0.99; h *= 0.99; }
			cout << "Current stress (" << current_stress / 1000000.0 << " MPa) is significantly below yield strength (" << sigma_yield / 1000000.0 << " MPa). Decreasing dimensions..." << endl;
			this_thread::sleep_for(0.001s);
		}
		else { robot_link.setIsOptimized(true);}
	}
	robot_link.setStress(current_stress);
	robot_link.setB(b);
	robot_link.setH(h);
	robot_link.setR(r);
	this_thread::sleep_for(0.5s);
	cout << "\n ======================================OPTIMIZATION RESULTS====================================-" << "\n \n";

	//if (is_Circular) 
	//	cout << "Safe Radius (r): " << r << "m" << " = " << r*100 << "cm" << "\n";
	//else 
	//	cout << "Safe Width(b):" << b << "m = " << b*100 << " cm. \n \n" << "Safe Height(h):" << h << "m = " << h * 100 << " cm. \n \n";

	createTableHeaders({ 40,20,20 }, { "  Dimensions","Link Mass(kg)","Max Stress(MPa)"});

	fillTable({ 40,20,20 }, { "Dimensions","Link Mass(kg)", "Max Stress(MPa)" }, { {robot_link.getIsCircular() ? "r = "+to_string(r) + "m or "+ to_string(r*100) + "cm" : "b = "+to_string(b) + "m & "+ "h = "+ to_string(h)+"m",to_string(m_link), to_string(current_stress / 1000000.0)},{robot_link.getIsCircular() ? "" : "b = " + to_string(b*100) + "cm & " + "h = " + to_string(h*100) + "cm","", ""}});

	//cout << "Final Link Mass:  " << m_link << " kg" << "\n \n";
	//cout << "Final Max Stress: " << current_stress / 1000000.0 << " MPa" << "\n \n";
	this_thread::sleep_for(3s);
	cout << "\n\n ##################################################################################################-" << "\n \n";

	cout << "\n ============================================MOTORS=============================================-" << "\n \n";


	// Load JSON files
	readjson(motors_json, "motors.json");
	readjson(gearboxes_json, "gearboxes.json");

	// Display tables
	//----------------------------------
	//Motors Table
	//---------------------------------
	createTableHeaders({ 4,15,15,10,10,10,10 }, { "ID", "Motor Name", "NOL Speed(rpm)", "Nom.T (Nm)","Stall T","Efficen.","Dia."});
	fillTable({ 4,15,15,10,10,10,10 }, motors_json, "motors", { "id","name","no_load_speed","nominal_torque","stall_torque","efficiency","diameter" });
	//----------------------------------
	//Gearboxes Table
	//---------------------------------
	cout << "\n \n ============================================GEARBOXES=============================================-" << "\n \n";
	createTableHeaders({ 4,15,10,10,10,10}, { "ID", "GPX Name", "Gear %", "Max.T (Nm)","Efficen.","Dia." });
	fillTable({ 4,15,10,10,10,10}, gearboxes_json, "gearboxes", { "id","name","gear_ratio","max_torque","efficiency","diameter" });
	// -----------------------------
	// USER INPUT
	// -----------------------------
	double omega_required;
	cout << "\n Enter required output speed (rad/s): ";
	omega_required = getValidateInput();

	// -----------------------------
	// REQUIRED TORQUE
	// -----------------------------
	double T_required = getRequiredTorque(robot_link.getMLink(), robot_link.getMPayload(),robot_link.getL(), robot_link.getAlphaMax());

	cout << "\nRequired Torque = " << T_required << " N.m\n";

	// -----------------------------
	// OPTIMIZATION LOOP
	// -----------------------------
	double best_cost = 1e9;
	json best_choice;

	for (const auto& m : motors_json["motors"])
	{
		for (const auto& g : gearboxes_json["gearboxes"])
		{
			// Use correct field names from JSON files
			double T_out = getOutputTorque(m["nominal_torque"], g["gear_ratio"], g["efficiency"]);
			double omega_out = getOutputSpeed(m["no_load_speed"], g["gear_ratio"]);

			if (T_out >= T_required && omega_out >= omega_required)
			{
				// Use 'weight' for mass and 'length' for width
				double mass = m["weight"].get<double>() + g["weight"].get<double>();
				double diameter = m["diameter"].get<double>() + g["diameter"].get<double>();
				double width = m["length"].get<double>() + g["length"].get<double>();

				double cost = mass + diameter / 100.0 + width / 100.0;

				if (cost < best_cost)
				{
					best_cost = cost;
					best_choice = {
						{"motor_id", m["id"]},
						{"gearbox_id", g["id"]},
						{"T_out", T_out},
						{"omega_out", omega_out},
						{"cost", cost}
					};
				}
			}
		}
	}
	robot_link.setMotor((int)best_choice["motor_id"]);
	robot_link.setGearbox((int)best_choice["gearbox_id"]);
	robot_link.setTorqueOut(best_choice["T_out"]);
	robot_link.setOmega(best_choice["omega_out"]);
	robot_link.setCost(best_choice["cost"]);

	// -----------------------------
	// OUTPUT RESULT
	// -----------------------------
	if (!best_choice.empty())
	{
		cout << "\n====== BEST COMBINATION ======\n";
		cout << "Motor ID: " << best_choice["motor_id"] << endl;
		cout << "Gearbox ID: " << best_choice["gearbox_id"] << endl;
		cout << "Output Torque: " << best_choice["T_out"] << " Nm\n";
		cout << "Output Speed: " << best_choice["omega_out"] << " rad/s\n";
		cout << "Cost: " << best_choice["cost"] << endl;
	}
	else
	{
		cout << "\nNo valid combination found.\n";
	}
	return 0;
}
