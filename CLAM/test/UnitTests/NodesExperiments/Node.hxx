#ifndef Node_hxx
#define Node_hxx

class CircularStream
{
};
class ListStream
{
};

template< typename Token, typename StreamImpl>
class Node
{
public:
	Node()
	{}
	Node(const Token& token) : _token(token)
	{}
	const Token& content() const
	{
		return _token;
	}
	void content(const Token& token)
	{
		_token = token;
	}
private:
	Token _token;
};

typedef CircularStream DefaultStreamImpl;

#endif


