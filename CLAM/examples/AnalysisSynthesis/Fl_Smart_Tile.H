class Fl_Smart_Tile : public Fl_Group {
private:
	int closing_;
	int shading_;
	int moving_;
	int	buttondown_;
	int minsize_;
	int recalc(void);
public:
	Fl_Smart_Tile(int X,int Y,int W,int H,const char*l=0);
	void draw(void);
	int handle(int e);
	int minsize(void) const { return minsize_; }
	void minsize(int i) { minsize_ = i; }
};
