//! Generate random component.
using cafea::LoadSet;
using cafea::LoadCell;
using cafea::LoadType;
using cafea::LoadDomain;
using cafea::DofLabel;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937 gen(seed);

int random_value(int a, int b)
{
    std::uniform_int_distribution<> dis(a, b);
    return dis(gen);
}

template <class T=float>
T random_value(T a, T b)
{
    std::uniform_real_distribution<T> dis(a, b);
    return dis(gen);
}

LoadType gen_load_type()
{
    int load_type = random_value(1, 7);
    LoadType lt;
    switch (load_type) {
    case 1: lt = LoadType::FORCE;  break;
    case 2: lt = LoadType::DISP;   break;
    case 3: lt = LoadType::VEL;    break;
    case 4: lt = LoadType::ACCEL;  break;
    case 5: lt = LoadType::PRES;   break;
    case 6: lt = LoadType::STRESS; break;
    case 7:
    default: lt = LoadType::UNKNOWN;
    }
    return lt;
}

DofLabel gen_dof_label()
{
    int dof_label = random_value(1, 11);
    DofLabel dl;
    switch (dof_label) {
    case 1: dl = DofLabel::UX;     break;
    case 2: dl = DofLabel::UY;     break;
    case 3: dl = DofLabel::UZ;     break;
    case 4: dl = DofLabel::URX;    break;
    case 5: dl = DofLabel::URY;    break;
    case 6: dl = DofLabel::URZ;    break;
    case 7: dl = DofLabel::WARP;   break;
    case 8: dl = DofLabel::U_ALL;  break;
    case 9: dl = DofLabel::UR_ALL; break;
    case 10: dl = DofLabel::ALL;   break;
    case 11:
    default : dl = DofLabel::UNKNOWN;
    }
    return dl;
}

template <class T=float>
LoadCell<T> gen_random_cell()
{
    int id = random_value(1, 10000000);

    int load_domain = 1;
    LoadDomain ld;
    switch (load_domain) {
    case 1: ld = LoadDomain::FREQ; break;
    case 2:
    default: ld = LoadDomain::TIME;
    }

    LoadCell<T> tmp{id, gen_load_type(), gen_dof_label(), ld};
    return std::move(tmp);
}
