#include "cafea.h"

using std::tuple;
using std::make_tuple;
using Eigen::VectorXd;

namespace cafea
{	
/**
 *  \brief Gauss-Legendre integrate point and weight.
 *  \param [in] n number of points.
 *  \return integrate point and weight values.
 */
tuple<VectorXd, VectorXd> gauss_quad(size_t n)
{
	assert(0<n&&n<11);
	VectorXd pt = VectorXd::Zero(n);//!< quadrature point
	VectorXd wt = pt;//!< quadrature weight
	switch(n){
	case 1:
		pt(0) = .0; wt(0) = 2.;
		break;
	case 2:
		pt(0) = -.577350269189626E0; wt(0) = 1.0E0;
		pt(1) =  .577350269189626E0; wt(1) = 1.0E0;
		break;
	case 3:
		pt(0) = -.774596669241483E0; wt(0) = .555555555555555E0;
		pt(1) =  .0E0;               wt(1) = .888888888888889E0;
		pt(2) =  .774596669241483E0; wt(2) = .555555555555555E0;
		break;
	case 4:
		pt(0) = -.861136311594053E0; wt(0) = .347854845137454E0;
		pt(1) = -.339981043584856E0; wt(1) = .652145154862546E0;
		pt(2) =  .339981043584856E0; wt(2) = .652145154862546E0;
		pt(3) =  .861136311594053E0; wt(3) = .347854845137454E0;
		break;
	case 5:
		pt(0) = -.906179845938664E0; wt(0) = .236926885056189E0;
		pt(1) = -.538469310105683E0; wt(1) = .478628670499367E0;
		pt(2) =  .000000000000000E0; wt(2) = .568888888888889E0;
		pt(3) =  .538469310105683E0; wt(3) = .478628670499367E0;
		pt(4) =  .906179845938664E0; wt(4) = .236926885056189E0;
		break;
	case 6:
		pt(0) = -.932469514203152E0; wt(0) = .171324492379169E0;
		pt(1) = -.661209386466264E0; wt(1) = .360761573048139E0;
		pt(2) = -.238619186083197E0; wt(2) = .467913934572692E0;
		pt(3) =  .238619186083197E0; wt(3) = .467913934572692E0;
		pt(4) =  .661209386466264E0; wt(4) = .360761573048139E0;
		pt(5) =  .932469514203152E0; wt(5) = .171324492379169E0;
		break;
	case 7:
		pt(0) = -.949107912342758E0; wt(0) = .129484966168870E0;
		pt(1) = -.741531185599394E0; wt(1) = .279705391489277E0;
		pt(2) = -.405845151377397E0; wt(2) = .381830050505119E0;
		pt(3) =  .000000000000000E0; wt(3) = .417959183673470E0;
		pt(4) =  .405845151377397E0; wt(4) = .381830050505119E0;
		pt(5) =  .741531185599394E0; wt(5) = .279705391489277E0;
		pt(6) =  .949107912342758E0; wt(6) = .129484966168870E0;
		break;
	case 8:
		pt(0) = -.960289856497536E0; wt(0) = .101228536290374E0;
		pt(1) = -.796666477413627E0; wt(1) = .222381034453375E0;
		pt(2) = -.525532409916329E0; wt(2) = .313706645877888E0;
		pt(3) = -.183434642495650E0; wt(3) = .362683783378363E0;
		pt(4) =  .183434642495650E0; wt(4) = .362683783378363E0;
		pt(5) =  .525532409916329E0; wt(5) = .313706645877888E0;
		pt(6) =  .796666477413627E0; wt(6) = .222381034453375E0;
		pt(7) =  .960289856497536E0; wt(7) = .101228536290374E0;
		break;
	case 9:
		pt(0) = -.968160239507626E0; wt(0) = .081274388361575E0;
		pt(1) = -.836031107326636E0; wt(1) = .180648160694857E0;
		pt(2) = -.613371432700590E0; wt(2) = .260610696402935E0;
		pt(3) = -.324253423403809E0; wt(3) = .312347077040003E0;
		pt(4) =  .000000000000000E0; wt(4) = .330239355001259E0;
		pt(5) =  .324253423403809E0; wt(5) = .312347077040003E0;
		pt(6) =  .613371432700590E0; wt(6) = .260610696402935E0;
		pt(7) =  .836031107326636E0; wt(7) = .180648160694857E0;
		pt(8) =  .968160239507626E0; wt(8) = .081274388361575E0;
		break;
	case 10:
		pt(0) = -.973906528517172E0; wt(0) = .066671344308688E0;
		pt(1) = -.865063366688985E0; wt(1) = .149451349150581E0;
		pt(2) = -.679409568299024E0; wt(2) = .219086362515982E0;
		pt(3) = -.433395394129247E0; wt(3) = .269266719309996E0;
		pt(4) = -.148874338981631E0; wt(4) = .295524224714753E0;
		pt(5) =  .148874338981631E0; wt(5) = .295524224714753E0;
		pt(6) =  .433395394129247E0; wt(6) = .269266719309996E0;
		pt(7) =  .679409568299024E0; wt(7) = .219086362515982E0;
		pt(8) =  .865063366688985E0; wt(8) = .149451349150581E0;
		pt(9) =  .973906528517172E0; wt(9) = .066671344308688E0;
		break;
	default:
		fmt::print("Input: {} exceede!\n", n);
	}
	return make_tuple(pt, wt);
}

std::vector<std::string> split(const std::string& input, const std::string& regex)
{
    // passing -1 as the submatch index parameter performs splitting
    std::regex re(regex);
    std::sregex_token_iterator first{input.begin(), input.end(), re, -1}, last;
    return {first, last};
};	
}