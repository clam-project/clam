#include "SMS_DataExplorer.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "Frame.hxx"
#include "Segment.hxx"
#include "Assert.hxx"

#include "Fl_SMS_Browsable_Playable_Audio.hxx"
#include "Fl_SMS_Spectrum.hxx"
#include "Fl_SMS_SinTracks.hxx"
#include "Fl_SMS_SpectrumAndPeaks.hxx"

#include "Fl_Smart_Tile.hxx"

namespace CLAMVM
{
	SMS_DataExplorer::SMS_DataExplorer()
		: mpOriginalAudioWidget( NULL ), mpSynthesizedAudioWidget( NULL ), mpSynthesizedResidualWidget( NULL ),
		  mpSynthesizedSinusoidalWidget( NULL ), mpSpectrumAndPeaksWidget( NULL ), mpSinusoidalSpectrum( NULL ),
		  mpResidualSpectrum( NULL ), mpSegmentSinTracks( NULL ), mpCanvas( NULL ), mCurrentFrameCenterTime( 0 )
	{
		// Slot wrapping
		NewSegment.Wrap( this, &SMS_DataExplorer::OnNewSegment );
		NewFrame.Wrap( this, &SMS_DataExplorer::OnNewFrame );
		SelectedTimeChanged.Wrap( this, &SMS_DataExplorer::OnSelectedTimeChanged );
		NewInputAudio.Wrap( this, &SMS_DataExplorer::OnNewInputAudio );
		NewSynthesizedAudio.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedAudio );
		NewSynthesizedSinusoidal.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedSinusoidal );
		NewSynthesizedResidual.Wrap( this, &SMS_DataExplorer::OnNewSynthesizedResidual );
		ShowInputAudio.Wrap( this, &SMS_DataExplorer::OnShowInputAudio );
		ShowSinTracks.Wrap( this, &SMS_DataExplorer::OnShowSinTracks );
		ShowSpectrumAndPeaks.Wrap( this, &SMS_DataExplorer::OnShowSpectrumAndPeaks );
		ShowSinusoidalSpectrum.Wrap( this, &SMS_DataExplorer::OnShowSinusoidalSpectrum );
		ShowResidualSpectrum.Wrap( this, &SMS_DataExplorer::OnShowResidualSpectrum );
		ShowSynthesizedAudio.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedAudio );
		ShowSynthesizedSinusoidal.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedSinusoidal );
		ShowSynthesizedResidual.Wrap( this, &SMS_DataExplorer::OnShowSynthesizedResidual );
	}
	
	SMS_DataExplorer::~SMS_DataExplorer()
	{
	}

	void SMS_DataExplorer::SetCanvas( Fl_Smart_Tile* pCanvas )
	{
		CLAM_ASSERT( pCanvas != NULL, "The provided canvas was null!" );
		mpCanvas = pCanvas;
	}

	void SMS_DataExplorer::OnNewSegment( const CLAM::Segment& segment )
	{
		mSinusoidalTracksAdapter.BindTo( segment );

		// first we create the necessary widgets 
		if ( !mpSegmentSinTracks )
		{
			mpSegmentSinTracks = new Fl_SMS_SinTracks( 0,0,800,600, "Sinusoidal Tracks");
			CLAM_ASSERT( mpSegmentSinTracks != NULL, "Unable to create widget");
			mpSegmentSinTracks->AttachTo( mSinusoidalTracksAdapter );
		}

		mSinusoidalTracksAdapter.Publish();
	}
	
	void SMS_DataExplorer::OnNewFrame( const CLAM::Frame& frame )
	{
		CLAM_ASSERT( frame.HasSpectrum(), "The given frame did not have the Spectrum field");
		mSpectrumAdapter.BindTo( frame.GetSpectrum() );
		// @todo: AnalysisSynthesis does not keep a SinusoidalSpec in the Frames
		//CLAM_ASSERT( frame.HasSinusoidalSpec(), "The given frame did not have the Sinusoidal Spectrum field" );
		//mSinusoidalSpectrumAdapter.BindTo( frame.GetSinusoidalSpec() );
		CLAM_ASSERT( frame.HasResidualSpec(), "The given frame did not have the Residual Spectrum field");
		mResidualSpectrumAdapter.BindTo( frame.GetResidualSpec() );
		CLAM_ASSERT( frame.HasSpectralPeakArray(), "The given frame did not have the SpectralPeakArray field" );
		mPeakArrayAdapter.BindTo( frame.GetSpectralPeakArray() );
		
		// now we create the necessary widgets
		if ( !mpSpectrumAndPeaksWidget )
		{
			mpSpectrumAndPeaksWidget = new Fl_SMS_SpectrumAndPeaks( 0,0,800,600, "Frame spectrum and spectral peaks" );
			CLAM_ASSERT( mpSpectrumAndPeaksWidget!=NULL, "The widget could not be created" );
			mpSpectrumAndPeaksWidget->AttachTo( mSpectrumAdapter, mPeakArrayAdapter );
		}
		if ( !mpSinusoidalSpectrum )
		{
			mpSinusoidalSpectrum = new Fl_SMS_Spectrum( 0,0,800,600, "Sinusoidal Spectrum ");
			CLAM_ASSERT( mpSinusoidalSpectrum!=NULL, "The widget could not be created");
			mpSinusoidalSpectrum->AttachTo( mSinusoidalSpectrumAdapter );
		}
		if ( !mpResidualSpectrum )
		{
			mpResidualSpectrum = new Fl_SMS_Spectrum( 0,0,800,600, "Residual Spectrum");
			CLAM_ASSERT( mpResidualSpectrum!=NULL, "The widget could not be created");
			mpResidualSpectrum->AttachTo( mResidualSpectrumAdapter );
		}
		
		mSpectrumAdapter.Publish();
		mSinusoidalSpectrumAdapter.Publish();
		mResidualSpectrumAdapter.Publish();
		mPeakArrayAdapter.Publish();
		
		mCurrentFrameCenterTime = frame.GetCenterTime();
		if ( mpOriginalAudioWidget )
		{
			mpOriginalAudioWidget->SetSelectedXValue( frame.GetCenterTime() );
		}
		if ( mpSynthesizedAudioWidget )
		{
			mpSynthesizedAudioWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}
		if ( mpSynthesizedSinusoidalWidget )
		{
			mpSynthesizedSinusoidalWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}
		if ( mpSynthesizedResidualWidget )
		{
			mpSynthesizedResidualWidget->SetSelectedXValue( frame.GetCenterTime() );		
		}

	}

	void SMS_DataExplorer::OnSelectedTimeChanged( double timeval )
	{
		SelectedTime.Emit( timeval );
	}

	void SMS_DataExplorer::OnNewInputAudio( const CLAM::Audio& audio )
	{
		mOriginalAudioAdapter.BindTo( audio );
		
		if ( !mpOriginalAudioWidget )
		{
			mpOriginalAudioWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,800,600, "Input Audio");
			CLAM_ASSERT( mpOriginalAudioWidget!=NULL, "The widget could not be created");
			mpOriginalAudioWidget->AttachTo( mOriginalAudioAdapter );
			mpOriginalAudioWidget->SetSelectedXValue( mCurrentFrameCenterTime );
		}

		mOriginalAudioAdapter.Publish();
	}

	void SMS_DataExplorer::OnNewSynthesizedAudio( const CLAM::Audio& audio )
	{
		mSynthesizedAudioAdapter.BindTo( audio );
		
		if ( !mpSynthesizedAudioWidget )
		{
			mpSynthesizedAudioWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,800,600, "Synthesized Audio");
			CLAM_ASSERT( mpSynthesizedAudioWidget!=NULL, "The widget could not be created");
			mpSynthesizedAudioWidget->AttachTo( mSynthesizedAudioAdapter );
			mpSynthesizedAudioWidget->SetSelectedXValue( mCurrentFrameCenterTime );
		}

		mSynthesizedAudioAdapter.Publish();
	}
	

	void SMS_DataExplorer::OnNewSynthesizedSinusoidal( const CLAM::Audio& audio )
	{
		mSynthesizedSinusoidalAdapter.BindTo( audio );
		
		if ( !mpSynthesizedSinusoidalWidget )
		{
			mpSynthesizedSinusoidalWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,800,600, "Synthesized Sinusoidal Component");
			CLAM_ASSERT( mpSynthesizedSinusoidalWidget!=NULL, "The widget could not be created");
			mpSynthesizedSinusoidalWidget->AttachTo( mSynthesizedSinusoidalAdapter );
			mpSynthesizedSinusoidalWidget->SetSelectedXValue( mCurrentFrameCenterTime );
		}

		mSynthesizedSinusoidalAdapter.Publish();

	}

	void SMS_DataExplorer::OnNewSynthesizedResidual( const CLAM::Audio& audio )
	{
		mSynthesizedResidualAdapter.BindTo( audio );
		
		if ( !mpSynthesizedResidualWidget )
		{
			mpSynthesizedResidualWidget = new Fl_SMS_Browsable_Playable_Audio( 0,0,800,600, "Synthesized Residual Component");
			CLAM_ASSERT( mpSynthesizedResidualWidget!=NULL, "The widget could not be created");
			mpSynthesizedResidualWidget->AttachTo( mSynthesizedResidualAdapter );
			mpSynthesizedResidualWidget->SetSelectedXValue( mCurrentFrameCenterTime );
		}

		mSynthesizedResidualAdapter.Publish();

	}

	void SMS_DataExplorer::OnShowInputAudio()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		// already in the canvas
		if ( mpCanvas->contains( mpOriginalAudioWidget ) )
			return;
		     
		int widgetWidth, widgetHeight;
		     
		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );
		     
		mpOriginalAudioWidget->resize( mpCanvas->x(), mpCanvas->y(), widgetWidth, widgetHeight );
		mpOriginalAudioWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpOriginalAudioWidget );
		mpOriginalAudioWidget->Show();
		mpCanvas->redraw();
	}

	void SMS_DataExplorer::OnShowSinTracks( )
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSegmentSinTracks ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSegmentSinTracks->resize( 0, 0, widgetWidth, widgetHeight );
		mpSegmentSinTracks->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSegmentSinTracks );
		mpSegmentSinTracks->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSpectrumAndPeaks()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSpectrumAndPeaksWidget ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSpectrumAndPeaksWidget->resize( 0, 0, widgetWidth, widgetHeight );
		mpSpectrumAndPeaksWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSpectrumAndPeaksWidget );
		mpSpectrumAndPeaksWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSinusoidalSpectrum()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");
/*
		if ( mpCanvas->contains( mpSinusoidalSpectrum ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSinusoidalSpectrum->resize( 0, 0, widgetWidth, widgetHeight );
		mpSinusoidalSpectrum->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSinusoidalSpectrum );
		mpSinusoidalSpectrum->Show();
		mpCanvas->redraw();
*/
	}

	void SMS_DataExplorer::OnShowResidualSpectrum()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpResidualSpectrum ) )// already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpResidualSpectrum->resize( 0, 0, widgetWidth, widgetHeight );
		mpResidualSpectrum->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpResidualSpectrum );
		mpResidualSpectrum->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSynthesizedAudio()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedAudioWidget ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSynthesizedAudioWidget->resize( 0, 0, widgetWidth, widgetHeight );
		mpSynthesizedAudioWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSynthesizedAudioWidget );
		mpSynthesizedAudioWidget->Show();
		mpCanvas->redraw();

	}
	
	void SMS_DataExplorer::OnShowSynthesizedSinusoidal()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedSinusoidalWidget ) ) // already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSynthesizedSinusoidalWidget->resize( 0, 0, widgetWidth, widgetHeight );
		mpSynthesizedSinusoidalWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSynthesizedSinusoidalWidget );
		mpSynthesizedSinusoidalWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::OnShowSynthesizedResidual()
	{
		CLAM_ASSERT( mpCanvas!=NULL, "No canvas attached to Explorer");

		if ( mpCanvas->contains( mpSynthesizedResidualWidget ) )// already in the canvas
			return;
		     
		int widgetWidth, widgetHeight;

		widgetWidth = mpCanvas->w();
		widgetHeight = mpCanvas->h() / ( mpCanvas->children() + 1 );

		mpSynthesizedResidualWidget->resize( 0, 0, widgetWidth, widgetHeight );
		mpSynthesizedResidualWidget->callback( (Fl_Callback*)sDetachCb, this );
		
		mpCanvas->add( *mpSynthesizedResidualWidget );
		mpSynthesizedResidualWidget->Show();
		mpCanvas->redraw();

	}

	void SMS_DataExplorer::Detach()
	{
	}

	void SMS_DataExplorer::sDetachCb( Fl_Widget* w, void* cbData )
	{
		SMS_DataExplorer* pExplorer = ( SMS_DataExplorer* )cbData;
		w->hide();
		CLAM_ASSERT( pExplorer->mpCanvas !=NULL , "It can't be");
		pExplorer->mpCanvas->remove( w );
		pExplorer->mpCanvas->redraw();
	}

	void SMS_DataExplorer::CloseAll()
	{
		if ( mpOriginalAudioWidget )
		{
			if ( mpCanvas->contains(mpOriginalAudioWidget) )
			{
				mpOriginalAudioWidget->Hide();
				mpCanvas->remove( mpOriginalAudioWidget );
			}
		}
		if ( mpSynthesizedAudioWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedAudioWidget) )
			{
				mpSynthesizedAudioWidget->Hide();
				mpCanvas->remove( mpSynthesizedAudioWidget );
			}
		}
		if ( mpSynthesizedResidualWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedResidualWidget) )
			{
				mpSynthesizedResidualWidget->Hide();
				mpCanvas->remove( mpSynthesizedResidualWidget );
			}
		}

		if ( mpSynthesizedSinusoidalWidget )
		{
			if ( mpCanvas->contains(mpSynthesizedSinusoidalWidget) )
			{
				mpSynthesizedSinusoidalWidget->Hide();
				mpCanvas->remove( mpSynthesizedSinusoidalWidget );
			}
		}
		if ( mpSpectrumAndPeaksWidget )
		{
			if ( mpCanvas->contains(mpSpectrumAndPeaksWidget) )
			{
				mpSpectrumAndPeaksWidget->Hide();
				mpCanvas->remove( mpSpectrumAndPeaksWidget );
			}
		}
		if ( mpSinusoidalSpectrum )
		{
			if ( mpCanvas->contains(mpSinusoidalSpectrum) )
			{
				mpSinusoidalSpectrum->Hide();
				mpCanvas->remove( mpSinusoidalSpectrum );
			}
		}
		if ( mpResidualSpectrum )
		{
			if ( mpCanvas->contains(mpResidualSpectrum) )
			{
				mpResidualSpectrum->Hide();
				mpCanvas->remove( mpResidualSpectrum );
			}
		}
		if ( mpSegmentSinTracks )
		{
			if ( mpCanvas->contains(mpSegmentSinTracks) )
			{
				mpSegmentSinTracks->Hide();
				mpCanvas->remove( mpSegmentSinTracks );
			}
		}


	}
	
}
