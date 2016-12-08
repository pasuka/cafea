/**
 *  \brief 
 */
template <class T, class U>
template <class ResType>
void Node<T, U>::set_result(SolutionType sol, LoadType lt, int n, matrix_<ResType> rst)
{
	if(!this->is_activated())return;
	if(std::type_index(typeid(ResType))==std::type_index(typeid(U))){
		return this->set_result(sol, lt, n, rst);
	}
	// fmt::print("Template Function Set result Input Type: {}", typeid(ResType).name());
	// fmt::print(" {}\n", typeid(U).name());
	switch(sol){
	case SolutionType::HARMONIC_FULL:
		switch(lt){
		case LoadType::DISP:
			if(0>n){
				this->disp_cmplx_ = rst;
			}
			else{
				this->disp_cmplx_.col(n) = rst;
			}
			break;
		case LoadType::STRESS:
			if(0>n){
				this->stress_cmplx_ = rst;
			}
			else{
				this->stress_cmplx_.col(n) = rst;
			}
			break;
		default:
			fmt::print("Unsupported result type\n");
		}
		break;
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	case SolutionType::STATIC:
	case SolutionType::MODAL:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
};

/**
 *  \brief
 */
template <class T, class U>
template <class ResType>
matrix_<ResType> Node<T, U>::get_result(SolutionType sol, LoadType lt, int n) const
{
	matrix_<ResType> tmp;
	
	if(!this->is_activated())return tmp;
	if(std::type_index(typeid(ResType))==std::type_index(typeid(U))){
		return this->get_result(sol, lt, n);
	}
	// fmt::print("Template Function Get result Input Type:{}\n", typeid(ResType).name());
	switch(sol){
	case SolutionType::HARMONIC_FULL:
		switch(lt){
		case LoadType::DISP:
			if(0>n){
				tmp = this->disp_cmplx_;
			}
			else{
				tmp = this->disp_cmplx_.col(n);
			}
			break;
		case LoadType::STRESS:
			if(0>n){
				tmp = this->stress_cmplx_;
			}
			else{
				tmp = this->stress_cmplx_.col(n);
			}
			break;
		default:
			fmt::print("Unsupported result type\n");
		}
		break;
	case SolutionType::HARMONIC_MODAL_SUPERPOSITION:
	case SolutionType::STATIC:
	case SolutionType::MODAL:
	default:
		fmt::print("Unsupported solution type definition\n");
	}
	return tmp;
};