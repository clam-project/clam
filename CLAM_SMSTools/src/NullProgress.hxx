#ifndef _NullProgress_hxx_
#define _NullProgress_hxx_

namespace CLAMGUI
{
	class NullProgress : public Progress
	{
		public:
			NullProgress() : Progress("",0,0)
			{
			}
			void Update(float val)
			{
			}
	};
}

#endif //  _NullProgress_hxx_
