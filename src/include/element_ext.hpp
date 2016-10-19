/**
 *  \brief Form element matrix.
 */
template <class T>
template <class U>
void Element<T>::form_matrix(const Node<U, T> p[], const Material<U> *matl, const Section<U> *sect)
{
	auto opt = this->get_option();
	switch(this->etype_){
	case ElementType::PIPE16:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::pipe16(&p[0], &p[1], matl, sect, opt.data());
		break;
	case ElementType::PIPE18:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::pipe18(&p[0], &p[1], &p[2], matl, sect);
		break;
	case ElementType::MASS21:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::mass21(&p[0], matl, sect, opt.data());
		break;
	case ElementType::COMBIN14:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::combin14(&p[0], &p[1], matl, sect, opt.data());
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
void Element<T>::form_matrix(const vector<Node<U, T>> pt, const Material<U> *mp, const Section<U> *sect)
{
	auto opt = this->get_option();
	switch(this->etype_){
	case ElementType::PIPE16:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::pipe16(pt, mp, sect, opt.data());
		break;
	case ElementType::PIPE18:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::pipe18(pt, mp, sect);
		break;
	case ElementType::MASS21:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::mass21(pt, mp, sect, opt.data());
		break;
	case ElementType::COMBIN14:
		std::tie(this->stif_,
			this->mass_,
			this->tran_,
			this->rhs_,
			this->attr_) = StructuralElement<U, T>::combin14(pt, mp, sect, opt.data());
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