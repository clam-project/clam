 #ifndef __APLAYER__ 
 #define __APLAYER__ 
 
 #include "Audio.hxx"
 #include "Player.hxx"
 
 namespace CLAM
 {
	namespace VM
	{
		class APlayer : public Player
		{
			public:
				APlayer();
				virtual ~APlayer();
				
				void SetData(const Audio& audio);
				
			protected:
				void thread_code();
				
			private:
				Audio _audio;
		};
	}
 }
 
 #endif
 
