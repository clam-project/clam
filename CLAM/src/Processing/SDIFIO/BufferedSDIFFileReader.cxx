#include "BufferedSDIFFileReader.hxx"
#include "DataUtil.hxx"
#include <cstddef>
#include <limits.h>
#include <thread>

namespace CLAM
{

BufferedSDIFFileReader::BufferedSDIFFileReader(const SDIFInConfig& argSDIFInConfig) :
	mSDIFFileReader(argSDIFInConfig),
	frameBufferPosition(1),
	mFrameLoadChunkSize( DEFAULT_FRAME_LOAD_CHUNK_SIZE ),
	mThreshholdForPreloading( DEFAULT_THRESHHOLD_FOR_PRELOADING ),
	mThreshholdForPreloadingOnThread( DEFAULT_THRESHHOLD_FOR_PRELOADING_ON_THREAD ),
	mReaderHasMoreFrames( true ),
	dequeMutex(),
	readSDIFMutex()
{
	Configure(argSDIFInConfig);
}

BufferedSDIFFileReader::~BufferedSDIFFileReader()
{
	if (mThread.joinable()) mThread.join();
	for (auto* frame : frameBuffer)
		delete frame;
}

bool BufferedSDIFFileReader::Configure(const SDIFInConfig& config)
{
	bool response;
	if ( (response = mSDIFFileReader.Configure(config)) )
	{
		// if the metadata has defined the number of milliseconds to preload
		// preload the corresponding number of frames.
		if ( config.HasNumberOfFramesToPreload() )
		{
			//std::cout << "Preloading " << config.GetNumberOfFramesToPreload()+1 << " frames" << std::endl;
			LoadFramesIntoBuffer( config.GetNumberOfFramesToPreload()+1 );
		}
		else
		{
			LoadFramesIntoBuffer( DEFAULT_INITIAL_NUMBER_OF_FRAMES_TO_BUFFER );
		}
		
		if ( config.HasNumberOfFramesToLoad() )
		{
			totalNumberOfFramesToLoad = config.GetNumberOfFramesToLoad();
		}
		else
		{
			if (config.HasNumberOfFramesToPreload())
				totalNumberOfFramesToLoad = config.GetNumberOfFramesToPreload()+1;
			else
				totalNumberOfFramesToLoad = INT_MAX;
		}
	}

	return response;
}

const SDIFInConfig BufferedSDIFFileReader::GetConfig()
{
	return dynamic_cast<const SDIFInConfig&>(mSDIFFileReader.GetConfig());
}

int BufferedSDIFFileReader::GetFrameBufferPosition()
{
	return frameBufferPosition;
}

void BufferedSDIFFileReader::SetFrameBufferPosition(int argFrameBufferPosition)
{
	frameBufferPosition = argFrameBufferPosition;
}

Frame* BufferedSDIFFileReader::GetFrame( int frameBufferPosition )
{
	std::lock_guard<std::mutex> lock(dequeMutex);

	Frame* requestedFrame = frameBuffer.at(frameBufferPosition);
	
	return requestedFrame;
}

Frame* BufferedSDIFFileReader::ReadFrame()
{
	int framesTillTheEnd = frameBuffer.size() - frameBufferPosition;
	if ((mReaderHasMoreFrames == true) && (framesTillTheEnd < mThreshholdForPreloading))
	{
		mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
	}

	if ( frameBuffer.size() == 0 )
	{
		return nullptr;
	}
	else
	{
		std::lock_guard<std::mutex> lock(dequeMutex);
		
		//std::cout << "Retrieving buffer pos: " << frameBufferPosition << ", size: " << frameBuffer.size() << std::endl;
		Frame* nextFrame = frameBuffer.at(frameBufferPosition);
		frameBufferPosition++;

		if (nextFrame == nullptr)
		{
			std::cout << "next frame is null" << std::endl;
		}

		return nextFrame;
	}
}

bool BufferedSDIFFileReader::LoadFramesIntoBuffer(int argNumberOfBuffers)
{
	// when we read in the SDIFFrames, let's first put them in a temporary
	// list object so we can avoid locking the frameBuffer every iteration
	// of the loop
	std::deque<Frame*> tempFrameBuffer;

	// this object locks the mSDIFFileReader until the scopedlock goes
	// out of scope and is garbage collected. its destructor frees the lock
	{
		std::lock_guard<std::mutex> lock(readSDIFMutex);
		
		// here's the loop where we read in the desired number of buffers
		for (int counter = 0; counter < argNumberOfBuffers; counter++)
		{
			Frame* aFrame = new Frame();
			aFrame->AddSpectralPeakArray();
			aFrame->AddResidualSpec();
			aFrame->AddFundamental();
			aFrame->AddSynthAudioFrame();
			aFrame->UpdateData();
	
			TTime frameCenterTime;
			
			// we read in the frame
			mReaderHasMoreFrames = mSDIFFileReader.ReadFrame( aFrame->GetFundamental(), aFrame->GetSpectralPeakArray(), aFrame->GetResidualSpec(), frameCenterTime );

			// we'll update the frame position variable when we actually add these to 
			// the frameVector
		
			// as long as there was something to read, add the frame to the temporary buffer
			if (mReaderHasMoreFrames)
			{
				aFrame->SetCenterTime(frameCenterTime);
		
				tempFrameBuffer.push_back( aFrame );
			}
			else
			{
				std::cout << "BufferedSDIFReader: could only load " << counter << " of ";
				std::cout << argNumberOfBuffers << " frames." << std::endl;
				delete aFrame;
				break;
			}
		}
	}

	// now let's copy the frames from the temporary buffer to the frameBuffer
	std::lock_guard<std::mutex> lock(dequeMutex);
	for (auto* frame : tempFrameBuffer)
		frameBuffer.push_back(frame);

	return mReaderHasMoreFrames;
}

int BufferedSDIFFileReader::GetNumberOfFramesLoaded()
{
	std::lock_guard<std::mutex> lock(dequeMutex);
	return frameBuffer.size();
}

void BufferedSDIFFileReader::LoadFramesIntoBufferOnThread()
{
	if (mThread.joinable()) mThread.join();
	try
	{
		mThread = std::thread([this]{ Run(); });
	}
	catch( std::exception& e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "BufferedSDIFFileReader: exception when starting thread. SDIFFile will be loaded on main thread." << std::endl;
	}
}

void BufferedSDIFFileReader::StopLoadingFramesIntoBufferOnThread()
{
	if (mThread.joinable()) mThread.join();
}

bool BufferedSDIFFileReader::IsThreaded()
{
	return mThread.joinable();
}

void BufferedSDIFFileReader::Run()
{
	while ( mReaderHasMoreFrames && static_cast<std::size_t>(totalNumberOfFramesToLoad) <= frameBuffer.size() )
	{
		mReaderHasMoreFrames = LoadFramesIntoBuffer(mFrameLoadChunkSize);
		std::this_thread::yield();
	}
}



} // END CLAM


