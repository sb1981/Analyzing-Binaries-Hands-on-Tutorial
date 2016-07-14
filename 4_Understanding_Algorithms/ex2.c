int main(int argc, char *argv[])
{
	int i = 0;
	
	if(argc != 3)
		return -1;
	
	while(1)
	{
		if(argv[1][i]==0 && argv[2][i]==0)
			return 0;
		if(argv[2][i]==0)
			return 1;
		if(argv[1][i]==0)
			return 2;
		i++;
	}
	return 0;
}
