/*
 * Copyright (c) 2001-2002 MUSIC TECHNOLOGY GROUP (MTG)
 *                         UNIVERSITAT POMPEU FABRA
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in thFl_Smart_Tile.e hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef __VISUALIZATIONCONTROLLER__
#define __VISUALIZATIONCONTROLLER__

#include <vector>

#include "Fl_Smart_Tile.hxx"

#include "Enum.hxx"
#include "Audio.hxx"
#include "Spectrum.hxx"
#include "Signalv1.hxx"
#include "Slotv1.hxx"

class Fl_Window;
class Fl_Smart_Tile;

using CLAM::TData;
using CLAM::Audio;
using CLAM::Spectrum;

using SigSlot::Signalv1;
using SigSlot::Slotv1;

namespace CLAMVM
{
	class AudioAdapter;
	class LogMagSpectrumAdapter;
	class Fl_Browsable_Playable_Audio;
	class Fl_Spectrum;

	class VisualizationController
	{
	public:
		enum DisplayList { 
			eAudioIn=0,
			eAudioOut,
			eAudioSinusoidal,
			eAudioResidual,
			eSpectrumIn,
			eSpectrumOut
		};
		enum { eNumDisplays=6 };
		enum { eAudioDisplays=4 };
		enum { eSpectrumDisplays=2 };

		Signalv1<TData> mFrameSignal;
		Signalv1<bool> mPaintSignal;
		Slotv1<double> mSlot;

		VisualizationController(  );
		VisualizationController( Fl_Smart_Tile* canvas );
		virtual ~VisualizationController(  );

		void CloseAll ( );
		void Display ( enum DisplayList view, Audio& data );
		void Display ( enum DisplayList view, Spectrum& data );
		void SetCanvas( Fl_Smart_Tile* canvas );

	private:
		struct DetachCallbackData
		{
			enum DisplayList view;
			VisualizationController* vc;
		}; 

		void Detach( enum DisplayList view );
		static void _Detach( Fl_Window* buffer, DetachCallbackData* ui );

		Fl_Smart_Tile* mCanvas;

		std::vector<DetachCallbackData> mDetachCallbackData;

		std::vector<AudioAdapter> mAudioAdapters;
		std::vector<Fl_Browsable_Playable_Audio*> mAudioPresentations;
		std::vector<LogMagSpectrumAdapter> mSpectrumAdapters;
		std::vector<Fl_Spectrum*> mSpectrumPresentations;

		std::vector<bool> mOpenDisplays;

		static CLAM::Enum::tEnumValue sDisplayName[];
	};

}

#endif //VisualizationController.hxx
