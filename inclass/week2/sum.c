long sum(long x, long y);

void sumstore(long x,long y, long *dest){
	long t = sum(x,y);
	*dest = t;
}
