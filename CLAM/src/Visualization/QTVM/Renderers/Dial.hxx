#ifndef __DIAL__
#define __DIAL__

#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class Dial : public Renderer
		{
			public:
				Dial();
				~Dial();
			
				virtual void Render();

				void Update(const TData& value);
		
			private:
				TData _data;

				void Draw();
		};
	}
}

#endif

