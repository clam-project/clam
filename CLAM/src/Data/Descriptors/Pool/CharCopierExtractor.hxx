#ifndef _CharCopierExtractor_hxx_
#define _CharCopierExtractor_hxx_

#include "Extractor.hxx"


namespace CLAMTest
{

class CharCopierExtractor
{
public:
	void SetHooks(CLAM::ReadHook<char> & inputHook, CLAM::WriteHook<char> & outputHook)
	{
		_inputHook = &inputHook;
		_outputHook = &outputHook;
	}

	void Extract()
	{
		const char  & input = _inputHook->GetForReading();
		char & output = _outputHook->GetForWriting();
		output = input;
	}
	bool IsInsideScope()
	{
		return _inputHook->IsInsideScope() && _outputHook->IsInsideScope();
	}
		
	void Next()
	{
		_inputHook->Next();
		_outputHook->Next();
	}

	void Init(CLAM::DescriptionDataPool & pool)
	{
		_inputHook->Init(pool);
		_outputHook->Init(pool);
	}
private:
	CLAM::ReadHook<char> * _inputHook;
	CLAM::WriteHook<char> * _outputHook;
};

class CharJoinExtractor
{
public:
	void SetHooks(CLAM::ReadIndirectRangedHook<char> & inputHook, CLAM::WriteHook<std::string> & outputHook)
	{
		_inputHook = &inputHook;
		_outputHook = &outputHook;
	}

	void Extract()
	{
		const char * input;
		const char * inputEnd;
		_inputHook->GetRangeForReading(input, inputEnd);
		std::string & output = _outputHook->GetForWriting();
		for (output = ""; input<inputEnd; input++)
			output += *input;
	}
	bool IsInsideScope()
	{
		return _inputHook->IsInsideScope() && _outputHook->IsInsideScope();
	}
		
	void Next()
	{
		_inputHook->Next();
		_outputHook->Next();
	}

	void Init(CLAM::DescriptionDataPool & pool)
	{
		_inputHook->Init(pool);
		_outputHook->Init(pool);
	}
private:
	CLAM::ReadIndirectRangedHook<char> * _inputHook;
	CLAM::WriteHook<std::string> * _outputHook;
};

} // namespace CLAMTest

#endif// _CharCopierExtractor_hxx_
