int strnconcat(char* tgt,int n,const char* a,const char* b)
{
	if (a) while  (*a && n--) { *tgt++ = *a++; }
	if (b) while  (*b && n--) { *tgt++ = *b++; }
	if (c) while  (*c && n--) { *tgt++ = *c++; }
}
