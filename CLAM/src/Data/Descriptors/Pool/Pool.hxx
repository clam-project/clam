#include <vector>
#include <string>
#include "StaticBool.hxx"

#include "DataTypes.hxx"

namespace CLAM
{
	template <typename baseType>
	struct vectorialExpressionTrait
	{
		typedef StaticTrue isVector;
		typedef baseType expressionType;
	};
	template <typename baseType>
	struct scalarExpressionTrait
	{
		typedef StaticFalse isVector;
		typedef baseType expressionType;
	};

	template <typename fT, typename gT>
	struct composition 
	{
		typedef fT fType;
		typedef gT gType;
		typedef typename gT::paramType paramType;
		typedef typename fT::resultType resultType;

		// Those two methods should be inherited from unary
		template <typename gType2>
		CLAM::composition<composition<fType,gType>,gType2> operator()(const gType2 & g)
		{
			return compose((composition<fType,gType>&)(*this),g);
		}
		resultType operator()(const paramType & x)
		{
			return apply(x);
		}

		composition(const composition<fType,gType> & other)
			: _f(other._f), _g(other._g) {}


		composition(const fType & f, const gType & g)
			: _f(f), _g(g) {}

		resultType apply(const paramType & x)
		{
			return _f.apply(_g.apply(x));
		}
	private:
		fType _f;
		gType _g;
	};

	template <typename fType, typename gType>
	composition<fType,gType> compose(const fType & f, const gType & g)
	{
		return composition<fType,gType>(f,g);
	}

	template <typename ConcreteType>
	struct unary
	{
		typedef TData paramType;
		typedef TData resultType;

		template <typename gType>
		CLAM::composition<ConcreteType,gType> operator()(const gType & g)
		{
			return compose((ConcreteType&)(*this),g);
		}
		resultType operator()(const paramType & x)
		{
			return ((ConcreteType*)this)->ConcreteType::apply(x);
		}

	};

	struct abs : public unary<abs>
	{
		resultType apply(const paramType & x)
		{
			return std::abs(x);
		}
	};


	struct neg : public unary<neg>
	{
		resultType apply(const paramType & x)
		{
			return -(x);
		}
	};




	class CalculationSpec
	{
	};

	class ScopeSpec
	{
		typedef std::map<std::string, CalculationSpec> CalculationsMap;
		CalculationsMap _calculations;
	public:
		void Add(const std::string & name, const CalculationSpec & calculation)
		{
			_calculations.insert(make_pair(name,calculation));
		}
		std::string DescriptorsList()
		{
			std::string result;
			CalculationsMap::iterator it = _calculations.begin();
			CalculationsMap::iterator end = _calculations.end();
			for (; it!=end; it++)
			{
				result += it->first;
				result += '\n';
			}
			return result;
		}
		
	};

	class Scope
	{
		ScopeSpec * _spec;
	public:
	};


}



