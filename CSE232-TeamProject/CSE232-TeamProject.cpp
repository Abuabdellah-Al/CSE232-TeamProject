#define _USE_MATH_DEFINES

#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace std;

// =====================================================
// SLOW PRINT FUNCTION
// =====================================================

void slowPrint(string text, int delay = 20)
{
    for (char c : text)
    {
        cout << c << flush;

        this_thread::sleep_for(
            chrono::milliseconds(delay)
        );
    }
}

// =====================================================
// CLASSES
// =====================================================

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

    Motor(
        int i,
        string n,
        string man,
        double t,
        double s,
        double w,
        double d,
        double l,
        double e
    )
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

    Gearbox(
        int i,
        string n,
        string man,
        double r,
        double e,
        double w,
        double d,
        double l,
        double mt
    )
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

// NOTE: Add an object of type gearbox and motor to this class insetd of seperate props
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

    Combination(
        int m,
        int g,
        string mn,
        string gn,
        double t,
        double s,
        double ma,
        double c
    )
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

// =====================================================
// ENGINEERING FUNCTIONS
// =====================================================

double getLinkMass(
    double density,
    double L,
    double b,
    double h,
    double r,
    bool circular
)
{
    if (circular)
        return density * M_PI * r * r * L;

    return density * b * h * L;
}

double getMoment(
    double ml,
    double mp,
    double L,
    double a
)
{
    double g = 9.81;

    return
        (ml * g * (L / 2.0)) +
        (mp * g * L) +
        ((ml * pow(L / 2.0, 2) + mp * pow(L, 2)) * a);
}

double getI(
    double b,
    double h,
    double r,
    bool circular
)
{
    if (circular)
        return (M_PI * pow(r, 4)) / 4.0;

    return (b * pow(h, 3)) / 12.0;
}

double getStress(
    double M,
    double h,
    double r,
    double I,
    bool circular
)
{
    if (circular)
        return (M * r) / I;

    return (M * (h / 2.0)) / I;
}

double getTorque(
    double ml,
    double mp,
    double L,
    double a
)
{
    double g = 9.81;

    return
        (ml * g * (L / 2.0)) +
        (mp * g * L) +
        ((ml * pow(L / 2.0, 2) + mp * pow(L, 2)) * a);
}

double getOutputTorque(
    double Tmotor,
    double ratio,
    double eff
)
{
    return Tmotor * ratio * (eff / 100.0);
}

double getOutputSpeed(
    double motor_rpm,
    double ratio
)
{
    double omega_motor =
        motor_rpm * (2.0 * M_PI / 60.0);

    return omega_motor / ratio;
}

// =====================================================
// DISPLAY FUNCTIONS
// =====================================================

void displayMaterials(vector<Material>& materials)
{
    slowPrint("\n=============== MATERIALS ===============\n");

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
    slowPrint("\n================ MOTORS =================\n");

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
    slowPrint("\n=============== GEARBOXES ===============\n");

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

// =====================================================
// MAIN
// =====================================================

int main()
{
    cout << fixed << setprecision(3);

    // =====================================================
    // DATABASES
    // =====================================================

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

        {1, "EC-i 30", "maxon", 0.020, 9000, 0.25, 30, 50, 88},
        {2, "EC-i 40", "maxon", 0.051, 7580, 0.48, 40, 59, 89},
        {3, "EC-i 52", "maxon", 0.120, 6000, 0.80, 52, 70, 90},
        {4, "RE 25",   "maxon", 0.015,10000, 0.13, 25, 45, 85},
        {5, "RE 35",   "maxon", 0.040, 8000, 0.34, 35, 57, 87},
        {6, "RE 40",   "maxon", 0.060, 7500, 0.50, 40, 60, 88},
        {7, "EC-max 30","maxon",0.025,11000, 0.22, 30, 55, 86},
        {8, "EC-max 40","maxon",0.070, 9000, 0.60, 40, 68, 89},
        {9, "EC-flat 45","maxon",0.100,5000, 0.40, 45, 25, 90}
    };

    vector<Gearbox> gears = {

        {1, "GP 22 A", "maxon", 50, 80, 0.12, 22, 40, 1.5},
        {2, "GP 26 A", "maxon", 66, 82, 0.15, 26, 45, 2.5},
        {3, "GP 32 A", "maxon", 84, 83, 0.25, 32, 60, 4.0},
        {4, "GP 42 C", "maxon",100, 81, 0.35, 42, 74, 5.0},
        {5, "GP 52 C", "maxon",120, 80, 0.60, 52, 85,10.0},
        {6, "GP 62 C", "maxon",150, 78, 1.10, 62,100,20.0},
        {7, "GPX 22",  "maxon", 30, 85, 0.10, 22, 35, 1.2},
        {8, "GPX 32",  "maxon", 70, 86, 0.20, 32, 55, 3.5},
        {9, "GPX 42",  "maxon", 90, 85, 0.30, 42, 70, 6.0}
    };

    // =====================================================
    // USER INPUT
    // =====================================================

    bool circular;
    char c;

    slowPrint("\n========================================\n");
    slowPrint(" SINGLE LINK ROBOT DESIGN OPTIMIZATION\n");
    slowPrint("========================================\n");

    slowPrint("\nChoose cross section:\n");
    slowPrint("c -> Circular\n");
    slowPrint("r -> Rectangular\n");

    cin >> c;

    circular = (c == 'c');

    displayMaterials(materials);

    int material_id;

    slowPrint("\nChoose material ID: ");
    cin >> material_id;

    Material mat = materials[material_id - 1];

    double L;
    double mp;
    double a;

    slowPrint("\nEnter link length (m): ");
    cin >> L;

    slowPrint("Enter payload mass (kg): ");
    cin >> mp;

    slowPrint("Enter max angular acceleration (rad/s^2): ");
    cin >> a;

    double b = 0;
    double h = 0;
    double r = 0;

    if (circular)
    {
        slowPrint("Enter initial radius (m): ");
        cin >> r;
    }
    else
    {
        slowPrint("Enter initial width b (m): ");
        cin >> b;

        slowPrint("Enter initial height h (m): ");
        cin >> h;
    }

    // =====================================================
    // PART 1 : LINK OPTIMIZATION
    // =====================================================

    double stress = 0;

    int iterations = 0;

    while (iterations < 10000)
    {
        double ml =
            getLinkMass(
                mat.density,
                L,
                b,
                h,
                r,
                circular
            );

        double M =
            getMoment(
                ml,
                mp,
                L,
                a
            );

        double I =
            getI(
                b,
                h,
                r,
                circular
            );

        stress =
            getStress(
                M,
                h,
                r,
                I,
                circular
            );

        if (stress > mat.sigma_yield)
        {
            if (circular)
            {
                r *= 1.01;
            }
            else
            {
                b *= 1.01;
                h *= 1.01;
            }
        }
        else if (stress < 0.97 * mat.sigma_yield)
        {
            if (circular)
            {
                r *= 0.99;
            }
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

    // =====================================================
    // FINAL LINK RESULTS
    // =====================================================

    double final_mass =
        getLinkMass(
            mat.density,
            L,
            b,
            h,
            r,
            circular
        );

    slowPrint("\n========================================\n");
    slowPrint(" PART 1 RESULTS\n");
    slowPrint("========================================\n");

    if (circular)
    {
        cout << "Optimized Radius = "
            << r << " m\n";
    }
    else
    {
        cout << "Optimized Width  = "
            << b << " m\n";

        cout << "Optimized Height = "
            << h << " m\n";
    }

    cout << "Final Stress = "
        << stress / 1e6
        << " MPa\n";

    cout << "Final Link Mass = "
        << final_mass
        << " kg\n";

    // =====================================================
    // PART 2 : MOTOR GEARBOX OPTIMIZATION
    // =====================================================

    displayMotors(motors);

    displayGearboxes(gears);

    double omega_required;

    slowPrint("\nEnter required output speed (rad/s): ");
    cin >> omega_required;

    int choice;

    slowPrint("\nOptimization Priority:\n");
    slowPrint("0 -> Minimize Cost\n");
    slowPrint("1 -> Minimize Weight\n");

    cin >> choice;

    double T_required =
        getTorque(
            final_mass,
            mp,
            L,
            a
        );

    cout << "\nRequired Torque = "
        << T_required
        << " Nm\n";

    // =====================================================
    // FIND VALID COMBINATIONS
    // =====================================================

    vector<Combination> valid;

    for (auto& m : motors)
    {
        for (auto& g : gears)
        {
            double T_out =
                getOutputTorque(
                    m.torque,
                    g.ratio,
                    g.efficiency
                );

            double omega_out =
                getOutputSpeed(
                    m.speed,
                    g.ratio
                );

            if (
                T_out >= T_required &&
                omega_out >= omega_required &&
                T_out <= g.max_torque
                )
            {
                double mass =
                    m.weight + g.weight;

                double cost =
                    mass +
                    (m.diameter + g.diameter) / 100.0 +
                    (m.length + g.length) / 100.0;

                valid.push_back(
                    Combination(
                        m.id,
                        g.id,
                        m.name,
                        g.name,
                        T_out,
                        omega_out,
                        mass,
                        cost
                    )
                );
            }
        }
    }

    // =====================================================
    // DISPLAY VALID COMBINATIONS
    // =====================================================

    slowPrint("\n========================================\n");
    slowPrint(" VALID COMBINATIONS\n");
    slowPrint("========================================\n");

    if (valid.empty())
    {
        slowPrint("No valid combination found.\n");
        return 0;
    }

    cout << "\nMotor | Gearbox | Torque | Speed | Mass | Cost\n";
    cout << "-------------------------------------------------------------\n";

    for (auto& v : valid)
    {
        cout
            << setw(5) << v.motor_id << " | "
            << setw(8) << v.gearbox_id << " | "
            << setw(7) << v.torque << " | "
            << setw(6) << v.speed << " | "
            << setw(5) << v.mass << " | "
            << setw(5) << v.cost << endl;
    }

    // =====================================================
    // BEST COMBINATION
    // =====================================================

    Combination best = valid[0];

    if (choice == 0)
    {
        for (auto& v : valid)
        {
            if (v.cost < best.cost)
            {
                best = v;
            }
        }
    }
    else
    {
        for (auto& v : valid)
        {
            if (v.mass < best.mass)
            {
                best = v;
            }
        }
    }

    // =====================================================
    // FINAL RESULTS
    // =====================================================

    slowPrint("\n========================================\n");
    slowPrint(" BEST COMBINATION\n");
    slowPrint("========================================\n");

    cout << "Motor ID: "
        << best.motor_id
        << " -> "
        << best.motor_name
        << endl;

    cout << "Gearbox ID: "
        << best.gearbox_id
        << " -> "
        << best.gearbox_name
        << endl;

    cout << "Output Torque = "
        << best.torque
        << " Nm\n";

    cout << "Output Speed = "
        << best.speed
        << " rad/s\n";

    cout << "Total Mass = "
        << best.mass
        << " kg\n";

    cout << "Cost Function = "
        << best.cost
        << endl;

    slowPrint("\n========================================\n");
    slowPrint(" PROGRAM FINISHED SUCCESSFULLY\n");
    slowPrint("========================================\n");

    return 0;
}
