/**
 *  \brief Form element matrix.
 */
template <class T>
template <class U>
void Element<T>::form_matrix(const Node<U, T> p[], const Material<U> *matl, const Section<U> *sect,
	const std::vector<LoadCell<U>> load)
{
	this->form_matrix<U>(p, matl, sect);

	assert(!load.empty());
	int n2 = load.size();

	switch(this->etype_){
		case ElementType::PIPE16:
		case ElementType::PIPE18:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(12, n2);
			this->load_cmplx_ = cmatrix_<T>::Zero(2, n2);
			for(int i=0; i<n2; i++) {
				this->rhs_cmplx_.col(i).real() = this->rhs_*std::real(load[i].val_cmplx_);
				this->rhs_cmplx_.col(i).imag() = this->rhs_*std::imag(load[i].val_cmplx_);
				this->load_cmplx_(0, i) = load[i].val_cmplx_;
			}
			break;
		case ElementType::MASS21:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(6, n2);
			break;
		case ElementType::COMBIN14:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(12, n2);
			break;
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::C3D4:
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::S3R:
		case ElementType::S4R:
		case ElementType::SHELL181:
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::SHELL281:
		case ElementType::S8R:
		case ElementType::S9R:
		case ElementType::SOLID186:
		case ElementType::C3D20:
		case ElementType::UNKNOWN:
		default: fmt::print("Unsupported element type\n");
	}
};
/**
 *  \brief Form element matrix.
 */
template <class T>
template <class U>
void Element<T>::form_matrix(const std::vector<Node<U, T>> pt, const Material<U> *mp,
	const Section<U> *sect, const std::vector<LoadCell<U>> load)
{
	this->form_matrix<U>(pt, mp, sect);

	assert(!load.empty());
	int n2 = load.size();

	switch(this->etype_){
		case ElementType::PIPE16:
		case ElementType::PIPE18:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(12, n2);
			this->load_cmplx_ = cmatrix_<T>::Zero(2, n2);
			for(int i=0; i<n2; i++) {
				this->rhs_cmplx_.col(i).real() = this->rhs_*std::real(load[i].val_cmplx_);
				this->rhs_cmplx_.col(i).imag() = this->rhs_*std::imag(load[i].val_cmplx_);
				this->load_cmplx_(0, i) = load[i].val_cmplx_;
			}
			break;
		case ElementType::MASS21:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(6, n2);
			break;
		case ElementType::COMBIN14:
			this->rhs_cmplx_ = cmatrix_<T>::Zero(12, n2);
			break;
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::C3D4:
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::S3R:
		case ElementType::S4R:
		case ElementType::SHELL181:
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::SHELL281:
		case ElementType::S8R:
		case ElementType::S9R:
		case ElementType::SOLID186:
		case ElementType::C3D20:
		case ElementType::UNKNOWN:
		default: fmt::print("Unsupported element type\n");
	}
};

/**
 *  \brief Form element matrix.
 */
template <class T>
template <class U>
void Element<T>::post_stress(const matrix_<U> x)
{
	if(std::type_index(typeid(T))==std::type_index(typeid(U)))return this->post_stress(x);
	switch(this->etype_){
		case ElementType::PIPE16:
		case ElementType::PIPE18:
			this->result_cmplx_ = StructuralElementPost<T>::pipe_cmplx(this->stif_, this->tran_,
				x, this->rhs_cmplx_, this->load_cmplx_, this->attr_);
			break;
		case ElementType::MASS21:
		case ElementType::COMBIN14:
			break;
		case ElementType::BEAM188:
		case ElementType::B31:
		case ElementType::C3D4:
		case ElementType::SOLID185:
		case ElementType::C3D8:
		case ElementType::S3R:
		case ElementType::S4R:
		case ElementType::SHELL181:
		case ElementType::BEAM189:
		case ElementType::B32:
		case ElementType::SHELL281:
		case ElementType::S8R:
		case ElementType::S9R:
		case ElementType::SOLID186:
		case ElementType::C3D20:
		case ElementType::UNKNOWN:
		default: fmt::print("Unsupported element type\n");
	}
};

/**
 *
 */
template <class T>
template <class U>
matrix_<U> Element<T>::get_rhs() const
{
	if(std::type_index(typeid(T))==std::type_index(typeid(U))){
		return this->get_rhs();
	}
	else{
		return this->rhs_cmplx_;
	}
};

/**
 *
 */
template <class T>
template <class U>
matrix_<U> Element<T>::get_result() const
{
	if(std::type_index(typeid(T))==std::type_index(typeid(U))){
		return this->get_result();
	}
	else{
		return this->result_cmplx_;
	}
};
