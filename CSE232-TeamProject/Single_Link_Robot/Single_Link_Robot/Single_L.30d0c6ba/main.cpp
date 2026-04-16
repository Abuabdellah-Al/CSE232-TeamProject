//main of part 2                                                                                                                     
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// -----------------------------
// GLOBAL JSON OBJECTS
// -----------------------------
json motors_json;
json gearboxes_json;

// -----------------------------
// JSON FUNCTIONS
// -----------------------------
void readjson(json& j, const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open " << filename << endl;
        return;
    }
    file >> j;
    file.close();
}

// -----------------------------
// TABLE DISPLAY FUNCTIONS
// -----------------------------
void DisplayMotors()
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

void DisplayGearboxes()
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

// -----------------------------
// CALCULATION FUNCTIONS
// -----------------------------
double RequiredTorque(double ml, double mp, double L, double amax)
{
    double g = 9.81;
    return (ml * g * (L / 2.0)) +
        (mp * g * L) +
        ((ml * pow(L / 2.0, 2) + mp * pow(L, 2)) * amax);
}

double OutputTorque(double Tmotor, double ratio, double eff)
{
    return Tmotor * ratio * (eff / 100.0);  // efficiency is stored as percentage (80-90)
}

double OutputSpeed(double motor_rpm, double ratio)
{
    double omega_motor = motor_rpm * (2 * M_PI / 60.0); // rpm → rad/s
    return omega_motor / ratio;
}

// -----------------------------
// MAIN
// -----------------------------
int main()
{
    cout << "\n=========== MOTOR-GEARBOX OPTIMIZATION ===========\n";

    // Load JSON files
    readjson(motors_json, "motors.json");
    readjson(gearboxes_json, "gearboxes.json");

    // Display tables
    DisplayMotors();
    DisplayGearboxes();

    // -----------------------------
    // USER INPUT
    // -----------------------------
    double ml, mp, L, amax, omega_required;

    cout << "\nEnter link mass (kg), payload (kg), length (m), max accel (rad/s^2):\n";
    cin >> ml >> mp >> L >> amax;

    cout << "Enter required output speed (rad/s): ";
    cin >> omega_required;

    // -----------------------------
    // REQUIRED TORQUE
    // -----------------------------
    double T_required = RequiredTorque(ml, mp, L, amax);

    cout << "\nRequired Torque = " << T_required << " Nm\n";

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
            double T_out = OutputTorque(m["nominal_torque"], g["gear_ratio"], g["efficiency"]);
            double omega_out = OutputSpeed(m["no_load_speed"], g["gear_ratio"]);

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