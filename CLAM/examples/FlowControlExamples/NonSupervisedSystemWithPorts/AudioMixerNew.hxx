
#ifndef _AUDIO_MIXER_NEW_HXX
#define _AUDIO_MIXER_NEW_HXX

#include "Processing.hxx"
#include "Port.hxx"
#include "Audio.hxx"
#include <string>

namespace CLAM
{

	class AudioMixerConfig: public ProcessingConfig
	{
	public:
		DYNAMIC_TYPE_USING_INTERFACE (AudioMixerConfig, 3, ProcessingConfig);
		DYN_ATTRIBUTE (0, public, std::string, Name);
		DYN_ATTRIBUTE (1, public, TData,       SampleRate);
		DYN_ATTRIBUTE (2, public, int,         FrameSize);

	protected:
		void DefaultInit(void)
		{
			AddAll();
			UpdateData();
			SetSampleRate(44100);
			SetFrameSize(512);
		}

	};

	template<unsigned int N>
	class AudioMixer: public Processing
	{
	public:

		InPortArrayTmpl<Audio> Input;
		OutPortTmpl<Audio>     Output;
		
		InControlArray Volumes;

	private:

		AudioMixerConfig  mConfig;
		unsigned int mFrameSize;
		inline void Do(Array<TData>* inp[N], Array<TData>& out);

		void InitializeControls();
		bool ConcreteStart();
	public:

		bool ConcreteConfigure(const ProcessingConfig& c);

		AudioMixer(const AudioMixerConfig& c = AudioMixerConfig());
		inline const char * GetClassName() const {return "Audio Mixer";}
		inline const ProcessingConfig &GetConfig() const { return mConfig;}

		inline bool Do(void);
		inline bool Do(Audio* inp[N], Audio& out);
	};


	////////////////// implementations ////////////////////////

	

	template<unsigned int N>
	AudioMixer<N>::AudioMixer(const AudioMixerConfig& c)
		: Input(N,"Input Audio",this,1),
		  Output("Output Audio",this,1),
		  Volumes(N,"Input Volume Controls",this)
	{
		Configure(c);
	}
	
	template<unsigned int N>
	bool AudioMixer<N>::ConcreteConfigure(const ProcessingConfig& c)
	{
		CopyAsConcreteConfig(mConfig, c);
		
		mFrameSize = mConfig.GetFrameSize();

		Input.SetParams(mFrameSize);

		Output.SetParams(mFrameSize);

		return true;
	}

	template<unsigned int N>
	bool AudioMixer<N>::Do()
	{
		bool res = Do(Input.GetDataArray().GetPtr(),
					  Output.GetData());
		Input.LeaveDataArray();
		Output.LeaveData();	  
		return res;
	}

	template<unsigned int N>
	void AudioMixer<N>::Do(Array<TData>* in_array[N], Array<TData>& out_array)
	{
		TData sum;

		for (unsigned i=0; i<mFrameSize; i++) {
			sum=0.0;
			for (unsigned a=0; a<N; a++)
				sum += ((*in_array[a])[i])*(Volumes[a].GetLastValue());
			out_array[i] = sum / TData(N);
		}
	}


	template<unsigned int N>
	bool AudioMixer<N>::Do(Audio* inp[N], Audio& out)
	{
		CLAM_ASSERT(inp[0]->GetSize() == int(mFrameSize) &&
					out.GetSize()     == int(mFrameSize),
					"AudioMixer::Do(...): Size mismatch");
		CLAM_ASSERT(inp[0]->GetSampleRate() == mConfig.GetSampleRate() &&
					out.GetSampleRate()     == mConfig.GetSampleRate(),
					"AudioMixer::Do(...): sample rate mismatch");

		Array<TData>* in_array[N];
		Array<TData> &out_array = out.GetBuffer();

		for (unsigned a=0; a<N; a++)
			in_array[a] = &inp[a]->GetBuffer();

		Do(in_array,out_array);
			
		return true;
	}

	template<unsigned int N>
	void AudioMixer<N>::InitializeControls()
	{
		for (unsigned int i=0;i<N;i++)
		{
			Volumes[i].DoControl(1);
		}
	}

	template<unsigned int N>
	bool AudioMixer<N>::ConcreteStart()
	{
		InitializeControls();
		return true;
	}



}

#endif
