#include "RhythmDescriptionHelpers.hxx"
#include "CLAM_Math.hxx"

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

	void TransientEventList::DefaultInit()
	{
	}

	bool TransientEventList::IsValid() const
	{
		return HasRelativePath() && HasEvents();
	}

	void IOIHistogramB2B::DefaultInit()
	{
	}

	bool IOIHistogramB2B::IsValid() const
	{
		return HasRelativePath() 
			&& HasIOIHistogram();
	}

	void EventListDifference::DefaultInit()
	{
		AddAll();
		UpdateData();
		
		SetAveragePositionDeviation( 0.0 );
		SetAverageWeightDeviation( 0.0 );
		SetMinTimePositionDeviation( 1e20 );
		SetMaxTimePositionDeviation( 1e-20 );
		SetMinWeightDeviation( 1e20 );
		SetMaxWeightDeviation( 1e-20 );
	}

	void EventListDifference::Compare( const CLAM::Array< CLAM::TimeIndex >& truth,
					   const CLAM::Array< CLAM::TimeIndex >& yield )
	{
		CLAM::Array< CLAM::TimeIndex >& diff = GetDifference();

		CLAM::TSize size = std::min( truth.Size(), yield.Size() );
		
		diff.Resize( size );
		diff.SetSize( size );

		CLAM::TData avgWDev = 0.0;
		CLAM::TData avgPDev = 0.0;

		for ( int i = 0; i < diff.Size(); i++ )
		{
			CLAM::TData diffPosition = truth[i].GetPosition() - yield[i].GetPosition();
			diff[i].SetPosition( diffPosition );
			CLAM::TData diffWeight = truth[i].GetWeight() - yield[i].GetWeight();
			diff[i].SetWeight( diffWeight );

			avgWDev += diffWeight;
			avgPDev += diffPosition;
			
			if ( GetMinTimePositionDeviation() > fabs( diffPosition ) )
				SetMinTimePositionDeviation( fabs(diffPosition ) );
			if ( GetMaxTimePositionDeviation() < fabs( diffPosition ) )
				SetMaxTimePositionDeviation( fabs( diffPosition ) );

			if ( GetMinWeightDeviation() > fabs( diffWeight ) )
				SetMinWeightDeviation( fabs(diffWeight ) );
			if ( GetMaxWeightDeviation() < fabs( diffWeight ) )
				SetMaxWeightDeviation( fabs( diffWeight ) );

		}

		SetAveragePositionDeviation( avgPDev / CLAM::TData( size ) );
		SetAverageWeightDeviation( avgWDev / CLAM::TData( size ) );
	}
}

}
