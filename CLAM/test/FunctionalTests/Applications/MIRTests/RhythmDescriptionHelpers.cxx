#include "RhythmDescriptionHelpers.hxx"

namespace CLAMTest
{

namespace RhythmDescriptionTests
{
	void RhythmEventsB2B::DefaultInit()
	{
		
	}

	bool RhythmEventsB2B::IsValid() const
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
