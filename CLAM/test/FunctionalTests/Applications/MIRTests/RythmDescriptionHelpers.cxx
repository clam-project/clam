#include "RythmDescriptionHelpers.hxx"

namespace CLAMTest
{

namespace RythmDescriptionTests
{
	void RythmEventsB2B::DefaultInit()
	{
		
	}

	bool RythmEventsB2B::IsValid() const
	{
		return HasRelativePath() 
			&& HasEvents();
			
	}

	void IOIHistogramB2B::DefaultInit()
	{
	}

	bool IOIHistogramB2B::IsValid() const
	{
		return HasRelativePath() 
			&& HasIOIHistogram();
	}
}

}
