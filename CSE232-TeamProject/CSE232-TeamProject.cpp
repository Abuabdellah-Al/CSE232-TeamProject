#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip> // input outpt manip
#include <string>
#include <thread> // for slep thred
using namespace std; // standrad namespace

// short hand cout func
void print(string text)
{
    cout << text;
}

// input validatorrs

// returns a valid double
double getValidatedDouble()
{
    while (true)
    {
        string cin_char;
        cin >> cin_char;
        try
        {
            double value = stod(cin_char); // stod string to doubl convert
            return value;
        }
        catch (const invalid_argument& e)
        {
            cout << "\n Please enter a valid number: ";
        }
    }
}

// returns valdiated char and accept any char in allowed string
// if acceptbothcases is true it lowercases first
char validateChar(string allowed, bool acceptBothCases) // validatechar("nc",true)
{
    while (true)
    {
        char cin_char;
        cin >> cin_char; // user input a

        if (acceptBothCases)
            cin_char = tolower(cin_char); // convert A to a

        for (char ch : allowed)
        {
            char compare = acceptBothCases ? tolower(ch) : ch; // if condtional
            if (cin_char == compare)
                return cin_char;
        }

        cout << "\n Please enter a valid character: ";
    }
}

// validates and returns id betweeen 1 and max
int getValidatedID(int max)
{
    while (true)
    {
        double val = getValidatedDouble();
        int id = (int)val;

        if (id >= 1 && id <= max && (double)id == val)
            return id;

        cout << "\n Please enter a valid ID between 1 and " << max << ": ";
    }
}

// classes

// class for materail with props and constr
class Material {
public:
    int id;
    string name;
    double sigma_yield;
    double density;

    Material(int i, string n, double s, double d)
    {
        id = i;
        name = n;
        sigma_yield = s;
        density = d;
    }
};

// class for motor props and constr
class Motor {
public:
    int id;
    string name;
    string manufacturer;

    double torque;
    double speed;
    double weight;
    double diameter;
    double length;
    double efficiency;

    Motor(int i, string n, string man, double t, double s, double w, double d, double l, double e)
    {
        id = i;
        name = n;
        manufacturer = man;

        torque = t;
        speed = s;
        weight = w;
        diameter = d;
        length = l;
        efficiency = e;
    }
};

// gearbox class
class Gearbox {
public:
    int id;
    string name;
    string manufacturer;

    double ratio;
    double efficiency;
    double weight;
    double diameter;
    double length;
    double max_torque;

    Gearbox(int i, string n, string man, double r, double e, double w, double d, double l, double mt)
    {
        id = i;
        name = n;
        manufacturer = man;

        ratio = r;
        efficiency = e;
        weight = w;
        diameter = d;
        length = l;
        max_torque = mt;
    }
};

// final combnation class
class Combination {
public:

    int motor_id;
    int gearbox_id;

    string motor_name;
    string gearbox_name;

    double torque;
    double speed;

    double mass;
    double cost;

    Combination(int m, int g, string mn, string gn, double t, double s, double ma, double c)
    {
        motor_id = m;
        gearbox_id = g;

        motor_name = mn;
        gearbox_name = gn;

        torque = t;
        speed = s;

        mass = ma;
        cost = c;
    }
};

// enginering funcitons

double getLinkMass(double density, double L, double b, double h, double r, bool is_Circular)
{
    if (is_Circular)
    {
        return density * (M_PI * pow(r, 2) * L);
    }
    else
    {
        return density * (b * h * L);
    }
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
    {
        return (M_PI * pow(r, 4)) / 4.0;
    }
    else
    {
        return (b * pow(h, 3)) / 12.0;
    }
}

double getMaxStress(double M_bending, double h, double r, double I_xx, bool is_Circular)
{
    if (is_Circular)
    {
        return (M_bending * r) / I_xx;
    }
    else
    {
        return (M_bending * (h / 2.0)) / I_xx;
    }
}

// part 2
double getRequiredTorque(double ml, double mp, double L, double amax)
{
    double g = 9.81;
    return (ml * g * (L / 2.0)) +
        (mp * g * L) +
        ((ml * pow(L / 2.0, 2) + mp * pow(L, 2)) * amax);
}

double getOutputTorque(double Tmotor, double ratio, double eff)
{
    return Tmotor * ratio * (eff / 100.0);  // effciency is stored as precentage
}

double getOutputSpeed(double motor_rpm, double ratio)
{
    double omega_motor = motor_rpm * (2 * M_PI / 60.0); // rpm to rad/s
    return omega_motor / ratio;
}

// disply fucntions

void displayMaterials(vector<Material>& materials)
{
    print("\n=============== MATERIALS ===============\n");

    for (auto& m : materials)
    {
        cout
            << "ID: " << m.id
            << " | " << m.name
            << " | Yield: " << m.sigma_yield / 1e6
            << " MPa"
            << " | Density: " << m.density
            << " kg/m^3\n";
    }
}

void displayMotors(vector<Motor>& motors)
{
    print("\n================ MOTORS =================\n");

    for (auto& m : motors)
    {
        cout
            << "ID: " << m.id
            << " | " << m.name
            << " | Torque: " << m.torque
            << " Nm"
            << " | Speed: " << m.speed
            << " rpm\n";
    }
}

void displayGearboxes(vector<Gearbox>& gears)
{
    print("\n=============== GEARBOXES ===============\n");

    for (auto& g : gears)
    {
        cout
            << "ID: " << g.id
            << " | " << g.name
            << " | Ratio: " << g.ratio
            << " | Efficiency: " << g.efficiency
            << "%\n";
    }
}

// main loop

int main()
{
    cout << fixed << setprecision(3);

    // databasses

    vector<Material> materials = {

        {1, "Cast iron",       130e6, 7300},
        {2, "Copper nickel",   130e6, 8940},
        {3, "Brass",           200e6, 8730},
        {4, "Aluminum",        241e6, 2700},
        {5, "Steel",           247e6, 7580},
        {6, "Acrylic",          72e6, 1160},
        {7, "Copper",           70e6, 8920},
        {8, "Stainless steel", 275e6, 7860},
        {9, "Tungsten",        941e6, 19250}
    };

    vector<Motor> motors = {
        // micro motors 10-16mm ultra light
        {1, "RE 10",                  "maxon", 0.0008, 14000, 0.011, 10, 25,  72},
        {2, "DCX 10 L",               "maxon", 0.0014, 12000, 0.015, 10, 25,  75},
        {3, "RE 13",                  "maxon", 0.0024, 11000, 0.025, 13, 28,  75},
        {4, "EC 13",                  "maxon", 0.0035, 12000, 0.030, 13, 30,  76},
        {5, "ECX SPEED 13 M",         "maxon", 0.0042, 16000, 0.032, 13, 30,  78},
        {6, "RE 16",                  "maxon", 0.0045, 11000, 0.035, 16, 40,  77},
        {7, "EC 16",                  "maxon", 0.0052, 14000, 0.030, 16, 34,  78},
        {8, "DCX 16 S",               "maxon", 0.0060, 10000, 0.040, 16, 35,  76},
        {9, "ECX SPEED 16 L",         "maxon", 0.0080, 15000, 0.045, 16, 40,  80},
        {10, "EC 20 flat",            "maxon", 0.0085, 10000, 0.015, 20, 14,  70},

        // small motors work horses
        {11, "RE 25",                 "maxon", 0.0150, 10000, 0.130, 25, 45,  85},
        {12, "DCX 22 L",              "maxon", 0.0160,  9500, 0.140, 22, 54,  82},
        {13, "EC 22",                 "maxon", 0.0180, 11000, 0.150, 22, 55,  83},
        {14, "ECX TORQUE 22 M",       "maxon", 0.0190,  9000, 0.160, 22, 45,  84},
        {15, "EC-i 30",               "maxon", 0.0200,  9000, 0.250, 30, 50,  88},
        {16, "RE 30",                 "maxon", 0.0250,  8500, 0.240, 30, 68,  86},
        {17, "EC-max 30",             "maxon", 0.0250, 11000, 0.220, 30, 55,  86},
        {18, "EC 32 flat",            "maxon", 0.0280,  8000, 0.060, 32, 18,  75},
        {19, "DCX 32 L",              "maxon", 0.0350,  8000, 0.260, 32, 60,  85},
        {20, "ECX TORQUE 22 XL",      "maxon", 0.0380,  8500, 0.200, 22, 65,  85},

        // meduim motors for robotics
        {21, "RE 35",                 "maxon", 0.0400,  8000, 0.340, 35, 57,  87},
        {22, "EC 40",                 "maxon", 0.0450,  7000, 0.400, 40, 70,  88},
        {23, "EC-i 40",               "maxon", 0.0510,  7580, 0.480, 40, 59,  89},
        {24, "EC 45 flat",            "maxon", 0.0600,  6000, 0.110, 45, 22,  80},
        {25, "RE 40",                 "maxon", 0.0949,  6380, 0.480, 40, 71,  88},
        {26, "EC-max 40",             "maxon", 0.0700,  9000, 0.600, 40, 68,  89},
        {27, "EC-i 40 High Torque",   "maxon", 0.0789,  6080, 0.180, 40, 50,  86},
        {28, "EC-flat 45 70W",        "maxon", 0.1000,  5000, 0.400, 45, 25,  90},
        {29, "DCX 35 L",              "maxon", 0.1100,  7000, 0.450, 35, 75,  86},
        {30, "ECX SPEED 36 M",        "maxon", 0.1150,  8000, 0.500, 36, 70,  87},

        // heavy mid range high torq
        {31, "EC-i 52",               "maxon", 0.1200,  6000, 0.800, 52, 70,  90},
        {32, "EC 45 flat 130W",       "maxon", 0.1300,  5500, 0.220, 45, 30,  82},
        {33, "RE 50 Standard",        "maxon", 0.1500,  6500, 0.800, 50, 80,  90},
        {34, "ECX FLAT 42 S",         "maxon", 0.1710,  9120, 0.120, 42, 16,  85},
        {35, "EC frameless DT 38 M",  "maxon", 0.1860,  6290, 0.160, 42, 22,  85},
        {36, "EC-i 52 XL",            "maxon", 0.2100,  5000, 0.850, 52, 90,  89},
        {37, "EC 60 flat short",      "maxon", 0.2100,  4500, 0.350, 60, 30,  83},
        {38, "ECX FLAT 42 L",         "maxon", 0.2500,  8000, 0.200, 42, 25,  86},
        {39, "EC frameless DT 50 M",  "maxon", 0.2800,  5000, 0.250, 50, 25,  85},
        {40, "EC 60 flat",            "maxon", 0.3150,  4000, 0.470, 60, 38,  85},

        // industrail heavy weights
        {41, "EC 60 flat 150W",       "maxon", 0.4050,  3480, 0.350, 60, 30,  85},
        {42, "RE 50 High Power",      "maxon", 0.4300,  5600, 1.100, 50, 108, 93},
        {43, "EC 90 flat short",      "maxon", 0.5000,  3500, 0.600, 90, 33,  85},
        {44, "EC 90 flat",            "maxon", 0.5400,  3240, 0.360, 90, 33,  85},
        {45, "EC 90 flat 160W",       "maxon", 0.6000,  3000, 0.650, 90, 40,  86},
        {46, "EC frameless DT 85 M",  "maxon", 0.7500,  3500, 0.600, 85, 30,  85},
        {47, "EC frameless HT 76 S",  "maxon", 0.8830,  4130, 0.820, 76, 40,  85},
        {48, "RE 65",                 "maxon", 0.9000,  4000, 2.100, 65, 130, 90},
        {49, "EC 90 flat 260W",       "maxon", 1.1000,  2500, 0.800, 90, 45,  87},
        {50, "EC frameless HT 90 M",  "maxon", 2.7500,  2490, 1.500, 90, 50,  85}
    };

    vector<Gearbox> gears = {
        
        {1, "GP 10 A (2-stage)",   "maxon",  16, 81, 0.005, 10, 10.0,  0.10},
        {2, "GP 10 K (3-stage)",   "maxon",  64, 73, 0.007, 10, 14.0,  0.15},
        {3, "GP 13 A (2-stage)",   "maxon",  12, 81, 0.009, 13, 12.0,  0.20},
        {4, "GP 13 A (3-stage)",   "maxon",  64, 73, 0.012, 13, 16.0,  0.25},
        {5, "GPX 14 A (2-stage)",  "maxon",  16, 85, 0.015, 14, 15.0,  0.30},
        {6, "GPX 14 A (3-stage)",  "maxon",  64, 75, 0.018, 14, 19.0,  0.40},
        {7, "GP 16 A (2-stage)",   "maxon",  29, 73, 0.015, 16, 18.0,  0.50},
        {8, "GP 16 A (4-stage)",   "maxon", 150, 65, 0.020, 16, 24.0,  0.60},
        {9, "GPX 16 A (2-stage)",  "maxon",  21, 80, 0.050, 16, 25.0,  0.60},
        {10, "GPX 16 A (4-stage)", "maxon", 156, 70, 0.065, 16, 35.0,  0.80},
        {11, "GP 22 A (1-stage)",  "maxon",   4, 90, 0.080, 22, 28.0,  0.80},
        {12, "GPX 22 Std (2-stg)", "maxon",  30, 85, 0.100, 22, 35.0,  1.20},
        {13, "GP 22 A (3-stage)",  "maxon",  84, 73, 0.120, 22, 45.0,  1.50},
        {14, "GPX 22 Std (4-stg)", "maxon", 156, 75, 0.140, 22, 55.0,  2.00},
        {15, "GP 26 A (1-stage)",  "maxon",   5, 90, 0.100, 26, 30.0,  1.50},
        {16, "GP 26 A (2-stage)",  "maxon",  25, 82, 0.150, 26, 45.0,  2.50},
        {17, "GPX 26 Std (3-stg)", "maxon", 103, 75, 0.180, 26, 50.0,  4.00},
        {18, "GPX 26 HP (3-stg)",  "maxon", 103, 85, 0.200, 26, 55.0,  5.00},
        {19, "GP 26 A (4-stage)",  "maxon", 150, 70, 0.220, 26, 60.0,  3.00},
        {20, "GPX 26 HP (4-stg)",  "maxon", 156, 80, 0.250, 26, 65.0,  6.00},
        {21, "GP 32 A (1-stage)",  "maxon",   6, 90, 0.200, 32, 40.0,  2.50},
        {22, "GPX 32 Std (2-stg)", "maxon",  28, 85, 0.250, 32, 45.0,  4.50},
        {23, "GPX 32 Std (3-stg)", "maxon", 103, 75, 0.280, 32, 59.5,  5.00},
        {24, "GP 32 C (3-stage)",  "maxon", 111, 75, 0.350, 32, 65.0,  6.00},
        {25, "GPX 32 HT (3-stg)",  "maxon", 121, 75, 0.280, 32, 65.0,  8.00},
        {26, "GPX 32 UP (3-stg)",  "maxon",  79, 87, 0.285, 32, 59.5, 12.00},
        {27, "GPX 32 Std (4-stg)", "maxon", 150, 75, 0.280, 32, 59.5,  5.00},
        {28, "GP 42 C (2-stage)",  "maxon",  26, 81, 0.400, 42, 60.0, 10.00},
        {29, "GP 42 C (3-stage)",  "maxon", 100, 81, 0.350, 42, 74.0, 15.00},
        {31, "GPX 42 HT (3-stg)",  "maxon", 156, 75, 0.450, 42, 85.0, 22.00},
        {32, "GPX 42 UP (2-stg)",  "maxon",  35, 93, 0.520, 42, 68.0, 18.00},
        {33, "GPX 42 UP (3-stg)",  "maxon",  74, 87, 0.610, 42, 85.0, 26.00},
        {34, "GPX 42 UP (4-stg)",  "maxon", 156, 80, 0.790, 42, 104.5, 35.00},
        {35, "GP 52 C (1-stage)",  "maxon",   4, 90, 0.500, 52, 60.0,  6.00},
        {36, "GP 52 C (2-stage)",  "maxon",  26, 81, 0.650, 52, 75.0, 15.00},
        {37, "GP 52 C (3-stage)",  "maxon", 120, 80, 0.600, 52, 85.0, 10.00},
        {38, "GP 52 C (4-stage)",  "maxon", 150, 75, 0.750, 52, 95.0, 30.00},
        {39, "GP 62 C (2-stage)",  "maxon",  26, 85, 1.000, 62, 80.0, 25.00},
        {40, "GP 62 C (3-stage)",  "maxon", 150, 78, 1.100, 62, 100.0, 20.00},
        {41, "GP 62 C HT (3-stg)", "maxon", 150, 80, 1.200, 62, 110.0, 50.00},
        {42, "GP 81 A (1-stage)",  "maxon",   6, 90, 1.300, 81, 80.0, 30.00},
        {43, "GP 81 A (2-stage)",  "maxon",  25, 82, 1.500, 81, 95.0, 80.00},
        {44, "GP 81 A Mid (3-stg)","maxon",  60, 78, 1.600, 81, 100.0, 120.00},
        {45, "GP 81 A (3-stage)",  "maxon", 100, 75, 1.800, 81, 110.0, 120.00},
        {46, "GP 81 A (4-stage)",  "maxon", 156, 70, 2.000, 81, 125.0, 120.00},
        {47, "GP 120 A (2-stage)", "maxon",  26, 80, 3.500, 120, 120.0, 150.00},
        {48, "GP 120 A Mid (3-stg)","maxon",  60, 75, 4.000, 120, 135.0, 200.00},
        {49, "GP 120 A (3-stage)", "maxon", 100, 75, 4.200, 120, 150.0, 200.00},
        {50, "GP 120 A (4-stage)", "maxon", 156, 70, 4.500, 120, 165.0, 200.00}
    };

    // user inpt
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
    this_thread::sleep_for(chrono::seconds(1));
    print("\n========================================\n");
    print(" WELCOME TO SINGLE LINK ROBOT DESIGN OPTIMIZATION\n");
    print("========================================\n");

    print("\nChoose cross section:\n");
    print("c -> Circular\n");
    print("r -> Rectangular\n");

    // valdiate char only c or r
    char inputchar = validateChar("cr", true);
    bool circular = (inputchar == 'c');

    displayMaterials(materials);

    int material_id = 0;
    cout << "\nType the material ID to choose, or to add a new press (n): ";

    while (true)
    {
        string input;
        cin >> input;

        // 1 check for new comand first
        if (input == "n" || input == "N")
        {
            string name;
            double sigma_yield, density;

            cout << "\nPlease enter the material name..."; cin >> name;
            cout << "\nmaterial yield strength (MPa)..."; cin >> sigma_yield;
            cout << "\nmaterial density (kg/m^3)..."; cin >> density;
            materials.push_back(Material(materials.size() + 1, name, sigma_yield * 1000000, density));
            material_id = materials.size();
            break;
        }
        else
        {
            // 2 if not n try parse as id
            try
            {
                int id = stoi(input);

                if (id > 0 && id <= materials.size())
                {
                    material_id = id;
                    break;
                }
                else
                {
                    cout << "\nID out of range. Please type a valid material ID or (n)..\n";
                }
            }
            // 3 catch errs if they type random stuff
            catch (const invalid_argument&)
            {
                cout << "\nInvalid input. Please enter a valid number or (n)...\n";
            }
        }
    }

    //// validated id must be whole number

    Material mat = materials[material_id - 1];

    print("\nEnter link length (m): ");

    // get double rejects non numbers
    double L = getValidatedDouble();

    print("\nEnter payload mass (kg): ");

    // get double rejects non numbers
    double mp = getValidatedDouble();

    print("\nEnter max angular acceleration (rad/s^2): ");

    // get double rejects non numbers
    double a = getValidatedDouble();

    double b = 0;
    double h = 0;
    double r = 0;

    if (circular)
    {
        print("\nEnter initial radius (m): ");

        // get double rejects non numbers
        r = getValidatedDouble();
    }
    else
    {
        print("\nEnter initial width b (m): ");

        // get double rejects non numbers
        b = getValidatedDouble();

        print("\nEnter initial height h (m): ");

        // get double rejects non numbers
        h = getValidatedDouble();
    }

    // part 1 link optimzation

    double stress = 0;
    int iterations = 0;

    while (iterations < 10000)
    {
        double ml = getLinkMass(mat.density, L, b, h, r, circular);

        double M = getBendingMoment(ml, mp, L, a);

        double I = getMomentofInertia(b, h, r, circular);

        stress = getMaxStress(M, h, r, I, circular);

        if (stress > mat.sigma_yield)
        {
            if (circular)
                r *= 1.01;
            else
            {
                b *= 1.01;
                h *= 1.01;
            }
        }
        else if (stress < 0.97 * mat.sigma_yield)
        {
            if (circular)
                r *= 0.99;
            else
            {
                b *= 0.99;
                h *= 0.99;
            }
        }
        else
        {
            break;
        }

        iterations++;
    }

    // final link resullts

    double final_mass = getLinkMass(mat.density, L, b, h, r, circular);

    print("\n========================================\n");
    print(" PART 1 RESULTS\n");
    print("========================================\n");

    if (circular)
    {
        cout << "Optimized Radius = " << r << " m\n";
    }
    else
    {
        cout << "Optimized Width  = " << b << " m\n";
        cout << "Optimized Height = " << h << " m\n";
    }

    cout << "Final Stress = " << stress / 1e6 << " MPa\n";
    cout << "Final Link Mass = " << final_mass << " kg\n";

    this_thread::sleep_for(chrono::seconds(3));

    // part 2 motor gbx optimzation

    displayMotors(motors);
    displayGearboxes(gears);

    double T_required = getRequiredTorque(final_mass, mp, L, a);

    cout << "\nRequired Torque = " << T_required << " Nm\n";

    print("\nEnter required output speed (rad/s): ");

    // get double rejects non numbers
    double omega_required = getValidatedDouble();

    // show powr required after choosing speed
    cout << "Required Power = " << T_required * omega_required << " W\n";

    print("\nOptimization Priority:\n");
    print("1 -> Minimize Cost\n");
    print("2 -> Minimize Weight\n");

    // valdiate char only 0 or 1
    char choiceChar = validateChar("12", false);
    int choice = choiceChar - '0';

    // find validd combos

    vector<Combination> valid;

    for (auto& m : motors)
    {
        for (auto& g : gears)
        {
            double T_out = getOutputTorque(m.torque, g.ratio, g.efficiency);

            double omega_out = getOutputSpeed(m.speed, g.ratio);

            if (
                T_out >= T_required &&
                omega_out >= omega_required &&
                T_out <= g.max_torque
                )
            {
                double mass = m.weight + g.weight;

                double cost =
                    mass +
                    (m.diameter + g.diameter) / 100.0 +
                    (m.length + g.length) / 100.0;

                valid.push_back(Combination(m.id, g.id, m.name, g.name, T_out, omega_out, mass, cost));
            }
        }
    }

    // show valid combis

    print("\n========================================\n");
    print(" VALID COMBINATIONS\n");
    print("========================================\n");

    if (valid.empty())
    {
        print("No valid combination found.\n");
        return 0;
    }

    cout << left
        << setw(25) << "Motor Name" << " | "
        << setw(25) << "Gearbox Name" << " | "
        << setw(10) << "Torque" << " | "
        << setw(10) << "Speed" << " | "
        << setw(10) << "Mass" << " | "
        << setw(10) << "Cost" << endl;
    cout << string(25 + 25 + 10 + 10 + 10 + 10 + 15, '-') << endl;

    for (auto& v : valid)
    {
        cout << left
            << setw(25) << v.motor_name << " | "
            << setw(25) << v.gearbox_name << " | "
            << setw(10) << v.torque << " | "
            << setw(10) << v.speed << " | "
            << setw(10) << v.mass << " | "
            << setw(10) << v.cost << endl;
    }

    // best combo

    Combination best = valid[0];

    if (choice == 1)
    {
        for (auto& v : valid)
            if (v.cost < best.cost)
                best = v;
    }
    else
    {
        for (auto& v : valid)
            if (v.mass < best.mass)
                best = v;
    }

    // fianl results

    print("\n========================================\n");
    print(" BEST COMBINATION\n");
    print("========================================\n\n\n");

    cout << left
        << setw(5) << "MTRId" << " | "
        << setw(15) << "MTR Name" << " | "
        << setw(5) << "GPXId" << " | "
        << setw(20) << "GPX Name" << " | "
        << setw(10) << "O/p Torque" << " | "
        << setw(10) << "O/p Speed" << " | "
        << setw(10) << "Total Mass" << " | "
        << setw(10) << "Cost Function" << endl;
    cout << string(5 + 15 + 5 + 20 + 10 + 10 + 10 + 10 + 24, '-') << endl;
    cout << left
        << setw(5) << best.motor_id << " | "
        << setw(15) << best.motor_name << " | "
        << setw(5) << best.gearbox_id << " | "
        << setw(20) << best.gearbox_name << " | "
        << setw(10) << best.torque << " | "
        << setw(10) << best.speed << " | "
        << setw(10) << best.mass << " | "
        << setw(10) << best.cost << endl;

    print("\n========================================\n");
    print(" PROGRAM FINISHED SUCCESSFULLY\n");
    print("========================================\n");

    return 0;
}
