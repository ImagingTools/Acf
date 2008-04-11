#ifndef icomp_TSimComponentModelWrap_included
#define icomp_TSimComponentModelWrap_included


namespace icomp
{


/**
	Simulation wrapper of changeable component making it to model.
	It allows to use components directly from static linked libraries, without component framework.
	\sa icomp::TSimComponentWrap and imod::TModelWrap.
*/
template <class Base>
class TSimComponentModelWrap: public imod::TModelWrap<TSimComponentWrap<Base> >
{
};


}//namespace icomp


#endif // !icomp_TSimComponentModelWrap_included


