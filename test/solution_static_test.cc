#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "cafea.h"

namespace
{
using cafea::SolutionBase;
using cafea::SolutionStatic;
using cafea::AnsysCdbReader;
using cafea::BcyReader;
//! Simplify.
// using SolutionSimple = SolutionStatic<AnsysCdbReader<float>, float, double>;
using SolutionSimple = SolutionStatic<BcyReader<float>, float, double>;

struct node_disp{
	int id_;
	double disp_[6];
};

node_disp nrc02_disp[]={
    { 1, {  0.0000    ,  0.0000    ,   0.0000    ,  0.0000     ,  0.0000     ,  0.0000     ,}}, 
    {19, {  0.0000    ,  0.0000    ,   0.0000    ,  0.0000     ,  0.0000     ,  0.0000     ,}},
	{ 2, {-0.17875e-02, 0.33544e-02, -0.67314e-03, -0.20649e-04,  0.16372e-04,  0.55236e-04,}},
    { 3, {-0.24549e-03, 0.11181e-02, -0.93133e-04, -0.89718e-05,  0.54575e-05,  0.23752e-04,}},
    { 4, {-0.87362e-03, 0.22363e-02, -0.33015e-03, -0.15855e-04,  0.10915e-04,  0.42164e-04,}},
    { 5, {-0.52741e-02, 0.67089e-02, -0.19589e-02, -0.22500e-04,  0.32745e-04,  0.62411e-04,}},
    { 6, {-0.28901e-02, 0.44726e-02, -0.10842e-02, -0.23355e-04,  0.21830e-04,  0.62967e-04,}},
    { 7, {-0.40846e-02, 0.55907e-02, -0.15254e-02, -0.23972e-04,  0.27287e-04,  0.65359e-04,}},
    { 8, {-0.34280e-02, 0.77018e-02, -0.34990e-02,  0.14714e-04,  0.33536e-04, -0.90078e-04,}},
    { 9, {-0.23288e-02, 0.60279e-02, -0.41002e-02,  0.23272e-04,  0.32546e-04, -0.94604e-04,}},
    {10, {-0.30616e-02, 0.71533e-02, -0.37011e-02,  0.17567e-04,  0.33430e-04, -0.92091e-04,}},
    {11, {-0.26952e-02, 0.65944e-02, -0.39020e-02,  0.20420e-04,  0.33100e-04, -0.93600e-04,}},
    {12, {-0.12297e-02, 0.43137e-02, -0.46657e-02,  0.31830e-04,  0.29536e-04, -0.94591e-04,}},
    {13, {-0.19624e-02, 0.54570e-02, -0.42944e-02,  0.26125e-04,  0.31767e-04, -0.95104e-04,}},
    {14, {-0.15961e-02, 0.48846e-02, -0.44834e-02,  0.28977e-04,  0.30764e-04, -0.95100e-04,}},
    {15, { 0.53364e-03, 0.49555e-03, -0.25305e-02,  0.18063e-04, -0.19080e-04, -0.34388e-04,}},
    {16, { 0.16904e-03, 0.15520e-03, -0.12652e-02,  0.12025e-04, -0.12946e-04, -0.17194e-04,}},
    {17, { 0.39900e-03, 0.36911e-03, -0.21087e-02,  0.16716e-04, -0.17792e-04, -0.28657e-04,}},
    {18, { 0.27573e-03, 0.25414e-03, -0.16870e-02,  0.14703e-04, -0.15748e-04, -0.22925e-04,}},
    {20, { 0.84068e-04, 0.76838e-04, -0.84351e-03,  0.86825e-05, -0.93879e-05, -0.11463e-04,}},
    {21, { 0.25993e-04, 0.23583e-04, -0.42172e-03,  0.46736e-05, -0.50721e-05, -0.57311e-05,}},
    {22, {-0.54395e-02, 0.84793e-02, -0.26598e-02, -0.99496e-06,  0.35346e-04, -0.10954e-04,}},
    {24, {-0.57088e-02, 0.73512e-02, -0.21832e-02, -0.15152e-04,  0.34883e-04,  0.42923e-04,}},
    {26, {-0.57621e-02, 0.79990e-02, -0.24168e-02, -0.77985e-05,  0.35640e-04,  0.17666e-04,}},
    {29, {-0.48330e-02, 0.86370e-02, -0.29162e-02,  0.49525e-05,  0.34541e-04, -0.40244e-04,}},
    {31, {-0.41014e-02, 0.83791e-02, -0.31931e-02,  0.10074e-04,  0.33802e-04, -0.67473e-04,}},
    {33, { 0.38757e-03, 0.18011e-02, -0.43003e-02,  0.35523e-04, -0.92911e-05, -0.64607e-04,}},
    {35, {-0.64024e-03, 0.33963e-02, -0.48187e-02,  0.35354e-04,  0.17107e-04, -0.86569e-04,}},
    {37, {-0.66450e-04, 0.25401e-02, -0.46863e-02,  0.36732e-04,  0.34510e-05, -0.76145e-04,}},
    {40, { 0.62230e-03, 0.11869e-02, -0.37584e-02,  0.31500e-04, -0.17971e-04, -0.53433e-04,}},
    {42, { 0.65964e-03, 0.75033e-03, -0.31420e-02,  0.25372e-04, -0.21728e-04, -0.43137e-04,}},
};

TEST_CASE("Internal Pressure", "[Solution Static]")
{
	std::unique_ptr<SolutionBase<BcyReader<float>>> example(new SolutionSimple);
	
	example->init();
	example->load("./bcy_data/nrc_ex02.bcy");
	example->analyze();
	example->assembly();
	example->solve();
	example->post_process();
	example->write2mat("nrc02.mat");
	for(const auto &p: nrc02_disp){
		auto res = example->get_node_result(p.id_, cafea::LoadType::DISP);
		for(int i=0; i<6; i++){
			auto tol = sqrt(cafea::EPS<>()) + fabs(p.disp_[i])*0.05;
			REQUIRE(res(i) == Approx(p.disp_[i]).epsilon(tol));
		}
	}
}

node_disp eblow01_disp[]={
    { 1, { 0.0000    , 0.0000    ,  0.0000    , 0.0000     ,  0.0000     ,  0.0000     ,}},
	{ 3, { 0.22455E-2, 0.22455E-2,  0.0000    , 0.0000     ,  0.0000     ,  0.60388E-18,}},
};

TEST_CASE("Internal Pressure Single Element", "[Solution Static]")
{
	std::unique_ptr<SolutionBase<BcyReader<float>>> example(new SolutionSimple);
	
	example->init();
	example->load("./bcy_data/elbow_01.bcy");
	example->analyze();
	example->assembly();
	example->solve();
	example->post_process();
	example->write2mat("eblow01.mat");
	for(const auto &p: eblow01_disp){
		auto res = example->get_node_result(p.id_, cafea::LoadType::DISP);
		for(int i=0; i<6; i++){
			auto tol = sqrt(cafea::EPS<>()) + fabs(p.disp_[i])*0.05;
			REQUIRE(res(i) == Approx(p.disp_[i]).epsilon(tol));
		}
	}
}
}