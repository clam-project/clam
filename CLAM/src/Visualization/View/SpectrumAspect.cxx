#include "SpectrumAspect.hxx"
#include "SpectrumAdapter.hxx"

namespace CLAMVM
{
		SpectrumAspect::SpectrumAspect( SpectrumAdapter& view )
				: mAdapter( view )
		{
		}

		SpectrumAspect::~SpectrumAspect()
		{
		}

		void SpectrumAspect::ForceViewRefresh()
		{
				mAdapter.Publish();
		}

}
