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

#include "SMSBase.hxx"
#include "Plot.hxx"
#include <iostream>
#include "StdOutProgress.hxx"
#include "StdOutWaitMessage.hxx"


using namespace CLAM;
using namespace std;

class SMSStdio:public SMSBase
{
public:
	CLAMGUI::Progress* CreateProgress(const char* title,float from,float to)
	{
		return new CLAMGUI::StdOutProgress(title,from,to);
	}
	CLAMGUI::WaitMessage* CreateWaitMessage(const char* title)
	{
		return new CLAMGUI::StdOutWaitMessage(title);
	}

	void Run(void);
};

void SMSStdio::Run(void)
{
	bool finish = false;
	while(!finish){
		std::cout <<"\n" << "\n";
		std::cout << "SMSBase Analysis/Synthesis Aplication" << "\n";
		std::cout << "MTG - UPF (Barcelona, Spain)"<<"\n" << "\n";
		std::cout << "Please choose one of the following options:" << "\n" << "\n";
		std::cout << "1. Load Analysis/Synthesis configuration file" << "\n";
		std::cout << "2. Load previously analyzed SMSBase file" << "\n";
		std::cout << "3. Analyze" << "\n";
		std::cout << "4. Store Analysis File" << "\n";
		std::cout << "5. Analyze Melody" << "\n";
		std::cout << "6. Store analyzed Melody" << "\n";
		std::cout << "7. Load Transformation Score" << "\n";
		std::cout << "8. Transform" << "\n";
		std::cout << "9. Synthesize" << "\n";
		std::cout << "0. Finish" << "\n" << "\n";

		int option;
		std::cin>>option;

		switch(option)
		{
			case 1://Load configuration
			{
				std::string inputXMLFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputXMLFileName;
				LoadConfig(inputXMLFileName);
				break;
			}
			case 2://Load analysis data
			{
				std::string inputFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML or SDIF File Name: \n";
				std::cin>>inputFileName;
				LoadAnalysis(inputFileName);
				break;
			}
			case 3://Analyze
			{
				if( !mDataState.Query(DataState::HaveConfig) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
				       	break;
				}
				LoadInputSound();
				CLAMVM::plot(mOriginalSegment.GetAudio(), "Input Audio");
				Analyze();
				break;
			}
			case 4://Store analysis data
			{
				if( !mDataState.Query(DataState::HaveConfig) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				if( !mDataState.Query(DataState::HaveAnalysis) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				std::string inputFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML or SDIF File Name: \n";
				std::cin>>inputFileName;
				StoreAnalysis( inputFileName );
				break;
			}
			case 5: //Analyze melody
			{
				if( !mDataState.Query(DataState::HaveConfig) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				if( !mDataState.Query(DataState::HaveAnalysis) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if( !mDataState.Query(DataState::HaveSpectrum) )
				{
					std::cout<<"\n"<<"\n"<<"Error, cannot analyze melody from loaded data"<<"\n";
					std::cout<<"\n"<<"Please select option 2 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				mDataState.Reached( DataState::HaveMelody );
				AnalyzeMelody();
				break;
			}
			case 6://Store melody
			{
				if( !mDataState.Query(DataState::HaveMelody) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available melody"<<"\n";
					std::cout<<"\n"<<"Please select option 5 of the menu first"<<"\n";
					break;
				}
				//tmpSegment=mySegment;
				std::string inputFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputFileName;
				StoreMelody( inputFileName.c_str() );
				break;
			}
			case 7://Load Transformation score
			{
				std::string inputXMLFileName;
				std::cout<<"\n"<<"\n"<<"Enter Input XML File Name: \n";
				std::cin>>inputXMLFileName;
				LoadTransformationScore(inputXMLFileName);
				mDataState.Reached( DataState::HaveTransformationScore );
				break;
			}
			case 8://Transform
			{
				if( !mDataState.Query(DataState::HaveTransformationScore) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available transformation score"<<"\n";
					std::cout<<"\n"<<"Please select option 7 of the menu first"<<"\n";
					break;
				}
				if( !mDataState.Query(DataState::HaveAnalysis) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				Transform();
				break;
			}

			case 9://Synthesize
			{
				if( !mDataState.Query(DataState::HaveAnalysis) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis"<<"\n";
					std::cout<<"\n"<<"Please select option 2 or 3 of the menu first"<<"\n";
					break;
				}
				if( !mDataState.Query(DataState::HaveConfig) )
				{
					std::cout<<"\n"<<"\n"<<"Error, there is no available analysis/synthesis configuration"<<"\n";
					std::cout<<"\n"<<"Please select option 1 of the menu first"<<"\n";
					break;
				}
				Synthesize();
				
				CLAMVM::plot(mAudioOut, "Output Sound");
				CLAMVM::plot(mAudioOutSin, "Sinusoidal Component");
				CLAMVM::plot(mAudioOutRes, "Residual Component");
				break;
			}
			case 0://Exit
			{
				finish=true;
				break;
			}
			default:
			{
				std::cout<<"\n"<<"\n"<<"Error, not a valid option"<<"\n"<<"\n"<<"\n";
				break;
			}
		}	
	}
}

int main(int argc,char** argv)
{
	try{
		SMSStdio example;
		example.Run();
	}
	catch(Err error)
	{
		error.Print();
		std::cerr << "Abnormal Program Termination" << std::endl;
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}
	
	std::clog << "Finished successfully!";
	return 0;
}
