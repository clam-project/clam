
#ifndef __OutControlSender_hxx__
#define __OutControlSender_hxx__

#include "OutControl.hxx"
#include "Processing.hxx"
#include "ProcessingConfig.hxx"
#include "Enum.hxx"

namespace CLAM
{

class OutControlSenderConfig : public ProcessingConfig
{	
public:

	class EControlRepresentation : public Enum
	{
	public:
		EControlRepresentation() : Enum(sEnumValues, sDefault) {}
		EControlRepresentation(tValue v) : Enum(sEnumValues, v) {};
		EControlRepresentation(std::string s) : Enum(sEnumValues, s) {};

		enum tEnum {
			eUndetermined,
			eVerticalSlider,
			eHorizontalSlider,
			eKnot,
			eSpinBox
		};

		// C++17 static inline so the table and default value are
		// provided in every TU that needs them, without requiring DLL
		// export of a static data member (which CMake's
		// WINDOWS_EXPORT_ALL_SYMBOLS does not handle).
		static inline tEnumValue sEnumValues[] = {
			{ eUndetermined,      "Undetermined" },
			{ eVerticalSlider,    "Vertical Slider" },
			{ eHorizontalSlider,  "Horizontal Slider" },
			{ eKnot,              "Knot" },
			{ eSpinBox,           "Spin Box" },
			{ 0, nullptr }
		};
		static inline tValue sDefault = eHorizontalSlider;

		Component* Species() const
		{
			return new EControlRepresentation;
		};

	};

	class EMapping : public Enum
	{
	public:
		EMapping() : Enum(sEnumValues, sDefault) {}
		EMapping(tValue v) : Enum(sEnumValues, v) {};
		EMapping(std::string s) : Enum(sEnumValues, s) {};

		enum tEnum {
			eLinear,		///< 1:1 relationship (default)
			eInverted,		///< Inverted linear relationship
			eLog, 			///< More resolution in the low range
			eReverseLog		///< More resolution in the upper range
		};

		// See EControlRepresentation::sEnumValues for the rationale.
		static inline tEnumValue sEnumValues[] = {
			{ eLinear,     "Linear" },
			{ eInverted,   "Inverted" },
			{ eLog,        "Log" },
			{ eReverseLog, "Reverse Log" },
			{ 0, nullptr }
		};
		static inline tValue sDefault = eLinear;

		Component* Species() const
		{
			return new EMapping;
		};

	};

	DYNAMIC_TYPE_USING_INTERFACE (OutControlSenderConfig, 6, ProcessingConfig);
	DYN_ATTRIBUTE (0, public, TControlData, Min );
	DYN_ATTRIBUTE (1, public, TControlData, Default );
	DYN_ATTRIBUTE (2, public, TControlData, Max );
	DYN_ATTRIBUTE (3, public, TControlData, Step );
	DYN_ATTRIBUTE (4, public, EControlRepresentation, ControlRepresentation );
	DYN_ATTRIBUTE (5, public, EMapping, Mapping );


protected:
	void DefaultInit(void);
};

class OutControlSender : public Processing
{
	OutControlSenderConfig _config;
	FloatOutControl _output;
	TControlData _lastValue;
	bool _firstDoAfterStart;
public:
	typedef OutControlSenderConfig Config;
	OutControlSender( const Config & = Config() );

	bool Do();
	const char * GetClassName() const {return "OutControlSender";}

	const ProcessingConfig &GetConfig() const { return _config;}
	void SendControl(TControlData value);
protected:
	bool ConcreteConfigure(const ProcessingConfig& c);
	bool ConcreteStart();
};

} //namespace CLAM

#endif //__OutControlSender_hxx__



