#ifndef __STDIOSINTRACKSPRESENTATION__
#define __STDIOSINTRACKSPRESENTATION__

#include "Presentation.hxx"
#include "Array.hxx"
#include "Slot.hxx"
#include "DataTypes.hxx"
#include "SinTrackListBuilder.hxx"
#include <list>

namespace CLAMGUI
{
		using CLAM::Array;
		using CLAM::TData;
		using CLAM::TTime;
		using CLAM::TSize;

		class StdioSinTracksPresentation : public Presentation
		{
		private:
			

				class dump_to_stdout
				{
				public:
						dump_to_stdout()
								: real_track_id ( 1 )
						{
						}

						void operator()( const SinusoidalTrack& st );
				private:
						
						TIndex real_track_id;
				};

				class decimate_sine_tracks
				{
				public:
					decimate_sine_tracks( TSize min_len )
						: minimum_len( min_len )
					{
					}

					void operator() ( SineTrackList& tl )
					{
						SineTrackList::iterator i, end, aux;
						i = tl.begin();
						end = tl.end();

						while ( i != end )
						{
							if ( i->size() < minimum_len )
							{
								aux = i;
								i++;
								tl.erase( aux );
							}

							i++;
							
						}
					}

				private:

					TSize   minimum_len;
				};


		protected:
				
				typedef SineTrackList::iterator iterator;

				SineTrackList               mSineTracks;
				Slot                        mPartialsSlot;
				TData                       mSpectralRange;
				Slot                        mSpectralRangeSlot;
				TTime                       mBeginTime;
				TTime                       mEndTime;
				TTime                       mLen;
				Slot                        mDurationSlot;

				SinTrackBuilder             mTrackListBuilder;

		protected:

				virtual void Bind( Aspect& ) throw( std::bad_cast );

				virtual void HandleIncomingPartials( const Array<Partial>& array, TIndex frames_to_go );

				virtual void HandleIncomingRange ( TData spec_rng );
				
				virtual void HandleIncomingDuration( TTime begin, TTime end );
		public:

				StdioSinTracksPresentation();

				virtual ~StdioSinTracksPresentation();

				virtual void Show();
		};
}

#endif // StdioSinTracksPresentation.hxx
