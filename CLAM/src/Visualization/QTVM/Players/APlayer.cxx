#include "AudioIO.hxx"
#include "AudioManager.hxx"
#include "AudioOut.hxx"
#include "APlayer.hxx"

namespace CLAM
{
	namespace VM
	{
		APlayer::APlayer()
		{
		}
		
		APlayer::~APlayer()
		{
		}
		
		void APlayer::SetData(const Audio& audio)
		{
			_audio = audio;
			MediaTime time;
			time.SetBegin(TData(0.0));
			time.SetEnd(TData(_audio.GetSize())/_audio.GetSampleRate());
			SetBounds(time);
			_thread.SetThreadCode(makeMemberFunctor0((*this), APlayer, thread_code));
			HaveData(true);
		}
		
		void APlayer::thread_code()
		{
			TSize nSamples = _audio.GetSize();         
			TData sampleRate = _audio.GetSampleRate(); 
			TSize frameSize = 512;                    

			AudioManager manager((int)sampleRate,(int)frameSize);  
			AudioOut channelL;   
			AudioOut channelR;
			AudioIOConfig audioOutCfgL;     
			AudioIOConfig audioOutCfgR; 
			audioOutCfgL.SetChannelID(0);    
			audioOutCfgR.SetChannelID(1);
			channelL.Configure(audioOutCfgL); 
			channelR.Configure(audioOutCfgR);
			AudioManager::Current().Start();                            
			channelL.Start();              
			channelR.Start();
    
			Audio samples;                
			samples.SetSize(frameSize);
								
		    TIndex leftIndex = TIndex(_time.GetBegin()*sampleRate);        
			TIndex rightIndex = leftIndex+frameSize;

			while(leftIndex < TIndex(_time.GetEnd()*sampleRate))
			{
				if(IsPaused())
				{
					_time.SetBegin(TData(leftIndex)/sampleRate);
					SetPlaying(false);
				}
				if(!IsPlaying()) break;
			    _audio.GetAudioChunk(leftIndex,rightIndex,samples);
				channelL.Do(samples);
				channelR.Do(samples);
				leftIndex += frameSize;
				rightIndex += frameSize;
			 }
			 channelL.Stop(); 
			 channelR.Stop();
			 if(!IsPaused()) _time.SetBegin(GetBeginTime());
		}
	}
}

// END

