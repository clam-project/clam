#ifndef __MULTIPLOTRENDERER__
#define __MULTIPLOTRENDERER__

#include "Array.hxx"
#include "Renderer.hxx"

namespace CLAM
{
	namespace VM
	{
		class CLAM_DLL_EXPORT MultiPlotRenderer : public Renderer
		{
		public:
			MultiPlotRenderer();
			~MultiPlotRenderer();

			void SetData(const DataArray& data);
			void Render();

		private:
			DataArray mData;
				
		};
	}
}

#endif


