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
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "AudioPlayer.hxx"

#include "Audio.hxx"
#include "AudioIO.hxx"
#include "AudioOut.hxx"
#include "AudioManager.hxx"

#include "DataTypes.hxx"

#include "Err.hxx"

using namespace CLAM;

bool AudioPlayer::sCancel = false;
void AudioPlayer::SetCancel( bool value ) { sCancel = value; }
bool AudioPlayer::GetCancel(  ) { return sCancel; }

AudioPlayer* AudioPlayer::sCurrentPlayer;

AudioPlayer::AudioPlayer( const TData& sampleRate, const TTime& beginTime, const DataArray& data, SigSlot::Slotv0& slot )
{
	mAudioReference.SetSampleRate( sampleRate );
	mAudioReference.SetBeginTime( beginTime );
	mAudioReference.SetBuffer( data );

	SetCancel( false );
	sCurrentPlayer = this;

	mSignal.Connect( slot );

	pthread_create( &mThread, 0, sPlayingThreadSafe, this );
}

AudioPlayer::~AudioPlayer(  ) 
{
	SetCancel( true );
	pthread_join( mThread, 0 );
}

void AudioPlayer::PlayingThreadSafe(  )
{
	TSize bufferSize=512;
	AudioManager audioManager( mAudioReference.GetSampleRate(), bufferSize );
	
	AudioIOConfig mOutCfgL;
	AudioIOConfig mOutCfgR;
	AudioOut mOutputL;
	AudioOut mOutputR;

	mOutCfgL.SetName( "left mOut" );
	mOutCfgL.SetChannelID( 0 );
	mOutCfgR.SetName( "right mOut" );
	mOutCfgR.SetChannelID( 1 );
		
	mOutputL.Configure( mOutCfgL );
	mOutputR.Configure( mOutCfgR );
	
	Audio tmpAudioBuffer;
	tmpAudioBuffer.SetSize(bufferSize);
	TSize dataSize = mAudioReference.GetSize();
	AudioManager::Current().Start();
		
	mOutputL.Start();
	mOutputR.Start();
	for( int i=0; i<dataSize && !GetCancel(); i+=bufferSize )
	{
		mAudioReference.GetAudioChunk( i, i + tmpAudioBuffer.GetSize(), tmpAudioBuffer, false );
		mOutputR.Do( tmpAudioBuffer );
		mOutputL.Do( tmpAudioBuffer );
	}

	if( !GetCancel() )
		mSignal.Emit(  );
}

//void AudioPlayer::Play( int i )

void* AudioPlayer::sPlayingThreadSafe(void* ptr)
{
 	((AudioPlayer*)ptr)->PlayingThreadSafe();
}

void AudioPlayer::Stop(  )
{
	if( !sCancel )
		delete sCurrentPlayer;
}
